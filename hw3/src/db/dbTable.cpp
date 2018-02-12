/****************************************************************************
  FileName     [ dbTable.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Table member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <string>
#include <cctype>
#include <cassert>
#include <set>
#include <algorithm>
#include <cfloat>
#include "dbTable.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream& operator << (ostream& os, const DBRow& r)
{
   // TODO: to print out a row.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
  for(size_t i = 0; i < r.size(); ++i){

      if(r[i] == INT_MAX)

         cout << ".";

      else

         cout << r[i];

      if(i != r.size() - 1)

         cout << " ";

   }
   return os;
}

ostream& operator << (ostream& os, const DBTable& t)
{
   // TODO: to print out a table
   // - Data are seperated by setw(6) and aligned right.
   // - Null cells are printed as '.'
  for(size_t i = 0; i < t.nRows(); ++i){

      for(size_t j = 0; j < t[i].size(); ++j){

         if(t[i][j] == INT_MAX)

            cout << setw(6) << right << ".";

         else

            cout << setw(6) << right << t[i][j];

      }

      cout << endl;

   }
   return os;
}

ifstream& operator >> (ifstream& ifs, DBTable& t)
{
   // TODO: to read in data from csv file and store them in a table
   // - You can assume the input file is with correct csv file format
   // - NO NEED to handle error file format
  string str;
   string tmpStr;

   unsigned int row_num = 0, column_num = 0; //the number of rows and columns.

   while(!ifs.eof()){

      getline(ifs,tmpStr);

      if(tmpStr[tmpStr.length() - 1] != '\r') tmpStr += '\r';

      str += tmpStr;

   }

   int idx = str.length() - 1;

   while(str[idx - 1] == '\r'){

      idx--;

   }

   str.erase(idx + 1);
  
   row_num = count(str.begin(), str.end(), '\r');
   column_num = count(str.begin(), str.end(), ',')/row_num + 1;

   for (size_t i = 0; i < row_num; ++i){
      int row_pivot = str.find('\r',0); //find the position of '^M'. 
      string tmp;
      tmp = tmp.assign(str, 0, row_pivot);  //store the row to another string temporarily.
      str.erase(str.begin(), str.begin()+row_pivot+1);  //delete the data of the row stored to the temporary string.
      tmp+=',';
     
      vector<int> _rowData;
    
      for (size_t j = 0; j < column_num; ++j){
         int col_pivot = tmp.find(',',0); //find the position of ','.
         string num;
         num = num.assign(tmp ,0 ,col_pivot); //store the number to another string.
         tmp.erase(tmp.begin(), tmp.begin()+col_pivot+1); //delete the stored string.
      
         int val = INT_MAX;
         if(num.length() != 0) val = atoi(num.c_str());

         _rowData.push_back(val);

      }
    
      DBRow _row(_rowData);

      t.addRow(_row); 

  }
 
   ifs.close();

   return ifs;
}

/*****************************************/
/*   Member Functions for class DBRow    */
/*****************************************/
void
DBRow::removeCell(size_t c)
{
   // TODO
  for(int i = c; i < _data.size() - 1; ++i)
      _data[i] = _data[i+1];
   _data.pop_back();
}

/*****************************************/
/*   Member Functions for struct DBSort  */
/*****************************************/
bool
DBSort::operator() (const DBRow& r1, const DBRow& r2) const
{
   // TODO: called as a functional object that compares the data in r1 and r2
   //       based on the order defined in _sortOrder
  for(size_t i = 0; i < _sortOrder.size(); ++i){

      if(r1[_sortOrder[i]] != r2[_sortOrder[i]])

         return (r1[_sortOrder[i]] < r2[_sortOrder[i]]);

   }

   return false;
}

/*****************************************/
/*   Member Functions for class DBTable  */
/*****************************************/
void
DBTable::reset()
{
   // TODO
  _table.clear();

}

void
DBTable::addCol(const vector<int>& d)
{
   // TODO: add a column to the right of the table. Data are in 'd'.
  for(size_t i = 0; i < _table.size(); ++i){

      _table[i].addData(d[i]);

   }
}

void
DBTable::delRow(int c)
{
   // TODO: delete row #c. Note #0 is the first row.
  for(size_t i = c; i < _table.size() - 1; ++i){

      _table[i] = _table[i + 1];

   }

   _table.pop_back();
}

void
DBTable::delCol(int c)
{
   // delete col #c. Note #0 is the first row.
   for (size_t i = 0, n = _table.size(); i < n; ++i)
      _table[i].removeCell(c);
}

bool
DBTable::isEmpty(size_t c) const
{

   int emptyNum = 0;

   for(int i = 0; i < _table.size(); ++i){

      if(_table[i][c] == INT_MAX)

         emptyNum++;

   }

   return (emptyNum == _table.size());

}

// For the following getXXX() functions...  (except for getCount())
// - Ignore null cells
// - If all the cells in column #c are null, return NAN
// - Return "float" because NAN is a float.
float
DBTable::getMax(size_t c) const
{
   // TODO: get the max data in column #c
   if(isEmpty(c))

      return nanf("");

   float maxNum = FLT_MIN;

   for(int i = 0; i < _table.size(); ++i){

      if(_table[i][c] > maxNum && _table[i][c] != INT_MAX)

         maxNum = _table[i][c];

   }

   return maxNum;
}

float
DBTable::getMin(size_t c) const
{
   // TODO: get the min data in column #c
   if(isEmpty(c))

      return nanf("");

   float minNum = FLT_MAX;

   for(int i = 0; i < _table.size(); ++i){

      if(_table[i][c] < minNum && _table[i][c] != INT_MAX)

         minNum = _table[i][c];

   }

   return minNum;
}

float 
DBTable::getSum(size_t c) const
{
   // TODO: compute the sum of data in column #c
   if(isEmpty(c))

      return nanf("");

   float sum = 0;

   for(int i = 0; i < _table.size(); ++i){

      if(_table[i][c] != INT_MAX)

         sum += _table[i][c];

   }

   return sum;
}

int
DBTable::getCount(size_t c) const
{
   // TODO: compute the number of distinct data in column #c
   // - Ignore null cells
   if(isEmpty(c))

      return 0;

   int count = 0;

   for(int i = 0; i < _table.size(); ++i){

      bool repeat = false;

      if(_table[i][c] != INT_MAX){

         for(int j = 0; j < i; ++j){

            if(_table[j][c] == _table[i][c]){

               repeat = true;

               break;

            }

         }

      if(!repeat)

         count++;

      }
    
   }
  
   return count;
}

int
DBTable::getEmptyNum(size_t c) const
{

   int emptyNum = 0;

   for(int i = 0; i < _table.size(); ++i){

      if(_table[i][c] == INT_MAX)

         ++emptyNum;

   }

   return emptyNum;

}

float
DBTable::getAve(size_t c) const
{
   // TODO: compute the average of data in column #c
   if(isEmpty(c))

      return nanf("");

   float avg = 0;
  
   for(int i = 0; i < _table.size(); ++i){
    
      if(_table[i][c] != INT_MAX)

         avg += _table[i][c];
  
   }
  
   avg /= (nRows() - getEmptyNum(c));
  
   return avg;
}

void
DBTable::sort(const struct DBSort& s)
{
   // TODO: sort the data according to the order of columns in 's'
  ::sort(_table.begin(),_table.end(),s);
}

void
DBTable::printCol(size_t c) const
{
   // TODO: to print out a column.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
  for(size_t i = 0; i < _table.size(); ++i){

      if(_table[i][c] == INT_MAX)

         cout << ".";

      else

         cout << _table[i][c];

      if(i != _table.size() - 1)

         cout << " ";

   }
}

void
DBTable::printSummary() const
{
   size_t nr = nRows(), nc = nCols(), nv = 0;
   for (size_t i = 0; i < nr; ++i)
      for (size_t j = 0; j < nc; ++j)
         if (_table[i][j] != INT_MAX) ++nv;
   cout << "(#rows, #cols, #data) = (" << nr << ", " << nc << ", "
        << nv << ")" << endl;
}

