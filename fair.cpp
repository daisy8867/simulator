#include "fifo.h"

void fair(set<App, function<bool (const App&, const App&)> >& apps, Node& n1, const vector<Model>& model) {
//    queue< boost::function< void() > > tasks_;
//    boost::thread_group threads_gpu;
//    boost::thread_group threads_cpu;
//	int gpu = atomic_load(&n1.num_gpu);
//	int cpu = atomic_load(&n1.num_cpu);
//	for(int i = 0; i < gpu; i++) {
//		threads_gpu.create_thread(boost::bind())
//	}
	vector<boost::thread> launchContainers;
	App a;
	atomic<int> io_mutex = ATOMIC_VAR_INIT(0);
//	atomic_init(&io_mutex, 0);
	launchContainers.emplace_back(monitor, boost::ref(n1), "io_fair", boost::ref(io_mutex));
	while(!apps.empty()){
		while(atomic_load(&n1.num_gpu) && apps.begin()->num_task) {
//			cout << "befor scheduling on gpu" << endl;
//			for(App t : apps) {
//				cout << t.app_name << " " << t.num_task << " " << t.num_gpu << " " << t.num_cpu << endl;
//			}
			a = *apps.begin();
			apps.erase(apps.begin());
			string logfile = to_string(a.num_task);
			launchContainers.emplace_back(launch_container, a, boost::ref(n1), GPU, logfile, model, boost::ref(io_mutex));
			a.num_task--;
			a.num_gpu++;
			atomic_fetch_sub(&n1.num_gpu, 1);
			if(a.num_task) {
				apps.insert(a);
			}
//			cout << "after scheduling on gpu" << endl;
//			for(App t : apps) {
//				cout << t.app_name << " " << t.num_task << " " << t.num_gpu << " " << t.num_cpu << endl;
//			}
		}
		while(atomic_load(&n1.num_cpu) && apps.begin()->num_task) {
//			cout << "befor scheduling on cpu" << endl;
//			for(App t : apps) {
//				cout << t.app_name << " " << t.num_task << " " << t.num_gpu << " " << t.num_cpu << endl;
//			}
			a = *apps.begin();
			apps.erase(apps.begin());
			string logfile = to_string(a.num_task);
			launchContainers.emplace_back(launch_container, a, boost::ref(n1), CPU, logfile, model, boost::ref(io_mutex));
			a.num_task--;
			a.num_cpu++;
			atomic_fetch_sub(&n1.num_cpu, 1);
			if(a.num_task) {
				apps.insert(a);
			}
//			cout << "after scheduling on cpu" << endl;
//			for(App t : apps) {
//				cout << t.app_name << " " << t.num_task << " " << t.num_gpu << " " << t.num_cpu << endl;
//			}
		}
	}
	for(auto& t : launchContainers) {
		t.join();
	}
}
