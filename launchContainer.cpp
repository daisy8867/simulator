#include "launchContainer.h"

void launch_container(App& a, Node& n, int res, string& logfile, const vector<Model>& model) {
	string file = a.app_name + "/" + logfile;
	cout << "log to " << file << endl;
	FILE *f = fopen(file.c_str(), "w");
	timeval tm;
	gettimeofday(&tm, 0);
	fprintf(f, "start time: %d%06d usec.\n", tm.tv_sec, tm.tv_usec);
	int i = 0, con_io;
	auto cmp = [&] (const Model& m1, const Model& m2) {
		if(m1.coRunner != m2.coRunner) {
			return m1.coRunner < m2.coRunner;
		}
		else {
			return m1.AggregateIO < m2.AggregateIO;
		}
	};
	if(res) {//cpu container
		con_io = a.splitsize * 1000 / a.seq_time;
		atomic_fetch_add(&n.io_bw, con_io);
		if(con_io > 7168) {
			atomic_fetch_add(&n.io_gpu, 1);
		}
		for(i = 0; i < a.seq_time; i += 1000) {
        	boost::this_thread::sleep(boost::posix_time::milliseconds(sleep_interval));
//			gettimeofday(&tm, 0);
//			fprintf(f, "cpu time: %d secs %d msec.\n", tm.tv_sec, tm.tv_usec);
		}		
		atomic_fetch_sub(&n.io_bw, con_io);
		if(con_io > 7168) {
			atomic_fetch_sub(&n.io_gpu, 1);
		}
		gettimeofday(&tm, 0);
		fprintf(f, "cpu end time: %d%06d usec.\n", tm.tv_sec, tm.tv_usec);
		atomic_fetch_add(&n.num_cpu, 1);
	}
	else {
		con_io = a.io_rate;
		atomic_fetch_add(&n.io_bw, con_io);
		if(con_io > 7168) {
			atomic_fetch_add(&n.io_gpu, 1);
		}
		for(int get_data = 0; get_data < a.splitsize; i++) {
			// sleep and wait for other threads issue io to simulate io contension, 
			// sleep statements in other place is to sync time interval with this.
        	boost::this_thread::sleep(boost::posix_time::milliseconds(sleep_interval * 0.95));
			int cur_io = atomic_load(&n.io_bw);
			Model state(atomic_load(&n.io_gpu) - 1, cur_io, 0);
			auto lb = lower_bound(model.begin(), model.end(), state, cmp);
//			fprintf(f, "current io: %d, delay factor: %lf\n", cur_io, lb->delayFactor);
//			fprintf(f, "currnet state: %d %d\n", state.coRunner, state.AggregateIO);
			get_data += (int)((double)con_io / lb->delayFactor);
//			gettimeofday(&tm, 0);
//			fprintf(f, "gpu reading time: %d secs %d msec. current aggregate io: %d, current io_gpu: %d \n", tm.tv_sec, tm.tv_usec, atomic_load(&n.io_bw), atomic_load(&n.io_gpu));
		}
		atomic_fetch_sub(&n.io_bw, con_io);
		gettimeofday(&tm, 0);
		fprintf(f, "gpu reading end time: %d%06d usec.\n", tm.tv_sec, tm.tv_usec);
		if(con_io > 7168) {
			atomic_fetch_sub(&n.io_gpu, 1);
		}
		for(i = 0; i < a.gpu_time; i += 100) {
        	boost::this_thread::sleep(boost::posix_time::milliseconds(sleep_interval * 0.1));
//			gettimeofday(&tm, 0);
//			fprintf(f, "gpu kernel time: %d secs %d msec.\n", tm.tv_sec, tm.tv_usec);
		}
		gettimeofday(&tm, 0);
		fprintf(f, "gpu kernel end time: %d%06d usec.\n", tm.tv_sec, tm.tv_usec);
		atomic_fetch_add(&n.num_gpu, 1);
	}
	gettimeofday(&tm, 0);
	fprintf(f, "end time: %d%06d usec.\n", tm.tv_sec, tm.tv_usec);
	fclose(f);
}
