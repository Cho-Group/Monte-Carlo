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

int main(int argc, char* argv[])//template fileToChange outputFile
{
	//set up the two lattice (template and the one to change as well as the goal)
	int overlap = 0; //the overlap between the changing lattice and the goal
	int temp;
	int counter = 0;
	const char* fileOut = argv[3];
	FILE* outputFile;
	outputFile = fopen(fileOut, "w");
	fprintf(outputFile, "");
	fclose(outputFile);
	string templateString = argv[1];
	string changeString = argv[2];
	lattice* templateLattice = new lattice();
	lattice* changeLattice = new lattice();
	lattice* tempLattice = new lattice();
	templateLattice -> fillpdb(templateString);
	changeLattice -> fillpdb(changeString);
	tempLattice -> fillpdb(changeString);
	
	goal* theTemplate = new goal(templateLattice);
	
	//print origional states
	
	int goalConnections = theTemplate -> getConnections();
	cout << "GOAL LATTICE\t goal state: "<< goalConnections << "\n";
	templateLattice -> printLattice();
	cout << "CHANGING LATTICE\n";
	changeLattice -> printLattice();
	
	changeLattice -> printLattice(fileOut);
	while(overlap < goalConnections)
	{
		if(changeLattice->shiftRandom())
		{	
			counter++;
			temp = theTemplate -> check(changeLattice);
			cout << "Overlap " << temp << "\n";
			if(temp >= overlap)//if the new state is the same or better keep it
			{
				changeLattice -> printLattice(fileOut);
				overlap = temp;
				tempLattice -> copyLattice(changeLattice);
			}
			else//change it
			{
				changeLattice -> copyLattice(tempLattice);
			}
					
		}
	}
	cout << "counter: " << counter << "\n";
	cout << "GOAL BONDS\n";
	theTemplate -> print();
	cout << "GOAL LATTICE\n";
	templateLattice -> printLattice();
	cout << "CHANGING LATTICE\n";
	changeLattice -> printLattice();
	return 1;
}
