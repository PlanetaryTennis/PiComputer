cd PiComputer
make
cd ..
cp /home/pi/PiComputer/$1 /home/pi/$1
scp -r $1 192.168.1.126:/home/pi
scp -r $1 192.168.1.131:/home/pi
scp -r $1 192.168.1.133:/home/pi
