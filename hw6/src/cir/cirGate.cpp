/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/

size_t CirGate::_globalRef = 0;

void
CirGate::reportGate() const
{
	stringstream ss;
	string gateInfo = "";
	ss << getTypeStr() << "(" << _id << ")";
	if(_name.length())
		ss << "\"" << _name << "\"";
	ss << ", line " << _line;
	gateInfo = ss.str();
	cout << "==================================================" << endl;
	cout << "="; cout << " ";
	cout << left << setw(47) << gateInfo;
	cout << "=" << endl;
	cout << "==================================================" << endl; 
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   CirGate::IncGlobalRef();
   cout << getTypeStr() << " " << _id << endl;
   if(level == 0) return;
   _ref = _globalRef;
   for(vector<AigGateV>::const_iterator it = _fanInList.begin(); it != _fanInList.end(); ++it){
   	reportFanin(1,level - 1,*it);
   }
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   CirGate::IncGlobalRef();
   cout << getTypeStr() << " " << _id << endl;
   if(level == 0) return;
   _ref = _globalRef;
   for(vector<AigGateV>::const_iterator it = _fanOutList.begin(); it != _fanOutList.end(); ++it){
   	reportFanout(1,level - 1,*it);
   }
}

void
CirGate::reportFanin(int space,int level,AigGateV ptr) const
{
	for(size_t count = 0; count < space; ++count) cout << "  ";
	if(ptr.isInv()) cout << "!";
	cout << ptr.gate() -> getTypeStr() << " " << ptr.gate() -> getGateID();
	if(ptr.gate() -> getTypeStr() == "PI" || ptr.gate() -> getTypeStr() == "CONST" || ptr.gate() -> getTypeStr() == "UNDEF") { cout << endl; return; }
	if(level == 0) { cout << endl; return; }
	if(ptr.gate() -> _ref == _globalRef) { cout << " " << "(*)" << endl; return; }
	if(((ptr.gate()) -> _fanInList).empty()) { cout << endl; return; }
	ptr.gate() -> _ref = _globalRef;
	cout << endl;
	for(vector<AigGateV>::const_iterator it = (ptr.gate()) -> _fanInList.begin(); it != (ptr.gate()) -> _fanInList.end(); ++it){
		reportFanin(space + 1,level - 1, *it);
	}
}

void
CirGate::reportFanout(int space,int level,AigGateV ptr) const
{
	for(size_t count = 0; count < space; ++count) cout << "  ";
	if(ptr.isInv()) cout << "!";
	cout << ptr.gate() -> getTypeStr() << " " << ptr.gate() -> getGateID();
	if(ptr.gate() -> getTypeStr() == "PO") { cout << endl; return; }
	if(level == 0) { cout << endl; return; }
	if(ptr.gate() -> _ref == _globalRef) { cout << " " << "(*)" << endl; return; }
	if(((ptr.gate()) -> _fanOutList).empty()) { cout << endl; return; }
	ptr.gate() -> _ref = _globalRef;
	cout << endl;
	for(vector<AigGateV>::const_iterator it = (ptr.gate()) -> _fanOutList.begin(); it != (ptr.gate()) -> _fanOutList.end(); ++it){
		reportFanout(space + 1,level - 1, *it);
	}
}