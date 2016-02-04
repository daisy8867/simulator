mkdir fair
echo "fair" > scheduler
for((k=1; k<26; k++))
do
	rm -r fair/$k
	mkdir fair/$k
	cp apps/$k app
	./a.out
	for a in $(awk '{print $1}' app)
	do
		rm $a/gputime
		awk '/gpu/ {print $5;}' $(ls $a/[0-9]* -ltr | awk '{print $9}') | tee $a/gputime
		awk -v s=start 'BEGIN{use=0} {if(NR%2) {use=1;} else {use=-1} print $1 " " use}' $a/gputime | tee -a fair/$k/gputime 
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
		awk 'BEGIN{start=1500000000000000; end=1400000000000000;}/start/ {if($3<start) {start=$3}} /^end/ {if($3>end) {end=$3}} END{print "start at: " start ", end at " end; print "Elapsed time: " (end-start)/100000 " s.";}' $(ls $a/[0-9]* -l | awk '{print $9}')   | tee -a $a/report
		awk 'BEGIN{getline; start=$3;} /gpu/ {print ($5-start)/100000;}' $(ls $a/[0-9]* -ltr | awk '{print $9}') | tee -a $a/gputilization
		mv $a fair/$k
   	done
	mv io_* fair/$k
	rm fair/$k/tmp
	rm fair/$k/plotgputime
	cat fair/$k/gputime | sort > fair/$k/tmp
	start=$(awk 'BEGIN{getline; print $2}' fair/$k/io_fair)
	awk -v s=$start 'BEGIN{use=0; print 0 " " 0;} {use+=$2; print ($1-s)/100000  " " use}' fair/$k/tmp | tee -a fair/$k/plotgputime
gnuplot << EOF
	set term pdfcairo font "Times New Roman, 15" size 9cm, 3cm
	set output "fair/$k/gpuusage.pdf"
	set style data lines
	set xlabel "Time (s)"
	set ylabel "Active GPU"
	#set xtics nomirror rotate by -45
	set key noenhanced
	set datafile missing "-"
	#set boxwidth 0.7
	set key height 1
	set key outside
	set key horizontal
	set xrange [0:2000]
	set xtics 0,500,2000
	set yrange [0:4.5]
	set ytics 0,1
	#set rmargin at screen 0.85
	plot "fair/$k/plotgputime"  with steps lw 2 lt 24 lc  rgb "#DC143C"  notitle 
	set output
EOF
done
