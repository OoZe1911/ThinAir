echo
echo "----------------"
echo " Quick ISO Rip"
echo "03/03/04 by OoZe"
echo "----------------"
echo
echo "This script will allow to rip NGC mini-DVD without ThinAir GUI."
echo "The 'setsuid' script should have been launched once."
echo
echo "ISO filename (with Unix path included) :"
read filename
echo
echo "-------------------------------------------"
echo "Laucnhing PSOLoad :"
echo "-------------------------------------------"
./psoload -i 192.168.1.100 riptool.dol
echo "-------------------------------------------"
echo "Waiting 10 seconds..."
sleep 10
echo "Rip Server called..."
echo "-------------------------------------------"
./thinair -rip "$filename"
echo "-------------------------------------------"
echo "ISO rip done."
echo "Thanks using Quick ISO Rip"
echo
