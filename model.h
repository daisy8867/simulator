#ifndef MODEL_H
#define MODEL_H
#include <bits/stdc++.h>
using namespace std;

class Model {
	public:
	int coRunner;
	int AggregateIO;
	double delayFactor;
	Model(int co, int Agg, double de);
};
void getIOmodel(vector<Model>& model, string& file);
#endif
