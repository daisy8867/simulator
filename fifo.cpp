#include "fifo.h"

void fifo(vector<App> apps, Node& n1) {
	vector<boost::thread> launchContainers;
	while(!apps.empty()){
		while(atomic_load(&n1.num_gpu) && apps[0].num_task) {
			string logfile = to_string(apps[0].num_task);
			launchContainers.emplace_back(launch_container, boost::ref(apps[0]), boost::ref(n1), GPU, logfile);
			apps[0].num_task--;
			atomic_fetch_sub(&n1.num_gpu, 1);
		}
		while(atomic_load(&n1.num_cpu) && apps[0].num_task) {
			string logfile = to_string(apps[0].num_task);
			launchContainers.emplace_back(launch_container, boost::ref(apps[0]), boost::ref(n1), CPU, logfile);
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
