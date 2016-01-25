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

#include "tokenize.h"

int evalUntilCharFound(const char expr[], int& pos, char end);
int handleParenthesis(const char expr[], int& pos);
int handleMultiplyLevelOperation(const char expr[], int& pos, int curr);
int handleAdditionLevelOperation(const char expr[], int& pos, int curr);
bool isNextOperatorMultiplication(const char expr[], int pos);
int getValue(const char expr[], int& pos, bool evalMultiplication);

// evaluate
// Evaluates the entire expression
// Parameters:
//     expression (input string) the expression to evaluate
// Pre-condition: String ends with \0
// Return: Value of the expression
int evaluate(const char expression[])
{
    int pos;
    findFirstToken(expression, pos);

    return evalUntilCharFound(expression, pos, '\0');
}

// evalUntilCharFound
// Evaluates the expression until it finds a given character
// Parameters:
//     expr (input        string)    expression to evaluate
//     pos  (input/output integer)   position to start at
//     end  (input        character) charater denoting end of evaluation
// Pre-condition:  pos is at a token
// Post-condition: expr[pos] == end
// Return: Value of the expression from pos to end
int evalUntilCharFound(const char expr[], int& pos, char end)
{
    int curr_value = getValue(expr, pos, false); //Do not handle multiplication (see getValue())

    while (currentToken(expr, pos) != end)
    {
        advance(expr, pos);
        switch (currentToken(expr, pos))
        {
        case '*':
        case '/':
        case '%':
            curr_value = handleMultiplyLevelOperation(expr, pos, curr_value);
            break;
        case '+':
        case '-':
            curr_value = handleAdditionLevelOperation(expr, pos, curr_value);
            break;
        }
    }

    return curr_value;
}

// handleParenthesis
// Helper function to handle evaluating parenthesis
// Parameters:
//     expr (input        string)  expression being evaluated
//     pos  (input/output integer) current position of evaluation
// Pre-condition:  expr[pos] == '('
// Post-condition: expr[pos] == ')'
// Return: Value of expression inside parenthesis
int handleParenthesis(const char expr[], int& pos)
{
    advance(expr, pos);
    return evalUntilCharFound(expr, pos, ')');
}

// handleMultiplyLevelOperation
// Handles * and /
// Parameters:
//     expr (input        string)  expression being evaluated
//     pos  (input/output integer) current position of evaluation
//     curr (input        integer) current value, i.e., the value of what's before the operator
// Pre-condition:  expr[pos] == '*' or '/'
// Post-condition: expr[pos] == end of what was multiplied (last int/parenthesis)
// Return: value of the operation
int handleMultiplyLevelOperation(const char expr[], int& pos, int curr)
{
    char operation = currentToken(expr, pos);

    advance(expr, pos);

    int nextValue = getValue(expr, pos, false); //Do not handle multiplication (see getValue())

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

    return curr;
}

//At current operation. Curr -> everything before it. End on end of last part.
// handleAdditionLevelOperation
// Handles +, -, and %
// Parameters:
//     expr (input        string)  expression being evaluated
//     pos  (input/output integer) current position of evaluation
//     curr (input        integer) current value, i.e., the value of what's before the operator
// Pre-condition:  expr[pos] == '+', '-', or '%'
// Post-condition: expr[pos] == end of what was multiplied (last int/parenthesis)
// Return: value of the operation
int handleAdditionLevelOperation(const char expr[], int& pos, int curr)
{
    char operation = currentToken(expr, pos);

    advance(expr, pos);

    int valueToAdd = getValue(expr, pos, true); //Do evaluate multiplication (see getValue())

    if (operation == '+')
    {
        curr += valueToAdd;
    }
    else
    {
        curr -= valueToAdd;
    }

    return curr;
}

// isNextOperatorMultiplication
// Checks if the next operation in the expression has multiplication precedence
// This is used to ensure multiplication is handled before addition
// Parameters:
//     expr (input string)  expression being evaluated
//     pos  (input integer) current position of evaluation
// Pre-condition:  expr[pos] is an integer
// Return: whether the next operation is multiplication
bool isNextOperatorMultiplication(const char expr[], int pos)
{
    bool isMultiplication;
    advance(expr, pos);

    switch (currentToken(expr, pos))
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
//     expr (input string) the expression being evaluated
//     pos  (intput/output integer) the current position of evaluation
//     evalMultiplication (input boolean) whether to evaluate multiplication, see above
// Pre-condition:  expr[pos] is an int or parenthesis
// Post-condition: expr[pos] is the end of the value
//                 If it was an int, it's on the int
//                 If it was a parenthesis, it's on the end parenthesis
//                 If handling multiplication, it's on the last multiplied int
// Return: value of int/expression
int getValue(const char expr[], int& pos, bool evalMultiplication)
{
    int value;

    if (currentIsInteger(expr, pos))
    {
        value = integerValue(expr, pos);

        while (evalMultiplication && isNextOperatorMultiplication(expr, pos))
        {
            advance(expr, pos);
            value = handleMultiplyLevelOperation(expr, pos, value);
        }
    }
    else
    {
        value = handleParenthesis(expr, pos);
    }

    return value;
}
