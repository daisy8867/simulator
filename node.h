#include <bits/stdc++.h>
using namespace std;

class Node {
public:
	atomic<int> num_gpu;
	atomic<int> num_cpu;
	atomic<int> io_bw;
	atomic<int> io_gpu; // #gpu containers issue io
	Node(int gpu, int cpu, int io);
};
