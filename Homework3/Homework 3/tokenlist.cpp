// Token List Implementation file
//
// This tokenizer will scan a character string representing
// an expression, and will return a list of tokens.
// --- only integers and operators are supported (no names)

// The standard C library has some useful functions for us
#include <string.h>
#include <ctype.h>

// And to get the definition of a token:
#include "tokenlist.h"

// TokenList constructor
// converts a character string into a list of tokens
// Parameter:
// 	expr	(input char pointer)	// string to examine
// Pre-condition:  str may not be a null pointer
//     and is assumed to actually point at a valid expression.
TokenList::TokenList( const char expr[])
{
 
    tail = NULL;
    head = NULL;

    int position = 0;
    
    //Go past any initial spaces
    while (expr[position] == ' ')
        ++position;

    //Go until we hit a null character
    while (expr[position] != '\0')
    {
        if (isdigit(expr[position]))
        {
            Token t(atoi(&expr[position]));
            push_back(t);
            while (isdigit(expr[position]))
                ++position;
        }
        else
        {
            Token t(expr[position]);
            push_back(t);
            ++position;
        }

        //Advance to next token if we're on spaces
        while (expr[position] == ' ')
            ++position;
    }
}

//  output operation
//  Display all of the tokens in the list
ostream& operator<<( ostream &stream, TokenList &t )
{
    for (ListIterator iter = t.begin(); iter != t.end(); iter.advance())
    {
	stream << iter.token() << " ";
    }
    return stream;
}

//  Creates an iterator to refer to the beginning of the list
ListIterator TokenList::begin()
{
    return ListIterator( this, head );
}

//  Creates an iterator to refer after the end of the list
//  This list is simple:  it ends with NULL
ListIterator TokenList::end()
{
    return ListIterator( this, NULL );
}

//  Add a new element to the back of the list
//  Parameter:
//       t	(input Token)	the new item to add
void TokenList::push_back(Token t)
{
    ListElement* newElement = new ListElement();
    newElement->token = t;
    newElement->next = NULL;

    if (tail != NULL)
    {
        tail->next = newElement;
    }

    if (head == NULL)
    {
        head = newElement;
    }

    tail = newElement;
}

//  Add a new element to the front of the list
//  Parameter:
//       t	(input Token)	the new item to add
void TokenList::push_front(Token t)
{
    ListElement* newElement = new ListElement();
    newElement->token = t;
    newElement->next = head;

    if (tail == NULL)
    {
        tail = newElement;
    }

    head = newElement;
}

//  Remove and return the element at the front of the list
Token TokenList::pop_front()
{
    Token t(head->token);
    ListElement* toDelete = head;
    head = head->next;

    if (head == NULL)
    {
        tail = NULL;
    }

    delete toDelete;

    return t;
}