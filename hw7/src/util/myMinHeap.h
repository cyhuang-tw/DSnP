/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { return *(_data.begin()); }

   void insert(const Data& d) {
   	_data.push_back(d);
   	int t = _data.size() - 1;
   	int p = (t-1)/2;
   	while(p >= 0 && p != t){
   		if(!(_data[t] < _data[p])) break;
   		swap(_data[t],_data[p]);
   		t = p;
   		p = (p-1)/2;
   	}
   }
   void delMin() {
   	if(_data.empty()) return;
   	swap(_data[0],_data[_data.size()-1]);
   	_data.pop_back();
   	if(_data.empty()) return;
   	int t = 0;
   	int c = 2*t + 1;
   	while(c < _data.size()){
   		if(c+1 < _data.size()){
   			if(_data[c+1] < _data[c])
   				++c;
   		}
   		if(!(_data[c] < _data[t])) break;
   		swap(_data[t],_data[c]);
   		t = c;
   		c = 2*c + 1;
   	}
   }
   void delData(size_t i) {
      if(_data.empty()) return;
      if(_data.size() == 1) { _data.pop_back(); return; }
      if(i == _data.size() - 1) { _data.pop_back(); return; }
      swap(_data[i],_data[_data.size()-1]);
      _data.pop_back();
      int p = (i-1)/2;
      int c = 2*i + 1;
      if(i == p) return;
      while(p >= 0 && p != i){
         if(!(_data[i] < _data[p])) break;
         swap(_data[i],_data[p]);
         i = p;
         p = (p-1)/2;
      }
      while(c < _data.size()){
         if(c+1 < _data.size()){
            if(_data[c+1] < _data[c]) ++c;
         }
         if(!(_data[c] < _data[i])) break;
         swap(_data[c],_data[i]);
         i = c;
         c = 2*c + 1;
      }
   }

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
