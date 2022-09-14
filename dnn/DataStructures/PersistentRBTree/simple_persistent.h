#pragma once

#include"RBTree.h"
#include<vector>
#include<array>
#include <algorithm>
#include <cstdlib>

using namespace std;



template <typename T>
class VersionTree;


template <typename T>
struct Version
{
	int parent; //index of the parent version
	RBTree<T>* tree;  //Binary search tree of this version
	Version(int p)
	{
		parent = p;
		tree = nullptr;
	}
	friend class VersionTree<T>;
};



template <typename T>
class VersionTree
{
public:
	/*
	Find the node equal to key
	*/
	Node<T>* search(int ver, T key) {
		return v[ver].tree->search(key);
	}
	/*
	Find the largest node lesser than key or equal
	*/
	Node<T>* searchLEQ(int ver, T key)
	{
		return v[ver].tree->searchLEQ(key);
	}

	/*
	Find the smallest node greater than key or equal
	*/
	Node<T>* searchGEQ(int ver, T key)
	{
		return v[ver].tree->searchGEQ(key);
	}
	void insertNode(int ver, T key) {
		v.emplace_back();
		v.back().parent = ver;
		if (ver == -1)
			v.back().tree = new RBTree<T>();
		else
			v.back().tree = new RBTree<T>(v[ver].tree);
		v.back().tree->insert(key);

	}
	void deleteNode(int ver, T key) {
		v.emplace_back();
		v.back().parent = ver;
		if (ver == -1)
			v.back().tree = new RBTree<T>();
		else
			v.back().tree = new RBTree<T>(v[ver].tree);
		v.back().tree->deleteNode(key);
	}

private:
	vector<Version<T>> v;

};