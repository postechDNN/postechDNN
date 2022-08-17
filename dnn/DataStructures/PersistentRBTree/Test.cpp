#include <iostream>
#include <vector>
#include <random>
#include "RBtree.h"
//#include "Original.h"
using namespace std;

int main() {
	//cout << "Hello World! C++" << endl;
	
	vector<int> v,v_d;

	
	
	for (int i = 0; i < 1000; i++) { //1000000±îÁö insertion clear
		v.push_back(i);
		v_d.push_back(i);
	}
	std::random_shuffle(v.begin(),v.end());
	std::random_shuffle(v_d.begin(), v_d.end());
	cout << "v.size=" << v.size() << "v.size/2" << int(v.size() / 2) << endl;
	/*
	cout << "v is" << endl;
	for (int i = 0; i < v.size(); i++) {
		cout << v[i] << endl;
	}
	cout << "\nv_d is" << endl;
	for (int i = 0; i < v.size(); i++) {
		cout << v_d[i] << endl;
	}
	*/


	RBTree<int> mytree;
	
	//mytree.insert_bst(3);
	mytree.print_linked_list();
	if (mytree.get_root() == mytree.get_leaf()) {
		cout << "leaf!!" << mytree.get_leaf()->key << endl;
	}
	cout << "HERE!!" << mytree.get_root()->ins_interval << endl;
	cout << "HERE!!" << mytree.get_root()->del_interval << endl;
	cout << "HERE!!" << true << endl;
	cout << "HERE!!" << false <<"\n\n" << endl;
	
	 
	for (int i = 0; i < v.size(); i++) {
		cout << "This is " << i+1 << "th iteration!" << endl;
		//cout << "Root is " << mytree.get_root()->key << endl;
		cout << "Item to be inserted is : " << v[i] << endl;
		mytree.insert_bst(v[i]);
		//if(i+1>540)
		//	mytree.print_linked_list();
		cout << "Root is " << mytree.get_root()->key << endl;
		
		cout << "\n\nItem to be deleted is : " << v_d[i] << endl;
		mytree.delete_bst(v_d[i]);
		if(i+1>750)
			mytree.print_linked_list();
		
		cout << "=========================================" << endl;
		cout << "=========================================\n\n" << endl;
	}
	cout << "HERE!!!!!" << endl;

	/*
	for (int i = 0; i < v.size(); i++) {
		cout << "This is " << i << "th iteration!" << endl;
		cout << "Item to be delete is : " << v[i] << endl;
		mytree.delete_bst(v[i]);
		mytree.print_linked_list();
	}
	*/
	
	
	
	 



	return 0;
}