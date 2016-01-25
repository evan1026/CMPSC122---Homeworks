#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;
#include "compile.h"

const int CODE  = 100;
const int STACK = 100;
const int TEMPS = 100;

int main( int argc, char *argv[] )
{
    ifstream infile;
    char fileLine[80];
    VarTree vars;		// initially empty tree
    FunctionDef funs;
    Instruction *program[CODE];	// space for CODE instructions
    int stack[STACK];		// stack space for STACK values
    int temps[TEMPS];		// up to TEMPS temporary registers

    int progBegin = -1;		// where to begin execution
    int progEnd = 0;		// where program ends (first unused spot)
    int stackPointer;		// pointer to stack memory
    int programCounter;		// pointer to instruction

    if (argc < 2)
    {
        cout << "Call this program with a name of a file afterwards" << endl;
    }
    else
    {
	    infile.open( argv[1] );
	    while (infile.getline( fileLine, 100 ))
	    {
	        cout << fileLine << endl << endl;;
	        compile( fileLine, vars, funs, program, progBegin, progEnd );
        }
	    for (int i=0; i<progEnd; i++)
	        cout << setw(2) << i << ": " << *program[i];
        cout << endl;
        programCounter = progBegin;
	    stackPointer = STACK - vars.size();
	    while (programCounter < progEnd)
	    {
	        programCounter++;		// prepare for the next
	        program[programCounter-1]->execute( 	// but execute this one
		    temps, stack, stackPointer, programCounter );	 
	    }
    }
}   
