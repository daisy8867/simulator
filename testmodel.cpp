#include "model.h"

int main() {
	string sLine = "";
	sLine = "model";
	vector<Model> model;
	getIOmodel(model, sLine);

	Model state(5, 0, (double)182436/74052);
	auto cmp = [&] (const Model& m1, const Model& m2) {
		if(m1.coRunner != m2.coRunner) {
			return m1.coRunner < m2.coRunner;
		}
		else {
			return (m1.AggregateIOFactor - m2.AggregateIOFactor) < 10e-6;
		}
	};
	sort(model.begin(), model.end(), cmp);
	auto lb = lower_bound(model.begin(), model.end(), state, cmp);
	auto ub = upper_bound(model.begin(), model.end(), state, cmp);
//	printf("%lf\n", lb->delayFactor);
	printf("%d\n", (int)((double)74105/5.71));
	cout << state.coRunner << " " << state.delayFactor << " " << state.AggregateIOFactor << endl;
	cout << lb->coRunner << " " << lb->delayFactor << " " << lb->AggregateIOFactor << endl;
	cout << ub->coRunner << " " << ub->delayFactor << " " << ub->AggregateIOFactor << endl;
	return 0;
}
