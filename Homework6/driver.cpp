#include <iostream>
#include <string>
using namespace std;
#include "evaluate.h"

#ifndef DEBUG
#define endfunction() do { } while(0)
#else
#define endfunction() cout << endl;
#endif

int main()
{
    //char userInput[80];
    VarTree vars;		// initially empty tree
    FunctionDef funs;

    string input;
    cout << "Interactive? (y|N): ";
    cout.flush();

    getline(cin, input);
    while (input != "y" && input != "Y" && input != "n" && input != "N" && input != "")
    {
        cout << "Interactive? (y|N): ";
        cout.flush();
        getline(cin, input);
    }

    if (input == "")
        input = "N";

    if (input == "n" || input == "N")
    {
        evaluate("deffn sqr(x)=x*x", vars, funs);
        endfunction();

        evaluate("deffn abs(x)=x>0?x:-x", vars, funs);
        endfunction();

        evaluate("deffn fact(n)=n<=1?1:n*fact(n-1)", vars, funs );
        endfunction();

        evaluate("deffn add(a,b)=a+b",vars,funs);
        endfunction();

        evaluate("deffn three()=3", vars, funs);
        endfunction();

        cout << "sqr(5): " << evaluate("sqr(5)", vars, funs) << endl;
        endfunction();

        cout << "Three = abs(-3): " << evaluate("Three =  abs(-3) ",vars, funs) << endl;
        endfunction();

        cout << "Fact(3): " << evaluate("fact(3)", vars, funs) << endl;
        endfunction();

        cout << "Fact(5): " << evaluate("fact(5)", vars, funs) << endl;
        endfunction();

        cout << "3 + add(2,4): " << evaluate("3 + add(Three - 1,Three + 1)", vars, funs) << endl;
        endfunction();

        cout << "three() * 9 + Three: " << evaluate("three() * 9 + Three", vars, funs) << endl;
        endfunction();
        /*cout << endl << "Try one yourself:  ";
        cin.getline(userInput,80);
        cout << userInput << " = " << evaluate(userInput, vars, funs) << endl;
        cout << endl << "Try another:  ";
        cin.getline(userInput,80);
        cout << userInput << " = " << evaluate(userInput, vars, funs) << endl;*/
    }
    else
    {
        cout << "Use an empty line to quit." << endl;
        cout << "A return of 0 on a function definition indicates success." << endl << endl;
        do {
            cout << "> ";
            cout.flush();
            getline(cin, input);
            if (input != "") cout << evaluate(input.c_str(), vars, funs) << endl << endl;
        } while (input != "");
    }
}
