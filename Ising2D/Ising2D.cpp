#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<fstream>
using namespace std;


int energy(int**a, int n, int m);
void printPDB(int m, int n, int x, int**a);

//input length and width
int main(int argc, char*argv[])
{
	ofstream output; //output of data
	int n; //length and width of array
	int m; 
	int x; //frame#
	int previousEnergy; //previous energy state
	int currentEnergy;
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	int* a[n]; //array
	for(int i=0;i<n;i++)
		a[i] = new int[m];
	int nlocation; // random location in the array
	int mlocation;
	//initialize the array with random numbers
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			a[i][j]=rand()%2;
	
	srand(time(NULL)); //set the random seed
	previousEnergy = energy(a,n,m);
	x = 0;
	output.open("output.txt");
	
	output << x << "\t" << previousEnergy << "\n"; //output origional state
	printPDB(m,n,x,a);
	
	x++; //set x to 1
	
	while(energy(a,n,m) != 0)//end when energy is 0
	{
		nlocation = rand()%n;//change random item in array
		mlocation = rand()%m;
		a[nlocation][mlocation] = (a[nlocation][mlocation]+1)%2;
		currentEnergy = energy(a,n,m);
		//check if condition improved
		if(currentEnergy >= previousEnergy)//switch back
			a[nlocation][mlocation] = (a[nlocation][mlocation]+1)%2;
		else
		{
			if(currentEnergy < previousEnergy)//only output if state is changing
			{
				output << x << "\t" << currentEnergy << "\n";
				printPDB(m,n,x,a);
				previousEnergy = currentEnergy;
			}
		}
		x++;//increment the counter
	}
	output.close();
}

void printPDB(int m, int n, int x, int**a)
{
	cout << "MODEL " << x << endl;
	int num = 1;
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
		{
			printf("%s%7d  %s %s %s%4d    %8.3f%8.3f%8.3f  %4.2f  %4.3f\n", "ATOM", num, "CA ", "GLY", "A", num, (double)i, (double)j, (double)a[i][j], 1.00, 0.00);
			num++;
		}
	cout << "ENDMDL" << endl;
	return;
}

int energy(int**a, int n, int m)
{
	int sum;//energy state of array
	sum = 0;
	for(int i=0;i<n;i++)//check if any of the array items differ
	{
		for(int j=0;j<m;j++)
		{
			if(a[i][j] != a[i][(j+1)%m])
				sum++;
			if(a[i][j] != a[(i+1)%n][j])
				sum++;
		}
	}
	return sum;
}
