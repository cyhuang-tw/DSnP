/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"

using namespace std;

class CirGate;
class AigGateV;
class AigGate;
class PIGate;
class POGate;
class Const0Gate;
class UnDefGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes

//------------------------------------------------------------------------
//   class AigGateV
//------------------------------------------------------------------------

class AigGateV{
  friend class CirGate;
  friend class AigGate;
  friend class PIGate;
  friend class POGate;
  friend class Const0Gate;
  friend class UnDefGate;
  friend class CirMgr;
  #define NEG 0x1
  AigGateV(CirGate* g, size_t phase): _gateV(size_t(g) + phase) {}
  CirGate* gate() const {return (CirGate*)(_gateV & ~size_t(NEG));}
  bool isInv() const {return (_gateV & NEG);}

  size_t _gateV;
};

//------------------------------------------------------------------------
//   class CirGate
//------------------------------------------------------------------------

class CirGate
{
public:
   CirGate() {}
   virtual ~CirGate() {}

   // Basic access methods
   virtual string getTypeStr() const = 0;
   unsigned getLineNo() const { return _line; }
   unsigned getGateID() const { return _id; }
   size_t& getRef() const { return _ref; }
   string getName() const { return _name; }
   vector<AigGateV>& getFanInList() { return _fanInList; }
   vector<AigGateV>& getFanOutList() { return _fanOutList; }
   static size_t getGlobalRef() { return _globalRef; }

   // Printing functions
   virtual void printGate() const = 0;
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

   //setting functions
   void push_fanInList(AigGateV ptr) { _fanInList.push_back(ptr); }
   void push_fanOutList(AigGateV ptr) { _fanOutList.push_back(ptr); }
   void setName(string& str) { _name = str; }
   //void IncRef() { ++_ref; }
   void setRef(size_t num) const { _ref = num; } 
   static void IncGlobalRef() { ++_globalRef; }  

private:
  void reportFanin(int,int,AigGateV) const;
  void reportFanout(int,int,AigGateV) const;

protected:
   string _name;
   unsigned _id;
   unsigned _line;
   vector<AigGateV> _fanInList;
   vector<AigGateV> _fanOutList;
   mutable size_t _ref;
   static size_t _globalRef;
};


//------------------------------------------------------------------------
//   class AigGate
//------------------------------------------------------------------------

class AigGate : public CirGate {
public:
   AigGate(unsigned i,unsigned l) { _name = ""; _id = i; _line = l; _ref = 0; }
   ~AigGate() {}
   string getTypeStr() const { return "AIG"; }
   void printGate() const {
    cout << "AIG" << " " << _id << " ";
    for(short i = 0; i < 2; ++i){
      if(_fanInList[i].gate() -> getTypeStr() == "UNDEF") cout << "*";
      if(_fanInList[i].isInv()) cout << "!";
      cout << _fanInList[i].gate() -> getGateID();
      if(i < 1) cout << " ";
    }
    if(_name != "") cout << " " << "(" << _name << ")";
    cout << endl;  
   }
};

//------------------------------------------------------------------------
//   class PIGate
//------------------------------------------------------------------------

class PIGate : public CirGate {
public:
   PIGate(unsigned i,unsigned l) { _name = ""; _id = i; _line = l; _ref = 0; }
   ~PIGate() {}
   string getTypeStr() const { return "PI"; }
   void printGate() const {
    cout << "PI" << "  " << _id;
    if(_name != "") cout << " " << "(" << _name << ")";
    cout << endl;
   }
};

//------------------------------------------------------------------------
//   class POGate
//------------------------------------------------------------------------

class POGate : public CirGate {
public:
   POGate(unsigned i,unsigned l,size_t p) : _out(p) { _name = ""; _id = i; _line = l; _ref = 0; }
   ~POGate() {}
   string getTypeStr() const { return "PO"; }
   bool isInv() const { return (_out%2 != 0); }
   size_t outGateIdx() const { return _out/2; }
   size_t getInfo() const { return _out; }
   void printGate() const {
    cout << "PO" << "  " << _id << " ";
    if(_fanInList[0].gate() -> getTypeStr() == "UNDEF") cout << "*";
    if(isInv()) cout << "!";
    cout << _out/2;
    if(_name != "") cout << " " << "(" << _name << ")";
    cout << endl;
   }
private:
   size_t _out;  
};

//------------------------------------------------------------------------
//   class Const0Gate
//------------------------------------------------------------------------

class Const0Gate : public CirGate {
public:
   Const0Gate(unsigned i,unsigned l) { _name = ""; _id = i; _line = l; _ref = 0; }
   ~Const0Gate() {}
   string getTypeStr() const { return "CONST"; }  
   void printGate() const {
    cout << "CONST0" << endl;
   }
};

//------------------------------------------------------------------------
//   class UnDefGate
//------------------------------------------------------------------------

class UnDefGate : public CirGate {
public:
   UnDefGate(unsigned i) { _name = ""; _id = i; _line = 0; _ref = 0; }  
   ~UnDefGate() {}
   string getTypeStr() const { return "UNDEF"; }
   void printGate() const {}
};


#endif // CIR_GATE_H
