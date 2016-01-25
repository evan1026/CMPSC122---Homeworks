#include <iostream>
using namespace std;
#include "evaluate.h"
int main()
{
    char userInput[80];
    cout << "2 + 3 * 4 = " << evaluate("2+3*4") << endl;
    cout << "(2+3) * 4 = " << evaluate("  (2+3) * 4") << endl;
    cout << endl << "Try one yourself:  ";
    cin.getline(userInput,80);
    cout << userInput << " = " << evaluate(userInput) << endl;
    cout << endl << "Try another:  ";
    cin.getline(userInput,80);
    cout << userInput << " = " << evaluate(userInput) << endl;
}
