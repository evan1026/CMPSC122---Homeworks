// Token List Implementation file
// Not a true linked list; just support for iterators
#include <iostream>
#include <string.h>
#include <ctype.h>
using namespace std;
#include "tokenlist.h"

//  output operation
//  Display all of the tokens in the list
//  Parameters:
//  	stream	(modified output stream)	where to output to
//	t	(input reference to TokenList)	what to output
//  Returns:
//  	output stream, for use by additional << operators
ostream& operator<<( ostream &stream, TokenList &t )
{
    for (ListIterator iter = t.begin(); iter != t.end(); iter.advance() )
    {
	if (iter.currentIsInteger())		// output a number
	    stream << iter.integerValue() << " ";
	else					// output a character
	    stream << iter.tokenChar() << " ";
    }
    return stream;
}

//  begin
//  Creates an iterator to refer to the beginning of the data
//  HINT:  This does the same thing as "findFirstToken"
ListIterator TokenList::begin()
{
    int pos = 0;
    while (data[pos] == ' ')
        ++pos;
    return ListIterator( this, &data[pos] );
}

//  end
//  Creates an iterator to refer to the end of the data
//  The end is indicated by a null character in the array.
ListIterator TokenList::end()
{
    return ListIterator( this, &data[strlen(data)] );
}

//  advance
//  moves the iterator to the next token
//  (indicated by a non-space character)
//  NOTE:  The current token could have been a number with multiple digits
void ListIterator::advance()	
{

    //Get away from what we have now, be it an integer or operator
    if (currentIsInteger())
    {
        while (currentIsInteger())
            ++curr;
    }
    else
    {
        ++curr;
    }

    //Advance to next token if we're on spaces
    while (*curr == ' ')
        ++curr;
}

void ListIterator::retreat()
{
    //Go back one
    --curr;

    //If we've met spaces, go back to the end of them
    while (*curr == ' ')
        --curr;

    //while the previous character is a digit, go back
    //this is so we can get to the start of a number
    while (curr > list->data && currentIsInteger() && isdigit(*(curr - 1)))
        --curr;
}
