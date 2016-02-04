declare -a sche=("fifo" "modelSchedule" "dynamic")
for s in ${sche[@]}
do
	echo $s
#	rm $s/delay
	for((i=1; i<26; i++))
	do
#		rm $s/$i/delay
		cpu_base=0
		gpu_base=0
		for a in $(awk '{print $1}' apps/$i)
		do
			if [[ $a == "knn" ]];
			then
				cpu_base=123321
				gpu_base=17070
			elif [[ $a == "kmeans2" ]];
			then
				cpu_base=4700000
				gpu_base=73780
			elif [[ $a == "rs1" ]];
			then
				cpu_base=7945070
				gpu_base=198076
			elif [[ $a == "nb" ]];
			then
				cpu_base=546935
				gpu_base=35028
			elif [[ $a == "bp" ]];
			then
				cpu_base=615034
				gpu_base=23502
			fi
			t=$(awk -v a=$a '/'"$a"'/ {print $2}' apps/$i)
			j=0
#			rm $s/$i/$a/delay
#			echo -n $s/$i/$a " "
#			for log in $(ls $s/$i/$a/[0-9]* -ltr | awk '{print $9}')
#			do
#				echo -n $log " " | tee -a $s/$i/$a/delay
#				awk -v c=$cpu_base -v g=$gpu_base '
#				BEGIN {
#					getline;
#					start=$3; 
#					getline;
#					res=$1;
#				}
#				END {
#					if(res=="gpu") {
#						print "gpu delay: " ($3-start)/100/g;
#					}	
#					else if(res="cpu"){
#						print "cpu delay: " ($3-start)/100/c;
#					}
#				}
#				' $log | tee -a $s/$i/$a/delay
#				j=$(($j+1));
#				if ((j==t))
#				then 
#					break;
#				fi
#			done
			echo -n $a " " #| tee -a $s/$i/delay
			awk 'BEGIN{gdelay=0; cdelay=0; g=0; c=0}/gpu/ {gdelay+=$4; g++;} /cpu/{cdelay+=$4;c++;} END{printf("%d %lf\n", g, gdelay);}' $s/$i/$a/delay | tee -a $s/$i/gpudelay
   	done
		echo -n $i " " | tee -a $s/gpudelay
		awk 'BEGIN{delay=0; t=0}{delay+=$2; t+=$3}END{printf("total delay: %lf\n", delay/t)}' $s/$i/delay | tee -a $s/$i/gpudelay | tee -a $s/gpudelay
	done
done
