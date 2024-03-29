#pragma once
#include <vector>

// Partially Persistent Binary Search Tree
template <typename T>
class P_PBST {
private:
	std::vector<std::vector<T>> v;
public:
	P_PBST() {
		v.push_back(std::vector<T>());
	}

	int lastVersion() {
		return v.size() - 1;
	}

	bool searchLEQ(int ver, T key, T& ret) {
		for (auto it = v[ver].rbegin(); it != v[ver].rend(); it++) {
			if (*it == key || *it < key) {
				ret = *it;
				return true;
			}
		}
		return false;
	}
	bool searchGEQ(int ver, T key, T& ret) {
		for (auto x : v[ver]) {
			if (x == key || key < x) {
				ret = x;
				return true;
			}
		}
		return false;
	}
	void insert(T key) {
		v.push_back(v.back());
		for (auto x : v.back()) {
			if (x == key) {
				return;
			}
		}
		v.back().push_back(key);
		sort(v.back().begin(), v.back().end());
	}

	void remove(T key) {
		v.push_back(v.back());
		for (auto it = v.back().begin(); it != v.back().end(); it++) {
			if (*it == key) {
				v.back().erase(it);
				break;
			}
		}
	}
};