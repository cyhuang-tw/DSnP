/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include "cirGate.h"

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   CirMgr() : _PIsize(0), _POsize(0), _AIGsize(0), _size(0) {}
   ~CirMgr() { for(size_t count = 0; count <= _size; ++count) delete _gateList[count]; delete[] _gateList; delete _PIList; delete _POList; delete _AIGList; }

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const { if(gid > _size) return 0; return _gateList[gid]; }

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) const;

private:
  bool lexOptions(const string&, vector<string>&, size_t nOpts = 0) const;
  void printNetlist(AigGateV,int&) const;
  vector<int> getAigList() const;
  void traverseFanIn(AigGateV,vector<int>&) const;
  static bool sortFanOut(AigGateV p1,AigGateV p2) { return (p1.gate() -> getGateID() < p2.gate() -> getGateID()); }

  CirGate** _gateList;
  size_t* _PIList;
  size_t* _POList;
  size_t* _AIGList;
  unsigned _size;
  size_t _PIsize;
  size_t _POsize;
  size_t _AIGsize;
  vector<int> headerInfo; //M I L O A
  set<size_t> FFIList; //gates with floating fanin(s)
  set<size_t> DNUList; //gates defined but not used

};

#endif // CIR_MGR_H
