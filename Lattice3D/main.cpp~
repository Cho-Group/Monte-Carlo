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
	const char* fileOut = argv[3];//output file string
	FILE* outputFile; //output file
	string templateString = argv[1];
	string changeString = argv[2];
	lattice* templateLattice = new lattice();//lattice to create goal class
	lattice* changeLattice = new lattice();//lattice to be changed to equal template lattice
	lattice* tempLattice = new lattice();//backup of changeLattice
	
	//fill lattices with appropriate pdb information
	templateLattice -> fillpdb(templateString);
	changeLattice -> fillpdb(changeString);
	tempLattice -> fillpdb(changeString);
	
	//clear output file
	outputFile = fopen(fileOut, "w");
	fprintf(outputFile, "");
	fclose(outputFile);
	
	goal* theTemplate = new goal(templateLattice);
	
	//print original states to command line
	int goalConnections = theTemplate -> getConnections();
	cout << "GOAL LATTICE\t goal state: "<< goalConnections << "\n";
	templateLattice -> printLattice();
	cout << "CHANGING LATTICE\n";
	changeLattice -> printLattice();
	
	changeLattice -> printLattice(fileOut);
	while(overlap < goalConnections)//while the two lattices still aren't the same
	{
		if(changeLattice->shiftRandom())//make a random move, continue if the move was sucessful (there was no overlap)
		{	
			temp = theTemplate -> check(changeLattice);
			if(temp >= overlap)//if the new state is the same or better keep it
			{
				cout << "Model Number " << changeLattice -> getModel() << "\n";
				changeLattice -> printLattice(fileOut);
				overlap = temp;
				tempLattice -> copyLattice(changeLattice);
			}
			else//change it back
			{
				cout << "TEST\n";
				changeLattice -> copyLattice(tempLattice);
			}					
		}
	}
	//print the final state
	cout << "GOAL BONDS\n";
	theTemplate -> print();
	cout << "GOAL LATTICE\n";
	templateLattice -> printLattice();
	cout << "CHANGING LATTICE\n";
	changeLattice -> printLattice();
	return 1;
}
