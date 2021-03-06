//
//  main.cpp
//  cpp_experiment
//
//  Created by Xerol Wong on 5/22/17.
//  Copyright © 2017 iXerol. All rights reserved.
//

#include <fstream>
#include "interface.h"
using namespace std;

int main(int argc, const char * argv[]) {
	ifstream indata("data.txt");
	string a, b, c, d, e, f;
	double g;
	int v;
	while (indata >> a >> b >> c >> d >> e >> f >> g){
		Account* t = new Account(a, b, c, d, e, f, g);
		accounts.push_back(*t);
		delete t;
	}
	do {
		v = interface0();
		switch (v) {
			case 1:
				interface1();
				break;
			case 2:
				interface2();
				break;
			case 3:
				interface3();
				break;
			case 4:
				interface4();
				break;
			case 5:
				interface5();
				break;
			case 6:
				interface6();
				break;
			case -1:
				break;
			default:
				interface7();
				break;
			}
	}
	while (v != -1);
	ofstream outdata("data.txt");
	for (auto &i : accounts)
		outdata << i.na() << ' ' << i.id() << ' ' << i.adr() << ' ' << i.ph() << ' ' << i.com() << ' ' << i.account() << ' ' << fixed << setprecision(2) << i.money() << endl;
	return 0;
}
