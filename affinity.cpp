#include "fifo.h"

void affinityScheduler(vector<App>& apps, Node& n, const vector<Model>& model) {
//    queue< boost::function< void() > > tasks_;
//    boost::thread_group threads_gpu;
//    boost::thread_group threads_cpu;
//	int gpu = atomic_load(&n.num_gpu);
//	int cpu = atomic_load(&n.num_cpu);
//	for(int i = 0; i < gpu; i++) {
//		threads_gpu.create_thread(boost::bind())
//	}
	vector<boost::thread> launchContainers;
	App a;
	int con_io;
	function<bool (const App&, const App&)> cmpSpeedup = [&](const App& a, const App& b) {
		return a.speedup > b.speedup;
	};
	function<bool (const App&, const App&)> cmpCPU = [&](const App& a, const App& b) {
		return a.seq_time * a.dps < b.seq_time * b.dps;
	};
	atomic<int> io_mutex = ATOMIC_VAR_INIT(0);
	launchContainers.emplace_back(monitor, boost::ref(n), "io_affinity", boost::ref(io_mutex));
	while(!apps.empty()){
		while(!apps.empty() && atomic_load(&n.num_gpu)) {
//			for(int i = 0; i < apps.size(); i++) {
//				cout << i << " " << apps[i].app_name << " " << apps[i].num_task << " " << apps[i].num_gpu << " " << apps[i].num_cpu << endl;
//			}
			sort(apps.begin(), apps.end(), cmpSpeedup);
			string logfile = to_string(apps[0].num_task);
			launchContainers.emplace_back(launch_container, apps[0], boost::ref(n), GPU, logfile, model, boost::ref(io_mutex));
			atomic_fetch_sub(&n.num_gpu, 1);
			con_io = apps[0].io_rate;
			atomic_fetch_add(&n.io_bw, con_io);
			if(con_io > 7168) {
				atomic_fetch_add(&n.io_gpu, 1);
			}
			apps[0].num_task--;
			apps[0].num_gpu++;
			if(apps[0].num_task == 0) {
				apps.erase(apps.begin());
			}
		}
		while(!apps.empty() && atomic_load(&n.num_cpu)) {
//			for(int i = 0; i < apps.size(); i++) {
//				cout << i << " " << apps[i].app_name << " " << apps[i].num_task << " " << apps[i].num_gpu << " " << apps[i].num_cpu << endl;
//			}
			int i = (apps.size() > 1) ? 1 : 0;
			cout << i << endl;
			sort(apps.begin() + i, apps.end(), cmpCPU);
			string logfile = to_string(apps[i].num_task);
			launchContainers.emplace_back(launch_container, apps[i], boost::ref(n), CPU, logfile, model, boost::ref(io_mutex));
			atomic_fetch_sub(&n.num_cpu, 1);
			con_io = 1048576000 / apps[i].seq_time;
			atomic_fetch_add(&n.io_bw, con_io);
			if(con_io > 7168) {
				atomic_fetch_add(&n.io_gpu, 1);
			}
			apps[i].num_task--;
			apps[i].num_cpu++;
			if((apps.begin() + i)->num_task == 0) {
				apps.erase(apps.begin() + i);
			}
		}
	}
	for(auto& t : launchContainers) {
		t.join();
	}
}
