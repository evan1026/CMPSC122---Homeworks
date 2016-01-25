// Machine Implementation File
// This implementation represents a simple machine that can evaluate
// arithmetic expressions.  In addition to the expected arithmetic
// operations, it includes relational operations and branching
// for the conditional operator, and simple support for function calls.
//
// The machine is expected to have a pre-allocated stack space
// for named variables and an arbitrarily long list of temporary
// registers, both represented by integer arrays here (to hold values).
// The machine also has a separate memory area holding all of the
// instructions defined below.

#include <iostream>
#include <sstream>
using namespace std;

#include "machine.h"

ostream& operator<<( ostream &stream, const Instruction &i )
{
    return stream << i.toString();
}

//  Here follow implementations of toString() and execute()
//  for all of the derived classes of the Instruction.

string Print::toString() const
{
    stringstream ss;
    ss << "print T" << valueTemp << endl;
    return ss.str();
}

void Print::execute(int regs[], int stack[], int& stackPointer, int& programCounter) const
{
    cout << regs[valueTemp] << endl;
}

string Val::toString() const
{
    stringstream ss;
    ss << "T" << valueTemp << " = " << val << endl;
    return ss.str();
}

void Val::execute(int regs[], int stack[], int& stackPointer, int& programCounter) const
{
    regs[valueTemp] = val;
}

string VarAssign::toString() const
{
    stringstream ss;
    ss << "stack[" << stackLoc << "] = T" << valueTemp << endl;
    return ss.str();
}

void VarAssign::execute(int regs[], int stack[], int& stackPointer, int& programCounter) const
{
    stack[stackLoc] = regs[valueTemp];
}

string VarLoad::toString() const
{
    stringstream ss;
    ss << "T" << valueTemp << " = stack[" << stackLoc << "]" << endl;
    return ss.str();
}

void VarLoad::execute(int regs[], int stack[], int& stackPointer, int& programCounter) const
{
    regs[valueTemp] = stack[stackLoc];
}

string Compute::toString() const
{
    stringstream ss;
    ss << "T" << valueTemp << " = T" << argA << " " << oper << " T" << argB << endl;
    return ss.str();
}

void Add::execute(int regs[], int stack[], int& stackPointer, int& programCounter) const
{
    regs[valueTemp] = regs[argA] + regs[argB];
}

void Subtract::execute(int regs[], int stack[], int& stackPointer, int& programCounter) const
{
    regs[valueTemp] = regs[argA] - regs[argB];
}

void Multiply::execute(int regs[], int stack[], int& stackPointer, int& programCounter) const
{
    regs[valueTemp] = regs[argA] * regs[argB];
}

void Divide::execute(int regs[], int stack[], int& stackPointer, int& programCounter) const
{
    regs[valueTemp] = regs[argA] / regs[argB];
}

void Mod::execute(int regs[], int stack[], int& stackPointer, int& programCounter) const
{
    regs[valueTemp] = regs[argA] % regs[argB];
}
