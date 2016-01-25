#ifndef MACHINE_H
#define MACHINE_H
// A simple RISC machine
// This structure represents a simple machine that can evaluate
// arithmetic expressions.  In addition to the expected arithmetic
// operations, it includes relational operations and branching
// for the conditional operator, and simple support for function calls.
//
// The machine is expected to have a pre-allocated stack space
// for named variables and an arbitrarily long list of temporary
// registers, both represented by integer arrays here (to hold values).
// The machine also has a separate memory area holding all of the
// instructions defined below.
//
// Most of these instructions are also immutable -- once constructed,
// they cannot be changed -- only displayed and executed.

#include <iostream>
using namespace std;

class Instruction
{
   protected:
	int valueTemp;		// register computed or tested
				// additional fields defined later
	Instruction( int temp )
	{
	    valueTemp = temp;
	}
   public:
	friend ostream& operator<<( ostream&, const Instruction & );
	virtual string toString() const = 0; // facilitates << operator
	virtual void execute( int regs[], int stack[], int& stackPointer, int& programCounter ) const = 0;
};

// here follow all the derived classes defining additional
// memory variables as necessary, and declaring the two functions
// above that will actually be implemented in "machine.cpp"

// one simple example is given here:
class Print: public Instruction
{
   public:
	string toString() const;
	void execute( int regs[], int stack[], int& stackPointer, int& programCounter ) const;
	Print( int temp ) : Instruction(temp) { }
};

class Val : public Instruction
{
    int val;
    public:
        string toString() const;
        void execute(int regs[], int stack[], int& stackPointer, int& programCounter) const;
        Val(int result, int value) : Instruction(result), val(value) {}
};

class VarAssign : public Instruction
{
    int stackLoc; // location in the stack
    public:
        string toString() const;
        void execute(int regs[], int stack[], int& stackPointer, int& programCounter) const;
        VarAssign(int fromReg, int loc) : Instruction(fromReg), stackLoc(loc) {} // No real good thing to send
                                                                                 // to instruction, so just pick one
};

class VarLoad : public Instruction
{
    int stackLoc; // location in the stack
    public:
        string toString() const;
        void execute(int regs[], int stack[], int& stackPointer, int& programCounter) const;
        VarLoad(int result, int loc) : Instruction(result), stackLoc(loc) {}
};

class Compute : public Instruction
{
    string oper;
    protected:
        int argA, argB; //registers for operands
    public:
        string toString() const;
        virtual void execute(int regs[], int stack[], int& stackPointer, int& programCounter) const = 0;
        Compute (int _result, int _argA, int _argB, string _oper) :
            Instruction(_result), argA(_argA), argB(_argB), oper(_oper) {}
};

// also, as was pointed out in the assignment description,
// all of the arithmetic instructions are very simple and can
// be made children of a more general class (not defined here yet)
// All that is declared here is a new override to the execute method,
// and a constructor that simply calls the constructor for Compute,
// passing along all three relevant registers and the addition symbol.

class Add: public Compute
{
   public:
    void execute( int [], int [], int &, int & ) const;
    Add( int result, int argA, int argB ) : 
		Compute(result, argA, argB, "+" ) {}
};

class Subtract: public Compute
{
   public:
	void execute( int [], int [], int &, int & ) const;
	Subtract( int result, int argA, int argB ) : 
		Compute(result, argA, argB, "-" ) { }
};

class Multiply: public Compute
{
   public:
	void execute( int [], int [], int &, int & ) const;
	Multiply( int result, int argA, int argB ) : 
		Compute(result, argA, argB, "*" ) { }
};

class Divide: public Compute
{
   public:
	void execute( int [], int [], int &, int & ) const;
	Divide( int result, int argA, int argB ) : 
		Compute(result, argA, argB, "/" ) { }
};

class Mod: public Compute
{
   public:
	void execute( int [], int [], int &, int & ) const;
	Mod( int result, int argA, int argB ) : 
		Compute(result, argA, argB, "%" ) { }
};

#endif
