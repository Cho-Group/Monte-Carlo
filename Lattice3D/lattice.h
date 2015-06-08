#ifndef LATTICE_H
#define LATTICE_H
#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include "point.h"
using namespace std;

/*
X+ = 1
Y+ = 2
Z+ = 3
Z- = 4
Y- = 5
X- = 6
*/

class lattice
{
	public:
		friend class goal;
		lattice();//create an empty lattice
		lattice(int n);//create a lattice with n points
		~lattice();
		void printLattice(const char* filename);//print the lattice in pdb format to a file
		void printLattice();//print the lattice in pdb format to command line
		void rotateTail();
		void fillpdb(string filename);//fill an empty lattice with pdb information
		bool shiftRandom(); //try to move a random point, return true if the move is successful, else return false
		bool shiftPoint(int n){return shiftPoint(shortList[n]->getX(),shortList[n]->getY(),shortList[n]->getZ());}
		bool shiftPoint(int x, int y, int z);//try to move one point checking to see if that point is an end a corner or a hinge, return true if move is successfull else return false
		bool emptyPoint(int x, int y, int z) {if(a[x][y][z]==NULL)return true; else return false;}
		int* generateRandom();
		float end2end(); //return the distance between the first and the last point in the lattice
		int getNumElements(){return numElements;}
		void copyLattice(lattice* otherLattice);
		int getModel(){return model;}
		void clearArray();
		void recenter();//recenter the molecule
		int getX(int num){return shortList[num] -> getX();}
		int getY(int num){return shortList[num] -> getY();}
		int getZ(int num){return shortList[num] -> getZ();}
		void printNum(){for(int i=0;i<numElements;i++)cout << i << ": " << shortList[i] -> getNum() << "\n";}
		point* getPoint(int x, int y, int z){return a[x][y][z];}
		point* getPoint(int n){return shortList[n];}
	private:
		point**** a;//three dimentional array of points
		point** shortList; //single dimentional array of all points
		int l; //max x coordinate
		int w; //max y coordinate
		int h; //max z coordinate
		int numElements; //number of elements
		point* head; //first element
		int model; //model number
		
		void fillLattice();
		point* chooseElement(); //choose a random element
		int determineRotation(point* pivot); //return transformation direction given the pivot
		bool checkRotation(point* pivot, int rotation);
		bool isEarly(point* pivot, point* current, int rotation);
		int* getNewPoints(int rotation, point* current, point* pivot);
		int getDirection(int x, int y, int z, point* temp);//determine out direction given two points
};
#endif
