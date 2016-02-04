#include "fifo.h"

void fair(set<App, function<bool (const App&, const App&)> >& apps, Node& n, const vector<Model>& model) {
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
	int con_io = 0;
	atomic<int> io_mutex = ATOMIC_VAR_INIT(0);
//	atomic_init(&io_mutex, 0);
	launchContainers.emplace_back(monitor, boost::ref(n), "io_fair", boost::ref(io_mutex));
	while(!apps.empty()){
		while(atomic_load(&n.num_gpu) && !apps.empty()) {
			cout << "befor scheduling on gpu" << endl;
			for(App t : apps) {
				cout << t.app_name << " " << t.num_task << " " << t.num_gpu << " " << t.num_cpu << endl;
			}
			a = *apps.begin();
			apps.erase(apps.begin());
			string logfile = to_string(a.num_task);
			launchContainers.emplace_back(launch_container, a, boost::ref(n), GPU, logfile, model, boost::ref(io_mutex));
			atomic_fetch_sub(&n.num_gpu, 1);
			con_io = a.io_rate;
			atomic_fetch_add(&n.io_bw, con_io);
			if(con_io > 7168) {
				atomic_fetch_add(&n.io_gpu, 1);
			}
			a.num_task--;
			a.num_gpu++;
			atomic_fetch_sub(&n.num_gpu, 1);
			if(a.num_task) {
				apps.insert(a);
			}
			cout << "after scheduling on gpu" << endl;
			for(App t : apps) {
				cout << t.app_name << " " << t.num_task << " " << t.num_gpu << " " << t.num_cpu << endl;
			}
		}
		while(atomic_load(&n.num_cpu) && !apps.empty()) {
			cout << "befor scheduling on cpu" << endl;
			for(App t : apps) {
				cout << t.app_name << " " << t.num_task << " " << t.num_gpu << " " << t.num_cpu << endl;
			}
			a = *apps.begin();
			apps.erase(apps.begin());
			string logfile = to_string(a.num_task);
			launchContainers.emplace_back(launch_container, a, boost::ref(n), CPU, logfile, model, boost::ref(io_mutex));
			atomic_fetch_sub(&n.num_cpu, 1);
			con_io = 1048576000 / a.seq_time;
			atomic_fetch_add(&n.io_bw, con_io);
			if(con_io > 7168) {
				atomic_fetch_add(&n.io_cpu, 1);
			}
			a.num_task--;
			a.num_cpu++;
			atomic_fetch_sub(&n.num_cpu, 1);
			if(a.num_task) {
				apps.insert(a);
			}
			cout << "after scheduling on cpu" << endl;
			for(App t : apps) {
				cout << t.app_name << " " << t.num_task << " " << t.num_gpu << " " << t.num_cpu << endl;
			}
		}
	}
	cout << "finish scheduling" << endl;
	for(auto& t : launchContainers) {
		t.join();
	}
}
