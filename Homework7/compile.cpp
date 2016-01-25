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
#include "exprtree.h"
#include "funmap.h"
#include "machine.h"
#include "compile.h"

using namespace std;

ExprNode* conditionalToTree(ListIterator& infix, TokenList& list, FunctionDef& funs);
ExprNode* testToTree       (ListIterator& infix, TokenList& list, FunctionDef& funs);
ExprNode* assignmentToTree (ListIterator& infix, TokenList& list, FunctionDef& funs);
ExprNode* sumToTree        (ListIterator& infix, TokenList& list, FunctionDef& funs);
ExprNode* prodToTree       (ListIterator& infix, TokenList& list, FunctionDef& funs);
ExprNode* factorToTree     (ListIterator& infix, TokenList& list, FunctionDef& funs);
void      makeFunction     (ListIterator& infix, TokenList& list, FunctionDef& funs);
bool isOperator(Token t);
string tokenText(ListIterator& infix, TokenList& list);

// Evaluate
// Tokenizes the string, converts to post-fix order, and evaluates that
// Parameters:
//     str (input char array) - string to evaluate
// Pre-condition:  str must be a valid integer arithmetic expression including matching parentheses.
void compile(const char str[], VarTree &vars, FunctionDef& funs, Instruction *prog[],
        int& pBegin, int& pEnd)
{
    TokenList list(str);
    ListIterator iter = list.begin();

    int tempCounter = 0;

    if (tokenText(iter,list) == "deffn")
    {
        //cout << list << endl;
        makeFunction(iter, list, funs);
        //return 0;
    }
    else
    {
        ExprNode* root = assignmentToTree(iter,list,funs);
#ifdef DEBUG
        cout << *root << endl;
#endif
        //return root->evaluate(vars, funs);
        int tempCounter = 0;
        int answerReg = root->toInstruction(prog, pEnd, tempCounter, vars);
        pBegin = 0;

        prog[pEnd++] = new Print(answerReg);
    }

    //cout << root->makedc() << endl
    //cout << *root << endl;
}

void makeFunction(ListIterator& infix, TokenList& list, FunctionDef& funs)
{
    infix.advance(); //advance past deffn

    string name = tokenText(infix, list);
    funs[name] = FunDef();

    FunDef* function = &funs[name]; //to avoid multiple lookups in this function body

    function->name = name;
    infix.advance(); //advance past function name
    infix.advance(); //advance past '('

    function->locals = new VarTree();

    int paramcount = 0;
    while (tokenText(infix, list) != ")")
    {
        string paramname = tokenText(infix, list);
        function->parameter[paramcount] = paramname;
        function->locals->assign(paramname, 0);
        ++paramcount;
        infix.advance();

        if (tokenText(infix, list) == ",")
            infix.advance();

    } //we are now on a ")"
    infix.advance(); //so advance past ")"

    for (int i = paramcount; i < 10; ++i)
        function->parameter[i] = "";

    function->functionBody = assignmentToTree(infix,list,funs);

#ifdef DEBUG
    cout << "Function:" << endl;
    cout << "    Name: " << function->name << endl;
    for (int i = 0; i < 10 && function->parameter[i] != ""; ++i)
        cout << "    Parameter " << i << ": " << function->parameter[i] << endl;
    cout << "    VarTree: " << function->locals << endl;
    cout << "    VarTree: " << *function->locals << endl;
    cout << "    ExprNode: " << function->functionBody << endl;
    cout << "    ExprNode: " << *function->functionBody << endl;
#endif
}

// assignmentToTree
// Converts an infix assignment expression into a tree
// Parameters:
//     infix    (input Token list iterator)  - expression to convert
// Returns:
//     root node of the tree representing the assignment
ExprNode* assignmentToTree(ListIterator& infix, TokenList& list, FunctionDef& funs)
{
    ExprNode* lhs  = NULL,
            * rhs  = NULL,
            * root = NULL;

    string oper;
    if (infix.tokenChar() == '-')    // if negative - This would count as improper formatting but I'll leave this in for the sake of keeping it from crashing
    {
        infix.advance();
        Operation* negation = new Operation(conditionalToTree(infix,list,funs),"*",new Value(-1));
        lhs = static_cast<ExprNode *>(negation);
    }
    else
        lhs = conditionalToTree(infix,list,funs);

    oper = tokenText(infix,list);
    while (oper == "=")
    {
        infix.advance();

        rhs = assignmentToTree(infix,list,funs); // Assignment MUST evaluate right to left, or else we try to assign to an r-value. This accounts for that.
        root = static_cast<ExprNode *>(new Operation(lhs, oper, rhs));
        lhs = root; //Doesn't actually apply here, but for consistency with other functions

        oper = tokenText(infix,list);
    }

    if (root == NULL)
        return lhs;  //there was no assignment, so just return what was evaluated for the left
    else
        return root;
}

// conditionalToTree
// Converts an infix conditional expression into a tree
// Parameters:
//     infix    (input Token list iterator)  - expression to convert
// Returns:
//     root node of the tree representing the conditional
ExprNode* conditionalToTree(ListIterator &infix, TokenList& list, FunctionDef& funs)
{
    ExprNode* test      = NULL,
            * truecase  = NULL,
            * falsecase = NULL,
            * root      = NULL;

    string oper;
    if (infix.tokenChar() == '-') //Would be improper for this to be true, but I'll leave it for stability
    {
        infix.advance();
        Operation* negation = new Operation(testToTree(infix,list,funs),"*",new Value(-1));
        test = static_cast<ExprNode *>(negation);
    }
    else
        test = testToTree(infix,list,funs);

    oper = tokenText(infix,list);
    while (oper == "?")
    {
        infix.advance();
        truecase = testToTree(infix,list,funs);

        infix.advance(); //Move past assumed ":"
        falsecase = testToTree(infix,list,funs);

        root = static_cast<ExprNode *>(new Conditional(test, truecase, falsecase));
        test = root; //return of a coditional could be test for another. i mean, there should be parentheses, but hey, supporting it isn't hard

        oper = tokenText(infix,list);
    }

    if (root == NULL)
        return test;  //there was no conditional, so just return what was evaluated for the test
    else
        return root;
}

// testToTree
// Converts an infix test expression into a tree
// Parameters:
//     infix    (input Token list iterator)  - expression to convert
// Returns:
//     root node of the tree representing the test
ExprNode* testToTree(ListIterator &infix, TokenList& list, FunctionDef& funs)
{
    ExprNode* lhs  = NULL,
            * rhs  = NULL,
            * root = NULL;

    string oper;
    if (infix.tokenChar() == '-')
    {
        infix.advance();
        Operation* negation = new Operation(sumToTree(infix,list,funs),"*",new Value(-1));
        lhs = static_cast<ExprNode *>(negation);
    }
    else
        lhs = sumToTree(infix,list,funs);

    oper = tokenText(infix,list);
    while (oper == ">" || oper == "<" || oper == ">=" || oper == "<=" || oper == "==" || oper == "!=")
    {
        infix.advance();

        rhs = sumToTree(infix,list,funs); // Assignment MUST evaluate right to left, or else we try to assign to an r-value. This accounts for that.
        root = static_cast<ExprNode *>(new Operation(lhs, oper, rhs));
        lhs = root; //Doesn't actually apply here, but for consistency with other functions

        oper = tokenText(infix,list);
    }

    if (root == NULL)
        return lhs;  //there was no assignment, so just return what was evaluated for the left
    else
        return root;
}


// sumToTree
// Converts an infix sum expression into a tree
// Parameters:
//     infix  (input Token list iterator) - expression to convert
// Returns:
//     root node of the tree representing the sum
ExprNode* sumToTree(ListIterator &infix, TokenList& list, FunctionDef& funs)
{
    ExprNode* lhs  = NULL,
            * rhs  = NULL,
            * root = NULL;

    string oper;
    if (infix.tokenChar() == '-')
    {
        infix.advance();
        Operation* negation = new Operation(prodToTree(infix,list,funs),"*",new Value(-1));
        lhs = static_cast<ExprNode *>(negation);
    }
    else
        lhs = prodToTree(infix,list,funs);

    oper = tokenText(infix,list);
    while (oper == "+" || oper == "-")
    {
        infix.advance();

        rhs = prodToTree(infix,list,funs);
        root = static_cast<ExprNode *>(new Operation(lhs, oper, rhs));
        lhs = root; //If we have multiple sums, the first sum becomes the left hand side of the first. This accounds for that.

        oper = tokenText(infix,list);
    }

    if (root == NULL)
        return lhs; //there was no sum, so just return what was evaluated for the left
    else
        return root;
}

// prodToTree
// Translates an infix product expression to a tree
// Parameters:
//     infix  (input Token list iterator) - expression to convert'
// Returns:
//     root node of the tree representing the sum
ExprNode* prodToTree(ListIterator &infix, TokenList& list, FunctionDef& funs)
{
    ExprNode* lhs  = NULL,
            * rhs  = NULL,
            * root = NULL;

    string oper;

    lhs = factorToTree(infix,list,funs);

    oper = tokenText(infix,list);
    while (oper == "*" || oper == "/" || oper == "%")
    {
        infix.advance();

        rhs = factorToTree(infix,list,funs);
        root = static_cast<ExprNode *>(new Operation(lhs, oper, rhs));
        lhs = root; //See this line in previous function for explaination of this line

        oper = tokenText(infix,list);
    }

    if (root == NULL)
        return lhs; //there was no product, for just return what was evaluated for the left
    else
        return root;
}

// Factor to Postfix
// Translates a factor from infix to a tree
// A factor may either be a number or parenthesized expression.
// Parameters:
//     infix  (input Token List iterator) - expression to convert
// Returns:
//     root node of the tree representing the factor
ExprNode* factorToTree(ListIterator &infix, TokenList& list, FunctionDef& funs)
{
    ExprNode* output = NULL;

    if (infix != list.end())
    {
        if (!isOperator(infix.token()))
        {
            if (infix.currentIsInteger())
            {
                output = static_cast<ExprNode *>(new Value(infix.integerValue()));
            }
            else if (funs.find(tokenText(infix, list)) != funs.end()) // function call
            {
                string name = tokenText(infix, list);
                infix.advance(); //now on '('
                infix.advance(); //now past '('

                ExprNode* params[10];
                for (int i = 0; i < 10; ++i)
                {
                    if (tokenText(infix, list) != ")")
                    {
                        params[i] = assignmentToTree(infix, list, funs); //now on either ',' or ')'
                        if (tokenText(infix, list) == ",")
                            infix.advance();
                        //now on either next param or ')'
                    }
                    else
                        params[i] = NULL;
                } //now on ')', which is handled by infix.advance() later

                output = static_cast<ExprNode *>(new Function(name, params));
            }
            else
            {
                output = static_cast<ExprNode *>(new Variable(infix.token().tokenText()));
            }
            infix.advance();
        }
        else if (infix.tokenChar() == '-')
        {
            infix.advance();
            output = static_cast<ExprNode *>(new Operation(factorToTree(infix,list,funs),"*",new Value(-1)));
        }
        else
        {
            infix.advance();        // go past assumed (
            output = assignmentToTree(infix, list, funs);
            infix.advance();        // go past assumed )
        }
    }

    return output; //It will return null if the factor is null, but that only happens if it's an improperly formatted
                   //expression......so....don't do that
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
    return (text == "+"  || text == "-" || text == "*"  || text == "/"  || text == "%"  || 
            text == "("  || text == ")" || text == "="  || text == "?"  || text == ":"  ||
            text == ">"  || text == "<" || text == ">=" || text == "<=" || text == "==" || 
            text == "!=" || text == ",");
}

string tokenText(ListIterator& infix, TokenList& list)
{
    if (infix != list.end())
        return infix.token().tokenText();
    else
        return "";
}
