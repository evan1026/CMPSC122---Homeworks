// Expression Tree Implementation File
// These functions manage an expression tree that describes
// arithmetic expressions using integer variables and values.
// Some functions construct a tree from a character string that
// represents the expression, and then the tree can be traversed
// and evaluated.
#include <iostream>
#include <sstream>
using namespace std;
#include "exprtree.h"
#include "tokenlist.h"

// Outputting any tree node will simply output its string version
ostream& operator<<( ostream &stream, const ExprNode &e )
{
    return stream << e.toString();
}

// A Value is just an integer value -- easy to evaluate
// Unfortunately, the string class does not have a constructor for it
string Value::toString() const
{
    ostringstream convert;	// output string stream
    convert << value;		// 'output' into the stream
    return convert.str();	// and extract its string equivalent
}

int Value::evaluate( VarTree &v ) const
{
    return value;
}

string Value::makedc() const
{
    return toString() + " ";
}

//  A variable is just an alphabetic string -- easy to display
//  To evaluate, would need to look it up in the data structure
string Variable::toString() const
{
    return name;
}

int Variable::evaluate( VarTree &v ) const
{
    return v.lookup( name );
}

string Variable::makedc() const
{
    stringstream output;
    output << name[0]; //registers can only be one character long, so I'm making sure we only use one character for the register name
    return output.str();
}


string Operation::toString() const
{
    stringstream output;

    output << left->toString();

    if (right != NULL)
        output << " " << right->toString();

    output << " " << oper;

    return output.str();
}

int Operation::evaluate(VarTree& v) const
{
    if (oper == "~") {

        return -left->evaluate(v); //Right value will be NULL, as is handled when adding these values

    } else if (oper == "=") {
        int value = right->evaluate(v);

        v.assign(left->toString(), value);

        return value;

    } else if (oper == "+") {

        return left->evaluate(v) + right->evaluate(v);

    } else if (oper == "-") {

        return left->evaluate(v) - right->evaluate(v);

    } else if (oper == "*") {

        return left->evaluate(v) * right->evaluate(v);

    } else if (oper == "/") {

        return left->evaluate(v) / right->evaluate(v);

    } else if (oper == "%") {

        return left->evaluate(v) % right->evaluate(v);

    } else if (oper == ">") {

        return left->evaluate(v) > right->evaluate(v);

    } else if (oper == "<") {

        return left->evaluate(v) < right->evaluate(v);

    } else if (oper == ">=") {

        return left->evaluate(v) >= right->evaluate(v);

    } else if (oper == "<=") {

        return left->evaluate(v) <= right->evaluate(v);

    } else if (oper == "==") {

        return left->evaluate(v) == right->evaluate(v);

    } else if (oper == "!=") {

        return left->evaluate(v) != right->evaluate(v);

    } else {
        cout << "Operation \"" << oper << "\" not recognized." << endl;
        return 0;
    }
}

string Operation::makedc() const
{
    stringstream output;
    Variable* leftvar  = dynamic_cast<Variable *>(left);  //Casts to a variable, returning null if it is not actually of type Variable
    Variable* rightvar = dynamic_cast<Variable *>(right); //This allows us to test later if left is a variable or a value

    if (oper == "~")
    {
        output << "_";
        
        if (leftvar)
            output << "l";
        output << left->makedc();

    }
    else if (oper == "=")
    {    
        if (rightvar)
            output << "l";
        output << right->makedc();

        output << "s" << left->makedc();
        output << "l" << left->makedc(); //This loads the value back on to the stack.
                                         //Otherwise, a statement like A = 2 + (B = 3) would fail
                                         //This also allows us to print the value of the last assignment easily
    }
    else if (oper == ">" || oper == "<" || oper == ">=" || oper == "<=" || oper == "==" || oper == "!=")
    {
        output << " Comparison operators not supported. "; //This is because comparison operators cannot run without
                                                           //running a macro stored in a register, which would be
                                                           //too difficult to implement acceptably
    }
    else
    {
        if (leftvar)
            output << "l";
        output << left->makedc();

        if (rightvar)
            output << "l";
        output << right->makedc();

        output << oper;
    }

    return output.str();
}


string Conditional::toString() const
{
    return "((" + test->toString() + ") ? (" + trueCase->toString() + ") : (" + falseCase->toString() + "))";
}

int Conditional::evaluate(VarTree& v) const
{
    return test->evaluate(v) ? trueCase->evaluate(v) : falseCase->evaluate(v);
}

string Conditional::makedc() const
{
    return " Conditional operator not supported. ";
}
