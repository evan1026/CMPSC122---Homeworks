// Simple Expression Evaluation 
// This program will evaluate simple arithmetic expressions
// represented as a linked-list of tokens.  Keyboard input
// will be accepted into a string, which will be converted
// into that linked list.
//
// If the first symbol in the input string is an operator,
// then the value of the previous expression will be taken
// as an implicit first operand.
//
// The expressions may consist of the following:
// -- integer values (which may have multiple digits)
// -- simple arithmetic opers ( +, -, *, /, % )
// -- matched parentheses for grouping

// This implementation consists of a set of mutually-recursive 
// functions. which will track the structure of the expression.
//
// A sum expression is the sum or difference of one or more products.
// A product expression is the product or quotient of one or more factors.
// A factor may be a number or a parenthesized sum expression.

#include <iostream>
#include "tokenlist.h"
#include "vartree.h"

using namespace std;

// three functions to convert an infix expression to postfix
void assignmentToPostfix(ListIterator& infix, TokenList& result);
void sumToPostfix(ListIterator& infix, TokenList& result);
void prodToPostfix(ListIterator& infix, TokenList& result);
void factorToPostfix(ListIterator& infix, TokenList& result);
bool isOperator(Token t);
int evaluatePostfix(TokenList& t, VarTree& vars);

// Evaluate
// Tokenizes the string, converts to post-fix order, and evaluates that
// Parameters:
//     str (input char array) - string to evaluate
// Pre-condition:  str must be a valid integer arithmetic expression including matching parentheses.
int evaluate(const char str[], VarTree &vars)
{
    TokenList postfixExpr;
    TokenList list(str);
    ListIterator iter = list.begin();

    assignmentToPostfix(iter, postfixExpr);
    //cout << "Original:  " << list << endl;
    cout << "Converted: " << postfixExpr << endl;
    return evaluatePostfix(postfixExpr, vars);
}

// evaluatePostfix
// Evaluates a postfix expression that operates on integers.
// Parameter:
//     postfix (input Token list) - expression to convert
// Returns:    (integer) - value of the expression
int evaluatePostfix(TokenList &t, VarTree& vars)
{
    TokenList values;
    Token operand1, operand2;
    int  value1, value2;
    int  calc;

    for (ListIterator curr = t.begin(); curr != t.end(); curr.advance())
    {
        if (!isOperator(curr.token())) // Must be a number or a variable if it's not an operator
            values.push_front(curr.token());
        else if (curr.token().tokenChar() == '~')
        {
            operand1 = values.pop_front();

            // Grab operand1's value
            if (operand1.isInteger())
                value1 = operand1.integerValue();
            else
                value1 = vars.lookup(operand1.tokenText());

            values.push_front(Token(-value1));
        }
        else if (curr.token().tokenChar() == '=')
        {
            operand2 = values.pop_front(); // The value being assigned
            operand1 = values.pop_front(); // The variable to assign it to

            // Grab operand2's value
            if (operand2.isInteger())
                value2 = operand2.integerValue();
            else
                value2 = vars.lookup(operand2.tokenText());

            vars.assign(operand1.tokenText(), value2);

            values.push_front(Token(value2)); // Push back on the result of the assignment
        }
        else
        {
            operand2 = values.pop_front();
            operand1 = values.pop_front();

            // Grab operand1's value
            if (operand1.isInteger())
                value1 = operand1.integerValue();
            else
                value1 = vars.lookup(operand1.tokenText());

            // Grab operand2's value
            if (operand2.isInteger())
                value2 = operand2.integerValue();
            else
                value2 = vars.lookup(operand2.tokenText());

            // Handle operation
            switch (curr.token().tokenChar()) {
                case '+': 
                    calc = value1 + value2;
                    break;
                case '-': 
                    calc = value1 - value2;
                    break;
                case '*':
                    calc = value1 * value2;
                    break;
                case '/':
                    calc = value1 / value2;
                    break;
                case '%': 
                    calc = value1 % value2;
                    break;
                default: cout << "Operator " << curr.token().tokenChar() << " not recognized."; // This should never run since all non-operators are assumed to be variables!!!
            }
            values.push_front(Token(calc));
        }
    }

    // In the case of the expression being just a variable, the last element left could be a variable, so we must account for this
    operand1 = values.pop_front();

    if (operand1.isInteger())
        value1 = operand1.integerValue();
    else
        value1 = vars.lookup(operand1.tokenText());

    return value1;
}

// assignmentToPostfix
// Converts an infix assignment expression into a postfix expression.
// Both expressions are represented as linked lists of tokens
// Parameters:
//     infix  (input Token list iterator) - expression to convert
//     result (modified Token list)       - result in postfix
void assignmentToPostfix(ListIterator &infix, TokenList &result)
{
    char oper = 0;
    if (infix.tokenChar() == '-')    // if negative - This would count as improper formatting but I'll leave this in for the sake of keeping it from crashing
    {
        infix.advance();
        sumToPostfix(infix, result);
        result.push_back(Token('~'));
    }
    else
        sumToPostfix(infix, result);

    oper = infix.tokenChar();
    while (oper == '=')
    {
        infix.advance();
        assignmentToPostfix(infix, result); // Assignment MUST evaluate right to left, or else we try to assign to an r-value. This accounts for that.
        result.push_back(Token(oper));
        oper = infix.tokenChar();
    }
}

// sumToPostfix
// Converts an infix sum expression into a postfix expression.
// Both expressions are represented as linked lists of tokens
// Parameters:
//     infix  (input Token list iterator) - expression to convert
//        result (modified Token list)          - result in postfix
void sumToPostfix(ListIterator &infix, TokenList &result)
{
    char oper = 0;
    if (infix.tokenChar() == '-')
    {
        infix.advance();
        prodToPostfix(infix, result);
        result.push_back(Token('~'));
    }
    else
        prodToPostfix(infix, result);

    oper = infix.tokenChar();
    while (oper == '+' || oper == '-')
    {
        infix.advance();
        prodToPostfix(infix, result);
        result.push_back(Token(oper));
        oper = infix.tokenChar();
    }
}

// prodToPostfix
// Translates an infix product expression to postfix
// Both expressions are represented as linked lists of tokens
// Parameters:
//     infix  (input Token list iterator) - expression to convert
//     result (modified Token list)          - result in postfix
void prodToPostfix(ListIterator &infix, TokenList &result)
{
    char oper = 0;
    factorToPostfix(infix, result);
    oper = infix.tokenChar();
    while (oper == '*' || oper == '/' || oper == '%')
    {
        infix.advance();
        factorToPostfix(infix, result);
        result.push_back(Token(oper));
        oper = infix.tokenChar();
    }
}

// Factor to Postfix
// Translates a factor from infix to postfix
// A factor may either be a number or parenthesized expression.
// Both expressions are represented as linked lists of tokens
// Parameters:
//     infix  (input Token List iterator) - expression to convert
//     result (modified Token list)       - result in postfix
void factorToPostfix(ListIterator &infix, TokenList &result)
{
    if (!isOperator(infix.token()))
    {
        result.push_back(infix.token());
        infix.advance();
    }
    else
    {
        infix.advance();        // go past assumed (
        assignmentToPostfix(infix, result);
        infix.advance();        // go past assumed )
    }
}

// isOperator
// Tells whether the token is an operator.
// This function knows all of the operators used in the postfix expression.
// Everything else is assumed to be either a variable or an integer
// Parameters:
//     t (input Token) - token to check if it's an operator
// Returns:
//     (bool) - whether or not the token is an operator
bool isOperator(Token t)
{
    string text = t.tokenText();
    return (text == "+" || text == "-" || text == "*" || text == "/" || text == "%" || text == "(" || text == ")" || text == "=" || text == "~");
}