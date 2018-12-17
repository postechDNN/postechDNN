#include <iostream>
#include <vector>
using namespace std;

class DataT {
private:
	int point[3];
	vector<int> parents;
public:
	DataT(vector<int>& v) {
		for (int i = 0; i < 3; i++)
			point[i] = v[i];
	}
	void set_parent(int _parent) {
		parents.push_back(_parent);
	}
	vector<int>& get_parent() {
		return parents;
	}

};