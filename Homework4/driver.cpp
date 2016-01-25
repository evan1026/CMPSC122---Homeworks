#include <iostream>
using namespace std;
#include "evaluate.h"
int main()
{
    char userInput[80];
    VarTree vars;        // initially empty tree

    cout << "Five = 2 + 3 : " << evaluate("Five = 2 + 3", vars) << endl;         //5
    cout << vars << endl << endl;                                                //Five = 5
    cout << "Five * 4 : " << evaluate(" Five * 4 ", vars) << endl;               //20
    cout << vars << endl << endl;                                                //Five = 5
    cout << "Age = 2 + 3 * Five : " << evaluate("Age = 2+3*Five", vars) << endl; //17
    cout << vars << endl << endl;                                                //Five = 5, Age = 17
    cout << "A = 3 + (B = 2) : " << evaluate("A = 3 + (B = 2)", vars) << endl;   //5
    cout << vars << endl << endl;                                                //Five = 5, Age = 17, B = 2, A = 5
    cout << "A * B : " << evaluate("A * B", vars) << endl;                       //10
    cout << vars << endl << endl;                                                //Five = 5, Age = 17, B = 2, A = 5
    cout << "C = D = A : " << evaluate("C = D = A", vars) << endl;               //5
    cout << vars << endl << endl;                                                //Five = 5, Age = 17, B = 2, A = 5, D = 5, C = 5
    cout << "E = F + (F = 1) : " << evaluate("E = F + (F = 1)", vars) << endl;   //2
    cout << vars << endl << endl;                                                //Five = 5, Age = 17, B = 2, A = 5, D = 5, C = 5, F = 1, E = 2
    /*cout << endl << "Try one yourself:  ";
    cin.getline(userInput,80);
    cout << userInput << " = " << evaluate(userInput, vars) << endl;
    cout << endl << "Try another:  ";
    cin.getline(userInput,80);
    cout << userInput << " = " << evaluate(userInput, vars) << endl;*/

    /*vars.assign("rawr",  1);
    vars.assign("poop",  2);
    vars.assign("cake",  3);
    vars.assign("apple", 4);
    vars.assign("ten",   5);
    vars.assign("q",     6);
    vars.assign("car",   7);
    vars.assign("name",  8);

    cout << vars;*/

}
