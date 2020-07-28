#include "SimpleBBST.h"

template <typename T>
SimpleBBSTNode<T>::SimpleBBSTNode(typename std::multiset<T>::iterator its){
	it=its;
}

template <typename T>
T SimpleBBSTNode<T>::getData(){
	return *it;
}

template <typename T>
bool SimpleBBST<T>::isEmpty(){
	return mt.empty();
}

template <typename T>
void SimpleBBST<T>::insert(T t){
	mt.insert(t);
}

template <typename T>
SimpleBBSTNode<T> SimpleBBST<T>::findLeft(T t){
	typename std::multiset<T>::iterator lb=mt.lower_bound(t);
	return SimpleBBSTNode<T>(lb);
}

template <typename T>
SimpleBBSTNode<T> SimpleBBST<T>::findRight(T t){
	typename std::multiset<T>::iterator rb=mt.upper_bound(t);
	return SimpleBBSTNode<T>(rb);
}

template <typename T>
SimpleBBSTNode<T> SimpleBBST<T>::getLeftNode(SimpleBBSTNode<T> nt){
	return SimpleBBSTNode<T>(nt.it--);
}

template <typename T>
SimpleBBSTNode<T> SimpleBBST<T>::getRightNode(SimpleBBSTNode<T> nt){
	return SimpleBBSTNode<T>(nt.it++);
}

template <typename T>
bool SimpleBBST<T>::isLeftMost(SimpleBBSTNode<T> nt){
	return nt.it==mt.begin();
}

template <typename T>
bool SimpleBBST<T>::isRightMost(SimpleBBSTNode<T> nt){
	return nt.it==mt.rbegin();
}

