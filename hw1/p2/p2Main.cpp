/****************************************************************************
  FileName     [ p2Main.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define main() function ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include "p2Table.h"
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <climits>


using namespace std;

int main()
{
   Table table;

   // Read in the csv file. Do NOT change this part of code.
   string csvFile;
   cout << "Please enter the file name: ";
   cin >> csvFile;
   if (table.read(csvFile))
      cout << "File \"" << csvFile << "\" was read in successfully." << endl;
   else {
      cerr << "Failed to read in file \"" << csvFile << "\"!" << endl;
      exit(-1); // csvFile does not exist.
   }

   // TODO read and execute commands
   while (true) {

   string str;
   	
   getline(cin,str);
   	
   int pivot = str.find(' ',0);
   string tmp;
   tmp = tmp.assign(str,0,pivot);
   str.erase(str.begin(),str.begin()+pivot+1);
   	
   if(tmp == "PRINT" || tmp == "print")
   	
      table.print();
   	
   if(tmp == "SUM" || tmp == "sum"){
   		
      table.getSum(str);
   		
   }
	
   if(tmp == "MAX" || tmp == "max"){
		
      int num = atoi(str.c_str());

      if(table.getMax(num) == INT_MAX)

         cout << "Error: This is a NULL column!!" << endl;

      else
		
         cout << "The maximum of data in column #" << num << " is " << table.getMax(num) << "." << endl;
			
   }
	
   if(tmp == "MIN" || tmp == "min"){
		
      int num = atoi(str.c_str());

      if(table.getMin(num) == INT_MAX)

         cout << "Error: This is a NULL column!!" << endl;

      else
		
         cout << "The minimum of data in column #" << num << " is " << table.getMin(num) << "." << endl;
		
   }
	
   if(tmp == "DIST" || tmp == "dist"){
		
      int num = atoi(str.c_str());

      if(table.getDist(num) == INT_MAX)

         cout << "Error: This is a NULL column!!" << endl;

      else
		
         cout << "The distinct count of data in column #" << num << " is " << table.getDist(num) << "." << endl;
		
   }
   	
   if(tmp == "AVE" || tmp == "ave"){
   		
      int num = atoi(str.c_str());

      if(table.getAve(num) == INT_MAX)

         cout << "Error: This is a NULL column!!" << endl;

      else
   		
         cout << "The average of data in column #" << num << " is " << fixed << setprecision(1) << table.getAve(num) << "." << endl;
   		
   }
	
   if(tmp == "ADD" || tmp == "add"){
		
      str += " ";
      table.add(str);
			
   }
	
   if(tmp == "EXIT" || tmp == "exit")
      return 0;	

   }
}
