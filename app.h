#ifndef APP_H
#define APP_H
#include <bits/stdc++.h>
using namespace std;

class App {
public:
	string app_name;
	int io_rate;
	double gpu_time;
	double seq_time;
	double splitsize;
	int num_task;
//	atomic<int> num_task;
	App(int io, double gpu, double seq, double ss, int num);
	App(const App& a);
	App();
	static App createApp(string name, int num);
};
class KNN : public App {
public:
	KNN(int num);
};
#endif
