/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include <fstream>
#include <set>
#include <sstream>
#include <algorithm>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool
CirMgr::readCircuit(const string& fileName)
{
  ifstream cirFile;
  string str;
  cirFile.open(fileName);

  if(!cirFile.is_open()) return false;

  getline(cirFile,str);
  vector<string> headerInfoStr;
  lexOptions(str,headerInfoStr,6);

  for(size_t count = 1; count < 6; ++count){
    int info = 0;
    myStr2Int(headerInfoStr[count],info);
    headerInfo.push_back(info);
  }

  _size = headerInfo[0] + headerInfo[3];
  _PIsize = headerInfo[1];
  _POsize = headerInfo[3];
  _AIGsize = headerInfo[4];

  _gateList = new CirGate*[_size + 1];
  for(size_t count = 0; count <= _size; ++count)
    _gateList[count] = 0;
  _PIList = new size_t[_PIsize];
  _POList = new size_t[_POsize];
  _AIGList = new size_t[_AIGsize];

  _gateList[0] = new Const0Gate(0,0);

  str.clear();

  for(size_t count = 0; count < _PIsize; ++count){
    string input;
    int gid = 0;
    getline(cirFile,input);
    myStr2Int(input,gid); gid/=2;
    _gateList[gid] = new PIGate(gid,count + 2);
    _PIList[count] = gid;
  }

  for(size_t count = 1; count <= _POsize; ++count){
    string output;
    int gid = 0;
    getline(cirFile,output);
    myStr2Int(output,gid);
    _gateList[headerInfo[0] + count] = new POGate(headerInfo[0] + count, count + _PIsize + 1,gid);
    _POList[count - 1] = headerInfo[0] + count;
  }

  size_t** AigInfo = new size_t*[_AIGsize];
  for(size_t i = 0; i < _AIGsize; ++i) AigInfo[i] = new size_t[3];

  for(size_t count = 0; count < _AIGsize; ++count){
    string aigInfo;
    vector<string> tokens;
    getline(cirFile,aigInfo);
    lexOptions(aigInfo,tokens,3);
    int gid = 0, pi1 = 0, pi2 = 0;
    myStr2Int(tokens[0],gid); gid/=2; AigInfo[count][0] = gid;
    myStr2Int(tokens[1],pi1); AigInfo[count][1] = pi1;
    myStr2Int(tokens[2],pi2); AigInfo[count][2] = pi2;

    _gateList[gid] = new AigGate(gid,count + _PIsize + _POsize + 2);
    _AIGList[count] = gid;
  }

  for(size_t count = 0; count < _POsize; ++count){
    POGate* ptr = (POGate*)_gateList[_POList[count]];
    size_t idx = ptr -> outGateIdx();
    bool isInv = ptr -> isInv();
    if(!_gateList[idx])
      _gateList[idx] = new UnDefGate(idx);
    _gateList[idx] -> push_fanOutList(AigGateV(_gateList[_POList[count]],isInv));
    _gateList[_POList[count]] -> push_fanInList(AigGateV(_gateList[idx],isInv));
  }

  for(size_t count = 0; count < _AIGsize; ++count){
    if(!_gateList[AigInfo[count][1]/2]) _gateList[AigInfo[count][1]/2] = new UnDefGate(AigInfo[count][1]/2);
    if(!_gateList[AigInfo[count][2]/2]) _gateList[AigInfo[count][2]/2] = new UnDefGate(AigInfo[count][2]/2);
    _gateList[AigInfo[count][0]] -> push_fanInList(AigGateV(_gateList[AigInfo[count][1]/2],AigInfo[count][1]%2));
    _gateList[AigInfo[count][0]] -> push_fanInList(AigGateV(_gateList[AigInfo[count][2]/2],AigInfo[count][2]%2));
    _gateList[AigInfo[count][1]/2] -> push_fanOutList(AigGateV(_gateList[AigInfo[count][0]],AigInfo[count][1]%2));
    _gateList[AigInfo[count][2]/2] -> push_fanOutList(AigGateV(_gateList[AigInfo[count][0]],AigInfo[count][2]%2));
  }
/*
  for(size_t count = 0; count <= _size; ++count){
    if(!_gateList[count]) continue;
    sort((_gateList[count] -> getFanOutList()).begin(),(_gateList[count] -> getFanOutList()).end(),sortFanOut);

  }
*/
  while(!cirFile.eof()){
    getline(cirFile,str);
    vector<string> tokens;
    bool doInput = false;

    if(str == "c" || str.length() == 0) break;
    if(str[0] == 'i') doInput = true;
    str.erase(str.begin());
    int i = 0;
    vector<string> varInfo;
    lexOptions(str,varInfo);
    int idx = 0;
    myStr2Int(varInfo[0],idx);
    str.erase(str.begin(),str.begin() + varInfo[0].length() + 1);
    if(doInput)
      _gateList[_PIList[idx]] -> setName(str);
    else
      _gateList[_POList[idx]] -> setName(str);
  }

  for(size_t count = 1; count <= _size; ++count){
    if(_gateList[count] == 0 || _gateList[count] -> getTypeStr() == "UNDEF") continue;
    if((_gateList[count] -> getFanOutList()).empty() && _gateList[count] -> getTypeStr() != "PO")
      DNUList.insert(count);
    for(vector<AigGateV>::const_iterator it = (_gateList[count] -> getFanInList()).begin(); it != (_gateList[count] -> getFanInList()).end(); ++it){
      if((*it).gate() -> getTypeStr() == "UNDEF"){
        FFIList.insert(count);
        break;
      }
    }
  }

  cirFile.close();

  return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
  cout << endl;
  cout << "Circuit Statistics" << endl;
  cout << "==================" << endl;
  cout << left << setw(5) << "  PI" << setw(11) << right << _PIsize << endl;
  cout << left << setw(5) << "  PO" << setw(11) << right << _POsize << endl;
  cout << left << setw(5) << "  AIG" << setw(11) << right << _AIGsize << endl;
  cout << "------------------" << endl;
  cout << left << setw(5) << "  Total" << setw(9) << right << _PIsize + _POsize + _AIGsize << endl;
}

void
CirMgr::printNetlist() const
{
  int num = 0;
  CirGate::IncGlobalRef();
  cout << endl;
  for(size_t count = 0; count < _POsize; ++count){
    for(vector<AigGateV>::const_iterator it = (_gateList[_POList[count]] -> getFanInList()).begin(); it != (_gateList[_POList[count]] -> getFanInList()).end(); ++it){
      printNetlist(*it,num);
    }
    cout << "[" << num << "]" << " ";
    _gateList[_POList[count]] -> printGate();
    _gateList[_POList[count]] -> setRef(CirGate::getGlobalRef());
    ++num;
    //cout << endl;
  }
}

void
CirMgr::printNetlist(AigGateV ptr,int& num) const
{
  if(ptr.gate() -> getRef() == CirGate::getGlobalRef()) return;
  if(ptr.gate() -> getTypeStr() == "UNDEF") return;
  for(vector<AigGateV>::const_iterator it = (ptr.gate() -> getFanInList()).begin(); it != (ptr.gate() -> getFanInList()).end(); ++it)
    printNetlist(*it,num);
  cout << "[" << num << "]" << " ";
  ptr.gate() -> printGate();
  ptr.gate() -> setRef(CirGate::getGlobalRef());
  //cout << endl;
  ++num;
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit: ";
   for(size_t count = 0; count < _PIsize;){
    cout << _PIList[count];
    if(++count < _PIsize) cout << " ";
   }
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit: ";
   for(size_t count = 0; count < _POsize;){
    cout << _POList[count];
    if(++count < _POsize) cout << " ";
   }
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
  if(FFIList.empty() && DNUList.empty()) return;
  if(!FFIList.empty()){
    cout << "Gates with floating fanin(s): ";
    for(set<size_t>::const_iterator it = FFIList.begin(); it != FFIList.end();){
      cout << *it;
      if(++it != FFIList.end()) cout << " ";
    }
    cout << endl;
  }
  if(!DNUList.empty()){
    cout << "Gates defined but not used  : ";
    for(set<size_t>::const_iterator it = DNUList.begin(); it != DNUList.end();){
      cout << *it;
      if(++it != DNUList.end()) cout << " ";
    }
    cout << endl;
  }
}

void
CirMgr::writeAag(ostream& outfile) const
{
  outfile << "aag";
  vector<int>aigList = getAigList();
  for(size_t count = 0; count < 4; ++count)
    outfile << " " << headerInfo[count];
  outfile << " " << (aigList.size())/3 << endl;
  for(size_t count = 0; count < _PIsize; ++count){
    outfile << _PIList[count] * 2 << endl;
  }
  for(size_t count = 0; count < _POsize; ++count){
    outfile << ((POGate*)_gateList[_POList[count]]) -> getInfo() << endl;
  }
  for(vector<int>::iterator it = aigList.begin(); it != aigList.end();){
    outfile << *it; ++it; outfile << " "; outfile << *it; ++it; outfile << " "; outfile << *it; ++it; outfile << endl;
  }
  for(size_t count = 0; count < _PIsize; ++count){
    if(_gateList[_PIList[count]] -> getName() != "")
      outfile << "i" << count << " " << _gateList[_PIList[count]] -> getName() << endl;
  }
  for(size_t count = 0; count < _POsize; ++count){
    if(_gateList[_POList[count]] -> getName() != "")
      outfile << "o" << count << " " << _gateList[_POList[count]] -> getName() << endl;
  }
  outfile << "c" << endl;
  outfile << "AAG output by Chung-Yang (Ric) Huang" << endl;


}

vector<int>
CirMgr::getAigList() const
{
  CirGate::IncGlobalRef();
  vector<int> list;
  for(size_t count = 0; count < _POsize; ++count){
    _gateList[_POList[count]] -> setRef(CirGate::getGlobalRef());
    for(vector<AigGateV>::const_iterator it = (_gateList[_POList[count]] -> getFanInList()).begin(); it != (_gateList[_POList[count]] -> getFanInList()).end(); ++it){
      traverseFanIn(*it,list);
    }
  }
  return list;
}

void
CirMgr::traverseFanIn(AigGateV ptr,vector<int>& list) const
{
  if(ptr.gate() -> getRef() == CirGate::getGlobalRef()) return;
  if(ptr.gate() -> getTypeStr() != "AIG") return;
  for(vector<AigGateV>::const_iterator it = (ptr.gate() -> getFanInList()).begin(); it != (ptr.gate() -> getFanInList()).end(); ++it){
    traverseFanIn(*it,list);
  }
  list.push_back((ptr.gate() -> getGateID())*2);
  list.push_back(((ptr.gate() -> getFanInList())[0].gate() -> getGateID())*2 + (ptr.gate() -> getFanInList())[0].isInv());
  list.push_back(((ptr.gate() -> getFanInList())[1].gate() -> getGateID())*2 + (ptr.gate() -> getFanInList())[1].isInv());
  ptr.gate() -> setRef(CirGate::getGlobalRef());
  return;
}

bool
CirMgr::lexOptions
(const string& option, vector<string>& tokens, size_t nOpts) const
{
   string token;
   size_t n = myStrGetTok(option, token);
   while (token.size()) {
      tokens.push_back(token);
      n = myStrGetTok(option, token, n);
   }
   if (nOpts != 0) {
      if (tokens.size() < nOpts) {
         return false;
      }
      if (tokens.size() > nOpts) {
         return false;
      }
   }
   return true;
}