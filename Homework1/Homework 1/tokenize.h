// Tokenize Header file
// This is a very crude tokenizer for use in an expression parser.
// At the moment, it is simply a collection of functions that
// will later be encapsulated into an object-oriented class.
// It has only a very rudimentary behavior at this time.
//
// This tokenizer will begin with a character string representing
// an expression, and will identify and return tokens as requested.
// These tokens may be separated by space characters within the string.
// It has the following limitations:
// --- only integers and operators are supported (no names)
// --- all tokens must be exactly one character, including integers

// findFirstToken
// Finds the first non-space character in the given string
// Parameters:
// 	expr	(ionput string)		expression to examine
// 	pos	(output integer)	position of first non-space char
// Post-condition:   expr[pos] is not a space
void findFirstToken( const char expr[], int &pos );

// currentToken
// Identifies the current non-space character in the given string
// Parameters:
//      expr    (input string)          expression to examine
//      pos     (input integer)         position to examine
// Returns:   a non-space character at the given position
// NOTE: The null terminating character (0) is also not a space.
char currentToken( const char expr[], int pos );

// advance
// advance to the next token in the given string
// Parameters:
//      expr    (input string)          expression to examine
//      pos     (modified integer)      position to consider
// Post-condition:   expr[pos] is not a space
// NOTE:  expr[pos] may be the null character at the end of the string
void advance( const char expr[], int &pos );

// identify whether current token is an integer
// Parameters:		
//      expr    (input string)          expression to examine
//      pos     (input integer)         position to examine
// Returns:		boolean, true if an integer, false if note
bool currentIsInteger( const char expr[], int pos );

// identify the value of a numeric token
// Parameters:	
//      expr    (input string)          expression to examine
//      pos     (input integer)         position to examine
// Returns:		an integer value between 0 to 9
// Pre-condition: The parameter is assumed to actually be a digit
int  integerValue( const char expr[], int pos );
