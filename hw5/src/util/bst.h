/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
	friend class BSTree<T>;
	friend class BSTree<T>::iterator;
	BSTreeNode(BSTreeNode<T>* p,BSTreeNode<T>* l,BSTreeNode<T>* r,T d) : _parent(p), _left(l), _right(r), _data(d) {}
	~BSTreeNode(){ delete _left; delete _right; }
	BSTreeNode<T>* _parent;
	BSTreeNode<T>* _left;
	BSTreeNode<T>* _right;
	T _data;
};


template <class T>
class BSTree
{
   // TODO: design your own class!!
public:
   BSTree() : _root(0), _dummy(0) {}
   ~BSTree() { clear(); }
   class iterator {
   	friend class BSTree;
   public:
   	iterator(BSTreeNode<T>* n) : _node(n) {}
   	iterator(const iterator& n) : _node(n._node) {}
   	~iterator(){}

   	const T& operator * () const { return _node -> _data; }
   	T& operator * () { return _node -> _data; }
   	iterator& operator ++ () {
   		if(_node -> _right != 0){
   			_node = _node -> _right;
   			while(_node -> _left != 0)
   				_node = _node -> _left;
   		}
   		else{
   			BSTreeNode<T>* p = _node -> _parent;
   			while(p != 0 && _node == p -> _right){
   				_node = p;
   				p = p -> _parent;
   			}
   			_node = p;
   		}
   		return *this;
   	}
   	iterator operator ++ (int) {
   		iterator it(*this);
   		++(*this);
   		return it;
   	}
   	iterator& operator -- () {
   		if(_node -> _left != 0){
   			_node = _node -> _left;
   			while(_node -> _right != 0)
   				_node = _node -> _right;
   		}
   		else{
   			BSTreeNode<T>* p = _node -> _parent;
   			while(p != 0 && _node == p -> _left){
   				_node = p;
   				p = p -> _parent;
   			}
   			_node = p;
   		}
   		return *this;
   	}
   	iterator operator -- (int) {
   		iterator it(*this);
   		--(*this);
   		return it;
   	}

   	iterator operator = (const iterator& i) { _node = i._node; }

   	bool operator != (const iterator& i) { return (_node != i._node); }
   	bool operator == (const iterator& i) { return (_node == i._node); }


   private:
   	BSTreeNode<T>* _node;
   };
   iterator begin() const {
   	if(empty()) return end();
   	iterator it(_root);
   	while(it._node != 0 && it._node -> _left != 0)
   		it._node = it._node -> _left;
   	return it;
   }
   iterator end() const { iterator it(_dummy); return it; }
   bool empty() const { return (_root == 0); }
   size_t size() const { size_t count = 0; for(iterator it = begin(); it != end(); ++it) ++count; return count; }

    void insert(const T& x, BSTreeNode<T>* _node,BSTreeNode<T>* _p){
   	if(_node == _dummy){
   		_node = new BSTreeNode<T>(_p,0,_dummy,x);
   		_p -> _right = _node;
   		_dummy -> _parent = _node;
   		return;
   	}
   	if(_node == 0){
   		_node = new BSTreeNode<T>(_p,0,0,x);
   		if(x > _p -> _data)
   			_p -> _right = _node;
   		else
   			_p -> _left = _node;
   	}
   	else
   		if(x <= _node -> _data)
   			insert(x,_node -> _left,_node);
   		else
   			insert(x,_node -> _right,_node);
   }

   void insert(const T& x){
   	if(_root == 0){
   		_root = new BSTreeNode<T>(0,0,0,x);
   		_dummy = new BSTreeNode<T>(_root,0,0,x);
   		_root -> _right = _dummy;
   	}
   	else{
   		if(x <= _root -> _data)
   			insert(x,_root -> _left,_root);
   		else
   			insert(x,_root -> _right,_root);
   	}
   }

   bool erase(iterator pos){
   	if(pos == end())
   		return false;
   	if(pos._node -> _left == 0 && pos._node -> _right == 0){
   		if(pos._node -> _parent != 0){
   			if(pos._node -> _parent -> _left == pos._node)
   				pos._node -> _parent -> _left = 0;
   			else
   				pos._node -> _parent -> _right = 0;
   			return true;
   		}
   	}
   	if(pos._node -> _left != 0 && pos._node -> _right != 0){
   		if(pos._node -> _right == _dummy){
   			BSTreeNode<T>* n = pos._node -> _left;
   			while(n -> _right != 0)
   				n = n -> _right;
   			n -> _right = _dummy;
   			_dummy -> _parent = n;
   			if(pos._node -> _parent != 0){
   				if(pos._node -> _parent -> _left == pos._node)
   					pos._node -> _parent -> _left = pos._node -> _left;
   				else
   					pos._node -> _parent -> _right = pos._node -> _left;
   				pos._node -> _left -> _parent = pos._node -> _parent;
   				pos._node -> _left = pos._node -> _right = pos._node -> _parent = 0;
   				delete pos._node;
   				pos._node = 0;
   				return true;
   			}
   			else{
   				BSTreeNode<T>* _rtd = _root;
   				_root = _root -> _left;
   				_rtd -> _left = _rtd -> _right = _rtd -> _parent = 0;
   				delete _rtd;
   				_rtd = 0;
   				_root -> _parent = 0;
   				return true;
   			}
   		}
   		BSTreeNode<T>* s = pos._node -> _right;
   		while(s -> _left != 0)
   			s = s -> _left;
   		pos._node -> _data = s -> _data;
   		if(s -> _right == 0){
   			if(s -> _parent -> _left == s)
   				s -> _parent -> _left = 0;
   			else
   				s -> _parent -> _right = 0;
   			s -> _left = s -> _right = s -> _parent = 0;
   			delete s;
   			s = 0;
   			return true;
   		}
   		else{
   			if(s -> _parent -> _left == s)
   				s -> _parent -> _left = s ->_right;
   			else
   				s -> _parent -> _right = s -> _right;
   			s -> _right -> _parent = s -> _parent;
   			s -> _left = s -> _right = s -> _parent = 0;
   			delete s;
   			s = 0;
   			return true;
   		}
   	}
   	if(pos._node -> _left != 0 && pos._node -> _right == 0){
   		if(pos._node -> _parent != 0){
   			if(pos._node -> _parent -> _left == pos._node)
   				pos._node -> _parent -> _left = pos._node -> _left;
   			else
   				pos._node -> _parent -> _right = pos._node -> _left;
   			pos._node -> _left -> _parent = pos._node -> _parent;
   			pos._node -> _left = pos._node -> _right = pos._node -> _parent = 0;
   			delete pos._node;
   			pos._node = 0;
   			return true;
   		}
   		return true;
   	}
   	if(pos._node -> _left == 0 && pos._node -> _right != 0){
   		if(pos._node -> _right == _dummy){
   			if(pos._node -> _parent != 0){
   				if(pos._node -> _parent -> _left == pos._node)
   					pos._node -> _parent -> _left = _dummy;
   				else
   					pos._node -> _parent -> _right = _dummy;
   				_dummy -> _parent = pos._node -> _parent;
   				pos._node -> _left = pos._node -> _right = pos._node -> _parent = 0;
   				delete pos._node;
   				pos._node = 0;
   				return true;   				
   			}
   			else{
   				pos._node -> _left = pos._node -> _right = pos._node -> _parent = 0;
   				_dummy -> _left = _dummy -> _right = _dummy -> _parent = 0;
   				delete pos._node;
   				delete _dummy;
   				_root = _dummy = 0;
   				return true;
   			}
   		}
   		if(pos._node -> _parent != 0){
   			if(pos._node -> _parent -> _left == pos._node)
   				pos._node -> _parent -> _left = pos._node -> _right;
   			else
   				pos._node -> _parent -> _right = pos._node -> _right;
   			pos._node -> _right -> _parent = pos._node -> _parent;
   			pos._node -> _left = pos._node -> _right = pos._node -> _parent = 0;
   			delete pos._node;
   			pos._node = 0;
   			return true;
   		}
   		else{
   			BSTreeNode<T>* _rtd = _root;
   			if(_rtd != pos._node) return false;
   			_root = _root -> _right;
   			_root -> _parent = 0;
   			_rtd -> _left = _rtd -> _right = _rtd -> _parent = 0;
   			delete _rtd;
   			_rtd = 0;
   			return true;
   		}
   	}
   	return false;
   }

   bool erase(const T& x) {
   	for(iterator it = begin(); it != end(); ++it){
   		if(*it == x){
   			erase(it);
   			return true;
   		}
   	}
   	return false;
   }

   void pop_front() { if(_root == 0) return; erase(begin()); }
   void pop_back() { if(_root == 0) return; erase(--end()); }

   void print() const {}

   void clear() { delete _root; _root = 0; _dummy = 0; }

   void sort() {}

private:
   BSTreeNode<T>* _root;
   BSTreeNode<T>* _dummy;
};

#endif // BST_H
