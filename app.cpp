#include "app.h"
App::App() {
	app_name = "";
}
App::App(int io, double gpu, double seq, double ss, int num) {
	app_name = "";
	io_rate = io;
	gpu_time = gpu;
	seq_time = seq;
	splitsize = ss;
	num_task = num;	
//	atomic_init(&num_task, num);
}

App::App(const App& a) {
	app_name = a.app_name;
	io_rate = a.io_rate;
	gpu_time = a.gpu_time;
	seq_time = a.seq_time;
	splitsize = a.splitsize;
	num_task = a.num_task;
//	atomic_init(&num_task, atomic_load(&a.num_task));
}

App App::createApp(string name, int num) {
	if(name == "knn") {
		return KNN(num);
	}
	if(name == "kmeans") {
		return Kmeans(num);
	}
	if(name == "rs1") {
		return RS1(num);
	}
	if(name == "nb") {
		return NB(num);
	}
	if(name == "bp") {
		return BP(num);
	}
}
KNN::KNN(int num) {
	app_name = "knn";
	io_rate = 74104;
	gpu_time = 2940;
	seq_time = 122155;
	splitsize = 1048576;
	num_task = num;
}

Kmeans::Kmeans(int num) {
	app_name = "kmeans";
	io_rate = 61213;
	gpu_time = 56328;
	seq_time = 4936209;
	splitsize = 1048576;
	num_task = num;
}

RS1::RS1(int num) {
	app_name = "rs1";
	io_rate = 1048576;
	gpu_time = 194598;
	seq_time = 7930809;
	splitsize = 1048576;
	num_task = num;
}

NB::NB(int num) {
	app_name = "nb";
	io_rate = 55512;
	gpu_time = 16149;
	seq_time = 545599;
	splitsize = 1048576;
	num_task = num;
}

BP::BP(int num) {
	app_name = "bp";
	io_rate = 1048576;
	gpu_time = 23596;
	seq_time = 613282;
	splitsize = 1048576;
	num_task = num;
}
