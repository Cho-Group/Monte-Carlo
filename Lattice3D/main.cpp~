#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "goal.h"
#include "lattice.h"
#include "point.h"
#include "neighbor.h"
using namespace std;

double* WangLandau(lattice* inputLattice, goal* theTemplate);
double min(double in1, double in2);
bool isFlat(int* H, int length, double percentage = .8);

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

/*//template fileToChange outputFile
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
}*/

int main(int argc, char* argv[])//template fileToChange
{
	string templateString = argv[1];
	string changeString = argv[2];
	
	lattice* templateLattice = new lattice();//lattice to create goal class
	lattice* changeLattice = new lattice();//lattice to be changed
	
	templateLattice -> fillpdb(templateString);
	changeLattice -> fillpdb(changeString);
	
	goal* theTemplate = new goal(templateLattice);
	
	double* densityOfStates = WangLandau(changeLattice,theTemplate);//get the density of states
	int length = theTemplate -> getConnections()/2+1;
	for(int i=0;i<length;i++)
		cout << i << ": " << densityOfStates[i] << "\n";
	return 1;
}

double* WangLandau(lattice* inputLattice, goal* theTemplate)
{
	int length = theTemplate -> getConnections()/2+1;//length of histogram and density of states array
	int sum = 0;//sum of all histogram values
	int* H = new int(length);//histogram of number of unique visits
	double* densityOfStates = new double(length);//array of the density of states for various energy levels [0,length)
	for(int i=0;i<length;i++)//set the arrays to their initial values
	{
		H[i] = 0;
		densityOfStates[i] = 1.0;
	}
	
	int oldIndex, newIndex; //energy states of new and old lattice
	lattice* newLattice;
	lattice* oldLattice;
	newLattice = oldLattice = inputLattice;
	oldIndex = newIndex = theTemplate -> check(oldLattice)/2;	
	int counter = 0;//counter that goes through the while loop
	int checkH =10000;//counter number at which the histogram is checked
	double f = 2.7;//length^(1/checkH+1);//f^checkH+1 = length
	
	while(f>1.1)//~exp(10^-8) this number was taken from the paper
	{
		while(!newLattice -> shiftRandom()){}//change a random value
		newIndex = theTemplate -> check(newLattice)/2;
		
		if(rand()/(double)RAND_MAX < min(densityOfStates[oldIndex]/densityOfStates[newIndex],1))
		{
			oldLattice -> copyLattice(newLattice);//confirm the switch
			oldIndex = newIndex;
			H[newIndex]++;
		}
		else
		{
			newLattice -> copyLattice(oldLattice);//go back
			newIndex = oldIndex;
		}
		densityOfStates[newIndex] += f;

		if(counter==0)//every checkH cycles
		{
			for(int i=0;i<length;i++)
				cout << i << ": " << densityOfStates[i] << "    \t" << H[i] << "\n";
			cout << "\n";
			if(isFlat(H,length))//check if histogram is flat
			{
				cout << "----------------------F CHANGING---------------------------\n";
				for(int j=0;j<length;j++)
					H[j] = 0;
				f = sqrt(f);
			}
		}
		counter = (counter+1)%(checkH);//loop from 0 to checkH-1
	}
	return densityOfStates;
}
	
double min(double in1, double in2)
{
	if(in1<in2)
		return in1;
	else
		return in2;
}

bool isFlat(int* H, int length, double percentage)
//see if the histogram is flat
//the histogram is flat if none of the bars are less than percentage*averageValue
{
	//calculate average
	int sum = 0;
	for(int i=0;i<length;i++)
		sum += H[i];
	if(sum == 0)
		return false;//return false if there is no data in the histogram
	double average = (double)sum/(double)length;
	//check for bars that aren't flat
	for(int j=0;j<length;j++)
		if((double)H[j] < percentage*average)
			return false;
	return true;
}



