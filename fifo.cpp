#include "fifo.h"

void fifo(vector<App>& apps, Node& n1, const vector<Model>& model) {
//    queue< boost::function< void() > > tasks_;
//    boost::thread_group threads_gpu;
//    boost::thread_group threads_cpu;
//	int gpu = atomic_load(&n1.num_gpu);
//	int cpu = atomic_load(&n1.num_cpu);
//	for(int i = 0; i < gpu; i++) {
//		threads_gpu.create_thread(boost::bind())
//	}
	vector<boost::thread> launchContainers;
	atomic<int> io_mutex = ATOMIC_VAR_INIT(0);
//	atomic_init(&io_mutex, 0);
	launchContainers.emplace_back(monitor, boost::ref(n1), "io_fifo", boost::ref(io_mutex));
	while(!apps.empty()){
		while(atomic_load(&n1.num_gpu) && apps[0].num_task) {
			string logfile = to_string(apps[0].num_task);
			launchContainers.emplace_back(launch_container, apps[0], boost::ref(n1), GPU, logfile, model, boost::ref(io_mutex));
			apps[0].num_task--;
			atomic_fetch_sub(&n1.num_gpu, 1);
		}
		while(atomic_load(&n1.num_cpu) && apps[0].num_task) {
			string logfile = to_string(apps[0].num_task);
			launchContainers.emplace_back(launch_container, apps[0], boost::ref(n1), CPU, logfile, model, boost::ref(io_mutex));
			apps[0].num_task--;
			atomic_fetch_sub(&n1.num_cpu, 1);
		}
		if(!apps[0].num_task) {
			apps.erase(apps.begin());
		}
	}
	for(auto& t : launchContainers) {
		t.join();
	}
}
