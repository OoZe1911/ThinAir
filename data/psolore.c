/**
 * psolore.c
 *
 * Simple reloader for "GameCube PSO Loader V2.0 by Costis!" and
 * "GameCube Action Replay SD Loader V1.00" resident stubs
 * Copyright (C) 2004-2005 Albert Herranz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>		/* close */
#include <stdarg.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>

#define _GNU_SOURCE
#include <getopt.h>

#define PSOLORE_VERSION "V1.0-21032005"

#define DOL_HEADER_SIZE 256

#define PL2_DEFAULT_UPLOAD_PORT 9002

#define PL2_BLK_HEADER_SIZE (sizeof(struct pl2_resp))
#define PL2_BLK_DATA_SIZE   1024
#define PL2_BLK_SIZE        (PL2_BLK_HEADER_SIZE+PL2_BLK_DATA_SIZE)

#define PL2_ACTION_START 0x0001
#define PL2_ACTION_XFER  0x0002
#define PL2_ACTION_END   0x0003

#define PL2_REQ_FULL  (sizeof(struct pl2_req))
#define PL2_REQ_SHORT (PL2_REQ_FULL - sizeof(unsigned long))


struct pl2_partner {
	int sd;
	struct sockaddr_in addr;
	int addr_len;
};

struct pl2_req {
	unsigned short packets_bitmap;	/* 16 bits */
	unsigned short action;	/* 16 bits */
	unsigned long offset;	/* 32 bits */
};

struct pl2_resp {
	unsigned short csum;	/* 16 bits */
	unsigned short action;	/* 16 bits */
	unsigned long offset;	/* 32 bits */
};

struct pl2_config {
	int	daemonize;
	int	standalone;
	int	once;
	int	syslog;
	int	syslog_prio;
	int	waittime;
	struct in_addr address;
	int	port;
};

const char *__progname;

static char block[PL2_BLK_SIZE];

static struct pl2_config pl2_config = {
	.daemonize = 0,
	.standalone = 1,
	.once = 1,
	.syslog = 0,
	.syslog_prio = 0,
	.waittime = 900,
	.address = { .s_addr = INADDR_ANY },
};

static int pl2_log(int prio, const char *fmt, ...);


static void set_signal(int signum, void (*handler) (int), int flags)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = flags;

	if (sigaction(signum, &sa, NULL)) {
		pl2_log(LOG_ERR, "sigaction: %m");
		exit(1);
	}
}

static void print_req(struct pl2_req *req)
{
	struct pl2_config *config = &pl2_config;

	if (config->standalone && !config->daemonize) {
		printf("packets_bitmap = 0x%04X\n"
		       "action         = %d\n"
		       "offset         = 0x%08lX\n",
		       req->packets_bitmap, req->action, req->offset);
	}
}

static void fix_req_from_net(struct pl2_req *req)
{
	req->packets_bitmap = ntohs(req->packets_bitmap);
	req->action = ntohs(req->action);
	req->offset = ntohl(req->offset);
}

static void fix_resp_to_net(struct pl2_resp *resp)
{
	resp->action = htons(resp->action);
	resp->offset = htonl(resp->offset);
}


/**
 *
 */
static int pl2_log(int prio, const char *fmt, ...)
{
	struct pl2_config *config = &pl2_config;
	char buf[1024];
	va_list ap;

	va_start(ap, fmt);
	if (config->syslog) {
		vsyslog(config->syslog_prio | prio, fmt, ap);
	} else {
		/* XXX fix %m */
		vfprintf(stderr, fmt, ap);
		fprintf(stderr, "\n");
	}
	return 0;
}

/**
 *
 */
int pl2_sendto(struct pl2_partner *p, void *buf, size_t len)
{
	return sendto(p->sd, buf, len, 0,
		      (struct sockaddr *)&p->addr, p->addr_len);
}

/**
 *
 */
int pl2_recvfrom(struct pl2_partner *p, void *buf, size_t len)
{
	p->addr_len = sizeof(p->addr);
	return recvfrom(p->sd, buf, len, 0,
			(struct sockaddr *)&p->addr, &p->addr_len);
}

/**
 *
 */
unsigned short pl2_csum(void *buf, size_t len)
{
	unsigned long csum = 0;
	unsigned short *p = buf;
	int hlen;

	if (len < 0)
		return -1;

	hlen = len / 2;
	while (hlen-- > 0) {
		csum += *p++;
		csum = (csum & 0xFFFF) + (csum >> 16);
		csum = (csum & 0xFFFF) + (csum >> 16);
	}
	return (unsigned short)(csum ^ 0xFFFF);
}

/**
 *
 */
int pl2_do_upload_packets(struct pl2_partner *p, struct pl2_req *req, int fd)
{
	struct pl2_resp *resp;
	void *data;
	int len, bytes_sent;
	int i, res;
	off_t offset, pos = -1;

	for (i = 0; i < 16; i++) {
		if ((req->packets_bitmap & (1 << i)) != 0) {
			if (pos != req->offset) {
				offset = lseek(fd, req->offset, SEEK_SET);
				if (offset < 0) {
					pl2_log(LOG_ERR, "seek error: %m");
					return (int)offset;
				}
				pos = offset;
			}

			data = block + PL2_BLK_HEADER_SIZE;
			resp = (struct pl2_resp *)block;

			len = read(fd, data, PL2_BLK_DATA_SIZE);
			if (len < 0) {
				pl2_log(LOG_ERR, "read error: %m");
				return len;
			}
			pos += len;	/* implicit seek */
			resp->csum = pl2_csum(data, len);

			resp->action = req->action;
			resp->offset = req->offset;
			fix_resp_to_net(resp);

			if (len < PL2_BLK_DATA_SIZE)
				memset(data + len, 0, PL2_BLK_DATA_SIZE - len);
			bytes_sent = pl2_sendto(p, block, PL2_BLK_SIZE);
			if (bytes_sent < 0) {
				pl2_log(LOG_ERR, "socket send error: %m");
				return bytes_sent;
			}
			if (bytes_sent < len + PL2_BLK_HEADER_SIZE) {
				pl2_log(LOG_WARNING, "socket send short"
					" write");
			}
		}
		req->offset += PL2_BLK_DATA_SIZE;
	}
	return 0;
}

/**
 *
 */
int pl2_do_upload(struct pl2_partner *p, struct pl2_req *req, int fd)
{
	int cli_addr_len;
	int len, len2;
	void *h;
	int res;
	off_t offset;

	req->offset = 0;
	fix_req_from_net(req);

	if (req->packets_bitmap != 0 || req->action != PL2_ACTION_START) {
		pl2_log(LOG_WARNING, "uhmm, strange initial request"
			" from partner...");
	}

	offset = lseek(fd, 0, SEEK_SET);
	if (offset < 0) {
		pl2_log(LOG_ERR, "seek error: %m");
		return (int)offset;
	}

	len = read(fd, block, DOL_HEADER_SIZE);
	if (len < 0) {
		pl2_log(LOG_ERR, "read error: %m");
		return len;
	}
	h = block;

	/* send the DOL header, twice */
	len = pl2_sendto(p, h, DOL_HEADER_SIZE);
	len2 = pl2_sendto(p, h, DOL_HEADER_SIZE);
	if (len < 0 || len2 < 0) {
		pl2_log(LOG_ERR, "socket sendto error: %m");
		return (len < 0)?len:len2;
	}

	while (1) {
		/* read transfer request from partner */
		len = pl2_recvfrom(p, req, sizeof(*req));
		if (len < 0) {
			pl2_log(LOG_ERR, "socket recvfrom error: %m");
			return len;
		}
		fix_req_from_net(req);

		if (req->action == PL2_ACTION_END)
			break;

		/* psoload sends XFER, sdload sends START */
		if (req->action != PL2_ACTION_XFER &&
		    req->action != PL2_ACTION_START) {
			pl2_log(LOG_WARNING, "uhmm, unexpected"
				" request 0x%02x ...", req->action);
		}

		res = pl2_do_upload_packets(p, req, fd);
		if (res < 0) {
			pl2_log(LOG_ERR, "upload error");
			return res;
		}
	}
	return 0;
}

/**
 *
 */
int upload_server_main(int fd)
{
	struct pl2_config *config = &pl2_config;
	struct pl2_partner *p, partner;
	struct sockaddr_in cli_addr, serv_addr;
	struct pl2_req req;
	pid_t pid;
	int sd, null_fd, len;
	fd_set fdset;
	struct timeval tv_waittime;
	int status;
	int res;

	if (config->standalone) {
		if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			pl2_log(LOG_ERR, "can't open socket: %m");
			return sd;
		}

		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		memcpy(&serv_addr.sin_addr, &config->address,
		       sizeof(serv_addr.sin_addr));
		serv_addr.sin_port = config->port;

		res = bind(sd, (struct sockaddr *)&serv_addr,
			   sizeof(serv_addr));
		if (res < 0) {
			pl2_log(LOG_ERR, "can't bind local address: %m");
			close(sd);
			return res;
		}

		if (config->daemonize) {
			pid = fork();

			if (pid > 0)
				exit(0);
			if (pid < 0) {
				pl2_log(LOG_ERR, "cannot fork: %m");
				exit(1);
			}

			null_fd = open("/dev/null", O_RDWR);
			if (null_fd >= 3) {
#ifdef HAVE_DUP2
				dup2(null_fd, 0);
				dup2(null_fd, 1);
				dup2(null_fd, 2);
#else
				close(0);
				dup(null_fd);
				close(1);
				dup(null_fd);
				close(2);
				dup(null_fd);
#endif
				close(null_fd);
			} else if (null_fd < 0) {
				close(0);
				close(1);
				close(2);
			}
#ifdef HAVE_SETSID
#ifndef __CYGWIN__		/* Kills the process on Cygwin? */
			setsid();
#endif
#endif
		}
	} else {
		/* when called from *inetd, 0 is our socket descriptor */
		sd = 0;
		close(1);
		close(2);
	}

#if 0
	/* This means we don't want to wait() for children */
#ifdef SA_NOCLDWAIT
	set_signal(SIGCHLD, SIG_IGN, SA_NOCLDSTOP | SA_NOCLDWAIT);
#else
	set_signal(SIGCHLD, SIG_IGN, SA_NOCLDSTOP);
#endif
#endif

	/* listening server processing loop */

	pl2_log(LOG_INFO, "Waiting for cubes on %s port %d ...",
		inet_ntoa(config->address), ntohs(config->port));

	p = &partner;
	memset(p, 0, sizeof(*p));
	p->sd = sd;

	while(1) {
		FD_ZERO(&fdset);
		FD_SET(sd, &fdset);
		tv_waittime.tv_sec = config->waittime;
		tv_waittime.tv_usec = 0;

		res = select(sd+1, &fdset, NULL, NULL,
			     config->standalone ? NULL : &tv_waittime);
		if (res == -1) {
			if (errno == EINTR)
				continue;
			pl2_log(LOG_ERR, "select error: %m");
			close(sd);
			return res;
		} else if (res == 0) {
			/* should only happen when lauched through inetd */
			exit(0);
		}

		/* read first request from GameCube */
		memset(&req, 0, sizeof(req));
		len = pl2_recvfrom(p, &req, PL2_REQ_SHORT);
		if (len < 0) {
			if (len == EINTR) {
				continue;
			} else {
				pl2_log(LOG_ERR, "socket recvfrom error: %m");
				close(sd);
				return len;
			}
		}

		/*
		 * XXX changes to config could be done here depending on
		 * where the request originates, etc.
		 * We could also open and serve a different file here, etc.
		 */

		/* just once, so avoid the fork and just do the job */
		if (config->once)
			break;

		pid = fork();
		if (pid < 0) {
			pl2_log(LOG_ERR, "fork error: %m");
			close(sd);
			/* return to inetd */
			return pid;
		} else if ( pid == 0 )
			break; 

		res = waitpid(pid, &status, 0);
		if (res < 0) {
			pl2_log(LOG_ERR, "waitpid error: %m");
			close(sd);
			return len;
		}

	}

	/*  child continues here */

	pl2_log(LOG_INFO, "reload request from %s",
		inet_ntoa(p->addr.sin_addr));

	res = pl2_do_upload(p, &req, fd);
	if (res < 0) {
		close(sd);
		return res;
	} else {
		pl2_log(LOG_INFO, "completed upload to %s",
			inet_ntoa(p->addr.sin_addr));
	}

	close(sd);
	return 0;
}

/**
 *
 */
int set_ip(char *optarg)
{
	struct pl2_config *config = &pl2_config;
	struct hostent *hostent;

	hostent = gethostbyname(optarg);
	if (!hostent || hostent->h_addrtype != AF_INET) {
		pl2_log(LOG_ERR, "can't resolve bind address: %s", optarg);
		return -1;
	}
	memcpy(&config->address, hostent->h_addr, hostent->h_length);
	return 0;
}

/**
 *
 */
int set_port(char *optarg)
{
	struct pl2_config *config = &pl2_config;
	struct servent *servent;
	unsigned long port;
	char *ep;

	servent = getservbyname(optarg, "udp");
	if (servent) {
		config->port = servent->s_port;
	} else if ((port = strtoul(optarg, &ep, 0)) && !*ep) {
		config->port = htons(port);
	} else {
		pl2_log(LOG_ERR, "cannot resolve bind port: %s", optarg);
		return -1;
	}
	return 0;
}

/**
 *
 */
void version(void)
{
	pl2_log(LOG_INFO, "version %s", PSOLORE_VERSION);
	exit(2);
}

/**
 *
 */
void usage(void)
{
	pl2_log(LOG_ERR,
		"Usage: %s [OPTION] [FILE]" "\n"
		"  -d, --daemonize    run in the background" "\n"
		"  -x, --inetd        run through inetd" "\n"
		"  -i, --ip=ADDRESS   use local bind ADDRESS" "\n"
		"                       (default ANY)" "\n"
		"  -p, --port=PORT    use local bind PORT" "\n"
		"                       (default 9002)" "\n"
		"  -m, --many         serve many requests" "\n"
		"                       (default ON for -d or -x)" "\n"
		, __progname);
	exit(1);
}

/**
 *
 */
int main(int argc, char *argv[])
{
	struct pl2_config *config = &pl2_config;
	char *pathname;
	int fd, ch, res;
	char *p;

	p = strrchr(argv[0], '/');
	__progname = (p && p[1]) ? p+1 : argv[0];

	config->port = htons(PL2_DEFAULT_UPLOAD_PORT);

	struct option long_options[] = {
		{"daemonize", 0, NULL, 'd'},
		{"inetd", 0, NULL, 'x'},
		{"ip", 1, NULL, 'i'},
		{"port", 1, NULL, 'p'},
		{"many", 0, NULL, 'm'},
		{"version", 0, NULL, 'v'},
		{"help", 0, NULL, 'h'},
		{0,0,0,0}
	};
#define SHORT_OPTIONS "dxi:p:mvh"

	while((ch = getopt_long(argc, argv, SHORT_OPTIONS,
				long_options, NULL)) != -1) {
		switch(ch) {
			case 'd':
				config->standalone = 1;
				config->daemonize = 1;
				config->once = 0;
				break;
			case 'x':
				config->standalone = 0;
				config->daemonize = 1;
				config->once = 0;
				break;
			case 'i':
				if (set_ip(optarg) < 0) {
					usage();
				}
				break;
			case 'p':
				if (set_port(optarg) < 0) {
					usage();
				}
				break;
			case 'm':
				config->once = 0;
				break;
			case 'v':
				version();
				break;
			case 'h':
			case '?':
			default:
				usage();
				break;
		}
	}

	if (argc-optind != 1) {
		usage();
	}

	if (config->daemonize) {
		config->syslog = 1;
	}

	pathname = argv[optind];

	fd = open(pathname, O_RDONLY);
	if (fd < 0) {
		pl2_log(LOG_ERR, "%s: open error: %m", pathname);
		exit(1);
	}
	res = upload_server_main(fd);
	if (res < 0) {
		pl2_log(LOG_ERR, "%s: reloader error", pathname);
		exit(1);
	}

	exit(0);
}

