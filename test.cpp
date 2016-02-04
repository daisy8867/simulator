#include <bits/stdc++.h>
using namespace std;

auto cmp = [&](const int& a, const int&b) {
	return a < b;
};
int main() {
	vector<int> a = {1};
	sort(a.begin(), a.end(), cmp);
}
