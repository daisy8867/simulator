#include <bits/stdc++.h>
using namespace std;

class App {
public:
	int io_rate;
	double gpu_time;
	double seq_time;
	double splitsize;
	int num_task;
	App(int io, double gpu, double seq, double ss, int num);
};
