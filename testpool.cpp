#include <bits/stdc++.h>
#include "threadpool.h"
using namespace std;
class test {
	public:
	void Print(int n);
};
void test::Print(int n) {
	cout << n << endl;
}
int main() {
	thread_pool pool(2);
	pool.run_task(boost::bind(&test::Print, 5));
}
