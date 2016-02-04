awk ' 
BEGIN{getline; s=$1; comb=$2; a=$3; total=1;
p["knn",1]=0;p["knn",2]=0;p["kmeans2",1]=0;p["kmeans2",2]=0;p["rs1",1]=0;p["rs1",2]=0;p["nb",1]=0;p["nb",2]=0;p["bp",1]=0;p["bp",2]=0;
p[a,1]=$4; p[a,2]=$5;
#print total " " p["knn",1] " " p["knn",2] " " p["kmeans2",1] " " p["kmeans2",2] " " p["rs1",1] " " p["rs1",2] " " p["nb",1] " " p["nb",2] " " p["bp",1] " " p["bp",2] 
}
{
if($2!=comb){
#total=p["knn",1]+p["knn",2]+p["kmeans2",1]+p["kmeans2",2]+p["rs1",1]+p["rs1",2]+p["nb",1]+p["nb",2]+p["bp",1]+p["bp",2];
#print s " " comb " " p["knn",1]/total " " p["knn",2]/total " " p["kmeans2",1]/total " " p["kmeans2",2]/total " " p["rs1",1]/total " " p["rs1",2]/total " " p["nb",1]/total " " p["nb",2]/total " " p["bp",1]/total " " p["bp",2]/total;
printf("%s %d", s, comb);
if(p["knn",1]+p["knn",2]) {printf(" %lf %lf", p["knn",1]/total/(p["knn",1]+p["knn",2]), p["knn",2]/total/(p["knn",1]+p["knn",2]));}
else {printf" 0.0 0.0";}
if(p["kmeans2",1]+p["kmeans2",2]) {printf(" %lf %lf", p["kmeans2",1]/total/(p["kmeans2",1]+p["kmeans2",2]), p["kmeans2",2]/total/(p["kmeans2",1]+p["kmeans2",2]));}
else {printf" 0.0 0.0";}
if(p["rs1",1]+p["rs1",2]) {printf(" %lf %lf", p["rs1",1]/total/(p["rs1",1]+p["rs1",2]), p["rs1",2]/total/(p["rs1",1]+p["rs1",2]));}
else {printf" 0.0 0.0";}
if(p["nb",1]+p["nb",2]) {printf(" %lf %lf", p["nb",1]/total/(p["nb",1]+p["nb",2]), p["nb",2]/total/(p["nb",1]+p["nb",2]));}
else {printf" 0.0 0.0";}
if(p["bp",1]+p["bp",2]) {printf(" %lf %lf", p["bp",1]/total/(p["bp",1]+p["bp",2]), p["bp",2]/total/(p["bp",1]+p["bp",2]));}
else {printf" 0.0 0.0";}
printf("\n");
p["knn",1]=0;p["knn",2]=0;p["kmeans2",1]=0;p["kmeans2",2]=0;p["rs1",1]=0;p["rs1",2]=0;p["nb",1]=0;p["nb",2]=0;p["bp",1]=0;p["bp",2]=0;
s=$1; comb=$2; a=$3; total=1;
p[a,1]=$4; p[a,2]=$5;
}
else {
a=$3;
p[a,1]=$4;
p[a,2]=$5;
total++;
}
#print total " " p["knn",1] " " p["knn",2] " " p["kmeans2",1] " " p["kmeans2",2] " " p["rs1",1] " " p["rs1",2] " " p["nb",1] " " p["nb",2] " " p["bp",1] " " p["bp",2] 
}
END{
#total=p["knn",1]+p["knn",2]+p["kmeans2",1]+p["kmeans2",2]+p["rs1",1]+p["rs1",2]+p["nb",1]+p["nb",2]+p["bp",1]+p["bp",2];
printf("%s %d", s, comb);
if(p["knn",1]+p["knn",2]) {printf(" %lf %lf", p["knn",1]/total/(p["knn",1]+p["knn",2]), p["knn",2]/total/(p["knn",1]+p["knn",2]));}
else {printf" 0.0 0.0";}
if(p["kmeans2",1]+p["kmeans2",2]) {printf(" %lf %lf", p["kmeans2",1]/total/(p["kmeans2",1]+p["kmeans2",2]), p["kmeans2",2]/total/(p["kmeans2",1]+p["kmeans2",2]));}
else {printf" 0.0 0.0";}
if(p["rs1",1]+p["rs1",2]) {printf(" %lf %lf", p["rs1",1]/total/(p["rs1",1]+p["rs1",2]), p["rs1",2]/total/(p["rs1",1]+p["rs1",2]));}
else {printf" 0.0 0.0";}
if(p["nb",1]+p["nb",2]) {printf(" %lf %lf", p["nb",1]/total/(p["nb",1]+p["nb",2]), p["nb",2]/total/(p["nb",1]+p["nb",2]));}
else {printf" 0.0 0.0";}
if(p["bp",1]+p["bp",2]) {printf(" %lf %lf", p["bp",1]/total/(p["bp",1]+p["bp",2]), p["bp",2]/total/(p["bp",1]+p["bp",2]));}
else {printf" 0.0 0.0";}
printf("\n");
}
' tmp | sort -nk2 | tee tmp1
awk 'BEGIN{getline; comb=$2; print;}
{
if($2!=comb) {print " "; comb=$2;}
print $0;
}
' tmp1 | tee tmp2
