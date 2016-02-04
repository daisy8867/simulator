#ifndef APP_H
#define APP_H
#include <bits/stdc++.h>
using namespace std;

typedef long long int64;

class App {
public:
	string app_name;
//	string appid; 
	int io_rate;
	double gpu_time;
	double seq_time;
	double splitsize;
	int num_task;
	int num_gpu;
	int num_cpu;
	double dps; // expected dps when it solo in the cluster, MB/s
	int64 start_time;
	int64 cur_end_time;
	double margin_gpu;
	double speedup;
	int delta_dps_ratio;
//	atomic<int> num_task;
	App(int io, double gpu, double seq, double ss, int num);
	App(const App& a);
	App();
	static App createApp(string name, int num, int64 starttime);
//	static App createApp(string name, int num, string appid);
};
class KNN : public App {
public:
	KNN(int num, int64 start);
};
class Kmeans : public App {
public:
	Kmeans(int num, int64 start);
};
class Kmeans2 : public App {
public:
	Kmeans2(int num, int64 start);
};
class RS1 : public App {
public:
	RS1(int num, int64 start);
};
class NB : public App {
public:
	NB(int num, int64 start);
};
class BP : public App {
public:
	BP(int num, int64 start);
};
#endif
