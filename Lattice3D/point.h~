#ifndef POINT_H
#define POINT_H

/*
X+ = 1
Y+ = 2
Z+ = 3
Z- = 4
Y- = 5
X- = 6
*/

class point
{
	public:
		point();
		point(int a,int b,int c);
		point(point* before, int d, int l, int w, int h);
		~point();
		point* generateNew(int d, int l=1, int w=1, int h=1);//creates a new point given a direction
		int shape();//0=end, 1=sraight, 2=corner
		void printPoint();
		
		//get and set stuff
		int getDirectionIn() {return directionIn;};
		int getDirectionOut() {return directionOut;};
		point* getPointIn() {return pointIn;};
		point* getPointOut() {return pointOut;};
		int getX() {return x;};
		int getY() {return y;};
		int getZ() {return z;};
		int getNum() {return number;};
		void setDirectionIn(int in) {directionIn = in;};
		void setDirectionOut(int in) {directionOut = in;};
		void setPointIn(point* in) {pointIn = in;};
		void setPointOut(point* in) {pointOut = in;};
		void setX(int in) {x=in;};
		void setY(int in) {y=in;};
		void setZ(int in) {z=in;};
		void setNum(int in) {number=in;};
	private:
		int number;//the unique number of the point
		int directionIn;
		int directionOut;
		point* pointIn;
		point* pointOut;
		int x;
		int y;
		int z;
		void movePoint(int d);//move one space based on input
};
#endif
