#include "launchContainer.h"

void launch_container(App& a, Node& n, int res, string& logfile) {
	FILE *f = fopen(logfile.c_str(), "w");
	timeval tm;
	gettimeofday(&tm, 0);
	fprintf(f, "start time: %d secs %d msec.\n", tm.tv_sec, tm.tv_usec);
	int i = 0;
	if(res) {
		for(i = 0; i < a.seq_time; i++) {
        	boost::this_thread::sleep(boost::posix_time::milliseconds(sleep_interval));
//			gettimeofday(&tm, 0);
//			fprintf(f, "cpu time: %d secs %d msec.\n", tm.tv_sec, tm.tv_usec);
		}		
		gettimeofday(&tm, 0);
		fprintf(f, "cpu end time: %d secs %d msec.\n", tm.tv_sec, tm.tv_usec);
		atomic_fetch_add(&n.num_cpu, 1);
	}
	else {
		int con_io = a.io_rate;
		if(atomic_load(&n.io_bw) + con_io < io_cap) {
			atomic_fetch_add(&n.io_bw, con_io);
		}
		else {
			atomic_store(&n.io_bw, io_cap);
		}
		if(con_io > 10) {
			atomic_fetch_add(&n.io_gpu, 1);
		}
		for(int get_data = 0; get_data < a.splitsize; i++) {
			// sleep and wait for other threads issue io to simulate io contension, 
			// sleep statements in other place is to sync time interval with this.
        	boost::this_thread::sleep(boost::posix_time::milliseconds(sleep_interval));
			if(atomic_load(&n.io_bw) < io_cap) {
				get_data += con_io;
			}
			else {
				get_data += io_cap / atomic_load(&n.io_gpu);
			}
//			gettimeofday(&tm, 0);
//			fprintf(f, "gpu reading time: %d secs %d msec. current aggregate io: %d, current io_gpu: %d \n", tm.tv_sec, tm.tv_usec, atomic_load(&n.io_bw), atomic_load(&n.io_gpu));
		}
		gettimeofday(&tm, 0);
		fprintf(f, "gpu reading end time: %d secs %d msec.\n", tm.tv_sec, tm.tv_usec);
		if(con_io > 10) {
			atomic_fetch_sub(&n.io_gpu, 1);
		}
		for(i = 0; i < a.gpu_time; i++) {
        	boost::this_thread::sleep(boost::posix_time::milliseconds(sleep_interval));
//			gettimeofday(&tm, 0);
//			fprintf(f, "gpu kernel time: %d secs %d msec.\n", tm.tv_sec, tm.tv_usec);
		}
		gettimeofday(&tm, 0);
		fprintf(f, "gpu kernel end time: %d secs %d msec.\n", tm.tv_sec, tm.tv_usec);
		atomic_fetch_add(&n.num_gpu, 1);
	}
	gettimeofday(&tm, 0);
	fprintf(f, "end time: %d secs %d msec.\n", tm.tv_sec, tm.tv_usec);
	fclose(f);
}
