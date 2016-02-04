#include "fifo.h"

void dynamicScheduler2(vector<App>& apps, Node& n, const vector<Model>& model) {
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
	int ma = 0, delta_io = 0, con_io = 0; 
	double max_delta_dps_ratio = 0.0, delta_dps_ratio = 0.0;
	auto cmpGPU = [&] (const Model& m1, const Model& m2) {
		if(m1.coRunner != m2.coRunner) {
			return m1.coRunner < m2.coRunner;
		}
		else {
			return m1.AggregateIO < m2.AggregateIO;
		}
	};
	atomic<int> io_mutex = ATOMIC_VAR_INIT(0);
	launchContainers.emplace_back(monitor, boost::ref(n), "io_dynamic", boost::ref(io_mutex));
	while(!apps.empty()){
		while(atomic_load(&n.num_gpu)) {
			ma = delta_io = 0; 
			max_delta_dps_ratio = delta_dps_ratio = 0.0;
			for(int i = 0; i < apps.size(); i++) {
				if(apps[i].io_rate) {
					int cur_io = atomic_load(&n.io_bw) + apps[i].io_rate;
					Model state(atomic_load(&n.io_gpu) + atomic_load(&n.io_cpu), cur_io, 0);
					auto lb = lower_bound(model.begin(), model.end(), state, cmp);
					double delay = min((double)(state.coRunner + 1), lb->delayFactor);
					delta_io = (int)((double)apps[i].io_rate / delay);
					double io_time = (double)apps[i].splitsize * 1000 / delta_io;
//					cout << "accounting " << apps[i].app_name << " current coRunner: " << state.coRunner << " delay factor is " << delay << " delta_io is " << delta_io << " io_time is: " << io_time << endl;
					delta_dps_ratio = ((double)apps[i].splitsize * 1000 / io_rate + apps[i].gpu_time) / (io_time + apps[i].gpu_time);
				else {
					delta_dps_ratio = 1;
				}
//				cout << "scheduling for gpu delta_dps_ratio of " << apps[i].app_name << " is " << delta_dps_ratio << endl;
				if(delta_dps_ratio > max_delta_dps_ratio) {
					ma = i;
					max_delta_dps_ratio = delta_dps_ratio;
				}
			}
			string logfile = to_string(apps[ma].num_task);
			launchContainers.emplace_back(launch_container, apps[ma], boost::ref(n), GPU, logfile, model, boost::ref(io_mutex));
			atomic_fetch_sub(&n.num_gpu, 1);
			con_io = apps[ma].io_rate;
			atomic_fetch_add(&n.io_bw, con_io);
			if(con_io > 7168) {
				atomic_fetch_add(&n.io_gpu, 1);
			}
			apps[ma].num_task--;
			apps[ma].num_gpu++;
			if(apps[ma].num_task == 0) {
				apps.erase(apps.begin() + ma);
			}
		}
		while(atomic_load(&n.num_cpu)) {
			ma = delta_io = 0; 
			max_delta_dps_ratio = delta_dps_ratio = 0.0;
			for(int i = 0; i < apps.size(); i++) {
				delta_dps_ratio = 1048576 / (apps[i].seq_time) / apps[i].dps;
//				cout << "scheduling for cpu delta_dps_ratio of " << apps[i].app_name << " is " << delta_dps_ratio << endl;
				if(delta_dps_ratio > max_delta_dps_ratio) {
					ma = i;
					max_delta_dps_ratio = delta_dps_ratio;
				}
			}
			string logfile = to_string(apps[ma].num_task);
			launchContainers.emplace_back(launch_container, apps[ma], boost::ref(n), CPU, logfile, model, boost::ref(io_mutex));
			atomic_fetch_sub(&n.num_cpu, 1);
			con_io = apps[ma].splitsize * 1000 / apps[ma].seq_time;
			atomic_fetch_add(&n.io_bw, con_io);
			if(con_io > 7168) {
				atomic_fetch_add(&n.io_cpu, 1);
			}
			apps[ma].num_task--;
			apps[ma].num_cpu++;
			if(apps[ma].num_task == 0) {
				apps.erase(apps.begin() + ma);
			}
		}
	}
	for(auto& t : launchContainers) {
		t.join();
	}
}
