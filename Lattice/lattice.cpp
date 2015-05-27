#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<cmath>
using namespace std;

struct point
{
	//0=North, 1=West, 2=East, 3=South 4=n/a (for the first and last points)
	int before;
	int after;
	int row;
	int column
}

int energy(point***a, int n, int m);
void initialize(point***a, int n, int m, int p);
void change(point***a, int n, int m);
void print(point***a,int n, int m);

int main(int argc, char*argv[])
{
	int n; //length and width of array
	int m;
	int p; //number of points
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	p = atoi(argv[3]);
	point** a[n]; //array
	for(int i=0;i<n;i++)
		a[i] = new point*[m];
	
	//initialize the array with random points
	initialize(a,n,m,p);
	
	while(energy(a,n,m)!=0)//end when energy is 0
	{
		change(a, n, m);//change random item in array
	}
	cout << "end of simulation\n";
	
	//print the array
	print(a,n,m);
}

void initialize(point**a, int n, int m, int p)
{
	int direction; //direction of the next point
	for(int i=0;i<n;i++) //set all to NULL
	{
		for(int j=0;j<m;j++)
			a[i][j] = NULL;
	}
	point* previous;
	point* current;
	
	int temp1, temp2; //initial cooridinates
	srand(time(NULL)); //set the random seed
	tempRow = rand()%n;
	tempCol = rand()&m;
	
	*previous = new point; //create an empty point at a random locatoin
	previous -> before = 5;
	previous -> after = 5;
	previous -> row = tempRow;
	previous -> column = tempCol;
	a[tempRow][tempCol] = previous; //store previous in the array		
	
	k=1; //first point was above
	while(k<p)//insert the additional points
	{
		
		direction = rand()%4
		if(direction == 0)//add the modulo
		{
			tempRow = ((previous -> row)+1)%n;
			tempCol = previous -> column;
		}
		if(direction == 1)
		{
			tempRow = previous -> row;
			tempCol = ((previous -> column)-1)%m;
		}
		if(direction == 2)
		{
			tempRow = previous -> row;
			tempCol = ((previous -> column)+1)%m;
		}
		if(direction == 3)
		{
			tempRow = ((previous -> row)-1)%n;
			tempCol = previous -> column;
		}
		if(a[tempRow][tempCol] == NULL)//only make a new point in an empty slot
		{
			previous -> after = direction
			*current = new point;
			current -> after = 5;
			current -> before = abs(3-direction);
			current -> row = tempRow
			current -> column = tempCol;
		
			previous = current;
			a[tempRow][tempCol] = current;//place the new point in the array
			k++;//increment counter
		}
	}	
}

int energy(int**a, int n, int m)
{
	//placeholder
}

int void change(point***a, int n, int m)
{
	point* aPoint; //point being moved
	while(true)
	{
		point = a[rand()%n][rand()%m];
		if(point->before == 5 || point->after == 5)//end point
		{
		}
		else if((point->before+point->after) == 3)//across from each other
		{
			// do nothing
		}
		else //corner
		{
		}
	}	
}
