#include "model.h"

Model::Model(int co, int Agg, double de) {
	coRunner = co;
	AggregateIO = Agg;
	delayFactor = de;
}

void getIOmodel(vector<Model>& model, string& file) {
	ifstream mf;
	string sLine = "";
	int coRunner;
	double delayFactor;
	double AggregateIO;
	mf.open(file.c_str());
	while(getline(mf, sLine)) {
		istringstream in(sLine);
		in >> coRunner;
		in >> AggregateIO;
		in >> delayFactor;
		Model m(coRunner, (int)(AggregateIO * 1024), delayFactor);
		model.emplace_back(m);
	}
	mf.close();
}
