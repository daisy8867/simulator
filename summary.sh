declare -a sche=("fifo")
for s in ${sche[@]}
do
	for((i=1; i<26; i++))
	do
		start=1500000000000000
		for a in $(awk '{print $1}' apps/$i)
		do
			t=$(awk -v a=$a '/'"$a"'/ {print $2}' apps/$i)
			j=0
			gpu=0;
			cpu=0;
			end=1400000000000000
			echo -n $s/$i/$a " "
			for log in $(ls $s/$i/$a/[0-9]* -ltr | awk '{print $9}')
			do
				res=$(awk 'BEGIN{getline; getline; print $1}' $log)
				start=$(awk -v s=$start '/start/ {if($3<s) {print $3} else{print s}}' $log)
				end=$(awk -v s=$end '/^end/ {if($3>s) {print $3} else{print s}}' $log)
				if [[ $res == "gpu" ]];
				then
					gpu=$(($gpu+1))
				elif [[ $res == "cpu" ]];
				then
					cpu=$(($cpu+1))
				fi
				j=$(($j+1));
				if ((j==t))
				then 
					break;
				fi
			done
			echo " " $gpu " " $cpu " " $start " " $end
		done
	done
done
