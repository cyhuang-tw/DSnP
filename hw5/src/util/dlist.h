/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
      _isSorted = false;
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node -> _data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node = _node -> _next; return *(this); }
      iterator operator ++ (int) { iterator it = *this; _node = _node -> _next; return it; }
      iterator& operator -- () { _node = _node -> _prev; return *(this); }
      iterator operator -- (int) { iterator it = *this; _node = _node -> _prev; return it; }

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const { return (_node != i._node); }
      bool operator == (const iterator& i) const { return (_node == i._node); }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { iterator it(_head -> _next); return it; }
   iterator end() const { iterator it(_head); return it; }
   bool empty() const { return (_head -> _next == _head -> _prev); }
   size_t size() const {  size_t count = 0; for(iterator it = this -> begin(); it != this -> end(); ++it) ++count; return count; }

   void push_back(const T& x) {
   	  DListNode<T>* newNode = new DListNode<T>(x, (this -> end())._node -> _prev, (this -> end())._node);
      (this -> end())._node -> _prev -> _next = newNode;
      (this -> end())._node -> _prev = newNode;
   }
   void pop_front() {
   	  if(!(this -> empty())){
   		DListNode<T>* tmp = (this -> begin())._node;
   		tmp -> _next -> _prev = _head;
   		_head -> _next = tmp -> _next;
         tmp -> _prev = tmp -> _next = 0;
   		delete tmp;
   	}
   }
   void pop_back() {
   	  if(!(this -> empty())){
         DListNode<T>* tmp = (this -> end())._node -> _prev;
         tmp -> _prev -> _next = (this -> end())._node;
         (this -> end())._node -> _prev = tmp -> _prev;
         tmp -> _prev = tmp -> _next = 0;
         delete tmp;
      }
   }

   // return false if nothing to erase
   bool erase(iterator pos) {
   	  if(pos == (this -> end()))
         return false;
      DListNode<T>* tmp = pos._node;
      tmp -> _prev -> _next = tmp -> _next;
      tmp -> _next -> _prev = tmp -> _prev;
      tmp -> _prev = tmp -> _next = 0;
      delete tmp;
      return true;
   }
   bool erase(const T& x) {
   	  for(iterator it = this -> begin(); it != this -> end(); ++it){
        if(*it == x){
         this -> erase(it);
         return true;
        }
      }
   	return false;
   }

   void clear() {
   	  for(iterator it = this -> begin(); it != this -> end();){
   		iterator tmp = it++;
   		this -> erase(tmp);
   	}
   }  // delete all nodes except for the dummy node

   void sort() const {
   	  for(iterator it = this -> begin(); it!= this -> end(); ++it){
         iterator min = it; iterator tmp = it;
         for(iterator pos = ++tmp; pos != this -> end(); ++pos){
            if(*pos < *min)
               min = pos;
         }
         if(it != min)
         	swap(*it, *min);
      }
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
