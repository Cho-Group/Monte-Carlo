#ifndef GOAL_H
#define GOAL_H
#include <cstdlib>
#include <string>
#include <fstream>
#include "neighbor.h"
#include "lattice.h"
using namespace std;

/*
X+ = 1
Y+ = 2
Z+ = 3
Z- = 4
Y- = 5
X- = 6
*/

class goal
{
	public:
		goal(lattice* input);
		~goal();
		void print();
		int check(lattice* input);//check lattice against goal(return the number of correct connections)
		int getConnections(){return connections;}
		void setConnections(int in){connections = in;return;}
	private:
		neighbor** neighbors;//a linked list of which non bonded atoms are neighbors
		int numNeighbors;//length of neighbors (first dimention)
		int connections;//total connections
		void insert(point* current, lattice* input, int num);//helper for constructor
		int compare(point* current, lattice* input, int num);//helper function for check
		bool isIn(int index, int element);//returns true if elementis in the index list
};
#endif
