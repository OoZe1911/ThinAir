/*
	ThinAir Server v0.7.2b
	Mac OS X version by OoZe

	Set DEBUG to 0 to have silent streaming
	Set DEBUG to 1 to have requests information during streaming
	Set DEBUG to 2 to have requests/answers details during streaming

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 5173
#define	SIMPORT 8191
#define BUFFERSIZE 1024
#define STREAM_BUFFERSIZE   1028

#define DEBUG   0

unsigned long	*Blocked_files;
unsigned long	fst_offset=0;

// This function could list all the files inside an ISO file
// and could also be used to initialize the list of files to block.
int	LoadFST(char *iso_filename,char *fb_filename)
{
	char	*bf[256];
	FILE	*fb_file;
	char	c;

	unsigned char	buffer[256];
	unsigned char	*FST;
	unsigned long	filename_offset=0,entries=0,string_table_offset=0,fst_size=0,file_offset=0,file_size=0;
	unsigned int	file=0,found=0,nb_files_to_block=0;
	unsigned long	i=0,j=0,k=0,offset=0,parent_offset=0,next_offset=0,name_offset=0;
	unsigned char	*filename=malloc(30),*path=malloc(256),*temp=malloc(256),*current_path=malloc(256);

	// If ThinAir is not in "list" mode :
	if (fb_filename!=NULL)
	{
		// Read the list of files to block
		fb_file=fopen(fb_filename,"rb");
		c=fgetc(fb_file);
		while(c!=EOF)
		{
			bf[i]=malloc(256);
			j=0;
			while(c!=(char)0x0a)
			{
				bf[i][j]=c;
				j++;
				c=fgetc(fb_file);
			}
			bf[i][j+1]=0;
			i++;
			c=fgetc(fb_file);
		}

		fclose(fb_file);
#if DEBUG==1 || DEBUG==2
		printf("------------------------------------------\n");
		printf("List of files to block :\n");
		for(j=0;j<i;j++)
			printf("File %i = %s\n",j,bf[j]);
		printf("------------------------------------------\n");
#endif
		nb_files_to_block=i;
	}

	// Load the FST from the ISO file :
	strcpy(filename,"");
	strcpy(path,"");
	strcpy(current_path,"/");
	file=open(iso_filename,O_RDONLY);
	if ((file==-1) || (file==0))
	{
		printf("Error opening %s file.\n",iso_filename);
		exit(-1);
	}

	// If ThinAir is in "list" mode :
	if(fb_filename==NULL)
	{
		printf("Listing all files inside %s.\n",iso_filename);
		printf("ISO file description : ");
		read(file, buffer, 256);
		buffer[255]=0;
		printf("%c%c%c (",buffer[0],buffer[1],buffer[2]);
		switch(buffer[3]){
			case 'E':
				printf("USA/NTSC");
				break;
			case 'P':
				printf("Europe/PAL");
				break;
			default:   
				printf("Japan/NTSC");
		}
		printf(") - %s\n",buffer+32);
	}

	// Load all the FST in memory :
	lseek(file,0x424,SEEK_SET);
	read(file,buffer,8);
	fst_offset=(unsigned int)buffer[0]*256*256*256+(unsigned int)buffer[1]*256*256+(unsigned int)buffer[2]*256+(unsigned int)buffer[3];
	fst_size=(unsigned int)buffer[4]*256*256*256+(unsigned int)buffer[5]*256*256+(unsigned int)buffer[6]*256+(unsigned int)buffer[7];
	FST=malloc(fst_size);
	lseek(file,fst_offset,SEEK_SET);
	read(file,FST,fst_size);
	close(file);

	entries=(unsigned int)FST[8]*256*256*256+(unsigned int)FST[9]*256*256+(unsigned int)FST[10]*256+(unsigned int)FST[11];
	string_table_offset=12*entries;
	// If ThinAir is not in "list" mode :
	if(fb_filename!=NULL)
		Blocked_files=(long *)malloc(nb_files_to_block*sizeof(long));

	for (i=1;i<entries;i++)
	{
		offset=i*0x0c;
		if(FST[offset]==0)
		{
			filename_offset=(unsigned int)FST[offset+1]*256*256+(unsigned int)FST[offset+2]*256+(unsigned int)FST[offset+3];
			strcpy(filename,&FST[string_table_offset+filename_offset]);
			file_offset=(unsigned int)FST[offset+4]*256*256*256+(unsigned int)FST[offset+5]*256*256+(unsigned int)FST[offset+6]*256+(unsigned int)FST[offset+7];
			file_size=(unsigned int)FST[offset+8]*256*256*256+(unsigned int)FST[offset+9]*256*256+(unsigned int)FST[offset+10]*256+(unsigned int)FST[offset+11];

			// Looking for the full path of the file :
			strcpy(path,"");
			strcpy(temp,"");
			j=0x0c;
			found=0;
			while ( (j<offset) && (found==0) )
			{
				// Does the file appear in a "next offset" reference ?
				// (search starts from the begining of the FST)
				if(FST[j]==1)
				{
					next_offset=(unsigned int)FST[j+8]*256*256*256+(unsigned int)FST[j+9]*256*256+(unsigned int)FST[j+10]*256+(unsigned int)FST[j+11];
					if (next_offset==i)
					{
						parent_offset=(unsigned int)FST[j+4]*256*256*256+(unsigned int)FST[j+5]*256*256+(unsigned int)FST[j+6]*256+(unsigned int)FST[j+7];
						while(parent_offset!=0)
						{
							// Folder's name :
							name_offset=(unsigned int)FST[parent_offset*0x0c+1]*256*256+(unsigned int)FST[parent_offset*0x0c+2]*256+(unsigned int)FST[parent_offset*0x0c+3];
							strcpy(temp,&FST[string_table_offset+name_offset]);
							strcat(temp,"/");
							strcat(temp,path);
							strcpy(path,temp);

							parent_offset=(unsigned int)FST[parent_offset*0x0c+4]*256*256*256+(unsigned int)FST[parent_offset*0x0c+5]*256*256+(unsigned int)FST[parent_offset*0x0c+6]*256+(unsigned int)FST[parent_offset*0x0c+7];
						}
						strcpy(temp,"/");
						strcat(temp,path);
						strcpy(path,temp);
						found=1;
					}
				}
				j=j+0x0c;
			}
			// If the file is not in a "next offset" reference
			//      check the if the previous entry is not a directory
			//      if YES path=path of the previous entry (which is a folder)
			//      if NO path=curent path (=the path of the previous file)
			// else
			//      path=path previously found
			if(found==1)
			{
				strcpy(current_path,path);
			}
			else
			{
				if ((offset>0x0c) && (FST[offset-0x0c]==1))
				{
					parent_offset=i-1;
					while(parent_offset!=0)
					{
						// Folder's name :
						name_offset=(unsigned int)FST[parent_offset*0x0c+1]*256*256+(unsigned int)FST[parent_offset*0x0c+2]*256+(unsigned int)FST[parent_offset*0x0c+3];
						strcpy(temp,&FST[string_table_offset+name_offset]);
						strcat(temp,"/");
						strcat(temp,path);
						strcpy(path,temp);

						parent_offset=(unsigned int)FST[parent_offset*0x0c+4]*256*256*256+(unsigned int)FST[parent_offset*0x0c+5]*256*256+(unsigned int)FST[parent_offset*0x0c+6]*256+(unsigned int)FST[parent_offset*0x0c+7];
					}
					strcpy(temp,"/");
					strcat(temp,path);
					strcpy(path,temp);
					strcpy(current_path,path);
				}
			}

			// If ThinAir is in "list" mode, we just print path+filename :
			if(fb_filename==NULL)
				printf("%s%s\n",current_path,filename);
			else
			{
				// Verify if the file should be blocked :
				j=0;
				while(j<nb_files_to_block)
				{
					strcpy(temp,"");
					strcat(temp,current_path);
					strcat(temp,filename);
					if(!strcmp(bf[j],temp))
					{
						// If the file should be blocked, we memorise its filename_offset inside the string table :
						Blocked_files[k]=string_table_offset+filename_offset;
						k++;
						j=nb_files_to_block;
					}
					j++;
				}
			}
		}
	}
	if(k>0)
		return(nb_files_to_block);
	else
		return(0);
}

// Function used to wait some ms :
void wait_ms(unsigned int ms)
{   struct timeval t1,t2;
	long	usec1=0,usec2=0;

	if(ms>0)
	{
		ms=ms*1000;
		gettimeofday(&t1, NULL);
		usec1=t1.tv_usec;
		do
		{
			gettimeofday(&t2,NULL);
			usec2=t2.tv_usec;
			if(usec2<usec1)
				usec2=usec2+1000000;
		}
		while((usec2-usec1)<ms);
	}
}

// Function used to wait subms :
void wait_subms(unsigned int ms)
{   struct timeval t1,t2;
	long	usec1=0,usec2=0;

	if(ms>0)
	{
		ms=ms*100;
		gettimeofday(&t1, NULL);
		usec1=t1.tv_usec;
		do
		{
			gettimeofday(&t2,NULL);
			usec2=t2.tv_usec;
			if(usec2<usec1)
				usec2=usec2+1000000;
		}
		while((usec2-usec1)<ms);
	}
}

// Ripping server :
void rip_server(char *filename)
{
	int mysock,addrsize,received,file,nbevents,nbfd,nb_buffer_received;
	char buffer[BUFFERSIZE];
	struct sockaddr_in myaddr,gcaddr;
	fd_set selects;
	struct timeval timer;

	fprintf(stderr,"Ripping Server started...\n");

	mysock=socket(AF_INET,SOCK_DGRAM,0);

	file=open(filename,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);

	bzero(&myaddr,sizeof(myaddr));
	myaddr.sin_family=AF_INET;
	myaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	myaddr.sin_port=htons(PORT);

	bind(mysock,(struct sockaddr *)&myaddr,sizeof(myaddr));

	//get hello message
	bzero(&gcaddr,sizeof(gcaddr));
	addrsize=sizeof(gcaddr);
	recvfrom(mysock,buffer,BUFFERSIZE,0,(struct sockaddr *)&gcaddr,&addrsize);
	sendto(mysock,"ok",2,0,(struct sockaddr *)&gcaddr,addrsize);

	nb_buffer_received=0;
	do{
		FD_ZERO(&selects);
		FD_SET(mysock,&selects);
		nbfd=mysock+1;

		timer.tv_sec=10;
		timer.tv_usec=0;

		nbevents=select(nbfd,&selects,NULL,NULL,&timer);
		
		if(FD_ISSET(mysock,&selects)){
			received=recvfrom(mysock,buffer,BUFFERSIZE,0,(struct sockaddr *)&gcaddr,&addrsize);
			write(file,buffer,received);
			nb_buffer_received++;
			sendto(mysock,"ok",2,0,(struct sockaddr *)&gcaddr,addrsize);
		}
		else
		{
			// Time out is reached and nothing was send from the GameCube
			if (nb_buffer_received<1425760) {
				// If the full DVD was not yet received
				// we are sending an "ok" (retry)
				sendto(mysock,"ok",2,0,(struct sockaddr *)&gcaddr,addrsize);
				nbevents=1;
			}
		}

	} while(nbevents!=0);

	fprintf(stderr,"Job done...\n");
	close(mysock);
	close(file);
}

// Streaming server :
void stream_server(int ms,char *filename,char* bf_filename)
{
	unsigned char	buffer[STREAM_BUFFERSIZE];
	unsigned int	mysock,addrsize,i,j,file,numpackets,dataread,nb_files_to_block=0;
	unsigned long	fileoffset,filesize;
	struct 			sockaddr_in myaddr,gcaddr;
	unsigned char	*FST_buffer;

	printf("Streaming server started...\n");

	file=open(filename,O_RDONLY);
	lseek(file, 0, SEEK_SET);
	read(file, buffer, 256);
	buffer[255]=0;
	printf("File to stream : %c%c%c (",buffer[0],buffer[1],buffer[2]);
	switch(buffer[3]){
		case 'E':
			printf("USA/NTSC");
			break;
		case 'P':
			printf("Europe/PAL");
			break;
		default:
			printf("Japan/NTSC");
	}
	printf(") - %s\n",buffer+32);

	printf("Delay is set to %i ms.\n",ms);

	// If block files option has been choosen :
	if (bf_filename!=NULL)
	{
		nb_files_to_block=LoadFST(filename,bf_filename);
		printf("Block files option is active.\n");
	}

	lseek(file, 0, SEEK_SET);

	bzero(&myaddr,sizeof(myaddr));
	myaddr.sin_family=AF_INET;
	myaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	myaddr.sin_port=htons(PORT);

	mysock=socket(AF_INET,SOCK_DGRAM,0);

	bind(mysock,(struct sockaddr *)&myaddr,sizeof(myaddr));

	// Init gcaddr
	bzero(&gcaddr,sizeof(gcaddr));

	while(1){
		addrsize=sizeof(gcaddr);
#if DEBUG==2
		printf("Listen\n");
#endif
		recvfrom(mysock,buffer,STREAM_BUFFERSIZE,0,(struct sockaddr *)&gcaddr,&addrsize);
		wait_ms(ms);

#if DEBUG==2
		printf("Request : %i %i%i%i %u %u %u %u -  %u %u %u %u\n",(int)buffer[0],(int)buffer[1],(int)buffer[2],(int)buffer[3],
		(unsigned int)buffer[4],(unsigned int)buffer[5],(unsigned int)buffer[6],(unsigned int)buffer[7],
		(unsigned int)buffer[8],(unsigned int)buffer[9],(unsigned int)buffer[10],(unsigned int)buffer[11]);
#endif

		fileoffset=((unsigned int)buffer[4]*256*256*256)+((unsigned int)buffer[5]*256*256)+((unsigned int)buffer[6]*256)+(unsigned int)buffer[7];
		filesize=((unsigned int)buffer[8]*256*256*256)+((unsigned int)buffer[9]*256*256)+((unsigned int)buffer[10]*256)+(unsigned int)buffer[11];
		numpackets=(filesize/(STREAM_BUFFERSIZE-4))+1;
#if DEBUG==1 || DEBUG==2
			printf("Received request : Offset=%ld Size=%ld Nbpackets=%i\n",fileoffset,filesize,numpackets);
#endif

		// If block files option has been choosen :
		if((nb_files_to_block>0) && (fileoffset==fst_offset))
		{
#if DEBUG==1 || DEBUG==2
				printf("\"toc.bin\" requested...blocking files management in progress...\n");
#endif
			// "toc.bin" file hase been requested :
			FST_buffer=malloc(filesize);

			// Load all the FST in memory :
			lseek(file, fileoffset, SEEK_SET);
			read(file,FST_buffer,filesize);

			// We change the name of the files to block inside the FST :
			for(i=0;i<nb_files_to_block;i++)
			{
				if(FST_buffer[Blocked_files[i]]==(unsigned char)"x")
				{
					FST_buffer[Blocked_files[i]]=(unsigned char)"z";
				}
				else
				{
					FST_buffer[Blocked_files[i]]=(unsigned char)"x";
				}
			}

			// Then we stream this new FST from memory :
			for(i=0;i<numpackets;i++)
			{
				if(i+1==numpackets)
					dataread=filesize%(STREAM_BUFFERSIZE-4);
				else
					dataread=STREAM_BUFFERSIZE-4;

				for(j=0;j<dataread;j++)
						buffer[j+4]=FST_buffer[i*(STREAM_BUFFERSIZE-4)+j];

				buffer[0]=1;
				buffer[2]=i/256;
				buffer[3]=i%256;

				sendto(mysock,buffer,dataread+4,0,(struct sockaddr *)&gcaddr,addrsize);
#if DEBUG==2
				printf("Packet sent : %i %i %i %i - size=%i\n",buffer[0],buffer[1],buffer[2],buffer[3],dataread);
#endif
				wait_ms(ms);
			}

#if DEBUG==1 || DEBUG==2
				printf("Modified \"toc.bin\" sent...\n");
#endif
			free(FST_buffer);
		}
		else
		{

			lseek(file, fileoffset, SEEK_SET);

			for(i=0;i<numpackets;i++)
			{
				if(i+1==numpackets)
					dataread=read(file, buffer+4, filesize%(STREAM_BUFFERSIZE-4));
				else
					dataread=read(file, buffer+4, STREAM_BUFFERSIZE-4);
				buffer[0]=1;
				buffer[2]=i/256;
				buffer[3]=i%256;

				sendto(mysock,buffer,dataread+4,0,(struct sockaddr *)&gcaddr,addrsize);
#if DEBUG==2
				printf("Packet sent : %i %i %i %i - size=%i\n",buffer[0],buffer[1],buffer[2],buffer[3],dataread);
#endif
				wait_ms(ms);
			}
		}
		//end of stream
		buffer[0]=2;
		sendto(mysock,buffer,4,0,(struct sockaddr *)&gcaddr,addrsize);
#if DEBUG==2
		printf("Packet sent : %i %i %i %i\n",buffer[0],buffer[1],buffer[2],buffer[3]);
#endif
#if DEBUG==1 || DEBUG==2
		printf("Request answered.\n");
#endif
	}

	// If block files option has been choosen :
	if (bf_filename!=NULL)
		free(Blocked_files);
	close(mysock);
	close(file);
}

// Remove garbage from GCM file :
void Shrink(char *filesource,char *filedest)
{
	FILE	*files=fopen(filesource,"rb");
	FILE	*filed=fopen(filedest,"wb+");
	unsigned long	fst_size=0,i=0,j=0,current_offset=0,old_offset=0,string_table_offset=0,entries=0,name_offset=0,temp=0,size=0;
	unsigned char	*name=(unsigned char *)malloc(256);

	printf("Shrinking %s into %s.\n",filesource,filedest);
	if( (files==NULL) || (filed==NULL) )
	{
		fprintf(stderr,"Error opening/creating file.\n");
		exit(-1);
	}

	// Copy the begining of the file (from begining to fst_offset+fst_size) :
	fseek(files,0x424,SEEK_SET);
	fst_offset=(unsigned int)fgetc(files)*256*256*256+(unsigned int)fgetc(files)*256*256+(unsigned int)fgetc(files)*256+(unsigned int)fgetc(files);
	fst_size=(unsigned int)fgetc(files)*256*256*256+(unsigned int)fgetc(files)*256*256+(unsigned int)fgetc(files)*256+(unsigned int)fgetc(files);
	fseek(files,fst_offset+8,SEEK_SET);
	entries=(unsigned int)fgetc(files)*256*256*256+(unsigned int)fgetc(files)*256*256+(unsigned int)fgetc(files)*256+(unsigned int)fgetc(files);
	string_table_offset=12*entries;

	printf("FST Offset = %ld - FST Size = %ld bytes - ",fst_offset,fst_size);
	printf("Number of entries = %ld.\n",entries);
	printf("Copying %s from 0 to %ld into %s\n",filesource,fst_offset+fst_size,filedest);

	fseek(files,0,SEEK_SET);
	for(i=0;i<fst_offset+fst_size;i++)
	{
		fputc(fgetc(files),filed);
	}

	// De-fragmentation :
	current_offset=i;
	for (i=1;i<entries;i++)
	{
		fseek(filed,fst_offset+i*0x0c,SEEK_SET);
		// If entry is a file :
		if(fgetc(filed)==0)
		{
			name_offset=(unsigned int)fgetc(filed)*256*256+(unsigned int)fgetc(filed)*256+(unsigned int)fgetc(filed);
			old_offset=(unsigned int)fgetc(filed)*256*256*256+(unsigned int)fgetc(filed)*256*256+(unsigned int)fgetc(filed)*256+(unsigned int)fgetc(filed);
			fseek(files,fst_offset+string_table_offset+name_offset,SEEK_SET);
			fgets(name,255,files);
			printf("Processing file \"%s\" - ",name);
			printf("Old offset = $%X - New offset = $%X\n",old_offset,current_offset);

			fseek(filed,fst_offset+i*0x0c+4,SEEK_SET);
			temp=current_offset / (256*256*256);
//			printf("%.2X ",temp);
			fputc(temp,filed);

			temp=( current_offset - temp*256*256*256 ) / (256*256);
//			printf("%.2X ",temp);
			fputc(temp,filed);

			temp=(current_offset%(256*256))/256;
//			printf("%.2X ",temp);
			fputc(temp,filed);

			temp=current_offset%256;
//			printf("%.2X\n",temp);
			fputc(temp,filed);

			// File creation :
			size=(unsigned int)fgetc(filed)*256*256*256+(unsigned int)fgetc(filed)*256*256+(unsigned int)fgetc(filed)*256+(unsigned int)fgetc(filed);
			fseek(files,old_offset,SEEK_SET);
			fseek(filed,current_offset,SEEK_SET);
			for(j=0;j<size;j++)
			{
				fputc(fgetc(files),filed);
			}

			current_offset+=size;
		}
	}
	printf("New file size = %ld bytes.\n",current_offset);
	free(name);
}

// GC SIM Preview Streaming server :
void stream_serverGCSIM(int ms,char *filename)
{
	unsigned char	buffer[STREAM_BUFFERSIZE];
	unsigned int	mysock,addrsize,i,j,k,nb,reset,status,file,numpackets,last,dataread;
	unsigned long	fileoffset,filesize;
	struct 			sockaddr_in myaddr,gcaddr;

	printf("Streaming GC Sim server started...\n");

	file=open(filename,O_RDONLY);
	lseek(file, 0, SEEK_SET);
	read(file, buffer, 256);
	buffer[255]=0;
	printf("File to stream : %c%c%c (",buffer[0],buffer[1],buffer[2]);
	switch(buffer[3])
	{
		case 'E':
			printf("USA/NTSC");
			break;
		case 'P':
			printf("Europe/PAL");
			break;
		default:
			printf("Japan/NTSC");
	}
	printf(") - %s\n",buffer+32);

	printf("Delay is set to %i ms.\n",ms);

	lseek(file, 0, SEEK_SET);

	bzero(&myaddr,sizeof(myaddr));
	myaddr.sin_family=AF_INET;
	myaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	myaddr.sin_port=htons(SIMPORT);

	mysock=socket(AF_INET,SOCK_DGRAM,0);

	bind(mysock,(struct sockaddr *)&myaddr,sizeof(myaddr));

	// Init gcaddr
	bzero(&gcaddr,sizeof(gcaddr));

	// Init server status :
	status=0;
	reset=0;

	// Status of the server could be :
	// 0 : waiting for Request
	// 1 : waiting for Go
	// 2 : waiting for Ack
	while(1)
	{
		addrsize=sizeof(gcaddr);
		recvfrom(mysock,buffer,STREAM_BUFFERSIZE,0,(struct sockaddr *)&gcaddr,&addrsize);
		wait_ms(ms);

		// Are we waiting for a Request :
		if(status==0)
		{
			if((unsigned int)buffer[0]==0xf8)
			{
				printf("--- Request received ---\n");
				fileoffset=(unsigned int)buffer[4]*256*256*256+(unsigned int)buffer[5]*256*256+(unsigned int)buffer[6]*256+(unsigned int)buffer[7];
				filesize=(unsigned int)buffer[8]*256*256*256+(unsigned int)buffer[9]*256*256+(unsigned int)buffer[10]*256+(unsigned int)buffer[11];
				printf("File requested : offset=%i - size=%i\n",fileoffset,filesize);
				buffer[0]=0;
				buffer[1]=0;
				buffer[2]=0;
				buffer[3]=1;
				sendto(mysock,buffer,4,0,(struct sockaddr *)&gcaddr,addrsize);
				wait_ms(ms);
				printf("Waiting GO\n");
				status=1;
			}
			else
			{
				printf("Warning : Request expected !\n");
			}
		}
		else
		{
			// Are we waiting for a GO :
			if(status==1)
			{
				if( ((unsigned int)buffer[0]==0x00) || ((unsigned int)buffer[1]==0xff))
				{
					reset=0;
					printf("--- GO received ---\n");
					printf("Sending Answer - ");
					numpackets=filesize/1024;
					last=numpackets;
					if(numpackets*1024<filesize)
					{
						numpackets++;
					}
					j=0;k=1;nb=0;
					for(i=0;i<numpackets;i++)
					{
						// Fill the buffer with the file :
						lseek(file, fileoffset, SEEK_SET);

						if(j==0)
						{
							buffer[0]=0;
							buffer[1]=k;
						}
						else
						{
							buffer[0]=k;
							buffer[1]=0;
						}
						if(i==last)
						{
							dataread=read(file, buffer+4, filesize%1024);
						}
						else
						{
							dataread=read(file, buffer+4, 1024);
						}
						sendto(mysock,buffer,dataread+4,0,(struct sockaddr *)&gcaddr,addrsize);
						wait_ms(ms);
						nb++;
						// We are preparing the next packet :
						fileoffset=fileoffset+1024;
						k=k*2;
						if(k==256)
						{
							k=1;
							j=1;
						}
						// Every 16 packets we are waiting for an ACK :
						if(nb==16 && i!=(numpackets-1))
						{
							while(1)
							{
								recvfrom(mysock,buffer,STREAM_BUFFERSIZE,0,(struct sockaddr *)&gcaddr,&addrsize);
								wait_ms(ms);
								if((unsigned int)buffer[0]!=0xf8)
								{
									j=0;
									k=1;
									nb=0;
									break;
								}
								else
								{
									reset=1;
									break;
								}
							}
							if(reset==1)
							{
								printf("<ERROR : Reseting server status>\n");
								break;
							}
						}
					}
					if(reset==1)
					{
						printf("Waiting Request\n");
						reset=0;
						status=0;
					}
					else
					{
						printf(" OK\n");
						printf("Waiting ACK\n");
						status=2;
					}
				}
				else
				{
					printf("Warning : GO expected !\n");
					if((unsigned int)buffer[0]==0xf8)
					{
						printf("<ERROR : GO expected / Request received - Sending NACK\n");
						// Send NACK :
						buffer[0]=0;
						buffer[1]=0;
						buffer[2]=0;
						buffer[3]=0;
						sendto(mysock,buffer,4,0,(struct sockaddr *)&gcaddr,addrsize);
						wait_ms(ms);
						reset=2;
					}
					reset++;
					if(reset==3)
					{
						printf("<ERROR : Reseting server status>\n");
						status=0;
						reset=0;
						printf("Waiting Request\n");
					}
				}
			}
			else
			{
				// Are we waiting for an ACK ?
				if(status==2)
				{
					// Check if the packet received is an ACK :
					if((unsigned int)buffer[0]==0x00)
					{
						reset=0;
						printf("--- ACK received ---\n");
						printf("Waiting Request\n");
						status=0;
					}
					else
					{
						printf("Warning : ACK expected !\n");
						if((unsigned int)buffer[0]==0xf8)
						{
							printf("<ERROR : Ack expected / Request received\n");
							reset=2;
						}
						reset++;
						if(reset==3)
						{
							printf("<ERROR : Reseting server status>\n");
							status=0;
							reset=0;
							printf("Waiting Request\n");
						}
					}
				}
			}
		}
	}

	close(mysock);
	close(file);
}

int main (int argc, const char * argv[]) {
	printf("ThinAir Server v0.7.2b\n");
	if(argc<3){
		fprintf(stderr,"Usage : %s [ -rip | -<sim>load <delay in ms> | -list | -shrink <filename.gcm> ] <filename.gcm> [ -bf <filename.ftb> ]\n",argv[0]);
		fprintf(stderr,"Flags usage :\n");
		fprintf(stderr,"\t-rip : start server in ripping mode.\n");
		fprintf(stderr,"\t-load : start server in streaming mode. Delay in ms should be set.\n");
		fprintf(stderr,"\t-simload : start server in streaming mode for GC SIM Preview. Delay in ms should be set.\n");
		fprintf(stderr,"\t-bf : block some files during the streaming process. Could only be used with -load flag.\n");
		fprintf(stderr,"\t-list : list all the files inside an GCM file.\n");
		fprintf(stderr,"\t-shrink : Remove garbage from GCM file.\n");
		exit(-1);
	}
	if (!strcmp(argv[1],"-rip"))
	{
		rip_server((char *)argv[2]);
	}
	else
	{
		if(!strcmp(argv[1],"-load"))
		{
			if(argc!=6)
				stream_server(atoi((char *)argv[2]),(char *)argv[3],NULL);
			else
			{
				if(!strcmp(argv[4],"-bf"))
					stream_server(atoi((char *)argv[2]),(char *)argv[3],(char *)argv[5]);
				else
				{
					fprintf(stderr,"Error on flag : use -bf flag.\n");
					exit(-1);
				}
			}
		}
		else
		{
			if(!strcmp(argv[1],"-list"))
			{
				LoadFST((char *)argv[2],NULL);
			}
			else
			{
				if(!strcmp(argv[1],"-shrink"))
				{
					Shrink((char *)argv[2],(char *)argv[3]);
				}
				else
				{
					if(!strcmp(argv[1],"-simload"))
					{
						stream_serverGCSIM(atoi((char *)argv[2]),(char *)argv[3]);
					}
					else
					{
						fprintf(stderr,"Please choose -rip , -load, -simload, -list or -shrink flag.\n");
						exit(-1);
					}
				}
			}
		}
	}
	return(0);
}