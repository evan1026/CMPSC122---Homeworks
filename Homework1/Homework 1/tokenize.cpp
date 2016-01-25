// Tokenize Implementation file
// This is a very crude tokenizer for use in an expression parser.
// At the moment, it is simply a collection of functions that
// that will later be encapsulated into an object-oriented class.
// It has only a very rudimentary behavior at this time.
//
// This tokenizer will begin with a character string representing
// an expression, and will identify and return tokens as requested.
// These tokens may be separated by space characters within the string.
// It has the following limitations:
// --- only integers and operators are supported (no names)
// --- all tokens must be exactly one character, including integers

// The standard C library has some useful functions for us
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// findFirstToken
// Finds the first non-space character in the given string
// Parameters:
// 	expr	(input string)		expression to examine
// 	pos	(output integer)	position of first non-space char
// Post-condition:   expr[pos] is not a space
void findFirstToken( const char expr[], int &pos )
{
    pos = 0;
    while (expr[pos] == ' ') ++pos;
}

// currentToken
// Identifies the current non-space character in the given string
// Parameters:
//      expr    (input string)          expression to examine
//      pos     (input integer)         position to examine
// Returns:   a non-space character at the given position
// Pre-Condition: expr[pos] is not a space
// NOTE: The null terminating character (0) is also not a space.
char currentToken( const char expr[], int pos )
{
    return expr[pos];
}

// advance
// advance to the next token in the given string
// Parameters:
//      expr    (input string)          expression to examine
//      pos     (modified integer)      position to consider
// Post-condition:   expr[pos] is not a space
// NOTE:  expr[pos] may be the null character at the end of the string
void advance( const char expr[], int &pos )
{
    ++pos;
    while (expr[pos] == ' ') ++pos;
}

// identify whether token is an integer or operator
// Parameters:
//      expr    (input string)          expression to examine
//      pos     (input integer)         position to examine
// Returns:		boolean, depending on the question
bool currentIsInteger( const char expr[], int pos  )
{
    return isdigit( expr[pos] );
}

// identify the value of an integer token
// Parameters:
//      expr    (input string)          expression to examine
//      pos     (input integer)         position to examine
// Returns:		the numeric value of the digit
// Pre-condition: the character actually is indeed a numeric digit
int integerValue( const char expr[], int pos )
{
    return atoi( &expr[pos] );
}
