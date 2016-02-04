#include "fifo.h"

void fifo(vector<App>& apps, Node& n, const vector<Model>& model) {
//    queue< boost::function< void() > > tasks_;
//    boost::thread_group threads_gpu;
//    boost::thread_group threads_cpu;
//	int gpu = atomic_load(&n.num_gpu);
//	int cpu = atomic_load(&n.num_cpu);
//	for(int i = 0; i < gpu; i++) {
//		threads_gpu.create_thread(boost::bind())
//	}
	vector<boost::thread> launchContainers;
	atomic<int> io_mutex = ATOMIC_VAR_INIT(0);
	int con_io = 0;
//	atomic_init(&io_mutex, 0);
	launchContainers.emplace_back(monitor, boost::ref(n), "io_fifo", boost::ref(io_mutex));
	while(!apps.empty()){
		while(atomic_load(&n.num_gpu) && apps[0].num_task) {
			string logfile = to_string(apps[0].num_task);
			launchContainers.emplace_back(launch_container, apps[0], boost::ref(n), GPU, logfile, model, boost::ref(io_mutex));
			apps[0].num_task--;
			apps[0].num_gpu++;
			atomic_fetch_sub(&n.num_gpu, 1);
			con_io = apps[0].io_rate;
			atomic_fetch_add(&n.io_bw, con_io);
			if(con_io > 7168) {
				atomic_fetch_add(&n.io_gpu, 1);
			}
		}
		while(atomic_load(&n.num_cpu) && apps[0].num_task) {
			string logfile = to_string(apps[0].num_task);
			launchContainers.emplace_back(launch_container, apps[0], boost::ref(n), CPU, logfile, model, boost::ref(io_mutex));
			con_io = 1048576000 / apps[0].seq_time;
			atomic_fetch_add(&n.io_bw, con_io);
			if(con_io > 7168) {
				atomic_fetch_add(&n.io_cpu, 1);
			}
			apps[0].num_task--;
			apps[0].num_cpu++;
			atomic_fetch_sub(&n.num_cpu, 1);
		}
		if(!apps[0].num_task) {
			apps.erase(apps.begin());
		}
	}
	for(auto& t : launchContainers) {
		t.join();
	}
}
