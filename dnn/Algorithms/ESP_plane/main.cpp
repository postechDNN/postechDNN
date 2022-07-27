#include "CS.h"
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

int main() {
	std::vector<i_quad*> vec;
	i_quad* q1 = new i_quad(3, 16.01, -8.01); vec.push_back(q1);
	i_quad* q2 = new i_quad(3, 7.9, 0.1); vec.push_back(q2);
	auto ret = myUnion(vec);
	/*
	auto ret = union_core(vec);
	for (auto elem : get<0>(ret)) {elem->print();} cout << "\n";
	auto ret2 = union_normal(vec);
	for (auto elem : get<1>(ret2)) { elem->print(); }
	// for (auto elem : get<1>(ret)) { elem->print(); }

	
	auto ret = union_normal(vec, true);
	for (auto elem: ret) elem->print(); cout << "\n";
	auto ret2 = union_normal(vec, false);
	for (auto elem : ret2) elem->print(); cout << "\n";
	*/
	// ret = union_normal(vec);
	// for (auto elem : ret) elem->print();
	// auto ret2 = myUnion(ret, 3);
	// ret2->print();
	return 0;
}

