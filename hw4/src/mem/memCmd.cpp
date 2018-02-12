/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO
   vector<string> tokens;
   bool doArr = false;
   int arrIdx = 0;
   if(!CmdExec::lexOptions(option,tokens,0)){
    return CMD_EXEC_ERROR;
   }
   if(tokens.size() == 0)
    return CmdExec::errorOption(CMD_OPT_MISSING, "");
   if(tokens.size() > 3)
    return CmdExec::errorOption(CMD_OPT_EXTRA, tokens[3]);
   for(vector<string>::iterator it = tokens.begin(); it != tokens.end();){
    if(myStrNCmp("-Array",*it,2) == 0){
      if(doArr)
        return CmdExec::errorOption(CMD_OPT_EXTRA,*it);
      if(it == --(tokens.end()))
        return CmdExec::errorOption(CMD_OPT_MISSING,*it);
      vector<string>::iterator idx = it;
      if(!myStr2Int(*(++idx),arrIdx))
        return CmdExec::errorOption(CMD_OPT_ILLEGAL,*idx);
      if(arrIdx <= 0)
        return CmdExec::errorOption(CMD_OPT_ILLEGAL,*idx);
      tokens.erase(it,++idx);
      doArr = true;
    }
    else
      ++it;
   }
   if(tokens.size() > 1)
    return CmdExec::errorOption(CMD_OPT_EXTRA,tokens[1]);
   int idx = 0;
   if(!myStr2Int(tokens[0],idx))
    return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[0]);
   if(idx <= 0)
    return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[0]);
   if(doArr){
    try{
      mtest.newArrs(idx,arrIdx);
    }
    catch(bad_alloc exception){
      return CMD_EXEC_DONE;
    }
    return CMD_EXEC_DONE;
   }
   else{
    try{
      mtest.newObjs(idx);
    }
    catch(bad_alloc exception){
      return CMD_EXEC_DONE;
    }
    return CMD_EXEC_DONE;
   }

  /*
   if(token.size() == 1){
    int num = 0;
    if(!myStr2Int(token[0], num))
      return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[0]);
    if(num <= 0)
      return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[0]);
      try{
        mtest.newObjs(num);
      }
      catch(bad_alloc exception){
        return CMD_EXEC_DONE;
      }
      return CMD_EXEC_DONE;
   }
   short idx = 0;
   bool count = false;
   for(short i = 0; i < token.size(); ++i){
    if(myStrNCmp("-Array",token[i],2) == 0){
      if(count){
        return CmdExec::errorOption(CMD_OPT_EXTRA, token[i]);
      }
      idx = i;
      count = true;
    }
   }
   vector<string> sortedToken;
   if(idx == 0)
    sortedToken.push_back(token[2]);
   if(idx == 1)
    sortedToken.push_back(token[0]);
   sortedToken.push_back(token[idx]);
   sortedToken.push_back(token[idx + 1]);

   if(myStrNCmp("-Array",sortedToken[1],2) != 0)
    return CmdExec::errorOption(CMD_OPT_ILLEGAL, sortedToken[1]);
   int num1 = 0;
   int num2 = 0;
   if(!myStr2Int(sortedToken[0], num1) || num1 <= 0)
    return CmdExec::errorOption(CMD_OPT_ILLEGAL, sortedToken[0]);
   if(!myStr2Int(sortedToken[2], num2) || num2 <= 0)
    return CmdExec::errorOption(CMD_OPT_ILLEGAL, sortedToken[2]);
   try{
    mtest.newArrs(num1,num2);
   }
   catch(bad_alloc exception){
    return CMD_EXEC_DONE;
   }

   return CMD_EXEC_DONE;
   */
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO
   vector<string> tokens;
   bool doArr = false;
   if(!lexOptions(option,tokens,0))
    return CMD_EXEC_ERROR;
   if(tokens.size() > 3)
    return CmdExec::errorOption(CMD_OPT_EXTRA,tokens[3]);
   if(tokens.empty())
    return CmdExec::errorOption(CMD_OPT_MISSING,"");
   for(vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it){
    if((myStrNCmp("-Index",*it,2) == 0) || (myStrNCmp("-Random",*it,2) == 0)){
      int t = 0;
      if(it == --(tokens.end()))
        return CmdExec::errorOption(CMD_OPT_MISSING,*it);
      vector<string>::iterator idx = it;
      if(!myStr2Int(*(++idx),t)){
        if(myStrNCmp("-Array",*idx,2) == 0)
          return CmdExec::errorOption(CMD_OPT_MISSING,*it);
        else
          return CmdExec::errorOption(CMD_OPT_ILLEGAL,*idx);
      }
    }
   }
   for(vector<string>::iterator it = tokens.begin(); it != tokens.end();){
    if(myStrNCmp("-Array",*it,2) == 0){
      if(doArr)
        return CmdExec::errorOption(CMD_OPT_EXTRA,*it);
      doArr = true;
      tokens.erase(it);
    }
    else
      ++it;
   }
   if(tokens.size() > 2)
    return CmdExec::errorOption(CMD_OPT_EXTRA,tokens[2]);
   if(tokens.size() < 2)
    return CmdExec::errorOption(CMD_OPT_MISSING,"");
   if(myStrNCmp("-Index",tokens[0],2) == 0){
    int idx = 0;
    if(!myStr2Int(tokens[1],idx))
      return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[1]);
    if(idx < 0)
      return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[1]);
    if(doArr){
      if(idx >= mtest.getArrListSize()){
        cerr << "Size of array list (" << mtest.getArrListSize() << ") is <= " << idx << "!!" << endl;
        return CMD_EXEC_ERROR;
      }
      mtest.deleteArr(idx);
      return CMD_EXEC_DONE;
    }
    if(idx >= mtest.getObjListSize()){
      cerr << "Size of object list (" << mtest.getObjListSize() << ") is <= " << idx << "!!" << endl;
      return CMD_EXEC_ERROR;
    }
    mtest.deleteObj(idx);
    return CMD_EXEC_DONE;
   }
   if(myStrNCmp("-Random",tokens[0],2) == 0){
    int num = 0;
    if(!myStr2Int(tokens[1],num))
      return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[1]);
    if(num <= 0)
      return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[1]);
    if(doArr){
      if(mtest.getArrListSize() == 0){
        cerr << "Size of array list is 0!!" << endl;
        return CMD_EXEC_ERROR;
      }
      for(size_t i = 0; i < num; ++i)
        mtest.deleteArr(rnGen(mtest.getArrListSize()));
      return CMD_EXEC_DONE;
    }
    if(mtest.getObjListSize() == 0){
      cerr << "Size of object list is 0!!" << endl;
      return CMD_EXEC_ERROR;
    }
    for(size_t i = 0; i < num; ++i)
      mtest.deleteObj(rnGen(mtest.getObjListSize()));
    return CMD_EXEC_DONE;
   }
   return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[0]);



  /*
   vector<string> token;
   int idx = 0;
   if(!lexOptions(option, token, 0))
    return CMD_EXEC_ERROR;
   if(token.size() > 3)
    return CmdExec::errorOption(CMD_OPT_EXTRA, token[3]);
   if(token.size() == 0)
    return CmdExec::errorOption(CMD_OPT_MISSING, "");
   short arrIdx = 0;
   bool count = false;
   for(short i = 0; i < token.size(); ++i){
    if(myStrNCmp("-Array", token[i], 2) == 0){
      if(count)
        return CmdExec::errorOption(CMD_OPT_EXTRA, token[i]);
      arrIdx = i;
      count = true;
    }
   }
   if(count){
   for(short i = arrIdx; i < token.size() - 1; ++i){
    token[i] = token[i + 1];
    }
   token[token.size() - 1] = "-Array";
   }
   if(myStrNCmp("-Index", token[0], 2) == 0){
    if(token.size() ==1)
      return CmdExec::errorOption(CMD_OPT_MISSING, token[0]);
    if(token.size() == 2){
      if(!myStr2Int(token[1], idx))
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[1]);
      if(idx < 0)
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[1]);
      if(idx >= mtest.getObjListSize()){
        cout << "Size of object list (" << mtest.getObjListSize() << ") is <= " << idx << "!!" << endl;
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[1]);
      }
      mtest.deleteObj(idx);
      return CMD_EXEC_DONE;
    }
    if(myStrNCmp("-Array", token[1], 2) == 0){
      if(!myStr2Int(token[2], idx))
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[2]);
      if(idx < 0)
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[2]);
      if(idx >= mtest.getArrListSize()){
        cout << "Size of array list (" << mtest.getArrListSize() << ") is <= " << idx << "!!" << endl;
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[2]);
      }
      mtest.deleteArr(idx);
      return CMD_EXEC_DONE;
    }
    if(myStrNCmp("-Array", token[2], 2) == 0){
      if(!myStr2Int(token[1], idx))
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[1]);
      if(idx < 0)
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[1]);
      if(idx >= mtest.getArrListSize()){
        cout << "Size of array list (" << mtest.getArrListSize() << ") is <= " << idx << "!!" << endl;
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[1]);
      }
      mtest.deleteArr(idx);
      return CMD_EXEC_DONE;
    }
    return CMD_EXEC_ERROR;
   }
   if(myStrNCmp("-Random", token[0], 2) == 0){
    if(token.size() == 1)
      return CmdExec::errorOption(CMD_OPT_MISSING, token[0]);
    if(token.size() == 2){
      if(!myStr2Int(token[1], idx))
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[1]);
      if(idx <= 0)
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[1]);
        if(mtest.getObjListSize() == 0){
          cout << "Size of object list is 0!!" << endl;
          return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[1]);
        }
      for(int i = 0; i < idx; ++i){
        mtest.deleteObj(rnGen(mtest.getObjListSize()));
      }
      return CMD_EXEC_DONE;
    }
    if(myStrNCmp("-Array", token[1], 2) == 0){
      if(!myStr2Int(token[2], idx))
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[2]);
      if(idx <= 0)
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[2]);
      if(mtest.getArrListSize() == 0){
        cout << "Size of array list is 0!!" << endl;
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[2]);
      }
      for(unsigned i = 0; i < idx; ++i){
        mtest.deleteArr(rnGen(mtest.getArrListSize()));
      }
      return CMD_EXEC_DONE;
    }
    if(myStrNCmp("-Array", token[2], 2) == 0){
      if(!myStr2Int(token[1], idx))
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[1]);
      if(idx <= 0)
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[1]);
      if(mtest.getArrListSize() == 0){
        cout << "Size of array list is 0!!" << endl;
        return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[1]);
      }
      for(unsigned i = 0; i < idx; ++i){
        mtest.deleteArr(rnGen(mtest.getArrListSize()));
      }
      return CMD_EXEC_DONE;
    }
   }
   return CmdExec::errorOption(CMD_OPT_ILLEGAL, token[0]);
   */
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


