// Token List Header file
// This header file primarily implements an Iterator for
// visiting the tokens of an arithmetic expression.
// This is not really a list yet (that comes later),
// but a class is defined to allow complete iterator support.

#include <stdlib.h>
#include <ctype.h>

class ListIterator;		// class definition below

class TokenList  {
    friend class ListIterator;
    friend ostream& operator<<( ostream &, TokenList &);
    private:
	const char *data;	// the actual string data
    public:
	TokenList( const char str[] )	// start with initial data
	{
	    data = str;
	}

	// and now some support for the List Iterator
	ListIterator begin();
	ListIterator end();
};

class ListIterator
{
    friend class TokenList;	// let that class create iterators
    private:
	TokenList   *list;	// which list
	const char  *curr;	// current position
	ListIterator( TokenList *l, const char *pos )
	{
	    list = l;
	    curr = pos;
	}
    public:
	char tokenChar() const
	{
	    return *curr;		// get this character
	}
	bool currentIsInteger()
	{
	    return isdigit( *curr );	// examine this character
	}
	int integerValue()
	{
	    return atoi( curr );	// convert string to int
	}
	void advance();			// move forward through the data
	void retreat();			// move backward through the data
	int operator !=( const ListIterator &other ) const
	{
	    return list != other.list || curr != other.curr;
	}
};
