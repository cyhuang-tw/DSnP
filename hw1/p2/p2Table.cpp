/****************************************************************************
  FileName     [ p2Table.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Row and Table ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include "p2Table.h"
#include <fstream> 
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <climits>


using namespace std;

// Implement member functions of class Row and Table here
bool
Table::read(const string& csvFile)
{
   fstream file;
   string str;
   int row_num = 0, column_num = 0; //the number of rows and columns.	
   file.open(csvFile.c_str(), ios::in);
   if(!file.is_open()) return false; //the case that the file is NOT open correctly.
	
   while(!file.eof()){

      string tmp;

      getline(file, tmp);

      str += tmp;

      if(tmp[tmp.length() - 1] != '\r')

         str += '\r';

   }

   int idx = str.length() - 1;

   while(str[idx - 1] == '\r'){

      idx--;

   }

   str.erase(idx + 1);

   row_num = count(str.begin(), str.end(), '\r');
   column_num = count(str.begin(), str.end(), ',')/row_num + 1;

   _nCols = column_num;

   for (int i = 0; i < row_num; ++i){
      int row_pivot = str.find('\r',0);	//find the position of '^M' 
      string tmp;
      tmp = tmp.assign(str, 0, row_pivot);	//store the row to another string temporarily.
      str.erase(str.begin(), str.begin()+row_pivot+1);	//delete the data of the row stored to the temporary string.
      tmp+=',';
		 
      Row row_data(column_num);
		
      for (int j = 0; j < column_num; ++j){
         int col_pivot = tmp.find(',',0);	//find the position of ','.
         string num;
         num = num.assign(tmp ,0 ,col_pivot);	//store the number to another string.
         tmp.erase(tmp.begin(), tmp.begin()+col_pivot+1);	//delete the stored string.
			
         int val = INT_MAX;
         if(num.length() != 0) val = atoi(num.c_str());
			
         row_data[j] = val;
      }
		
      _rows.push_back(row_data); 
   }
 
   file.close();


   return true;
}

void Table::print(){
	
   for(int i = 0; i < _rows.size(); ++i){
		
      for(int j = 0; j < _nCols; ++j){
			
         if(_rows[i][j] == INT_MAX){
				
            cout << setw(4) << right << ".";				
				
         }else

         cout << setw(4) << right << _rows[i][j];

      }
      cout << endl;
   }	
}

void Table::getSum(const string str){

   int num = atoi(str.c_str());
	
   int sum = 0;
   int emp = 0;
	
   for(int i = 0; i < _rows.size(); ++i){
		
      if (_rows[i][num] == INT_MAX){

         emp++;

         continue;

		}
		
      sum += _rows[i][num];
   }
	
   if(emp == _rows.size())

      cout << "Error: This is a NULL column!!" << endl;

   else

      cout << "The summation of data in column #" << num <<" is " << sum << "." << endl;

	
}

int Table::getMax(const int num){
	
   int max_num = INT_MIN;
   int emp = 0;
	
   for(int i = 0; i < _rows.size(); ++i){
		
      if(_rows[i][num] > max_num && _rows[i][num] != INT_MAX)

         max_num = _rows[i][num];

      if(_rows[i][num] == INT_MAX)

         emp++;
		
   }

   if(emp == _rows.size())

      return INT_MAX;
	
   return max_num;
	
}


int Table::getMin(const int num){
	
   int min_num = INT_MAX;
	
   for(int i = 0; i < _rows.size(); ++i){
		
      if(_rows[i][num] < min_num && _rows[i][num] != INT_MAX)
		
         min_num = _rows[i][num];
		
   }
	
   return min_num;
	
}

int Table::getDist(const int num){
	
   int val = 0;
   int emp = 0;

   for(int i = 0; i < _rows.size(); ++i){

      bool repeat = false;

      if(_rows[i][num] == INT_MAX){

         emp++;

         continue;

      }

      for(int j = 0; j < i; ++j){

         if(_rows[j][num] == _rows[i][num]){

            repeat = true;

            break;
         }

      }

      if(!repeat)
         val++;
		
   }

   if(emp == _rows.size())

      return INT_MAX;
	
   return val;
	
}

float Table::getAve(const int num){
	
   float avg = 0, emp = 0;
	
   for(int i = 0; i < _rows.size(); ++i){
		
      if(_rows[i][num] != INT_MAX)

         avg += _rows[i][num];
		
         else
		
         emp++;
		
   }

   if(emp == _rows.size())

      return INT_MAX;
	
   avg /= (_rows.size() - emp);
	
   return avg;
	
}

void Table::add(const string& cmd){
	
   string str = cmd;
	
   Row row_data(_nCols);
	
   for(int i = 0; i < _nCols; ++i){
		
      string tmp;
      int pivot = str.find(' ',0);
      tmp = tmp.assign(str,0,pivot);

      str.erase(str.begin(),str.begin()+pivot+1);
		
      int val = 0;

      if(tmp.length() == 1 && tmp[0] == '.')

         val = INT_MAX;

      else

         val = atoi(tmp.c_str());
		
      row_data[i] = val;
		
   }
	
   _rows.push_back(row_data);
	
}

