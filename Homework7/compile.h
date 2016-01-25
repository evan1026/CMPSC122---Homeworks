#ifndef COMPILE_H
#define COMPILE_H
// Simple Expression Compilation Header
// This describes the functionality for compiling some simple
// arithmetic expressions into a machine code representation.  
// Given a string consisting of operators and operands, 
// it will translate and store some representative operations.
//
// The expressions ma consist of the following:
// -- integer values
// -- integer variable names
// -- simple arithmetic operators ( +, -, *, /, % )
// -- the assignment operator to assign to variables
// -- matched parentheses for grouping
//
// All expressions are expected to have valid syntax.
// There is no specification on the length of any expression.

#include "vartree.h"
#include "funmap.h"
#include "machine.h"

// Compile
// Compile the given expression into a machine code, with 
// the given variables defined
// New variables may be defined when this function is called
// Parameters:
//	expr	(input char array)	expression to evaluate
//	vars	(modified VarTree)	variables to work with
//	funs	(modified FunctionDef)	functions to define or call
//	prog	(modified Inst array)	program code being generated
//	pBegin	(output integer)	first instruction not in a function
//	pEnd	(output integer)	program end (first unused spot)
void compile( const char expr[], VarTree &vars, FunctionDef &funs,
	Instruction *prog[], int &pBegin, int &pEnd );

#endif
