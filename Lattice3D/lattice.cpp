#include "lattice.h"
#include "point.h"
#include <cmath>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
using namespace std;

/*
X+ = 1
Y+ = 2
Z+ = 3
Z- = 4
Y- = 5
X- = 6
*/

//create a 3D array of point pointers that all point to null
lattice::lattice()
{
	l = 100;
	w = 100;
	h = 100;
	numElements = 0;
	head =  NULL;
	a = new point***[l];
	for(int i=0;i<l;i++)
	{
		a[i] = new point**[w];
		for(int j=0;j<w;j++)
		{
			a[i][j] = new point*[h];
			for(int k=0;k<h;k++)
				a[i][j][k] = NULL;
		}
	}
	model = 1;
	shortList = NULL;
}

lattice::lattice(int n)
{
	point* cursor;
	l = 100;
	w = 100;
	h = 100;
	numElements = n;
	head = NULL;
	a = new point***[100];
	for(int i=0;i<l;i++)
	{
		a[i] = new point**[100];
		for(int j=0;j<w;j++)
		{
			a[i][j] = new point*[100];
			for(int k=0;k<h;k++)
				a[i][j][k] = NULL;
		}
	}
	fillLattice();
	//create an array with only the active points in them
	model = 1;
	shortList = new point*[numElements];
	point* trace = head;
	int i = 0;
	while(trace!=NULL)
	{
		shortList[i] = trace;
		trace = trace->getPointOut();
		i++;
	}
	
	
}

lattice::~lattice()
{
	for(int i =0;i<l;i++)
	{
		for(int j=0;j<w;j++)
		{
			for(int k=0;k<h;k++)
				delete a[i][j][k];
			delete a[i][j];
		}
		delete a[i];
	}
	delete a;			
}

/*void lattice::fillpdb(string filename)//read in a pdb file and fill the lattice with points
{
	int x, y, z, direction;
	string dummy, first, tempLine, xIn, yIn, zIn;
	point* temp;
	const char* inputFile = filename.c_str();
	ifstream input;
	input.open(inputFile);
	//check for beginning and end lines
	int k = 0;
	while(getline(input, tempLine))
	{
		
		stringstream inputLine(tempLine);
		inputLine >> first >> dummy >> dummy >> dummy >> dummy >> dummy >> xIn >> yIn >> zIn >> dummy >> dummy;
		x = atoi(xIn.c_str());
		y = atoi(yIn.c_str());
		z = atoi(zIn.c_str());
		if(first == "ENDMDL")
			break;
		if(first=="ATOM")
		{
			
			if(head == NULL)//first item
			{
				head = a[x][y][z] = new point(x,y,z);
				temp = head;
			}
			else
			{
				direction = getDirection(x,y,z, temp);
				if (direction == 0)//points are not next to each other
				{
					cout << "ERROR:invalid input file\n";
					input.close();
					return;
				}
				temp = temp->generateNew(direction,l,w,h);
				if(a[x][y][z] == NULL)
					a[x][y][z] = temp;
				else//check for duplicate points
				{
					cout << "ERROR: duplicate point in pdb file\n";
					input.close();
					return;
				}
			}
			numElements++;
		}
	}
	input.close();
	
	//create an array with only the active points in them
	shortList = new point*[numElements];
	point* trace = head;
	int i = 0;
	while(trace!=NULL)
	{
		shortList[i] = trace;
		trace = trace->getPointOut();
		i++;
	}
	return;
}*/

void lattice::fillpdb(string filename)//read in a pdb file and fill the lattice with points
{
	numElements = 0;
	int x, y, z, direction, xMin, yMin, zMin, xMax, yMax, zMax, xOffset, yOffset, zOffset;
	string dummy, first, tempLine, xIn, yIn, zIn;
	point* temp;
	const char* inputFile = filename.c_str();
	ifstream input;	
	input.open(inputFile);
	
	//check for beginning and end lines
	int k = 0;
	while(getline(input, tempLine))
	{
		stringstream inputLine(tempLine);
		inputLine >> first >> dummy >> dummy >> dummy >> dummy >> dummy >> xIn >> yIn >> zIn >> dummy >> dummy;
		x = atoi(xIn.c_str());
		y = atoi(yIn.c_str());
		z = atoi(zIn.c_str());
		if(first == "TER" || first=="ENDMDL")
		{
			break;
		}
		if(first=="HETATM" || first=="ATOM")//creating the linked list
		{
			numElements++;			
			if(head == NULL)//first item
			{
				head = new point(x,y,z);
				xMin = xMax = x;
				yMin = yMax = y;
				zMin = zMax = z;
				temp = head;
			}
			else
			{
				if(x<xMin)
					xMin = x;
				if(x>xMax)
					xMax = x;
				if(y<yMin)
					yMin = y;
				if(y>yMax)
					yMax = y;
				if(z<zMin)
					zMin = z;
				if(z>zMax)
					zMax = z;					
				direction = getDirection(x,y,z, temp);
				if (direction == 0)//points are not next to each other
				{
					cout << "ERROR:invalid input file\n";
					input.close();
					return;
				}
				temp = temp->generateNew(direction,l,w,h);
			}
		}
	}
	xOffset = l/2 - (xMin+xMax)/2;
	yOffset = w/2 - (yMin+yMax)/2;
	zOffset = h/2 - (zMin+zMax)/2;
	shortList = new point*[numElements];
	
	temp = head;
	for(int i = 0;i<numElements;i++)
	{
		temp->setX(temp->getX()+xOffset);
		temp->setY(temp->getY()+yOffset);
		temp->setZ(temp->getZ()+zOffset);
		
		if(a[temp->getX()][temp->getY()][temp->getZ()] == NULL)
		{
			temp -> setNum(i);//sets number
			shortList[i] = a[temp->getX()][temp->getY()][temp->getZ()] = temp;
			temp = temp->getPointOut();
		}
		else
			cout << "ERROR:	duplicate points\n";
	}
	input.close();
	return;
}

int lattice::getDirection(int x,int y,int z, point* temp)
{
	if(x-(temp->getX())==1 && (y-temp->getY())==0 && (z-temp->getZ())==0)
		return 1;
	else if(x-(temp->getX())==-1 && (y-temp->getY())==0 && (z-temp->getZ())==0)
		return 6;
	else if(x-(temp->getX())==0 && (y-temp->getY())==1 && (z-temp->getZ())==0)
		return 2;
	else if(x-(temp->getX())==0 && (y-temp->getY())==-1 && (z-temp->getZ())==0)
		return 5;
	else if(x-(temp->getX())==0 && (y-temp->getY())==0 && (z-temp->getZ())==1)
		return 3;
	else if(x-(temp->getX())==0 && (y-temp->getY())==0 && (z-temp->getZ())==-1)
		return 4;
	else
		return 0;
}
bool lattice::shiftRandom()
{
	int randInt = rand()%numElements;
	point* temp = shortList[randInt];
	return shiftPoint(temp->getX(),temp->getY(),temp->getZ());
}

bool lattice::shiftPoint(int x, int y, int z)
{
	if(a[x][y][z] == NULL)//check if the spot in the array contains a point
	{
		cout << "No data at point specified (" << x << "," << y << "," << z << ")\n";
		return false;
	}
	
	point* mobile;//the point that's moving
	point* mobile2; //the second point that's moving (for hinge case)
	point* hinge;//for the hinge case and the end point case
	point* hinge2; //hinge connected to mobile2
	int newDirection; //for end point
	int newX, newY, newZ, newX2, newY2, newZ2;
	mobile = a[x][y][z];
	if(mobile->getDirectionIn()==0 || mobile->getDirectionOut()==0)//end point
	{
		//get the hinge(point next to the point that moved)
		if(mobile->getDirectionIn()==0)
		{
			hinge = mobile->getPointOut();
		}
		else
		{
			hinge = mobile->getPointIn();
		}
		
		newX = hinge->getX();
		newY = hinge->getY();
		newZ = hinge->getZ();
		
		//the new point is one point off of the hinge, this generates the random direction
 		newDirection = rand()%6+1;
		while(newDirection == hinge->getDirectionOut() || newDirection == hinge->getDirectionIn())
			newDirection = rand()%6+1;
		if(newDirection == 1)
			newX++;
		if(newDirection == 2)
			newY++;
		if(newDirection == 3)
			newZ++;
		if(newDirection == 4)
			newZ--;
		if(newDirection == 5)
			newY--;
		if(newDirection == 6)
			newX--;
		if(a[newX][newY][newZ]==NULL)//change the point
		{
			a[x][y][z] = NULL;
			if(mobile->getDirectionIn()==0)
			{
				mobile->setDirectionOut(7-newDirection);
				hinge->setDirectionIn(newDirection);
			}
			else
			{
				mobile->setDirectionIn(7-newDirection);
				hinge->setDirectionOut(newDirection);
			}
			
			mobile->setX(newX);
			mobile->setY(newY);
			mobile->setZ(newZ);
			a[newX][newY][newZ]=mobile;
			cout << "End point move successfull\n";
			return true;//transformation has been made
		}
		else
		{
			cout << "Space (" << newX << "," << newY << "," << newZ << ") already occupied\n";
			return false;
		}
	}
	if((mobile -> getDirectionIn())+(mobile -> getDirectionOut())!=7)//corner or hinge (check to make sure the directions in and out aren't apart from each other)
	{
		if(mobile->getDirectionIn()==mobile->getPointOut()->getDirectionOut() || mobile->getDirectionOut()==mobile->getPointIn()->getDirectionIn())//if true -> hinge
		{
			//set it up so that mobile 1 is before mobile 2 (this won't change anything because you're moving them both the same)
			if(mobile->getDirectionIn()==mobile->getPointOut()->getDirectionOut())
			{
				mobile2 = mobile->getPointOut();
				hinge = mobile->getPointIn();
				hinge2 = mobile2->getPointOut();
			}
			if(mobile->getDirectionOut()==mobile->getPointIn()->getDirectionIn())
			{
				mobile2 = mobile;
				mobile = mobile->getPointIn();
				hinge = mobile->getPointIn();
				hinge2 = mobile2->getPointOut();
			}
			
			newDirection = rand()%6+1;//determine new direction for the hinge
			while((newDirection == hinge->getDirectionOut()) || (newDirection == mobile->getDirectionOut()) || (newDirection == 7-mobile->getDirectionOut()))
				newDirection = rand()%6+1;
				
			//create both new points based on the randomly generated new direction
			newX = hinge->getX();
			newY = hinge->getY();
			newZ = hinge->getZ();
			if(newDirection == 1)
				newX++;
			if(newDirection == 2)
				newY++;
			if(newDirection == 3)
				newZ++;
			if(newDirection == 4)
				newZ--;
			if(newDirection == 5)
				newY--;
			if(newDirection == 6)
				newX--;
				
			newX2 = hinge2->getX();
			newY2 = hinge2->getY();
			newZ2 = hinge2->getZ();
			if(newDirection == 1)
				newX2++;
			if(newDirection == 2)
				newY2++;
			if(newDirection == 3)
				newZ2++;
			if(newDirection == 4)
				newZ2--;
			if(newDirection == 5)
				newY2--;
			if(newDirection == 6)
				newX2--;
				
			if(a[newX][newY][newZ] != NULL)
			{
				cout << "Space (" << newX << "," << newY << "," << newZ << ") already occupied\n";
				return false;
			}
			if(a[newX2][newY2][newZ2] != NULL)//check to make sure both new points don't intersect with existing array
			{
				cout << "Space (" << newX2 << "," << newY2 << "," << newZ2 << ") already occupied\n";
				return true;
			}
			else
			{
				//alter hinge direction outs
				hinge->setDirectionOut(newDirection);
				hinge2->setDirectionIn(newDirection);
				
				//set old points in array equal to NULL
				a[mobile->getX()][mobile->getY()][mobile->getZ()] = NULL;
				a[mobile2->getX()][mobile2->getY()][mobile2->getZ()] = NULL;
				
				//alter mobile
				mobile->setX(newX);
				mobile->setY(newY);
				mobile->setZ(newZ);
				mobile->setDirectionIn(7-newDirection);
				
				//alter mobile2
				mobile2->setX(newX2);
				mobile2->setY(newY2);
				mobile2->setZ(newZ2);
				mobile2->setDirectionOut(7-newDirection);
				
				//set new points in array equal to mobile and mobile2
				a[mobile->getX()][mobile->getY()][mobile->getZ()] = mobile;
				a[mobile2->getX()][mobile2->getY()][mobile2->getZ()] = mobile2;
				cout << "Hinge move successfull\n";
				return true;//transformation has been made
			}
		}		
		else//corner angle
		{
			hinge = mobile->getPointIn();
			newX = hinge->getX();
			newY = hinge->getY();
			newZ = hinge->getZ();
			newDirection=mobile->getDirectionOut();
			if(newDirection == 1)
				newX++;
			if(newDirection == 2)
				newY++;
			if(newDirection == 3)
				newZ++;
			if(newDirection == 4)
				newZ--;
			if(newDirection == 5)
				newY--;
			if(newDirection == 6)
				newX--;
			if(a[newX][newY][newZ]==NULL)//only move if the place to move to is empty
			{
				a[mobile->getX()][mobile->getY()][mobile->getZ()] = NULL;//set old point in array to NULL
				
				//alter the new points and directions of mobile
				mobile->setX(newX);
				mobile->setY(newY);
				mobile->setZ(newZ);
				int oldIn = mobile -> getDirectionIn();				
				mobile->setDirectionIn(7-mobile->getDirectionOut());
				mobile->setDirectionOut(7-oldIn);
				mobile->getPointIn()->setDirectionOut(7-mobile->getDirectionIn());
				mobile->getPointOut()->setDirectionIn(7-mobile->getDirectionOut());
				
				a[mobile->getX()][mobile->getY()][mobile->getZ()] = mobile;//set new point in array to mobile
				cout << "corner move successfull\n";
				return true;//transformation has been made
			}
			else
			{
				cout << "Space (" << newX << "," << newY << "," << newZ << ") already occupied\n";
				return false;
			}
		}
	}
	else
	{
		cout << "straight angle, no moves possible\n";
		return false;
	}
}

void lattice::fillLattice()//create n points in the lattice
{
	int x,y,z,direction;
	
	srand(time(NULL));
	//the first point goes in the middle
	x = l/2;
	y = w/2;
	z = h/2;
	head = a[x][y][z] = new point(x,y,z);
	point* duplicate;
	point* temp = head;
	for(int i=0;i<numElements;i++)//create new points and add them to the grid
	{
		direction = (rand()%6)+1;
		temp = temp->generateNew(direction,l,w,h);
		if(a[temp->getX()][temp->getY()][temp->getZ()] == NULL)
			a[temp->getX()][temp->getY()][temp->getZ()] = temp;
		else
		{
			duplicate = temp;
			temp = temp->getPointIn();
			delete duplicate;
			i--;
		}	
	}
	return;
}

point* lattice::chooseElement()
{
	point* toReturn;
	int n; //#point to change
	n = (rand()%(numElements-2))+1;//choose a random number that isn't one of the ends
	toReturn = head;
	for(int i=1;i<n;i++)
		toReturn = toReturn->getPointOut();
	return toReturn;
}

void lattice::printLattice(const char* fileOut)
{
	FILE* outputFile;
	outputFile = fopen(fileOut,"a");
	point* trace = head;
	int num = 1;
	fprintf(outputFile, "MODEL %i\n", model);
	while(trace != NULL)
	{
		fprintf(outputFile, "%s%7d  %s %s %s%4d    %8.3f%8.3f%8.3f  %4.2f  %4.3f\n", "ATOM", num, "CA ", "GLY", "A", num, (double)(trace->getX()), (double)(trace->getY()), (double)(trace->getZ()), 1.00, 0.00);
		num++;
		trace = trace -> getPointOut();
	}
	fprintf(outputFile,"ENDMDL\n\n");
	model++;
	fclose(outputFile);
	return;
}

void lattice::printLattice()
{	
	point* trace = head;
	int num = 1;
	printf("MODEL %i\n", model);
	while(trace != NULL)
	{
		printf("%s%7d  %s %s %s%4d    %8.3f%8.3f%8.3f  %4.2f  %4.3f\n", "ATOM", num, "CA ", "GLY", "A", num, (double)(trace->getX()), (double)(trace->getY()), (double)(trace->getZ()), 1.00, 0.00);
		num++;
		trace = trace -> getPointOut();
	}
	printf("ENDMDL\n\n");
	model++;
	return;
}
	
void lattice::rotateTail()
{
	int* values;
	point* cursor;
	
	point* pivot = chooseElement();
	int rotation = determineRotation(pivot);
	while(!checkRotation(pivot, rotation))//keep changing
	{
		pivot = chooseElement();
		rotation = determineRotation(pivot);
	}
	cursor = pivot;
	while(cursor!=NULL)
	{
		a[cursor->getX()][cursor->getY()][cursor->getZ()] = NULL;//set old array pointer to NULL
		
		values = getNewPoints(rotation, cursor, pivot);//change cursor values
		cursor->setX(values[0]);
		cursor->setY(values[1]);
		cursor->setZ(values[2]);
		
		a[cursor->getX()][cursor->getY()][cursor->getZ()] = cursor;//connect cursor back to array
		cursor = cursor -> getPointOut();
	}
	model++;
	return;
}


int lattice::determineRotation(point* pivot)
{
	int before, after;
	before = pivot -> getDirectionOut();//old direction out
	after = (rand()%6)+1;//determine new direction out
	while(after==before || after==pivot->getDirectionIn())
		after = (rand()%6)+1;
	
	if((before==3&&after==1)||(before==1&&after==4)||(before==4&&after==6)||(before==6&&after==3))//cwFlat
		return 1;
	if((before==1&&after==3)||(before==4&&after==1)||(before==6&&after==4)||(before==3&&after==6))//ccFlat
		return -1;
	if((before==2&&after==4)||(before==4&&after==5)||(before==5&&after==3)||(before==3&&after==2))//cwWheel
		return 2;
	if((before==4&&after==2)||(before==5&&after==4)||(before==3&&after==5)||(before==2&&after==3))//ccWheel
		return -2;
	if((before==2&&after==1)||(before==1&&after==5)||(before==5&&after==6)||(before==6&&after==2))//cwCircle
		return 3;
	if((before==1&&after==2)||(before==5&&after==1)||(before==6&&after==5)||(before==2&&after==6))//ccCircle
		return -3;
	if((before==1&&after==6)||(before==6&&after==1))//flipX
	{
		if(rand()%2 == 1)
			return 4;
		else
			return 5;
	}
	if((before==2&&after==5)||(before==5&&after==2))//flipX
	{
		if(rand()%2 == 1)
			return 6;
		else
			return 7;
	}
	if((before==3&&after==4)||(before==4&&after==3))//flipX
	{
		if(rand()%2 == 1)
			return 8;
		else
			return 9;
	}
	else
		cout << "Invalid inputs in determine rotation\n Before: " << before << "\nAfter: " << after << "\n";
}

bool lattice::checkRotation(point* pivot, int rotation) //check the rotation of all points after the pivot with all points before the pivot
{
	point* cursor = pivot->getPointOut();
	while(cursor != NULL)
	{	
		//check current cursor against all early items before pivot
		if(isEarly(pivot, cursor, rotation))//return true if you find a match
			return false;
		cursor = cursor -> getPointOut();
	}
	return true;//if you get to the end return true
}

bool lattice::isEarly(point* pivot, point* current, int rotation)
{
	int x,y,z;
	int* values;
	point* cursor = head;
	values = getNewPoints(rotation, current, pivot);//values current will be changed to after rotation
	x = values[0];
	y = values[1];
	z = values[2];
	while(cursor!=pivot)//check all before pivot
	{
		if((cursor->getX()==x)&&(cursor->getY()==y)&&(cursor->getZ()==z))//if new location will result in overlap with existing point
			return true;
		cursor = cursor -> getPointOut();
	}
	return false;
}

int* lattice::getNewPoints(int rotation, point* current, point* pivot)
{
	int xDisplace, yDisplace, zDisplace,funcX, funcY, funcZ;//displacement based on the pivot
	xDisplace = pivot->getX();
	yDisplace = pivot->getY();
	zDisplace = pivot->getZ();
	funcX = (current->getX())-xDisplace;//x,y,z to be swapped/negated
	funcY = (current->getY())-yDisplace;
	funcZ = (current->getZ())-zDisplace;
	
	int* toReturn = new int[3];
	if(rotation==1)
	{
		toReturn[0] = funcZ;
		toReturn[1] = funcY;
		toReturn[2] = -(funcX);
	}
	if(rotation==-1)
	{
		toReturn[0] = -(funcZ);
		toReturn[1] = funcY;
		toReturn[2] = funcX;
	}
	if(rotation==2)
	{
		toReturn[0] = funcY;
		toReturn[1] = -(funcX);
		toReturn[2] = funcZ;
	}
	if(rotation==-2)
	{
		toReturn[0] = -(funcY);
		toReturn[1] = funcX;
		toReturn[2] = funcZ;
	}
	if(rotation==3)
	{
		toReturn[0] = funcX;
		toReturn[1] = -(funcZ);
		toReturn[2] = funcY;
	}
	if(rotation==-3)
	{
		toReturn[0] = funcX;
		toReturn[1] = funcZ;
		toReturn[2] = -(funcY);
	}
	if(rotation==4||rotation==-4)
	{
		toReturn[0] = -(funcX);
		toReturn[1] = -(funcY);
		toReturn[2] = funcZ;
	}
	if(rotation==5||rotation==-5)
	{
		toReturn[0] = -(funcX);
		toReturn[1] = funcY;
		toReturn[2] = -(funcZ);
	}
	if(rotation==6||rotation==-6)
	{
		toReturn[0] = -(funcX);
		toReturn[1] = -(funcY);
		toReturn[2] = funcZ;
	}
	if(rotation==7||rotation==-7)
	{
		toReturn[0] = funcX;
		toReturn[1] = -(funcY);
		toReturn[2] = -(funcZ);
	}
	if(rotation==8||rotation==-8)
	{
		toReturn[0] = funcX;
		toReturn[1] = -(funcY);
		toReturn[2] = -(funcZ);
	}
	if(rotation==9||rotation==-9)
	{
		toReturn[0] = -(funcX);
		toReturn[1] = funcY;
		toReturn[2] = -(funcZ);
	}	
	if(rotation==0||rotation>9||rotation<-9)
		cout << "Error with getNewPoints\tRotation: " << rotation << "\n";
	//reverse displacement
	toReturn[0]+=xDisplace;
	toReturn[1]+=yDisplace;
	toReturn[2]+=zDisplace;	

	return toReturn;
}

	/*
	determine rotation
	flat 	(looking down)(from 2)
	wheel	(coming out)(from 1)
	circle	(looking at)(from 3)
	cwflat	3->1, 1->4, 4->6, 6->3	(x,y,z)->(z,y,-x)		1
	ccflat  1->3, 3->6, 6->4, 4->1	(x,y,z)->(-z,y,x)		-1
	cwwheel	2->4, 4->5, 5->3, 3->2	(x,y,z)->(y,-x,z)		2
	ccwheel 4->2, 5->4, 3->5, 2->3	(x,y,z)->(-y,x,z)		-2
	cwcirc  2->1, 1->5, 5->6, 6->2	(x,y,z)->(x,-z,y)		3
	cccric	1->2, 5->1, 6->5, 2->6	(x,y,z)->(x,z,-y)		-3
	flipx	1->6, 6->1		(x,y,z)->(-x,-y,z)		4,-4
					(x,y,z)->(-x,y,-z)		5,-5
	flipy	2->5, 5->2		(x,y,z)->(-x,-y,z)		6,-6
					(x,y,z)->(x,-y,-z)		7,-7
	flipz	3->4, 4->3		(x,y,z)->(x,-y,-z)		8,-8
					(x,y,z)->(-x,y,-z)		9,-9
	There is some overlap among the flips
	*/

int* lattice::generateRandom()
{
	int* toReturn = new int(3);
	int index = rand()%numElements;
	toReturn[0] = shortList[index]->getX();
	toReturn[1] = shortList[index]->getY();
	toReturn[2] = shortList[index]->getZ();
	return toReturn;
}

float lattice::end2end()//return the distance between the first and the last point of the lattice
{
	point* tail = shortList[numElements-1];
	cout << "(" << head->getX() << "," << head->getY() << "," << head->getZ() << ") (" << tail->getX() << "," << tail->getY() << "," << tail->getZ() << ")\n";
	return sqrt(((head->getX() - tail->getX())*(head->getX() - tail->getX())) + ((head->getY() - tail->getY())*(head->getY() - tail->getY())) + ((head->getZ()-tail->getZ())*(head->getZ()-tail->getZ()))); //distance formula
}

void lattice::copyLattice(lattice* otherLattice)
{
	int x,y,z;
	point* cursor = head;
	point* otherCursor = otherLattice -> getHead();
	model = otherLattice -> getModel();
	clearArray();
	while(cursor != NULL)//change the linked list and the array
	{
		cursor -> copyPoint(otherCursor);
		x = cursor -> getX();
		y = cursor -> getY();
		z = cursor -> getZ();
		a[x][y][z] = cursor;
		cursor = cursor -> getPointOut();
		otherCursor = otherCursor -> getPointOut();
	}
	
}

void lattice::clearArray()
{
	for(int i=0;i<l;i++)
		for(int j=0;j<w;j++)
			for(int k=0;k<h;k++)
				a[i][j][k] = NULL;
}




