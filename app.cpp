#include "app.h"
App::App() {
	app_name = "";
	num_gpu = 0;
	num_cpu = 0;
	margin_gpu = 0.0;
	delta_dps_ratio = 0;
}
App::App(int io, double gpu, double seq, double ss, int num) {
	app_name = "";
	io_rate = io;
	gpu_time = gpu;
	seq_time = seq;
	splitsize = ss;
	num_task = num;	
	num_gpu = 0;
	num_cpu = 0;
//	atomic_init(&num_task, num);
}

App::App(const App& a) {
	app_name = a.app_name;
	io_rate = a.io_rate;
	gpu_time = a.gpu_time;
	seq_time = a.seq_time;
	splitsize = a.splitsize;
	num_task = a.num_task;
	num_gpu = a.num_gpu;
	num_cpu = a.num_cpu;
	dps = a.dps;
	speedup = a.speedup;
	delta_dps_ratio = a.delta_dps_ratio;
	start_time = a.start_time;
	cur_end_time = a.cur_end_time;
//	atomic_init(&num_task, atomic_load(&a.num_task));
}

App App::createApp(string name, int num, int64 start) {
	if(name == "knn") {
		return KNN(num, start);
	}
	if(name == "kmeans") { // kmeans with 100 cluster
		return Kmeans(num, start);
	}
	if(name == "kmeans2") { // kmeans with 8192 cluster
		return Kmeans2(num, start);
	}
	if(name == "rs1") {
		return RS1(num, start);
	}
	if(name == "nb") {
		return NB(num, start);
	}
	if(name == "bp") {
		return BP(num, start);
	}
}
KNN::KNN(int num, int64 start) {
	app_name = "knn";
	io_rate = 74104;
	gpu_time = 2940;
	seq_time = 122155;
	splitsize = 1048576;
	num_task = num;
	dps = 66; 
	speedup = 1.4;
	start_time = start;
	cur_end_time = start_time;
}

Kmeans::Kmeans(int num, int64 start) {
	app_name = "kmeans";
	io_rate = 61212;
	gpu_time = 1570;
	seq_time = 206342;
	splitsize = 1048576;
	num_task = num;
	start_time = start;
	cur_end_time = start_time;
}

Kmeans2::Kmeans2(int num, int64 start) {
	app_name = "kmeans2";
	io_rate = 61212;
	gpu_time = 55880;
	seq_time = 4691818;
	splitsize = 1048576;
	num_task = num;
	dps = 24.26648;
	speedup = 16.1;
	start_time = start;
	cur_end_time = start_time;
}

RS1::RS1(int num, int64 start) {
	app_name = "rs1";
	io_rate = 0;
	gpu_time = 194598;
	seq_time = 7930809;
	splitsize = 0;
	num_task = num;
	dps = 11.04072;
	speedup = 12.5;
	delta_dps_ratio = 104857600 / gpu_time / dps;
	start_time = start;
	cur_end_time = start_time;
}

NB::NB(int num, int64 start) {
	app_name = "nb";
	io_rate = 55512;
	gpu_time = 16049;
	seq_time = 545599;
	splitsize = 1048576;
	num_task = num;
	dps = 50.40961;
	speedup = 4.25;
	start_time = start;
	cur_end_time = start_time;
}

BP::BP(int num, int64 start) {
	app_name = "bp";
	io_rate = 0;
	gpu_time = 23096;
	seq_time = 613282;
	splitsize = 0;
	num_task = num;
	dps = 94.08972;
	speedup = 10.8;
	delta_dps_ratio = 104857600 / gpu_time / dps;
	start_time = start;
	cur_end_time = start_time;
}
