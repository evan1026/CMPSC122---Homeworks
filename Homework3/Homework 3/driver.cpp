#include <iostream>
using namespace std;
#include "evaluate.h"
int main()
{
    char userInput[80];

    int value = evaluate("2+3*4");
    cout << "2 + 3 * 4 = " << value << endl;

    value = evaluate("  (2+3) * 4");
    cout << "(2+3) * 4 = " << value << endl;

    cout << endl << "Try one yourself:  ";
    cin.getline(userInput,80);
    value = evaluate(userInput);
    cout << userInput << " = " << value << endl;

    cout << endl << "Try another:  ";
    cin.getline(userInput,80);
    value = evaluate(userInput);
    cout << userInput << " = " << value << endl;
}
