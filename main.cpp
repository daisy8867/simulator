#include "fifo.h"
#include "node.h"
#include "app.h"

int main() {
	Node n1(1, 4, 80);
	vector<App> apps;
	ifstream af;
	string sLine = "";
	string app_name;
	int n;
	af.open("app");
	while(getline(af, sLine)) {
		istringstream in(sLine);
		in >> app_name;
		in >> n;
		App a = App::createApp(app_name, n);
		apps.emplace_back(a);
	}
	af.close();
	fifo(apps, n1);
	return 0;
}
