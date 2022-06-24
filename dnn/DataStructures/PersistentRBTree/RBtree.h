#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <string>
using namespace std;

/*Red Black Tree with Lazy Coloring
* Followings are 3 rules for RB tree
* (1)[External Node Convention] Every external node is regarded as being black
* (2)[Red Constraint] Every red node has a black parent or has no parent(root)
* (3)[Black Constraint] From any node, all paths to an external node which is
* in its subtree contain the same number of black node
*/

enum Color{
	red,
	black,
	double_black
};

enum Op {
	insert_node,
	delete_node
};

template <typename T>
struct Node {
	T key;
	Color color;
	bool ins_interval;
	bool del_interval;
	Node* top;
	Node* bottom;
	Node* parent;
	Node* right_child;
	Node* left_child;
	Node* pre;
	Node* next;
};

template <typename T>
class RBTree {
protected:
	Node<T>* root;
	Node<T>* leaf;
	Node<T>* head;
	Node<T>* tail;
	int rbt_size;
public:
	RBTree() {
		leaf = new Node<T>;
		head = new Node<T>;
		tail = new Node<T>;
		leaf->color = black;
		leaf->parent = nullptr;
		leaf->right_child = nullptr;
		leaf->left_child = nullptr;
		leaf->pre = nullptr;
		leaf->next = nullptr;
		root = leaf;

		head->next = tail;
		head->pre = nullptr;
		tail->pre = head;
		tail->next = nullptr;

		rbt_size = 0;
	}
	~RBTree() {
		delete leaf;
		delete head;
		delete tail;
	}
	void insert_bst(T key) {
		Node<T>* node = new Node<T>;
		node->color = red;
		node->key = key;
		node->parent = this->leaf;
		node->right_child = this->leaf;
		node->pre = head;
		node->next = tail;

		Node<T>* tmp, * tmp_parent, *start, *last,*tmp_last;
		tmp = root;
		start = nullptr;
		last = nullptr;
		if (tmp != leaf) {
			while (tmp != leaf) {  //tmp !=nullptr;
				if (tmp->ins_interval || tmp->del_interval) {
					start = tmp;
					last = start;
				}
				tmp_parent = tmp;
				compare(tmp->key, node->key) ? tmp = tmp->left_child : tmp = tmp->right_child;

				if (start != nullptr) {
					start->bottom->key < tmp_parent ? tmp_last = tmp_parent->left_child : tmp_last = tmp_parent->right_child;
					if (tmp == leaf) {
						start = nullptr;
					}
					else {
						last = tmp;
						if (tmp_last != tmp) {
							start = nullptr;
							last->top = start->top;
							last->bottom = start->bottom;
						}
					}
				}
			}
			if (compare(tmp_parent->key, node->key)) {
				Node<T>* pre = tmp_parent->pre;
				tmp_parent->left_child = node;
				node->next = tmp_parent;
				node->pre = tmp_parent->pre;
				tmp_parent->pre = node;
				pre->next = node;
			}
			else {
				Node<T>* next = tmp_parent->next;
				tmp_parent->right_child = node;
				node->pre = tmp_parent;
				node->next = tmp_parent->next;
				tmp_parent->next = node;
				next->pre = node;
			}
			node->parent = tmp_parent;
			insert_balance(node, key);
		}
		else {
			root = node;
			node->color = black;
			head->next = node;
			tail->pre = node;
		}
		rbt_size++;
	}





	void delete_bst(T key) {
		Node<T>* original = root;
		Node<T>* actually = nullptr;
		Node<T>* tmp = root;
		Node<T>* replaced_node = nullptr;
		Node<T>* parent = nullptr;
		Color delete_color;
		while (original != leaf) {
			if (original->key == key)
				break;
			else
				compare_delete(original->key, key) ? original = original->left_child : original = original->right_child;
		}
		if (original == leaf)
			return;
		Node<T>* pre = original->pre;
		Node<T>* next = original->next;
		pre->next = next;
		next->pre = pre;
		//delete leaf node
		if (original->left_child == leaf && original->right_child == leaf) {
			cout << "					delete leaf case" << endl;
			actually = original;
			parent = actually->parent;
			delete_color = actually->color;
			replaced_node = leaf;
			if (actually == root) {
				cout << "						root" << endl;
				root = leaf;
			}
			else {
				cout << "						not root" << endl;
				actually->parent->left_child == actually ? actually->parent->left_child = replaced_node : actually->parent->right_child = replaced_node;
			}
		}
		//delete node with 1 child
		else if (original->left_child == leaf || original->right_child == leaf) {
			cout << "					delete node with 1 child case" << endl;
			actually = original;
			delete_color = actually->color;
			parent = actually->parent;
			original->left_child != leaf ? replaced_node = original->left_child : replaced_node = original->right_child;
			replaced_node->parent = parent;
			actually->left_child == leaf ? replaced_node = actually->right_child : replaced_node = actually->left_child;
			if (actually == root) {
				cout << "						root" << endl;
				root = replaced_node;
			}
			else {
				cout << "						not root" << endl;
				parent->left_child == actually ? parent->left_child = replaced_node : parent->right_child = replaced_node;
			}
		}
		//delete node with 2 child -> find smallest key of right sub tree
		else {
			cout << "					delete node with 2 child case" << endl;
			actually = find_right_smallest(original);
			parent = actually->parent;
			if (parent == original)
				parent = actually;
			delete_color = actually->color;
			//smallest == leaf
			if (actually->right_child == leaf) {
				cout << "						smallest == leaf" << endl;
				replaced_node = leaf;

				if (actually->parent != original) actually->parent->left_child = leaf;
				else original->right_child = leaf;
				actually->color = original->color;
				actually->left_child = original->left_child;
				actually->left_child->parent = actually;
				actually->right_child = original->right_child;
				if (original->right_child != leaf)
					actually->right_child->parent = actually;
				actually->parent = original->parent;
				if (original != root)
					original->parent->left_child == original ? original->parent->left_child = actually : original->parent->right_child = actually;
				else
					root = actually;
				actually = original;
				actually->color = delete_color;
			}
			//smallest has right child
			else {
				cout << "						smallest has right child" << endl;
				replaced_node = actually->right_child;
				if (actually->parent != original) {
					replaced_node->parent = parent;
					parent->left_child = replaced_node;
				}
				//parent->left_child == actually ? parent->left_child = replaced_node : parent->right_child = replaced_node;
				//parent != original ? parent->left_child = replaced_node : parent->right_child = replaced_node;
				actually->color = original->color;
				actually->left_child = original->left_child;
				actually->left_child->parent = actually;
				if (actually->parent != original) {
					actually->right_child = original->right_child;
					actually->right_child->parent = actually;
				}
				actually->parent = original->parent;
				if (original != root)
					original->parent->left_child == original ? original->parent->left_child = actually : original->parent->right_child = actually;
				else
					root = actually;
				actually = original;
				actually->color = delete_color;
			}
		}
		//delete_color = actually->color;
		delete actually;

		if (delete_color == black)
			delete_balance(replaced_node, parent);

		if (rbt_size != 0)
			rbt_size--;
	}
	Node<T>* search(T key) {
		Node<T>* tmp = root;
		while (tmp != leaf) {
			if (tmp->key == key)
				return tmp;
			else {
				tmp->key > key ? tmp = tmp->left_child : tmp = tmp->right_child;
			}
		}
		return nullptr;
	}
	Color get_color(Node<T>*& node) {
		return node->color;
	}
	void set_children(Node<T>*& node, Node<T>*& left, Node<T>*& right) {
		node->left_child = left;
		node->right_child = right;
	}
	void print() {
		inorder(root, 0);
		cout << endl;
	}
	void print_linked_list() {
		Node<T>* tmp = head;
		cout << "print linked list" << endl;
		while (1) {
			if (tmp->next != nullptr && tmp->next != tail) {
				cout << tmp->next->key << endl;
				tmp = tmp->next;
			}
			else {
				cout << "end linked list" << endl;
				break;
			}
		}
	}
	bool is_empty() {
		return root == leaf;
	}
	int size() {
		return rbt_size;
	}
	Node<T>* grandparent(Node<T>*& node){
		if ((node != leaf) && (node->parent != leaf))
			return node->parent->parent;
		else
			return leaf;
	}
	Node<T>* uncle(Node<T>*& node) {
		Node<T>* gp = grandparent(node);
		if (gp == leaf)
			return leaf;
		if (node->parent == gp->left_child)
			return gp->right_child;
		else
			return gp->left_child;
	}
	Node<T>* sibling(Node<T>*& node) {
		if (node->parent == leaf)
			return leaf;
		else if (node == node->parent->left_child)
			return node->parent->right_child;
		else
			return node->parent->left_child;
	}

	Node<T>* find_same_or_biggest_small(T key) {
		Node<T>* tmp = tail; //start from biggest
		while (tmp->pre != nullptr && tmp->pre != head) {
			if (tmp->pre->key <= key)
				return tmp->pre;
			tmp = tmp->pre;
		}
		return nullptr;//all keys > input key
	}
	Node<T>* find_same_or_smallest_big(T key) {
		Node<T>* tmp = head; //start from smallest
		while (tmp->next != nullptr && tmp->next != tail) {
			if (tmp->next->key >= key)
				return tmp->next;
			tmp = tmp->next;
		}
		return nullptr;//all keys < input key
	}
	//protected:
	void left_rotation(Node<T>*& node) {
		Node<T>* right_child = node->right_child;
		right_child->parent = node->parent;
		if (node->parent != leaf) { //node->parent != nullptr
			Node<T>* parent = node->parent;
			node == parent->left_child ? parent->left_child = right_child : parent->right_child = right_child;
		}
		else
			root = right_child;

		node->parent = right_child;
		if (right_child->left_child != leaf) { //right_child->left_child != nullptr
			Node<T>* left_child = right_child->left_child;
			node->right_child = left_child;
			left_child->parent = node;
		}
		else
			node->right_child = leaf;
		right_child->left_child = node;
	}
	void right_rotation(Node<T>*& node) {
		Node<T>* left_child = node->left_child;
		left_child->parent = node->parent;
		if (node->parent != leaf) { //node->parent != nullptr
			Node<T>* parent = node->parent;
			node == parent->left_child ? parent->left_child = left_child : parent->right_child = left_child;
		}
		else
			root = left_child;

		node->parent = left_child;
		if (left_child->right_child != leaf) { //right_child->left_child != nullptr
			Node<T>* right_child = left_child->right_child;
			node->left_child = right_child;
			right_child->parent = node;
		}
		else
			node->left_child = leaf;
		left_child->right_child = node;
	}
	void insert_balance(Node<T>*& node, T key) {
		Node<T>* parent = node->parent;
		if (parent->color == black)
			return;
		//double red == parent is not root.
		if (parent->parent->left_child->color == parent->parent->right_child->color) { //uncle & parent both red
			insert_recoloring(node, parent, key);
		}
		else {
			insert_restructuring(node, parent, key);
		}
	}
	void insert_case1(Node<T>*& node, Node<T>*& last){      //Case1: node is root
		if (node->parent == leaf)  //when node is the root node
			node->color = black;   //we need to deal with delete interval!!!
		else
			insert_case2(node,last);
	}
	void insert_case2(Node<T>*& node, Node<T>*& last) {      //Case2: parent of the node is black
		if (node->parent->color == black) //if node->parent is black in interval tree, then node->parent is fringe node. So, we don't need to do anything
			return;      //Tree is valid  
		else
			insert_case4(node,last);
	}
	bool candidate(Node<T>*& node){  //this determines whether node is a candidate for insert_case3
		Node<T>* gp = grandparent(node);
		Node<T>* u = unlce(node);
		if ((gp != leaf) && (u != leaf) && (gp->color == black) && (gp->left_child->color == red) && (gp->right_child->color == red))
			return true;
		return false;
	}
	void insert_case3(Node<T>*& node, Node<T>*& last){
		Node<T>* temp = node;
		if (candidate(temp)) {         //recoloring part
			Node<T>* u = uncle(temp);
			Node<T>* g = grandparent(temp);
			if (node->parent == last) {
				last->parent->top = last->top;
				last->parent->bottom = last->bottom;
				split_insert_interval(last->parent);
			}
			else {
				while (candidate(g)) {  //we change temp as the last candidate for recoloring
					if (g == last) {
						last->parent->top = last->top;
						last->parent->bottom = last->bottom;
						split_insert_interval(last->parent);
						break;
					}
					else if (g->parent == last) {
						last->parent->top = last->top;
						last->parent->bottom = last->bottom;
						split_insert_interval(last->parent);
						break;
					}
					else {
						temp = g;
						u = uncle(temp);
						g = grandparent(temp);
					}
				}
				u->color = black;
				g->color = red;
				if (node == temp)        //There is only one recoloring
					node->parent->color = black;
				else {                   //There are at least 2 recoloring
					temp->parent->ins_interval = true;
					temp->parent->bottom = node->parent;
					node->parent->top = temp->parent;
				}
				temp = g;   //we consider the rebalancing of g
			}
		}
		insert_case1(temp,last);
	}
	void split_insert_interval(Node<T>*& middle) {
		if (middle->color == red) {
			if (middle != middle->top) {//split upper chain with lower chain
				middle->parent->color = red;
				sibling(middle)->color = black;
				Node<T>* new_bottom = grandparent(middle);
				if (new_bottom == middle->top) {  //delete upper chain
					new_bottom->ins_interval = false;
					new_bottom->color = black;
				}
				else {
					middle->top->bottom = new_bottom;
					new_bottom->top = middle->top;
				}
			}
			if (middle != middle->bottom) {//split lower chain with upper chain
				Node<T>* new_top=middle;
				new_top->key > middle->bottom->key ? new_top = new_top->left_child : new_top = new_top->right_child;//we need to find next candidate for new top
				new_top->key > middle->bottom->key ? new_top = new_top->left_child : new_top = new_top->right_child;  
				new_top->parent->color = red;
				sibling(new_top)->color = black;
				if (new_top == middle->bottom) {   //delete lower chain
					new_top->color = black;
				}
				else {
					new_top->ins_interval = true;
					new_top->bottom = middle->bottom;
					middle->bottom->top = new_top;
				}
			}
			middle->color = black;
			middle->top = leaf;
			middle->bottom = leaf;
		}
		else {//when middle->color==black 
			//split upper chain with lower chain
			Node<T>* new_bottom = middle->parent;
			if (new_bottom == middle->top) {  //delete upper chain
				new_bottom->ins_interval = false;
				new_bottom->color = black;
			}
			else {
				middle->top->bottom = new_bottom;
				new_bottom->top = middle->top;
			}
			
			//split lower chain with upper chain
			Node<T>* new_top = middle;
			new_top->key > middle->bottom->key ? new_top = new_top->left_child : new_top = new_top->right_child;//we need to find next candidate for new top
			new_top->parent->color = red;
			sibling(new_top)->color = black;
			if (new_top == middle->bottom) {   //delete lower chain
				new_top->color = black;
			}
			else {
				new_top->ins_interval = true;
				new_top->bottom = middle->bottom;
				middle->bottom->top = new_top;
			}
			//middle->color = red;  we've already done this when new_top->parent->color=red;
			middle->top = leaf;
			middle->bottom = leaf;
		}
	}
	void insert_case4(Node<T>*& node, Node<T>*& last) {
		Node<T>* g = grandparent(node);
		if ((node == node->parent->right_child) && (node->parent == g->left_child)) {
			left_rotation(node->parent);
			node = node->left_child;
		}
		else if ((node == node->parent->left_child) && (node->parent == g->right_child)) {
			right_rotation(node->parent);
			node = node->right_child;
		}
		insert_case5(node, last);
	}
	void insert_case5(Node<T>*& node, Node<T>*& last) {
		Node<T>* g = grandparent(node);
		node->parent->color = black;
		g->color = red;
		if (node == node->parent->left)
			right_rotation(g);
		else
			left_rotation(g);
	}

	void insert_recoloring(Node<T>*& node, Node<T>*& parent, T key) {
		Node<T>* grand = parent->parent;
		Node<T>* uncle = nullptr;
		grand->left_child == parent ? uncle = grand->right_child : uncle = grand->left_child;

		set_color(parent, black);
		set_color(uncle, black);
		set_color(grand, red);
		cout << "					recoloring" << endl;

		if (grand == root || (grand->parent == root && root->color == red)) {
			cout << "						1 step end" << endl;
			root->color = black;
		}
		else {
			insert_balance(grand, grand->key);
			cout << "						next step need" << endl;
		}
	}
	void insert_restructuring(Node<T>*& node, Node<T>*& parent, T key) {
		Node<T>* grand = parent->parent;
		bool com_gp = compare(grand->key, parent->key);
		bool com_np = compare(node->key, parent->key);
		cout << "					restructuring" << endl;

		if (com_gp == com_np) {
			cout << "						com_gp == com_np" << endl;
			set_color(node, black);
			set_color(grand, red);
			set_color(parent, red);
			if (grand == this->root) {
				node->parent = this->leaf;
				root = node;
				cout << "							grand == root" << endl;
			}
			else {
				node->parent = grand->parent;
				grand == grand->parent->left_child ? grand->parent->left_child = node : grand->parent->right_child = node;
				cout << "							grand != root" << endl;
			}
			if (com_gp) { // grand key > parent key
				set_children(node, parent, grand);
				//
				parent->right_child = leaf;
				grand->left_child = leaf;
				//set_children(parent, grand->right_child, this->leaf);
				cout << "							grand > parent" << endl;
			}
			else {
				set_children(node, grand, parent);
				//
				parent->left_child = leaf;
				grand->right_child = leaf;
				//set_children(parent, this->leaf, grand->left_child);
				cout << "							grand <= parent" << endl;
			}
			parent->parent = grand->parent = node;
			//set_children(grand, this->leaf, this->leaf);
		}
		else {
			cout << "						com_gp != com_np" << endl;
			set_color(parent, black);
			set_color(node, red);
			set_color(grand, red);
			if (grand == this->root) {
				parent->parent = this->leaf;
				root = parent;
				cout << "							grand == root" << endl;
			}
			else {
				parent->parent = grand->parent;
				grand == grand->parent->left_child ? grand->parent->left_child = parent : grand->parent->right_child = parent;

				cout << "							grand != root" << endl;
			}
			if (com_gp) { // grand key > parent key
				//
				grand->left_child = parent->right_child;
				parent->right_child->parent = grand;
				//set_children(grand, grand->right_child, this->leaf);
				set_children(parent, node, grand);
				cout << "							grand > parent" << endl;
			}
			else {
				grand->right_child = parent->left_child;
				parent->left_child->parent = grand;
				//set_children(grand, this->leaf, grand->left_child);
				set_children(parent, grand, node);
				cout << "							grand <= parent" << endl;
			}
			//node->parent = parent;
			grand->parent = parent;
			//set_children(node, this->leaf, this->leaf);
		}
	}
	void delete_all(Node<T>*& node) {
		if (node->left_child != leaf) {
			delete_all(node->left_child);
		}
		if (node->right_child != leaf) {
			delete_all(node->right_child);
		}
		//cout << "delete: " << node->key << " )";
		if (node == root) {
			root = leaf;
			delete node;
		}
		else {
			node->parent->left_child == node ? node->parent->left_child = leaf : node->parent->right_child = leaf;
			delete node;
		}
	}
	void delete_balance(Node<T>*& child, Node<T>*& parent) {
		//(child->color)++;
		cout << "					delete balance" << endl;
		if (child->color == red) {
			cout << "						child == red" << endl;
			child->color = black;
			return;
		}

		child->color = double_black;

		Node<T>* sibling = nullptr;
		while (child != root && child->color == double_black) {
			if (child == parent->left_child) {
				cout << "						left child case" << endl;
				sibling = parent->right_child;
				if (sibling->color == red) {
					//case 1. sibling == red
					sibling->color = black;
					parent->color = red;
					left_rotation(parent);

					sibling = parent->right_child;
					cout << "							case 1" << endl;
				}
				else {
					if (sibling->right_child->color == red) {
						//case 4. sibling == black && sibling's right_child == red
						sibling->color = parent->color;
						parent->color = black;
						sibling->right_child->color = black;
						left_rotation(parent);
						child->color = black;
						cout << "							case 4" << endl;
						break;
					}
					else {
						if (sibling->left_child == leaf || sibling->left_child->color == black) {
							//case 2. sibling == black && sibling's 2 childs both black
							cout << "							case 2" << endl;
							sibling->color = red;
							child->color = black;
							if (parent->color == red) {
								parent->color = black;
								cout << "								parent == red" << endl;
								break;
							}
							parent->color = double_black;
							child = parent;
							if (child == root) {
								cout << "								child == root" << endl;
								break;
							}
							cout << "								else" << endl;
							parent = child->parent;
							child == parent->left_child ? sibling = parent->right_child : sibling = parent->left_child;
						}
						else {
							//case 3. sibling == black && sibling's left child == red, right_child == black
							cout << "							case 3" << endl;
							sibling->color = red;
							sibling->left_child->color = black;
							right_rotation(sibling);

							sibling = parent->right_child;
						}
					}
				}
			}
			else {
				cout << "						right child case" << endl;
				sibling = parent->left_child;
				if (sibling->color == red) {
					//case 1. sibling == red
					sibling->color = black;
					parent->color = red;
					right_rotation(parent);

					sibling = parent->left_child;
					cout << "							case 1" << endl;
				}
				else {
					if (sibling->left_child->color == red) {
						//case 4. sibling == black && sibling's right_child == red
						sibling->color = parent->color;
						parent->color = black;
						sibling->left_child->color = black;
						right_rotation(parent);
						child->color = black;
						cout << "							case 4" << endl;
						break;
					}
					else {
						if (sibling->right_child == leaf || sibling->right_child->color == black) {
							//case 2. sibling == black && sibling's 2 childs both black
							cout << "							case 2" << endl;
							sibling->color = red;
							child->color = black;
							if (parent->color == red) {
								parent->color = black;
								cout << "								parent == red" << endl;
								break;
							}
							parent->color = double_black;
							child = parent;
							if (child == root) {
								cout << "								child == root" << endl;
								break;
							}
							cout << "								else" << endl;
							parent = child->parent;
							child == parent->left_child ? sibling = parent->right_child : sibling = parent->left_child;
						}
						else {
							//case 3. sibling == black && sibling's left child == red, right_child == black
							cout << "							case 3" << endl;
							sibling->color = red;
							sibling->right_child->color = black;
							left_rotation(sibling);

							sibling = parent->left_child;
						}
					}
				}
			}
		}

		if (child == root && child->color == double_black)
			child->color = black;
	}
	void inorder(Node<T>*& node, int depth) {
		if (node == root && node == leaf)
			return;
		string color;
		if (node->color == red)
			color = "red";
		else if (node->color == black)
			color = "black";
		if (node->left_child != leaf) {
			inorder(node->left_child, depth + 1);
		}
		cout << "( " << depth << " : " << node->key << " , " << color << " )";
		if (node->right_child != leaf) {
			inorder(node->right_child, depth + 1);
		}
	}
	void set_color(Node<T>*& node, Color c) {
		node->color = c;
	}
	Node<T>* find_right_smallest(Node<T>*& node) {
		Node<T>* tmp = node->right_child;
		while (tmp != leaf) {
			if (tmp->left_child != leaf) {
				tmp = tmp->left_child;
				continue;
			}
			break;
		}
		return tmp;
	}
	bool compare(T key1, T key2) {
		return key1 > key2;
	}
	bool compare_delete(T key1, T key2) {
		return key1.compare_delete(key2);
	}
};