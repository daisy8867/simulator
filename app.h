#ifndef APP_H
#define APP_H
#include <bits/stdc++.h>
using namespace std;

class App {
public:
	string app_name;
//	string appid; 
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
//	static App createApp(string name, int num, string appid);
};
class KNN : public App {
public:
	KNN(int num);
};
class Kmeans : public App {
public:
	Kmeans(int num);
};
class RS1 : public App {
public:
	RS1(int num);
};
class NB : public App {
public:
	NB(int num);
};
class BP : public App {
public:
	BP(int num);
};
#endif
