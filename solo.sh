# solo
declare -a apps=("kmeans2 10")
for a in ${!apps[@]}
do 
	echo ${apps[$a]} | tee app
	./a.out
	for a in $(awk '{print $1}' app)
	do
		rm $a/report
		for log in $(ls $a -ltr | awk '{print $9}')
		do 
			echo -n $log " " | tee -a $a/report
			awk '
			BEGIN {
				gpurd=0;
				gpuk=0;
				cpu=0;
				getline;
				start=$3; 
				getline;
				res=$1;
				if(res=="gpu") {
					gpurd=$5;
					getline;
					gpuk=$5;
				}	
				else if(res="cpu"){
					cpu=$4;
				}
			}
			END {
				if(res=="gpu") {
					print "gpu reading time: " (gpurd-start)/100000 " s gpu kernel: " (gpuk-gpurd)/100000 " s.";
				}	
				else if(res="cpu"){
					print "cpu time: " (cpu-start)/100000 " s.";
				}
			}
			' $a/$log | tee -a $a/report
		done
		awk 'BEGIN{getline; start=$3;} END{print "Elapsed time: " ($3-start)/100000 " s.";}' $(ls $a/[0-9]* -ltr | awk '{print $9}') | tee -a $a/report
		awk 'BEGIN{getline; start=$3;} /gpu/ {print ($5-start)/100000;}' $(ls $a/[0-9]* -ltr | awk '{print $9}') | tee -a $a/gputilization
		mv $a solo/$k
	done
done
