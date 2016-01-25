// Simple Expression Evaluation Implementation file
// This describes the functionality for evaluating very simple
// arithmetic expressions.  Given a string consisting of operators
// and operands, it will evaluate that expression and return its value.
//
// The expressions may consist of the following:
// -- single-digit integer values
// -- simple arithmetic operators ( +, -, *, /, % )
// -- matched parentheses for grouping

// This implementation consists of a set of mutually-recursive 
// functions. which will track the structure of the expression.
//
// A sum expression is the sum or difference of one or more products.
// A product expression is the product or quotient of one or more factors.
// A factor may be a number or a parenthesized sum expression.

//I really shouldn't have to add these two lines here so tokenlist.h will build
//Too bad I'm not allowed to change it
#include <iostream>
using namespace std;

#include "tokenlist.h"

void evalStep(ListIterator& iter, int& value);
int  handleParenthesis(ListIterator& iter);
void handleMultiplyLevelOperation(ListIterator& iter, int& curr);
void handleAdditionLevelOperation(ListIterator& iter, int& curr);
bool isNextOperatorMultiplication(ListIterator iter);
int  getValue(ListIterator& iter, bool evalMultiplication);

// evaluate
// Evaluates the entire expression
// Parameters:
//     expression (input string) the expression to evaluate
// Return: Value of the expression
int evaluate(const char expression[])
{
    TokenList expr(expression);
    ListIterator iter = expr.begin();

    int value = getValue(iter,false);
    
    while (iter != expr.end())
    {
        evalStep(iter, value);
    }

    return value;
}

// evalStep
// Evaluates the current operator
// Parameters:
//     iter  - the iterator for the list of tokens
//     value - the running total for the expression
// Pre-condition: Next token is an operator (although, it will just advance if it isn't)
// Post-condition: New value stored in value
//                 Iter is on the last token evaluated in this step (so iter.advance() next time will
//                     bring it to the next thing to be evaluated)
void evalStep(ListIterator& iter, int& value)
{
    iter.advance();
    switch (iter.tokenChar())
    {
    case '*':
    case '/':
    case '%':
        handleMultiplyLevelOperation(iter, value);
        break;
    case '+':
    case '-':
        handleAdditionLevelOperation(iter, value);
        break;
    }
}

// handleParenthesis
// Helper function to handle evaluating parenthesis
// Parameters:
//     iter  - the iterator for the list of tokens
// Pre-condition:  iter.tokenChar() == '('
// Post-condition: iter.tokenChar() == ')'
// Return: Value of expression inside parenthesis
int handleParenthesis(ListIterator& iter)
{
    iter.advance();

    int value = getValue(iter, false);

    while (iter.tokenChar() != ')')
    {
        evalStep(iter, value);
    }

    return value;
}

// handleMultiplyLevelOperation
// Handles *, /, and %
// Parameters:
//     iter  - the iterator for the list of tokens
//     curr  - current running total to be multiplied by the next value
// Pre-condition:  iter.tokenChar() == '*' or '/' or '%'
// Post-condition: iter.tokenChar() == end of what was multiplied (last int/parenthesis)
// Return: value of the operation
void handleMultiplyLevelOperation(ListIterator& iter, int& curr)
{
    char operation = iter.tokenChar();
    iter.advance();

    int nextValue = getValue(iter, false); //Do not handle multiplication (see getValue())

    if (operation == '*')
    {
        curr *= nextValue;
    }
    else if (operation == '/')
    {
        curr /= nextValue;
    }
    else
    {
        curr %= nextValue;
    }
}

// handleMultiplyLevelOperation
// Handles + and -
// Parameters:
//     iter  - the iterator for the list of tokens
//     curr  - current running total to add the next value to
// Pre-condition:  iter.tokenChar() == '+' or '-'
// Post-condition: iter.tokenChar() == end of what was added (last int/parenthesis)
// Return: value of the operation
void handleAdditionLevelOperation(ListIterator& iter, int& curr)
{
    char operation = iter.tokenChar();
    iter.advance();

    int valueToAdd = getValue(iter, true); //Do evaluate multiplication (see getValue())

    if (operation == '+')
    {
        curr += valueToAdd;
    }
    else
    {
        curr -= valueToAdd;
    }
}

// isNextOperatorMultiplication
// Checks if the next operation in the expression has multiplication precedence
// This is used to ensure multiplication is handled before addition
// Parameters:
//     iter - the iterator for the list of tokens - not sent by reference, as in the other
//            functions, so that we can look ahead without having to go back again.
// Pre-condition: Next token is an operator (although, if it's not, I suppose the next operation isn't
//                technically multiplication)
// Return: whether the next operation is multiplication
bool isNextOperatorMultiplication(ListIterator iter)
{
    bool isMultiplication;
    iter.advance();

    switch (iter.tokenChar())
    {
    case '*':
    case '/':
    case '%':
        isMultiplication = true;
        break;
    default:
        isMultiplication = false;
        break;
    }

    return isMultiplication;
}

// getValue
// Gets the value of the number/expression at the current position
// If the current value is an integer, return it
// If it's a parenthetical expression, evaluate, and return
// evalMultiplication is a flag used to ensure multiplication happens before addition
// If true, it will handle any multiplication that is applied to the current value
// That way, what is returned to the addition function is what we actually want to add
// Parameters:
//     iter  - the iterator for the list of tokens
//     evalMultiplication (input boolean) whether to evaluate multiplication, see above
// Pre-condition:  expr[pos] is an int or parenthesis
// Post-condition: expr[pos] is the end of the value
//                 If it was an int, it's on the int
//                 If it was a parenthesis, it's on the end parenthesis
//                 If handling multiplication, it's on the last multiplied int
// Return: value of int/expression
int getValue(ListIterator& iter, bool evalMultiplication)
{
    int value;
    bool negative = false;

    if (iter.tokenChar() == '-')
    {
        negative = true;
        iter.advance();
    }

    if (iter.currentIsInteger())
    {
        value = (negative ? -1 : 1) * iter.integerValue();

        while (evalMultiplication && isNextOperatorMultiplication(iter))
        {
            iter.advance();
            handleMultiplyLevelOperation(iter, value);
        }
    }
    else
    {
        value = handleParenthesis(iter);
    }

    return value;
}
