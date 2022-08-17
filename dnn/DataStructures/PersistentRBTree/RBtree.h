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
* (2)[Red Constraint] Every red node has a black parent
* (3)[Black Constraint] From any node, all paths to an external node which is
* in its subtree contain the same number of black node
*/

enum Color{
	red,
	black
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
		leaf->ins_interval = false;
		leaf->del_interval = false;
		leaf->top = nullptr;
		leaf->bottom = nullptr;

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

	bool is_leaf(Node<T>*& node) {
		return node == leaf;
	}

	void insert_bst(T key) {
		Node<T>* node = new Node<T>;
		node->color = red;
		node->key = key;
		node->parent = leaf;
		node->right_child = leaf;
		node->left_child = leaf;
		node->pre = head;
		node->next = tail;
		node->ins_interval = false;
		node->del_interval = false;
		node->top = leaf;
		node->bottom = leaf;

		Node<T> *tmp, * tmp_parent, *start, *start_last, *last, *tmp_last;
		tmp = root;
		tmp_parent = root;
		start = leaf; // start means the start of current insertion or deletion interval
		start_last = leaf;  //start_last means the last start
		last = leaf;  // This means the last element of insertion or deletion interval before we reach tartget element
		if (tmp != leaf && tmp != nullptr) {
			while (tmp != leaf && tmp != nullptr) {  //tmp !=nullptr;
				if (tmp->ins_interval || tmp->del_interval) {
					start = tmp;
					last = start;
				}
				tmp_parent = tmp;
				compare(tmp->key, node->key) ? tmp = tmp->left_child : tmp = tmp->right_child;

				if ((start != nullptr)&&(start->bottom!=nullptr)&&(start != leaf)&&(start->bottom!=leaf)) {
					start->bottom->key < tmp_parent->key ? tmp_last = tmp_parent->left_child : tmp_last = tmp_parent->right_child;
					last = tmp_parent;
					if (((tmp != leaf)&&(tmp_last!=tmp)) || ((tmp_last == tmp) && (tmp == start->bottom))) {
						last = tmp;
						start_last = start;
						start = leaf;
					}
				}
			}

			if (start_last != leaf) {
				last->top = start_last;
				last->bottom = start_last->bottom;
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
			/*
			cout << "Current node is " << node->key << endl;
			cout << "< Last information > " <<  endl;
			myprint(last);
			cout << "\n\n" << endl;
			*/
			insert_balance(node,last);
			/*
			cout << "< Last information > " << endl;
			myprint(last);
			cout << "\n\n" << endl;
			*/
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
		Node<T>* original_parent;
		Node<T>* actually = nullptr;
		Node<T>* tmp = root;
		Node<T>* replaced_node = nullptr;
		Node<T>* parent = nullptr;
		Node<T>* start = nullptr;
		Node<T>* last = nullptr;
		Node<T>* tmp_last;
		Color delete_color;
		while (original != leaf) {
			if (original->key == key)
				break;
			else {
				if (original->ins_interval || original->del_interval) {//
					start = original;
					last = start;
				}
				original_parent = original;
				compare(original->key, key) ? original = original->left_child : original = original->right_child;
				if (original == leaf) //By this code, original is not leaf in the below codes
					break;
				if (start != nullptr) {
					compare(original->parent->key, start->bottom->key) ? tmp_last = original->left_child : tmp_last = original->right_child;
					last = original_parent;
					if (tmp_last != original) {
						last->top = start->top;
						last->bottom = start->bottom;
						start = nullptr;
					}
				}
			}
		}
		if (original == leaf) //this case means there is no node, of which key is key
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
			cout << "delete node with 1 child case" << endl;
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
			cout << "delete node with 2 child case" << endl;
			actually = find_right_smallest(original);
			parent = actually->parent;
			if (parent == original)
				parent = actually;
			delete_color = actually->color;
			//smallest == leaf
			if (actually->right_child == leaf) {
				cout << "smallest == leaf" << endl;
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
			delete_balance(replaced_node, last);
		//	delete_balance(replaced_node, parent);
    
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
	void print_all() {
		cout << "root=" << root->key << endl;
		myprint(root);
	}
	void myprint(Node<T>*& node) {
		
		if ((node != leaf)&&(node!=nullptr)) {
			//cout << "key= " << node->key << endl;

			cout << "key= " << node->key << " color= " << node->color <<" top = " << node->top->key << " bottom = " << node->bottom->key << endl;
			cout << "ins_interval= " << node->ins_interval <<" del_interval= " << node->del_interval << endl;
			cout << "parent=" << node->parent->key << " left_child= " << node->left_child->key << " right_child= " << node->right_child->key <<"\n\n" << endl;
		}
		
		
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
				//cout << tmp->next->key << endl;
				myprint(tmp->next);
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
		if ((node == leaf) || (node == nullptr) || (node->parent == leaf) || (node->parent == nullptr))
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
		if (node != leaf) {
			Node<T>* child = node->right_child;
			Node<T>* parent = node->parent;
			Node<T>* temp = node;
			if (child->left_child != leaf && child->left_child != nullptr) {
				child->left_child->parent = temp;
			}
			temp->right_child = child->left_child;
			temp->parent = child;
			child->left_child = temp;
			child->parent = parent;
			if (parent != leaf) {
				if (parent->left_child == temp)
					parent->left_child = child;
				else
					parent->right_child = child;
			}
		}
	}

	void right_rotation(Node<T>*& node) {
		if (node != leaf) {
			Node<T>* child = node->left_child;
			Node<T>* parent = node->parent;
			Node<T>* temp = node;

			if (child->right_child != leaf && child->right_child != nullptr) {
				child->right_child->parent = temp;
			}
			temp->left_child = child->right_child;
			temp->parent = child;
			child->right_child = temp;
			child->parent = parent;
			if (parent != leaf) {
				if (parent->right_child == temp)
					parent->right_child = child;
				else
					parent->left_child = child;
			}
		}
	}

	void insert_balance(Node<T>*& node, Node<T>*& last) {
		Node<T>* temp = node;
		//We need to deal with recoloring using below if sentences
		if (candidate_insertion(node)) {         //recoloring part
			Node<T>* u = uncle(temp);
			Node<T>* g = grandparent(temp);
			if ((node->parent == last)&&(last!=leaf)) {
				last->parent->top = last->top;
				last->parent->bottom = last->bottom;
				split_insert_interval(last->parent);
			}
			else {
				while (candidate_insertion(g)) {  //we change temp as the last candidate for recoloring
					if ((g == last)&&(last!=leaf)) {
						last->parent->top = last->top;
						last->parent->bottom = last->bottom;
						split_insert_interval(last->parent);
						last->top = leaf;
						last->bottom = leaf;
						last = leaf;
						break;
					}
					else if ((g->parent == last)&&(last!=leaf)) {
						last->parent->top = last->top;
						last->parent->bottom = last->bottom;
						split_insert_interval(last->parent);
						last->top = leaf;
						last->bottom = leaf;
						last = leaf;
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
		//We need to deal with rebalancing using insert_case1~insert_case4
		insert_case1(temp, last);
	}

	void insert_case1(Node<T>*& node, Node<T>*& Last){      //Case1: node is root
		Node<T>* u = uncle(node);
		Node<T>* last = Last;
		//We need to rule out the case when node is the middle of any interval
		if ((node == last)&&(last!=leaf)) {
			if (last->top->ins_interval == true) {
				node->parent->top = last->top;
				split_insert_interval(node->parent);
				last->top = leaf;
				last->bottom = leaf;
				last = leaf;
			}
			else if (last->top->del_interval == true) {
				split_del_interval(node);
				last->top = leaf;
				last->bottom = leaf;
				last = leaf;
			}
		}
		
		if ((node->parent != leaf) && ( (node->parent->top != leaf)||(node->parent->bottom!=leaf))) {
			//we need to rule out the case that parent is the bottom of insertion interval
			if ((node->parent->top->ins_interval == true) && (node->parent->bottom == node->parent))
				split_insert_interval(node->parent);

			//we need to rule out the case that parent is the bottom of deletion interval
			if ((node->parent->top->del_interval == true) && (node->parent->bottom == node->parent))
				split_del_interval(node->parent);
		}
		if ((node->parent!=leaf)&&(node->parent->parent != leaf) && (node->parent->parent->top != leaf)&&(node->parent->parent->bottom!=leaf)) {
			//we need to rule out the case that grandparent is the bottom of insertion interval
			if ((node->parent->parent->top->ins_interval == true) && (node->parent->parent->bottom == node->parent->parent))
				split_insert_interval(node->parent->parent);

			//we need to rule out the case that grandparent is the bottom of deletion interval
			if ((node->parent->parent->top->del_interval == true) && (node->parent->parent->bottom == node->parent->parent))
				split_del_interval(node->parent->parent);
		}
		if (u != leaf) {
			//We need to rule out the case when sibling is the start of the insertion interval
			if (u->ins_interval == true)
				split_insert_interval(u);

			//We need to rule out the case when sibling is the start of the deletion interval
			if (u->del_interval == true)
				split_del_interval(u);
		}
		if (node->parent == leaf)  //when node is the root node
			node->color = black;   //we need to deal with delete interval!!!
		else
			insert_case2(node,last);
	}

	void insert_case2(Node<T>*& node, Node<T>*& last) {      //Case2: parent of the node is black
		if (node->parent->color == black) //if node->parent is black in interval tree, then node->parent is fringe node. So, we don't need to do anything
			return;      //Tree is valid  
		else
			insert_case3(node,last);
	}

	bool candidate_insertion(Node<T>*& node){  //this determines whether node is a candidate for insert_case3
		Node<T>* gp = grandparent(node);
		Node<T>* u = uncle(node);
		if ((gp != leaf) && (u != leaf) && (gp->color == black) && (gp->left_child->color == red) && (gp->right_child->color == red))
			return true;
		return false;
	}
		
	void split_insert_interval(Node<T>*& middle) {	
		if (middle != leaf && middle != nullptr) {
			cout << "middle" << endl;
			myprint(middle);
			if (middle->top != leaf && middle->top != nullptr)
				middle->bottom = middle->top->bottom;
			if (middle->bottom != leaf && middle->bottom != nullptr)
				middle->top = middle->bottom->top;

			if (middle->color == red) {
				if (middle != middle->top && middle->top!=leaf && middle->top!=nullptr) {//split upper chain with lower chain
					middle->parent->color = red;
					sibling(middle)->color = black;
					Node<T>* new_bottom = grandparent(middle);
					if (new_bottom != leaf && new_bottom != nullptr) {
						if (new_bottom == middle->top) {  //delete upper chain
							new_bottom->color = black;
							new_bottom->bottom = leaf;
							new_bottom->ins_interval = false;
						}
						else {
							new_bottom->top = middle->top;
							middle->top->bottom = new_bottom;
						}
					}					
				}
				if (middle != middle->bottom && middle->bottom!=leaf && middle->bottom!=nullptr) {//split lower chain with upper chain
					Node<T>* new_top = middle;
					new_top->key > middle->bottom->key ? new_top = new_top->left_child : new_top = new_top->right_child;//we need to find next candidate for new top
					if (new_top != leaf && new_top != nullptr) {
						new_top->key > middle->bottom->key ? new_top = new_top->left_child : new_top = new_top->right_child;
						if (new_top != leaf) {
							new_top->parent->color = red;
							sibling(new_top)->color = black;
							if (new_top == middle->bottom) {   //delete lower chain
								new_top->color = black;
								new_top->top = leaf;
							}
							else {
								new_top->ins_interval = true;
								new_top->bottom = middle->bottom;
								middle->bottom->top = new_top;
							}
						}
					}

				}
				middle->color = black;
				middle->top = leaf;
				middle->bottom = leaf;
				middle->ins_interval = false;
			}
			else {//when middle->color==black 
				//split upper chain with lower chain
				Node<T>* new_bottom = middle->parent;
				if (new_bottom != leaf && new_bottom != nullptr) {
					if (new_bottom != leaf && new_bottom != nullptr && new_bottom == middle->top) {  //delete upper chain
						new_bottom->color = black;
						new_bottom->bottom = leaf;
						new_bottom->ins_interval = false;
					}
					else {
						new_bottom->top = middle->top;
						middle->top->bottom = new_bottom;
					}
				}			

				//split lower chain with upper chain
				Node<T>* new_top = middle;
				new_top->key > middle->bottom->key ? new_top = new_top->left_child : new_top = new_top->right_child;//we need to find next candidate for new top
				if (new_top != leaf && new_top != nullptr) {
					new_top->parent->color = red;
					sibling(new_top)->color = black;
					if (new_top == middle->bottom) {   //delete lower chain
						new_top->color = black;
						new_top->top = leaf;
					}
					else {
						new_top->ins_interval = true;
						new_top->bottom = middle->bottom;
						middle->bottom->top = new_top;
					}
					//middle->color = red;  we've already done this when new_top->parent->color=red;
					middle->ins_interval = false;
					middle->top = leaf;
					middle->bottom = leaf;
				}
			}
		}
	}

	void insert_case3(Node<T>*& node, Node<T>*& last) {
		if ((last!=leaf) && ((node->parent == last) || (node->parent->parent==last))) { //By this conditional statement, we don't need to consdier any insertion interval from insert_case1 ~ insert_case 5
			last->parent->top = last->top;
			last->parent->bottom = last->bottom;
			split_insert_interval(last->parent);
			insert_case1(node, leaf);
		}
		else {
			Node<T>* g = grandparent(node);
			Node<T>* temp=leaf;
			/*check
			cout << "Insert case3!!" << "Target Node = " << node->key << endl;
			myprint(node);
			cout << "<parent> " << endl;
			myprint(node->parent);
			cout << "<grandparent> " << endl;
			myprint(g);
			*/
			if ((node == node->parent->right_child) && (node->parent == g->left_child)) {
				left_rotation(node->parent);
				temp = node->left_child;
			}
			else if ((node == node->parent->left_child) && (node->parent == g->right_child)) {
				right_rotation(node->parent);
				temp = node->right_child;
			}
			/*
			cout << "After rotation!!" << endl;
			myprint(node);
			cout << "<left child> " << endl;
			myprint(node->left_child);
			cout << "<parent> " << endl;
			myprint(node->parent);
			*/
			if (temp == leaf) {
				insert_case4(node, last);
			}
			else {
				insert_case4(temp, last);
			}
			/*check
			cout << "after insert_case3" << endl;
			myprint(node);
			cout << "<parent> " << endl;
			myprint(node->parent);
			cout << "<grandparent> " << endl;
			myprint(g);
			*/
		}
	}
	
	void insert_case4(Node<T>*& node, Node<T>*& last) {
		Node<T>* g = grandparent(node);
		/*
		cout << "Insert case4!!" << "Target Node = " << node->key << endl;
		myprint(node);
		cout << "<parent> " << endl;
		myprint(node->parent);
		cout << "<grandparent> " << endl;
		myprint(g);
		*/
		node->parent->color = black;
		g->color = red;

		if (root == g)  //This code is needed when g is root
			root = node->parent;

		if (node == node->parent->left_child) {
			cout << "right_rotation" << endl;
			right_rotation(g);
		}
		else {
			cout << "left_rotation" << endl;
			left_rotation(g);
		}


		/*
		cout << "after insert_case4"<< endl;
		myprint(node);
		cout << "<parent> " << endl;
		myprint(node->parent);
		cout << "<grandparent> " << endl;
		myprint(g);
		*/
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

	void delete_handling(Node<T>*& node, Node<T>*& last) {
		if (node->color == red) {
			node->color = black;
		}
		else
			delete_case1(node,last);
	}

	void delete_balance(Node<T>*& node, Node<T>*& last) {
		Node<T>* temp = node;
		//We need to deal with lazy coloring using below if sentences
		if (candidate_deletion(node)) {
			temp = node->left_child;
			while (candidate_deletion(temp->parent)) {
				if (temp->parent != last)
					temp = temp->parent;
				else if ((sibling(temp->parent)!=leaf)&&(sibling(temp->parent)->del_interval == true)) {
					Node<T>* s = sibling(temp->parent);
					split_del_interval(s);
					//split_del_interval(sibling(temp->parent));
					break;
				}
				else {   //This means ((temp->parent == last) && (last->top->del_interval == true))
					split_del_interval(last);
					break;
				}
			}
			if (temp == node) {
				sibling(temp)->color = red;
			}
			else {
				temp->del_interval = true;
				temp->bottom = node;
				node->top = temp;
			}
			temp = temp->parent;
		}
		//We need to deal wtih rebalancing using delete_case1~delete_case5
		if ((temp != leaf) && (temp != nullptr)) {
			delete_case1(temp, last);
		}
	}

	void delete_case1(Node<T>*& node, Node<T>*& last) {
		Node<T>* s = sibling(node);

		//We need to rule out the case when node is the middle of any interval
		if (node == last) {
			if (last->top->ins_interval == true)
				split_insert_interval(node);
			else if (last->top->del_interval == true)
				split_del_interval(node);
		}
		//we need to rule out the case that parent is the bottom of insertion interval
		if ((node != leaf) && (node != nullptr) && (node->parent->top->ins_interval == true) && (node->parent->bottom == node->parent))
			split_del_interval(node->parent);

		//we need to rule out the case that parent is the bottom of deletion interval
		if ((node != leaf) && (node != nullptr) && (node->parent->top->del_interval == true) && (node->parent->bottom == node->parent))
			split_del_interval(node->parent);

		//We need to rule out the case when sibling is the start of the insertion interval
		if (s->ins_interval == true)
			split_insert_interval(s);

		//We need to rule out the case when sibling is the start of the deletion interval
		if (s->del_interval == true)
			split_del_interval(s);

		if (node->parent != leaf) {    //If the node is root, there is nothing to do.
			delete_case2(node, last);
		}
	}

	void delete_case2(Node<T>*& node, Node<T>*& last) {
		Node<T>* s = sibling(node);
		if (s->color == red) {
			node->parent->color = red;
			s->color = black;
			if (node == node->parent->left_child)
				left_rotation(node->parent);
			else
				right_rotation(node->parent);
		}
		delete_case3(node,last);
	}

	bool candidate_deletion(Node<T>*& node) {  //this determines whether node is a candidate for insert_case3
		Node<T>* s = sibling(node);
		if ((s != leaf) && (node->parent != leaf) && (s->color == black) && (node->parent->color == black) && (s->left_child->color==black) && (s->right_child->color == black))
			return true;
		return false;
	} 
	
	void split_del_interval(Node<T>*& middle) {
		//In deletion interval, there are only black nodes!
		if ((middle != leaf) && (middle != nullptr)) {
			Node<T>* new_bottom = middle->parent;
			Node<T>* new_top = (middle->key > middle->bottom->key) ? new_top = middle->left_child : new_top = middle->right_child;

			//Handle upper chain
			if (middle != middle->top && middle->top != leaf && middle->top != nullptr) {
				new_bottom->top = middle->top;
				middle->top->bottom = new_bottom;
				sibling(middle)->color = red;
				if (new_bottom == middle->top) {
					new_bottom->del_interval = false;
					new_bottom->top = leaf;
					new_bottom->bottom = leaf;
				}
			}

			//Handle lower chain
			if (middle != middle->bottom && middle->bottom != leaf && middle->bottom != nullptr) {
				new_top->del_interval = true;
				new_top->bottom = middle->bottom;
				middle->bottom->top = new_top;
				sibling(new_top)->color = red;
				if (new_top == middle->bottom) {
					new_top->top = leaf;
					new_top->bottom = leaf;
				}
			}
		}		
	}


	void delete_case3(Node<T>*& node, Node<T>*& last) {
		if (node != leaf && node != nullptr) {
			Node<T>* s = sibling(node);
			//we need to rule out the case that children of s could be the start of deletion interval.
			if ((s->left_child != nullptr) && (s->left_child->del_interval == true))
				split_del_interval(s->left_child);
			if ((s->right_child != nullptr) && (s->right_child->del_interval == true))
				split_del_interval(s->right_child);

			if ((node->parent->color == red) && (s->color == black) && (s->left_child != nullptr) && (s->left_child->color == black) && (s->right_child != nullptr) && (s->right_child->color == black)) {
				s->color = red;
				node->parent->color = black;
			}
			else {
				delete_case4(node, last);
			}
		}		
	}

	void delete_case4(Node<T>*& node, Node<T>*& last) {
		Node<T>* s = sibling(node);
		if ((s->color == black)&&(s->left_child!=nullptr)&&(s->right_child!=nullptr)) {
			if ((node == node->parent->left_child) && (s->right_child->color == black) && (s->left_child->color == red)) {
				s->color = red;
				s->left_child->color = black;
				right_rotation(s);
			}
			else if ((node == node->parent->right_child) && (s->left_child!=nullptr)&&(s->left_child->color == black) &&(s->right_child!=nullptr)&& (s->right_child->color == red)) {
				s->color = red;
				s->right_child->color = black;
				left_rotation(s);
			}
		}
		delete_case5(node, last);
	}

	void delete_case5(Node<T>*& node, Node<T>*& last) {
		Node<T>* s = sibling(node);
		s->color = node->parent->color;
		node->parent->color = black;
		if ((node == node->parent->left_child)&&(s->right_child!=nullptr)) {
			s->right_child->color = black;
			left_rotation(node->parent);
		}
		else {
			if (s->left_child != nullptr) {
				s->left_child->color = black;
				right_rotation(node->parent);

			}
			
		}
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
	Node<T>* get_root() {
		return root;
	}
	Node<T>* get_leaf() {
		return leaf;
	}

	bool compare(T key1, T key2) {
		return key1 > key2;
	}
};