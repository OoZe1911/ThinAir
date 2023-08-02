echo
echo "----------------------"
echo "ThinAir v0.5b  by OoZe"
echo "----------------------"
echo
echo "This script should be launch only once in order to fix the PSOLoad permissions."
echo "You will have to type your 'root' password, but do not worry only PSOLoad file"
echo "will be touched."
echo
sudo chown 0:0 ./psoload
echo "PSOLoad given to root."
sudo chmod 6777 ./psoload
echo "PSOLoad SUID flag set."
echo
echo "Job finished, you can run ThinAir now."
echo
