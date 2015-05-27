#include <iostream>
#include <stdlib.h>
#include "goal.h"
#include "lattice.h"
#include "point.h"
#include "neighbor.h"
using namespace std;

//basic Monte-Carlo
/*int main(int argc, char*argv[])//fileIn fileOut numShifts 
{	
	if(argc > 4)
	{
		cout << "invalid number of inputs\n";	
		return 0;
	}
	srand(time(NULL));//randomize the random seed
	int shifts;
	int success = 0;//#of successful moves
	int failure = 0;//#of unsuccessful moves
	int* coord = new int(3);
	string fileIn;
	const char* fileOut;
	lattice* theLattice = new lattice();
	fileIn  = argv[1];//the name of the input file
	fileOut = argv[2];
	shifts = atoi(argv[3]);
	theLattice -> fillpdb(fileIn);//fill the lattice with data from the pdb file
	FILE* outputFile;
	outputFile = fopen(fileOut,"w");
	fprintf(outputFile,"");
	fclose(outputFile);
	theLattice->printLattice(fileOut);//print the initial lattice
	cout << "Distance between endpoints:" << theLattice->end2end() << "\n";
	for(int i = 0; i<shifts; i++)//shift based on the initial input
	{
		if(theLattice->shiftRandom())//if shift is successful
		{
			theLattice->printLattice(fileOut);//print the lattice only if something changed
			cout << "Distance between endpoints:" << theLattice->end2end() << "\n";
		}
	}
	return 1;
}*/

//Metropolis Algorithm

int main(int argc, char* argv[])//filein
{
	string fileIn = argv[1];
	string fileIn2 = argv[2];
	lattice* theLattice = new lattice();
	lattice* lattice2 = new lattice();
	theLattice -> fillpdb(fileIn);
	lattice2 -> fillpdb(fileIn2);
	goal* theTemplate = new goal(theLattice);
	cout << theTemplate -> check(lattice2) << "\n";
	return 1;
	
}
