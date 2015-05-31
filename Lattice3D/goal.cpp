#include <cmath>
#include <cstdlib>
#include <iostream>
#include "goal.h"
using namespace std;

goal::goal(lattice* input)
{
	connections = 0;

	neighbor* nCursor = NULL;
	neighbor* nTrace = NULL;
	
	numNeighbors = input -> numElements;
	neighbors = new neighbor* [numNeighbors];
	//go through short list
		//find all the non bonded adjacent atoms
		//place those atoms in a linked list
	for(int i=0;i<numNeighbors;i++)
	{
		insert(input -> shortList[i],input,i);
	}
	
	//remove the worthless pointers
	for(int j=0;j<numNeighbors;j++)
	{
		nTrace = nCursor = neighbors[j];
		if(nCursor -> data == -1)
		{
			neighbors[j] = 0;
			delete nCursor;
		}
		else
		{
			nCursor = nCursor -> next;
			while(nCursor->data != -1)
			{
				nTrace = nCursor;
				nCursor = nCursor -> next;
			}
			nTrace -> next = 0;
			delete nCursor;
		}
	}
}

void goal::insert(point* current, lattice* input,int num)
{
	point* adjacent;
	neighbor* nCursor = NULL;
	neighbor* nTrace = NULL;
	nTrace = nCursor = neighbors[num] = new neighbor();
	int xValue = current -> getX();
	int yValue = current -> getY();		
	int zValue = current -> getZ();
	
	//look through all 6 adjacent spaces
	adjacent = input -> a[xValue+1][yValue][zValue];
	if(adjacent != 0 && abs(adjacent->getNum() - current->getNum()) > 1)//not bonded and not empty
	{
		connections++;
		nCursor -> data = adjacent->getNum();
		nCursor = new neighbor();
		nTrace -> next = nCursor;
		nTrace = nCursor;
	}
	
	adjacent = input -> a[xValue-1][yValue][zValue];
	if(adjacent != 0 && abs(adjacent->getNum() - current->getNum()) > 1)//not bonded and not empty
	{
		connections++;
		nCursor -> data = adjacent->getNum();
		nCursor = new neighbor();
			nTrace -> next = nCursor;
		nTrace = nCursor;
	}
	
	adjacent = input -> a[xValue][yValue+1][zValue];
	if(adjacent != 0 && abs(adjacent->getNum() - current->getNum()) > 1)//not bonded and not empty
	{
		connections++;
		nCursor -> data = adjacent->getNum();
			nCursor = new neighbor();
		nTrace -> next = nCursor;
		nTrace = nCursor;
	}
	
	adjacent = input -> a[xValue][yValue-1][zValue];
	if(adjacent != 0 && abs(adjacent->getNum() - current->getNum()) > 1)//not bonded and not empty
	{
		connections++;
		nCursor -> data = adjacent->getNum();
		nCursor = new neighbor();
		nTrace -> next = nCursor;
		nTrace = nCursor;
	}
	
	adjacent = input -> a[xValue][yValue][zValue+1];
	if(adjacent != 0 && abs(adjacent->getNum() - current->getNum()) > 1)//not bonded and not empty
	{
		connections++;
		nCursor -> data = adjacent->getNum();
		nCursor = new neighbor();
		nTrace -> next = nCursor;
		nTrace = nCursor;
	}
	
	adjacent = input -> a[xValue][yValue][zValue-1];
	if(adjacent != 0 && abs(adjacent->getNum() - current->getNum()) > 1)//not bonded and not empty
	{
		connections++;
		nCursor -> data = adjacent->getNum();
		nCursor = new neighbor();
		nTrace -> next = nCursor;
		nTrace = nCursor;
	}
}
goal::~goal()//delete goal
{
	neighbor* cursor;
	neighbor* forward;
	for(int i =0;i<numNeighbors;i++)
	{
		forward = cursor = neighbors[i];
		while(cursor != 0)
		{
			forward = cursor -> next;
			delete cursor;
			cursor = forward;
		}
	}
	delete neighbors;
}

void goal::print()//print with format "atomNumber": "adjacentNumbers"
{
	neighbor* cursor;
	for(int i=0;i<numNeighbors;i++)
	{
		cursor = neighbors[i];
		cout << i << ": ";
		while(cursor != 0)
		{
			cout << cursor -> data << " ";
			cursor = cursor -> next;
		}
		cout << "\n";		
	}
	cout << "Total Connections: " << connections << "\n";
	
}

int goal::check(lattice* input)//check lattice against goal and return the number of neighbors they both share
{
	int toReturn = 0;
	if(input -> numElements != numNeighbors)
		cout << "ERROR: molecules are not the same size\n";
	else
	{
		for(int i=0;i<numNeighbors;i++)
		{
			toReturn += compare(input -> shortList[i],input,i);
		}
	}
	return toReturn;
}

int goal::compare(point* current, lattice* input, int num)//return the number of same neighbors for one point of the lattice/goal
{
	int toReturn = 0;
	point* adjacent;
	int xValue = current -> getX();
	int yValue = current -> getY();		
	int zValue = current -> getZ();
	//look through all 6 adjacent spaces
	adjacent = input -> a[xValue+1][yValue][zValue];
	if(adjacent != 0 && abs(adjacent->getNum() - current->getNum()) > 1)//not bonded and not empty
	{
		if(isIn(num, adjacent -> getNum()))
			toReturn++;
	}
	
	adjacent = input -> a[xValue-1][yValue][zValue];
	if(adjacent != 0 && abs(adjacent->getNum() - current->getNum()) > 1)//not bonded and not empty
	{
		if(isIn(num, adjacent -> getNum()))
			toReturn++;
	}
	
	adjacent = input -> a[xValue][yValue+1][zValue];
	if(adjacent != 0 && abs(adjacent->getNum() - current->getNum()) > 1)//not bonded and not empty
	{
		if(isIn(num, adjacent -> getNum()))
			toReturn++;
	}
	
	adjacent = input -> a[xValue][yValue-1][zValue];
	if(adjacent != 0 && abs(adjacent->getNum() - current->getNum()) > 1)//not bonded and not empty
	{
		if(isIn(num, adjacent -> getNum()))
			toReturn++;
	}
	
	adjacent = input -> a[xValue][yValue][zValue+1];
	if(adjacent != 0 && abs(adjacent->getNum() - current->getNum()) > 1)//not bonded and not empty
	{
		if(isIn(num, adjacent -> getNum()))
			toReturn++;
	}
	
	adjacent = input -> a[xValue][yValue][zValue-1];
	if(adjacent != 0 && abs(adjacent->getNum() - current->getNum()) > 1)//not bonded and not empty
	{
		if(isIn(num, adjacent -> getNum()))
			toReturn++;
	}
	return toReturn;
}

bool goal::isIn(int index, int element)//returns true if element is a neighbor for neighbors[index]
{
	neighbor* cursor = neighbors[index];
	while(cursor != 0)
	{
		if(cursor -> data == element)
			return true;
		else
			cursor = cursor -> next;
	}
	return false;
}





