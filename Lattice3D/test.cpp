#include <iostream>
#include "lattice.h"
#include "point.h"
using namespace std;

int main()
{
	lattice* theLattice = new lattice();
	theLattice -> fillpdb("test3.pdb");
	
	theLattice -> printLattice();
	for(int i=0;i<10;i++)
	{
		theLattice -> shiftPoint(1);
		theLattice -> printLattice();
	}
	return 1;
}
