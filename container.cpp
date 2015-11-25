#include <bits/stdc++.h>
using namespace std;

class Container {
	int resource; // 0 indicate gpu, 1 indicate cpu;
	Container(int res) {
		resource = res;
		if(resource) {
			Node.num_cpu--;

		}
	}
};
