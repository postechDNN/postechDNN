#include <iostream>
#include <vector>
#include <random>
#include "RBtree.h"
#include "simple_persistent.h"
//#include "Original.h"
using namespace std;


int main()
{
	std::random_device rd;
	std::mt19937 g(rd());
	RBTree<int> tree;
	int size = 1000000;
	int intv = 10000;
	vector<int> vec;
	vector<int> evec;
	for (size_t i = 0; i < size; i++)
		vec.push_back(i);
	for (size_t i = 0; i < size / 2; i++)
		evec.push_back(i * 2 + 1);
	shuffle(vec.begin(), vec.end(), g);
	shuffle(evec.begin(), evec.end(), g);
	for (int i : vec)
		tree.insert(i);
	for (size_t i = 0; i < size / intv; i++)
		cout << tree.getkthNode(i * intv)->data << endl;
	int count = 0;
	for (int i : evec)
	{
		tree.deleteNode(i);
		count++;
	}
	for (size_t i = 0; i < size / (2 * intv); i++)
		cout << tree.getkthNode(i * intv)->data << endl;
	return 0;
}