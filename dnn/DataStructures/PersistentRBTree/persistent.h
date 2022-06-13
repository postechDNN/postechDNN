#pragma once
#include"Red Black Tree.h"
#include<vector>
#include<array>

using namespace std;

struct NodeP;

template <typename T>
class VersionTree;

struct NodeC
{
	NodeC* prev;
	NodeC* next;
	NodeP* parent;
	long long tag;
	int version;
	NodeC(int v)
	{
		prev = nullptr;
		next = nullptr;
		parent = nullptr;
		tag = 0;
		version = v;
	}
};

struct NodeP
{
	NodeP* prev;
	NodeP* next;
	NodeC* child; //head of the child list
	long long tag;
	int size;
	NodeP()
	{
		prev = nullptr;
		next = nullptr;
		child = nullptr;
		tag = 0;
		size = 1;
	}
	~NodeP()
	{
		NodeC* cur = child;
		while (cur)
		{
			NodeC* next = cur->next;
			delete cur;
			cur = next;
		}
	}
};

template <typename T>
struct Pnode {
	T key;
	int version;
	bool isChange;
	Color color;
	Pnode* parent;
	Pnode* right_child;
	Pnode* left_child;
	Pnode* pre;
	Pnode* next;
	Pnode* change;
};


template <typename T>
struct Version
{
	array<NodeC*, 2> adds{ nullptr,nullptr }; //addresss of second level for two lists
	int parent;
	Pnode<T>* root;
	Version(int p)
	{
		parent = p;
	}
	friend class VersionTree<T>;
};



template <typename T>
class VersionTree
{
public:
	~VersionTree();
	void newVersion(int pindex);
	bool testAncestor(int v1, int v2);
	Pnode<T>* search(int ver, T key);
	void replace(int ver, int pVer, Pnode<T>* n);
private:
	vector<Version<T>> v;
	array<NodeP*, 2> heads{nullptr,nullptr};
	bool testOrder(int list, int v1, int v2);
	array<NodeP*, 3> search1(int ver, T key);
};

template <typename T>
VersionTree<T>::~VersionTree()
{
	for (size_t i = 0; i < 2; i++)
	{
		NodeP* cur = heads[i];
		while (cur)
		{
			NodeP* next = cur->next;
			delete cur;
			cur = next;
		}
	}
}




