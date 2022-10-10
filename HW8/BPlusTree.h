/* ASSUMPTIONS:
1. When a node must be split and has an odd number of keys, the extra key will go to the new right-hand node.
2. There will never be a duplicate key passed to insert.
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#ifndef DS_BPLUSTREE
#define DS_BPLUSTREE

///////////////////////////////////////////////////////////////////////////////
//DO NOT CHANGE THIS CODE
///////////////////////////////////////////////////////////////////////////////

//Do not implement the class here, this is a forward declaration. Implement at
//the bottom of the .h file
template <class T> class BPlusTree;

template <class T>
class BPlusTreeNode{
public:
	BPlusTreeNode() : parent(NULL) {};
	bool is_leaf();
	bool contains(const T& key);

	//For grading only. This is bad practice to have, because
	//it exposes a private member variable.
	BPlusTreeNode* get_parent() { return parent; }

	//We need this to let BPlusTree access private members
	friend class BPlusTree<T>;
private:
	bool contains(const T& key,std::size_t low,std::size_t high);
	std::vector<T> keys;
	std::vector<BPlusTreeNode*> children;
	BPlusTreeNode* parent;
};

template <class T>
bool BPlusTreeNode<T>::is_leaf(){
	for(unsigned int i=0; i<children.size(); i++){
		if(children[i]){
			return false;
		}
	}
	return true;
}

template <class T>
bool BPlusTreeNode<T>::contains(const T& key){
	return contains(key,0,keys.size()-1);
}

//Private method takes advantage of the fact keys are sorted
template <class T>
bool BPlusTreeNode<T>::contains(const T& key,std::size_t low,std::size_t high){
	if(low>high){
		return false;
	}
	if(low==high){
		return key == keys[low];
	}
	std::size_t mid = (low+high)/2;
	if(key<=keys[mid]){
		return contains(key,0,mid);
	}
	else{
		return contains(key,mid+1,high);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//Your BPlusTree implementation goes below this point.
///////////////////////////////////////////////////////////////////////////////////////
template <class T> class BPlusTree {
public:
	BPlusTree(const int& v) : max_degree(v), root(NULL) {};
	BPlusTree(const BPlusTree<T>& old) {this->copy_tree(old);}
	~BPlusTree() {
		this->destroy_tree(root);
		root = NULL;
	}
	BPlusTree& operator=(const BPlusTree<T>& old) {
      if (&old != this) {
        this->destroy_tree(root);
        root = this->copy_tree(old.root);
      }
      return *this;
    }
	BPlusTreeNode<T>* find(const T& key_value);
	void insert(const T& key_value);
	void print_sideways(std::ostream& outfile) const {
		print_sideways(outfile, root, 0);
	}
	void print_BFS(std::ostream& outfile) const {
		print_BFS(std::ostream& outfile, const BPlusTreeNode<T>* ptr);
	};
private:
	int max_degree;
	BPlusTreeNode<T>* root;
	void split(BPlusTreeNode<T>*& ptr);
	void copy_tree(BPlusTreeNode<T>& old) {

		if (old.root == NULL) return;
		root = new BPlusTreeNode<T>;
		for (unsigned int i = 0; i < old.root->keys; ++i) {
			root->keys.push_back(old.root->keys[i]);
		}
		for (unsigned int i = 0; i < old.root->children.size(); ++i) {
			BPlusTreeNode<T>* newp = new BPlusTreeNode<T>;
			for (unsigned int j = 0; j < old.root->children[i]->keys.size(); ++j) {
				newp->keys.push_back(old.root->children[i]->keys[j]);
			}
			root->children.push_back(newp);
		}
		/*
	}
	void destroy_tree(BPlusTreeNode<T>*& root) {
		//IF THE TREE IS EMPTY DO NOTHING
		if (!root) return;
		//IF THE TREE HAS MORE THAT JUST THE ROOT
		if (root->children.size() > 0) {
			//GO AS DEEP AS POSSIBLE INTO THE TREE
			BPlusTreeNode<T>* newp = root;
			while (newp->children.size() > 0) {
				newp = newp->children[0];
			}
			while (newp->parent != NULL) {
				//DELETE THE KEYS FOR THE DEEPEST NODES IN TREE
				for (unsigned int i = 0; i < newp->parent->children.size(); ++i) {
					newp->parent->children[i]->keys.clear();
				}
				//ONCE THE KEYS FOR THOSE HAVE BEEN ERASED, ERASE THE POINTER TO THOSE NODES FROM ITS PARENT
				//THEN MOVE UP THE TREE UNTIL THE ROOT HAS BEEN REACHED
				newp->parent->children.clear();
				newp = newp->parent;
				if (newp == root) {
					newp->keys.clear();
				}
			}
			newp = NULL;
		//IF THE ONLY NODE IS THE ROOT ERASE THE KEYS FOR THE ROOT
		} else {
			root->keys.clear();
		}
	}
	void print_sideways(std::ostream& outfile, const BPlusTreeNode<T>* p, int depth) const {
		if (p) {
			if (p->children.size() > 0) {
				print_sideways(outfile, p->children[0], depth+1);
				for (int i=0; i<depth; ++i) outfile << "    ";
				for (int i = 0; i < p->keys.size(); ++i) {
					outfile << p->keys[i] << std::endl;
				}
				for (unsigned int i = 1; i < p->children.size(); ++i) {
					print_sideways(outfile, p->children[i], depth+1);
				}
			} else {
				for (unsigned int i = 0; i < p->keys.size(); ++i) {
					if (i == 0) {
						outfile << p->keys[0];
					} else {
						outfile << "," << p->keys[i];
					}
				}
				outfile << std::endl;
			}
		} else {
			outfile << "Tree is empty." << std::endl;
		}
	}
	void BPlusTree<T>::print_BFS(std::ostream& outfile, const BPlusTreeNode<T>* ptr) {
		if (ptr == root) {
			for (unsigned int i = 0; i < ptr->keys.size(); ++i) {
				if (i == 0) {
					outfile << ptr->keys[0];
				} else {
					outfile << "," << ptr->keys[i];
				}
			}
			outfile << std::endl;
			if (ptr->children.size() > 0) {
				ptr = ptr->children[0];
				print_BFS(outfile, ptr);
			}
		} else {
			for (unsigned int i = 0; i < ptr->keys.size(); ++i) {
				if (i == 0) {
					outfile << ptr->keys[0];
				} else {
					outfile << "," << ptr->keys[i];
				}
			}
			if (ptr->parent->children.size() > 1) {
				outfile << "\t";
				for (unsigned int i = 1; i < ptr->parent->children.size(); ++i) {
					for (unsigned int j = 0; j < ptr->parent->children.size(); ++i) {
						if (j == 0) {
							outfile << (ptr->parent->children[i])->keys[j];
						} else {
							outfile << "," << ptr->keys[i];
						}
					}
					outfile << "\t";
				}
			}
		}
	}
};

template <class T>
void BPlusTree<T>::split(BPlusTreeNode<T>*& ptr) {
	BPlusTreeNode<T>* node = ptr->parent;
	if ( node->children.size() == max_degree) {
		if (!node->parent) {
			BPlusTreeNode<T>* temp1_ptr = new BPlusTreeNode<T>;
			BPlusTreeNode<T>* temp2_ptr = new BPlusTreeNode<T>;
			temp1_ptr->keys.push_back(node->keys[(node->keys.size()/2)-1]);
			for (unsigned int t = node->keys.size()-1; t > (node->keys.size()/2)-1; --t) {
				temp2_ptr->keys.push_back(node->keys[t]);
				node->keys.erase(node->keys.begin()+t);
			}
			node->keys.pop_back();
			std::sort(temp2_ptr->keys.begin(), temp2_ptr->keys.end());
			temp2_ptr->parent = temp1_ptr;
			node->parent = temp1_ptr;
			temp1_ptr->children.push_back(temp2_ptr);
			temp1_ptr->children.push_back(node);
			for (unsigned int t = node->children.size()-1; t > (node->children.size()/2)-1; --t) {
				temp2_ptr->children.push_back(node->children[t]);
				node->children.erase(node->children.begin()+t);
			}
			if (node == root) {
				root = temp1_ptr;
			}
			temp1_ptr = NULL;
			temp2_ptr = NULL;
		} else {
			node->parent->keys.push_back(node->keys[(node->keys.size()/2)-1]);
			BPlusTreeNode<T>* temp2_ptr = new BPlusTreeNode<T>;
			for (unsigned int t = node->keys.size()-1; t > (node->keys.size()/2)-1; --t) {
				temp2_ptr->keys.push_back(node->keys[t]);
				node->keys.erase(node->keys.begin()+t);
			}
			std::sort(temp2_ptr->keys.begin(), temp2_ptr->keys.end());
			temp2_ptr->parent = node->parent;
			node->parent->children.push_back(temp2_ptr);
			for (unsigned int t = node->children.size()-1; t > (node->children.size()/2)-1; --t) {
				temp2_ptr->children.push_back(node->children[t]);
				node->children.erase(node->children.begin()+t);
			}
			split(node);
		}
	}
}
template <class T>
void BPlusTree<T>::insert(const T& key_value) {
	//IF THE NODE BEING ADDED TO IS THE VERY FIRST
	if (!root) {
		root = new BPlusTreeNode<T>;
		root->keys.push_back(key_value);
	//IF THE ELEMENT ADDED GOES TO THE ROOT
	} else if (root->children.size() == 0) {
		if (root->keys.size() < max_degree - 1) {
		root->keys.push_back(key_value);
		std::sort(root->keys.begin(), root->keys.end());
		} else if (root->keys.size() == max_degree - 1) {
		//THE NODE REACHES THE MAX AMOUNT OF VALUE IT CAN HOLD AND NEEDS TO SPLIT
			root->keys.push_back(key_value);
			std::sort(root->keys.begin(), root->keys.end());
			//CREATES A NODE THAT WILL BECOME THE NEW ROOE
			BPlusTreeNode<T>* top_ptr = new BPlusTreeNode<T>;
			root->parent = top_ptr;
			top_ptr->keys.push_back(root->keys[(root->keys.size()/2)-1]);
			//CREATES A NEW NODE THAT HOLDS HALF THE VALUES FROM THE OLD ROOT
			BPlusTreeNode<T>* newp = new BPlusTreeNode<T>;
			for (unsigned int t = root->keys.size()-1; t > (root->keys.size()/2)-1; --t) {
				newp->keys.push_back(root->keys[t]);
				root->keys.erase(root->keys.begin()+t);
			}
			std::sort(newp->keys.begin(), newp->keys.end());
			newp->parent = top_ptr;
			top_ptr->children.push_back(newp);
			top_ptr->children.push_back(root);
			root = top_ptr;
			newp = NULL;
		}
	//IF THE ELEMENT GOES TO THE SOME OTHER NODE
	} else {
		BPlusTreeNode<T>* newp = root;
		//FIND THE SPOT THE NODE GOES BY FINDING WHICH NODE HAS NO CHILDREN
		while (newp->children.size() > 0) {
			for (unsigned int i = 0; i < newp->children.size(); ++i) {
				if (key_value < (*root->children[i]).keys.back()) {
					newp = newp->children[i];
				}
			}
		}
		newp->keys.push_back(key_value);
		std::sort(newp->keys.begin(), newp->keys.end());
		//IF THE NEW ELEMENT CAUSES THE NODE TO OVERFLOW
		if (newp->keys.size() == max_degree - 1) {
			BPlusTreeNode<T>* temp_ptr = new BPlusTreeNode<T>;
			newp->parent->keys.push_back(newp->keys[(newp->keys.size()/2)+1]);
			for (unsigned int t = 0; t < (newp->keys.size()/2)+1; ++t) {
				temp_ptr->keys.push_back(newp->keys[t]);
				newp->keys.erase(newp->keys.begin()+t);
			}
			newp->parent->children.push_back(temp_ptr);
			split(newp);
		}
	}
}

template <class T>
BPlusTreeNode<T>* BPlusTree<T>::find(const T& key_value) {
	BPlusTreeNode<T>* newp = root;
	if (!newp) {
		newp = NULL;
		return newp;
	} else {
		while (newp->children.size() > 0) {
			for (unsigned int i = 0; i < newp->children.size(); ++i) {
				if (key_value <= (*newp->children[i]).keys.back()) {
					newp = newp->children[i];
				}
			}
		}
	}
	return newp;
}


#endif
