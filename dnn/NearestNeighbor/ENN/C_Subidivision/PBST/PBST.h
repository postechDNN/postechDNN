#pragma once
#include <vector>
#include <algorithm>

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

	bool searchLEQ(const int ver, const T key, T& ret) {
		for (auto it = v[ver].rbegin(); it != v[ver].rend(); it++) {
			if (*it <= key) {
				ret = *it;
				return true;
			}
		}
		return false;
	}

	bool searchGEQ(const int ver, const T key, T& ret) {
		for (auto x : v[ver]) {
			if (x >= key) {
				ret = x;
				return true;
			}
		}
		return false;
	}

	void insert(const T key) {
		v.push_back(v.back());
		for (auto x : v.back()) {
			if (x == key) {
				return;
			}
		}
		v.back().push_back(key);
		sort(v.back().begin(), v.back().end());
	}

	void remove(const T key) {
		v.push_back(v.back());
		for (auto it = v.back().begin(); it != v.back().end(); it++) {
			if (*it == key) {
				v.back().erase(it);
				break;
			}
		}
	}
};