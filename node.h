#ifndef NODE_H
#define NODE_H
#include <bits/stdc++.h>
using namespace std;

class Node {
public:
	atomic<int> num_gpu;
	atomic<int> num_cpu;
	atomic<int> io_rate;
	atomic<int> io_bw;
	atomic<int> io_gpu; // #gpu containers issue io
	atomic<int> io_cpu; // #gpu containers issue io
	Node(int gpu, int cpu);
};
#endif
