declare -a apps=("knn" "kmeans2" "rs1" "nb" "bp")
k=0
for i in ${!apps[@]}
do
	for((j=$i+1; j<${#apps[@]}; j++))
	do
		k=$(($k+1))
		rm apps/$k
		echo ${apps[$i]} " 103" | tee -a apps/$k
		echo ${apps[$j]} " 103" | tee -a apps/$k
	done
done
