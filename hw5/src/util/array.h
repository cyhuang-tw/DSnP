/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   // TODO: decide the initial value for _isSorted
   Array() : _data(0), _size(0), _capacity(0), _isSorted(false) {}
   ~Array() { delete []_data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return (*_node); }
      T& operator * () { return (*_node); }
      iterator& operator ++ () { _node += 1; return (*this); }
      iterator operator ++ (int) { iterator it = *this; _node += 1; return it; }
      iterator& operator -- () { _node -= 1; return (*this); }
      iterator operator -- (int) { iterator it = *this; _node -= 1; return it; }

      iterator operator + (int i) const { iterator it = *this; it._node += i; return it; }
      iterator& operator += (int i) { _node += i; return (*this); }

      iterator& operator = (const iterator& i) { _node = i._node; return (*this); }

      bool operator != (const iterator& i) const { return (_node != i._node); }
      bool operator == (const iterator& i) const { return (_node == i._node); }

   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const { iterator it(_data); return it; }
   iterator end() const { iterator it(_data + _size); return it; }
   bool empty() const { return (_size == 0); }
   size_t size() const { return _size; }

   T& operator [] (size_t i) { return _data[i]; }
   const T& operator [] (size_t i) const { return _data[i]; }

   void push_back(const T& x) {
      if(_size == _capacity){
         T* _newData;
         (_size == 0) ? _newData = new T[1] : _newData = new T[2 * _size];
         for(iterator it = begin(), newIt(_newData); it != end(); ++it, ++newIt){
            *newIt = *it;
         }
         delete []_data;
         _data = _newData;
         (_capacity == 0) ? _capacity = 1 : _capacity *= 2;
      }
      iterator it = end();
      *it = x;
      ++_size;
   }
   void pop_front() {
      if(_size > 1)
         _data[0] = _data[_size - 1];
      if(_size > 0)
         --_size;
   }
   void pop_back() { --_size; }

   bool erase(iterator pos) {
      if(empty())
         return false;
      *pos = _data[_size - 1];
      --_size;
      return true;
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

   void clear() { _size = 0; }

   // [Optional TODO] Feel free to change, but DO NOT change ::sort()
   void sort() const { if (!empty()) ::sort(_data, _data+_size); }

   // Nice to have, but not required in this homework...
   // void reserve(size_t n) { ... }
   // void resize(size_t n) { ... }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   T*            _data;
   size_t        _size;       // number of valid elements
   size_t        _capacity;   // max number of elements
   mutable bool  _isSorted;   // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] Helper functions; called by public member functions
};

#endif // ARRAY_H
