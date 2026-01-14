segFaults=0; 
i=0; 
doubleFrees=0
while [ 1==1 ]; do 
	clear && ./main.o
	err=$?
	echo "exit code : $err"
	echo "iteration $i"
	echo "segFaults : $segFaults"
	echo "double frees : $doubleFrees"
	i=$(($i+1)) 
	if [ "$err" == "1" ]; then 
		break
	elif [ "$err" == "139" ]; then
		segFaults=$(($segFaults+1)) 
	elif [ "$err" == "134" ]; then
		doubleFrees=$(($doubleFrees+1))
	elif [ "$err" != "0" ]; then
		echo -en "Unexpected error code $err."
		break;
	fi 
	sleep 1	
done

