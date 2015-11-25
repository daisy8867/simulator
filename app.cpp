#include "app.h"
App::App() {}
App::App(int io, double gpu, double seq, double ss, int num) {
	io_rate = io;
	gpu_time = gpu;
	seq_time = seq;
	splitsize = ss;
	num_task = num;	
//	atomic_init(&num_task, num);
}

App::App(const App& a) {
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
}
KNN::KNN(int num) {
//	App::App(73, 3, 134, 1024, num);
	app_name = "knn";
	io_rate = 73;
	gpu_time = 3;
	seq_time = 134;
	splitsize = 1024;
	num_task = num;
}
