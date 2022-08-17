#include"persistent.h"
#include <climits>
#include<iostream>

int SIZE_LIMIT = 60;
double CONST_T = 1.5;

using namespace std;


/*
Create the version after the choosen parent version.
*/
template <typename T>
void VersionTree<T>::newVersion(int pindex)
{
	v.emplace_back(pindex);
	int index = v.size() - 1;
	if (-1 == pindex)
	{
		for (size_t i = 0; i < 2; i++)
		{
			NodeP* p = new NodeP();
			NodeC* c = new NodeC(index);
			p->child = c;
			c->parent = p;
			heads[i] = p;
			v[index].adds.at(i) = c;
		}
		return;
	}
	for (size_t i = 0; i < 2; i++)
	{
		NodeP* parent = v[pindex].adds.at(i)->parent;
		NodeC* c = new NodeC(index);
		v[index].adds.at(i)= c;
		parent->size++;
		//update preorder list(insert after the parent in VTree)
		if (0 == i)
		{
			NodeC* prev = v[pindex].adds.at(i);
			c->prev = prev;
			c->next = prev->next;
			prev->next = c;
			if (c->next)
				c->next->prev = c;
			long long ptag = prev->tag;
			long long ntag = c->next ? c->next->tag : LLONG_MAX;
			c->tag = (ptag + ntag) / 2;
		}
		//update postorder list(insert before the parent in VTree)
		else
		{
			NodeC* next = v[pindex].adds.at(i);
			c->next = next;
			c->prev = next->prev;
			next->prev = c;
			if (c->prev)
				c->prev->next = c;
			else
				c->parent->child = c;
			long long ptag = c->prev ? c->prev->tag : LLONG_MIN;
			long long ntag = next->tag;
			c->tag = (ptag + ntag) / 2;
		}
		//if second level is overflowing
		if (parent->size > SIZE_LIMIT)
		{
			NodeP* p = new NodeP();
			NodeP* prev = parent;
			p->prev = prev;
			p->next = prev->next;
			prev->next = p;
			if (p->next)
				p->next->prev = p;
			p->child = prev->child;
			for (size_t j = 0; j < SIZE_LIMIT/2; j++)
				p->child = p->child->next;
			p->child->prev->next = nullptr;
			p->child->prev = nullptr;
			p->size = prev->size - SIZE_LIMIT / 2;
			prev->size = SIZE_LIMIT / 2;
			array<NodeP*, 2> parents{ prev,p };
			for (size_t j = 0; j < 2; j++)
			{
				long long intv = LLONG_MAX / (parents[j]->size / 2 + 1);
				long long newTag = LLONG_MIN + intv;
				NodeC* cur = parents[j]->child;
				while (cur)
				{
					cur->tag = newTag;
					newTag += intv;
					cur = cur->next;
				}
			}
			long long ptag = prev->tag;
			long long ntag = p->next ? p->next->tag : LLONG_MAX;
			if ((ptag + ntag) / 2 != ptag)
				p->tag = (ptag + ntag) / 2;
			else //relable some tags in first level
			{
				bool separate = ntag % 2;
				NodeP* start = separate ? prev : p->next;
				NodeP* end = p->next ? p->next->next : p->next;
				long long rStart = separate ? ptag : ptag + 1;
				long long rEnd = separate ? ntag : ntag + 1;
				long long ex = 1;
				long long num = separate ? 1 : 2;
				while ((static_cast<long long>(1) << ex) < num * pow(CONST_T,ex))
				{
					long long temp = (static_cast<long long>(1) << ex);
					if (rStart % temp)
						rStart -= temp;
					else
						rEnd += temp;
					ex++;
					while (end && end->tag <= rEnd)
					{
						end = end->next;
						num++;
					}
					while (start && start->prev && start->prev->tag >= rStart)
					{
						start = start->prev;
						num++;
					}
				}
				NodeP* cur = start;
				long long intv = (rEnd - rStart) / num;
				long long newTag = rStart + intv;
				while (cur != end)
				{
					cur->tag = newTag;
					newTag += intv;
					cur = cur->next;
				}
				ptag = prev->tag;
				ntag = p->next ? p->next->tag : LLONG_MAX;
				p->tag = (ptag + ntag) / 2;
			}
		}
	}
	return;
}



/*
Test whether v1 is the ancestor of v2 in the version tree.
*/
template <typename T>
bool VersionTree<T>::testAncestor(int v1, int v2)
{
	return (testOrder(0, v1, v2) && !testOrder(1, v1, v2)) || v1 == v2; //need to debug for v1 == v2
}


/*
Test whether v1 occurs before v2 in first or second data structure.
*/
template <typename T>
bool VersionTree<T>::testOrder(int list, int v1, int v2)
{
	NodeC* add1 = v[v1].adds.at(list);
	NodeC* add2 = v[v2].adds.at(list);
	long long pVal1 = add1->parent->tag;
	long long pVal2 = add2->parent->tag;
	if (pVal1 == pVal2)
	{
		return add1->tag < add2->tag;
	}
	return pVal1 < pVal2;
}

template<typename T>
array<NodeP*, 3> VersionTree<T>::search1(int ver, T key)
{
	Pnode<T>* tmp = v[ver].root;
	Pnode<T>* displaced = nullptr;
	Pnode<T>* rDisplaced = nullptr;
	Pnode<T>* escape = nullptr;
	while (tmp != nullptr) {
		if (tmp->key == key)
		{
			if (displaced == nullptr)
				return { tmp, rDisplaced, escape };
			else
				return { tmp, rDisplaced, tmp };
		}
		else {
			if (displaced && tmp->parent && (tmp->parent->key > displaced->key) == (tmp->parent->key > key));
			else if (tmp->parent && tmp->parent->change)
			{
				displaced = ((tmp->parent->key > tmp->parent->change->key) == (tmp->parent->key > key)) ? tmp->parent->change : nullptr;
				if ((tmp->parent->key > tmp->parent->change->key) == (tmp->parent->key > key))
					rDisplaced = tmp->parent->change;
				else
					escape = tmp->parent;
			}
			else
			{
				displaced = nullptr;
				escape = nullptr;
			}
			if (tmp->change && (tmp->change->key == tmp->key) && testAncestor(tmp->change->version, ver) && tmp->change->version > tmp->version)
				tmp = tmp->change;
			if (displaced && (displaced->key == tmp->key) && testAncestor(displaced->version, ver) && displaced->version > tmp->version)
				tmp = displaced;
			tmp->key > key ? tmp = tmp->left_child : tmp = tmp->right_child;
		}
	}
	return { nullptr, nullptr, nullptr };
}

template <typename T>
Pnode<T>* VersionTree<T>::search(int ver, T key) {
	Pnode<T>* tmp = v[ver].root;
	Pnode<T>* displaced = nullptr;
	while (tmp != nullptr) {
		if (tmp->key == key)
			return tmp;
		else {
			if (displaced && tmp->parent && (tmp->parent->key > displaced->key) == (tmp->parent->key > key));
			else if (tmp->parent && tmp->parent->change)
				displaced = ((tmp->parent->key > tmp->parent->change->key) == (tmp->parent->key > key)) ? tmp->parent->change : nullptr;
			else
				displaced = nullptr;
			if (tmp->change && (tmp->change->key == tmp->key) && testAncestor(tmp->change->version, ver) && tmp->change->version > tmp->version)
				tmp = tmp->change;
			if (displaced && (displaced->key == tmp->key) && testAncestor(displaced->version, ver) && displaced->version > tmp->version)
				tmp = displaced;
			tmp->key > key ? tmp = tmp->left_child : tmp = tmp->right_child;
		}
	}
	return nullptr;
}


//Replace the node for the new version
template<typename T>
void VersionTree<T>::replace(int ver, int pVer, Pnode<T>* n)
{
	array<NodeP*, 3> arr = search1(pVer, n->key);
	Pnode<T>* tmp = arr[0];
	Pnode<T>* displaced = arr[1];
	Pnode<T>* escape = arr[2];
	if (tmp == nullptr)
		return;
	bool isEnd = false;
	while(!isEnd)
	{
		//adjacent change record
		if (!tmp->isChange && (tmp->change == nullptr))
		{
			tmp->change = n;
			return;
		}
		//shirinking
		if (displaced->key == tmp->key && testAncestor(displaced->version, ver))
		{
			Pnode<T>* newNode = new Pnode<T>(*displaced);
			*displaced = *(tmp->parent);
			if (displaced->key > tmp->key)
				displaced->left_child = newNode;
			else
				displaced->right_child = newNode;
			newNode->change = n;
			return;
		}
		//splitting(shirnk in next step)
		if (tmp == escape)
		{
			Pnode<T>* newNode = new Pnode<T>(*tmp);
			newNode->change = new Pnode<T>(*displaced);
			*displaced = *(tmp->parent);
			if (displaced->key > tmp->key)
				displaced->left_child = newNode;
			else
				displaced->right_child = newNode;
		}
		if (tmp->parent)
			tmp = tmp->parent;
		else
			isEnd = true;
	} 

	//tmp back up past the root
	Pnode<T>* root = new Pnode<T>(*tmp);
	root->change = n;
	v[ver].root = root;
	return;
}
