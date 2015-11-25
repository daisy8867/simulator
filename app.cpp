#include "app.h"

App::App(int io, double gpu, double seq, double ss, int num) {
	io_rate = io;
	gpu_time = gpu;
	seq_time = seq;
	splitsize = ss;
	num_task = num;
}
