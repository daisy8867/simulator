#include "launchContainer.h"

void monitor(Node& n, const string& logfile, atomic<int>& io_mutex) {
	FILE *f = fopen(logfile.c_str(), "w");
	int cur_io = 0;
	timeval tm;
	while(atomic_load(&io_mutex) < 0 && !(cur_io = atomic_load(&n.io_rate))) {
       	boost::this_thread::sleep(boost::posix_time::milliseconds(sleep_interval));
	}
	while(atomic_load(&io_mutex) < 0 && (cur_io = atomic_load(&n.io_rate))) {
		gettimeofday(&tm, 0);
		fprintf(f, "time: %d%06d , io: %d.\n", tm.tv_sec, tm.tv_usec, cur_io);
       	boost::this_thread::sleep(boost::posix_time::milliseconds(sleep_interval));
	}
	fclose(f);
}
