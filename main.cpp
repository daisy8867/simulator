#include "fifo.h"
#include "node.h"
#include "app.h"
#include "model.h"

int main() {
	ifstream af;
	int n, m;
	string sLine = "";
	af.open("node");
	while(getline(af, sLine)) {
		istringstream in(sLine);
		in >> m;
		in >> n;
		// can be a cluster consists of nodes with different capacities
	}
	af.close();
	Node n1(m, n);
	// get app from conf file
	vector<App> apps;
	function<bool (const App&, const App&)> cmpApp = [&](const App& a, const App& b) {
		if(a.num_gpu != b.num_gpu) return a.num_gpu < b.num_gpu;
		else if (a.num_cpu != b.num_cpu) return a.num_cpu < b.num_cpu;
		else if (a.num_task != b.num_task) return a.num_task < b.num_task;
		else if (a.io_rate != b.io_rate) return a.io_rate > b.io_rate;
		else if (a.seq_time != b.seq_time) return a.seq_time > b.seq_time;
	};
	set<App, decltype(cmpApp)> appset(cmpApp);
	string app_name;
	string appid;
//	timeval tm;
	af.open("app");
	while(getline(af, sLine)) {
		istringstream in(sLine);
		in >> app_name;
		in >> n;
//		gettimeofday(&tm, 0);
//		appid = app_name + to_string(tm.tv_sec) + to_string(tm.tv_usec);
		App a = App::createApp(app_name, n);
		apps.emplace_back(a);
		appset.insert(a);
		string cmd = "mkdir " + app_name;
		system(cmd.c_str());
	}
	af.close();
	cout << "apps to schedule" << endl;
	for(App t : apps) {
		cout << t.app_name << " " << t.num_task << " " << t.num_gpu << " " << t.num_cpu << endl;
	}
	for(App t : appset) {
		cout << t.app_name << " " << t.num_task << " " << t.num_gpu << " " << t.num_cpu << endl;
	}

	// get io model
	sLine = "model";
	vector<Model> model;
	getIOmodel(model, sLine);
	auto cmp = [&] (const Model& m1, const Model& m2) {
		if(m1.coRunner != m2.coRunner) {
			return m1.coRunner < m2.coRunner;
		}
		else {
			return (m1.AggregateIO < m2.AggregateIO);
		}
	};
	sort(model.begin(), model.end(), cmp);

	// schedule with fifo
//	fifo(apps, n1, model);
	fair(appset, n1, model);
	return 0;
}
