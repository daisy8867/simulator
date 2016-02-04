#include <bits/stdc++.h>
#include <string>
using namespace std;

int main() {
	ifstream af;
	string sLine = "";
	string app_name;
	string log_path;
	int n;
	af.open("app");
	while(getline(af, sLine)) {
		istringstream in(sLine);
		in >> app_name;
		in >> n;
		in >> log_path;
		string cmd = "mkdir " + log_path;
		system(cmd.c_str());
		log_path += "/" + to_string(2);
		FILE *f = fopen(log_path.c_str(), "w");
		fprintf(f, "test");
		fclose(f);
	}
	af.close();
	return 0;
}
