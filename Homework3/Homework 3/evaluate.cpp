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

#include "tokenlist.h"

void evalStep(ListIterator& iter, TokenList& postExpr);
void handleParenthesis(ListIterator& iter, TokenList& postExpr);
void handleMultiplyLevelOperation(ListIterator& iter, TokenList& postExpr);
void handleAdditionLevelOperation(ListIterator& iter, TokenList& postExpr);
bool isNextOperatorMultiplication(ListIterator iter);
void getValue(ListIterator& iter, TokenList& postExpr, bool evalMultiplication);
void convertToPostfix(TokenList& expr, TokenList& postExpr);

// evaluate
// Evaluates the entire expression
// Parameters:
//     expression (input string) the expression to evaluate
// Pre-condition: The expression is correctly formatted
// Return: Value of the expression
int evaluate(const char expression[])
{
    TokenList expr(expression), //Original expression
              postExpr,         //Expression converted to postfix
              numberStack;      //Stack of numbers to facilitate evaluation

    //Converts expr into postfix and store it into postExpr
    convertToPostfix(expr, postExpr);

    std::cout << "Original expression:          " << expr << std::endl;
    std::cout << "Converted postfix expression: " << postExpr << std::endl;

    
    for (ListIterator iter = postExpr.begin(); iter != postExpr.end(); iter.advance())
    {
        Token t = iter.token();

        if (t.isInteger())
        {
            numberStack.push_front(t);
        }
        else
        {
            //This is the huge bit that handles operators
            //val2 is created but uninitialized to handle binary operators and
            //unary operators ('~') easily in the same switch
            int value = numberStack.pop_front().integerValue();
            int val2 = 0;
            switch (t.tokenChar())
            {
            case '*':
                val2 = numberStack.pop_front().integerValue();
                val2 *= value;
                numberStack.push_front(Token(val2));
                break;
            case '/':
                val2 = numberStack.pop_front().integerValue();
                val2 /= value;
                numberStack.push_front(Token(val2));
                break;
            case '%':
                val2 = numberStack.pop_front().integerValue();
                val2 %= value;
                numberStack.push_front(Token(val2));
                break;
            case '+':
                val2 = numberStack.pop_front().integerValue();
                val2 += value;
                numberStack.push_front(Token(val2));
                break;
            case '-':
                val2 = numberStack.pop_front().integerValue();
                val2 -= value;
                numberStack.push_front(Token(val2));
                break;
            case '~':
                value = -value;
                numberStack.push_front(Token(value));
                break;
            }
        }
    }

    //Value left on the top of the number stack is the final value of the expression
    return numberStack.pop_front().integerValue();
}

// evalStep
// Evaluates the current operator
// Parameters:
//     iter  - the iterator for the list of tokens
//     postExpr - the postfix expression we are converting to
// Pre-condition: Next token is an operator (although, it will just advance if it isn't)
// Post-condition: All values related to this operation pushed to postExpr in the order dictated by postfix
//                 Iter is on the last token evaluated in this step (so iter.advance() next time will
//                     bring it to the next thing to be evaluated)
void evalStep(ListIterator& iter, TokenList& postExpr)
{
    iter.advance();
    switch (iter.tokenChar())
    {
    case '*':
    case '/':
    case '%':
        handleMultiplyLevelOperation(iter, postExpr);
        break;
    case '+':
    case '-':
        handleAdditionLevelOperation(iter, postExpr);
        break;
    }
}

// handleParenthesis
// Helper function to handle evaluating parenthesis
// Parameters:
//     iter  - the iterator for the list of tokens
//     postExpr - the postfix expression we are converting to
// Pre-condition:  iter.tokenChar() == '('
// Post-condition: iter.tokenChar() == ')'
//                 postExpr has everything inside the parenthesis pushed to it
void handleParenthesis(ListIterator& iter, TokenList& postExpr)
{
    iter.advance();

    getValue(iter, postExpr, false);

    while (iter.tokenChar() != ')')
    {
        evalStep(iter, postExpr);
    }
}

// handleMultiplyLevelOperation
// Handles *, /, and %
// Parameters:
//     iter  - the iterator for the list of tokens
//     postExpr - the postfix expression we are converting to
// Pre-condition:  iter.tokenChar() == '*' or '/' or '%'
// Post-condition: iter.tokenChar() == end of what was multiplied (last int/parenthesis)
//                 postExpr has both operands and the operator pushed to it
void handleMultiplyLevelOperation(ListIterator& iter, TokenList& postExpr)
{
    Token operation(iter.token());
    iter.advance();

    getValue(iter, postExpr, false); //Do not handle multiplication (see getValue())

    postExpr.push_back(operation);
}

// handleMultiplyLevelOperation
// Handles + and -
// Parameters:
//     iter  - the iterator for the list of tokens
//     postExpr - the postfix expression we are converting to
// Pre-condition:  iter.tokenChar() == '+' or '-'
// Post-condition: iter.tokenChar() == end of what was added (last int/parenthesis)
//                 postExpr has both operands and the operator pushed to it
void handleAdditionLevelOperation(ListIterator& iter, TokenList& postExpr)
{
    Token operation(iter.token());
    iter.advance();

    getValue(iter, postExpr, true); //Do evaluate multiplication (see getValue())

    postExpr.push_back(operation);
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
//     postExpr - the postfix expression we are converting to
//     evalMultiplication (input boolean) whether to evaluate multiplication, see above
// Pre-condition:  expr[pos] is an int or parenthesis
// Post-condition: expr[pos] is the end of the value
//                     If it was an int, it's on the int
//                     If it was a parenthesis, it's on the end parenthesis
//                     If handling multiplication, it's on the last multiplied int
//                 postExpr has the values handled here pushed to it
void getValue(ListIterator& iter, TokenList& postExpr, bool evalMultiplication)
{
    bool negative = false;

    if (!iter.currentIsInteger() && iter.tokenChar() == '-')
    {
        negative = true;
        iter.advance();
    }

    if (iter.currentIsInteger())
    {
        postExpr.push_back(iter.integerValue());

        if (negative)
        {
            postExpr.push_back(Token('~'));
        }

        while (evalMultiplication && isNextOperatorMultiplication(iter))
        {
            iter.advance();
            handleMultiplyLevelOperation(iter, postExpr);
        }
    }
    else
    {
        handleParenthesis(iter, postExpr);

        if (negative)
        {
            postExpr.push_back(Token('~'));
        }
    }
}

// convertToPostfix
// Converts the expression to postfix
// Parameters:
//     expr     (input  expression) - the expression to convert to postfix
//     postExpr (output expression) - the converted expression
void convertToPostfix(TokenList& expr, TokenList& postExpr)
{
    ListIterator iter = expr.begin();

    getValue(iter, postExpr, false);

    while (iter != expr.end())
    {
        evalStep(iter, postExpr);
    }
}
