#include "launchContainer.h"

void monitor(Node& n, const string& logfile, atomic<int>& io_mutex) {
	FILE *f = fopen(logfile.c_str(), "w");
	int cur_io = 0;
	timeval tm;
	gettimeofday(&tm, 0);
	fprintf(f, "time: %d%06d , io: %d.\n", tm.tv_sec, tm.tv_usec, cur_io);
	while(atomic_load(&io_mutex) < 0 && !(cur_io = atomic_load(&n.io_rate))) {
//		cout << "there are " << atomic_load(&io_mutex) << "cur threads, cur io is: " << cur_io << endl;
       	boost::this_thread::sleep(boost::posix_time::milliseconds(sleep_interval));
	}
	while(atomic_load(&io_mutex) < 0 && (cur_io = atomic_load(&n.io_rate))) {
//		cout << "there are " << atomic_load(&io_mutex) << "cur threads, cur io is: " << cur_io << endl;
		gettimeofday(&tm, 0);
		fprintf(f, "time: %d%06d , io: %d.\n", tm.tv_sec, tm.tv_usec, cur_io);
       	boost::this_thread::sleep(boost::posix_time::milliseconds(sleep_interval));
	}
	fclose(f);
}
