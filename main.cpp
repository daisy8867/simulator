#include "node.h"
#include "app.h"
#include <boost/thread.hpp>
#include <boost/ref.hpp>
const int io_cap = 80;
const int GPU = 0;
const int CPU = 1;
void launch_container(App& a, Node& n, int res) {
	int i = 0;
	if(res) {
		for(i = 0; i < a.seq_time; i++);
		cout << "cpu time: " << i << " secs." << endl;
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
			if(atomic_load(&n.io_bw) < io_cap) {
				get_data += con_io;
			}
			else {
				get_data += io_cap / atomic_load(&n.io_gpu);
			}
		}
		cout << "gpu reading time: " << i << " secs." << endl;
		if(con_io > 10) {
			atomic_fetch_sub(&n.io_gpu, 1);
		}
		for(i = 0; i < a.gpu_time; i++);
		cout << "gpu time: " << i << " secs." << endl;
	}
}

void fifo(vector<App> apps, Node& n1) {
	vector<boost::thread> launchContainers;
	while(!apps.empty()){
		while(atomic_load(&n1.num_gpu)) {
			launchContainers.emplace_back(launch_container, boost::ref(apps[0]), boost::ref(n1), GPU);
		}
		while(atomic_load(&n1.num_cpu)) {
			launchContainers.emplace_back(launch_container, boost::ref(apps[0]), boost::ref(n1), CPU);
		}
	}
}

int main() {
	Node n1(2, 4, 80);
	App a1(73, 3, 134, 1024, 10);
	vector<App> apps;
	apps.emplace_back(a1);
	fifo(apps, n1);
	return 0;
}
