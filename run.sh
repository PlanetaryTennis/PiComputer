echo :---: >> measures.txt
echo Looking for $3 >> measures.txt
echo With $1 proccesses >> measures.txt
date +%T.%N >> measures.txt
mpiexec -f /home/pi/machinefile -n $1 /home/pi/$2 $3 > $3.txt
date +%T.%N >> measures.txt
