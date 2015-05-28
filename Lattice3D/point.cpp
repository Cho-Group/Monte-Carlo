#include "point.h"
#include <iostream>
using namespace std;

/*
X+ = 1
Y+ = 2
Z+ = 3
Z- = 4
Y- = 5
X- = 6
*/

point::point()
{
	directionIn = 0;
	directionOut = 0;
	point* pointIn;
	point* pointOut;
	x = 0;
	y = 0;
	z = 0;
	number = -1;
}

point::point(int a, int b, int c)
{
	directionIn = 0;
	directionOut = 0;
	pointIn = NULL;
	pointOut = NULL;
	x = a;
	y = b;
	z = c;
	number = -1;
}

point::point(point* before, int d, int l, int w, int h)
{
	directionIn = (7-before->getDirectionOut());
	directionOut = 0;
	pointIn = before;
	pointOut = NULL;
	x = before->getX();
	y = before->getY();
	z = before->getZ();
	movePoint(before->getDirectionOut());
	number = -1;
}

point::~point()
{
}

point* point::generateNew(int d, int l, int w, int h)
{
	directionOut = d;
	pointOut = new point(this, d, l, w, h);
	return pointOut;
}

int point::shape()//0=end, 1=sraight, 2=corner
{
	if(directionIn == 0 || directionOut == 0)
		return 0;
	if(directionIn+directionOut==7)
		return 1;
	else
		return 2;
}

void point::movePoint(int d)//move one space based on input
{
	if(d==1)
		x++;
	if(d==2)
		y++;
	if(d==3)
		z++;
	if(d==4)
		z--;
	if(d==5)
		y--;
	if(d==6)
		x--;
	if(d<1||d>6)
		cout << "Invalid Argument for movePoint\tValue:" << d << "\n";
	return;
}

void point::printPoint()
{
	cout << "X:" << x << " Y:" << y << " Z:" << z << "\n";
}

void point::copyPoint(point* otherPoint)
{
	directionIn = otherPoint -> getDirectionIn();
	directionOut = otherPoint -> getDirectionOut();
	pointIn = otherPoint -> getPointIn();
	pointOut = otherPoint -> getPointOut();
	x = otherPoint -> getX();
	y = otherPoint -> getY();
	z = otherPoint -> getZ();
	number = otherPoint -> getNum();
}




