#include<iostream>
#include<stdlib.h>
#include<time.h>
using namespace std;


int energy(int* a, int n);

int main(int argc, char*argv[])
{
	int n; //length of array
	n = atoi(argv[1]);
	int a[n]; //array
	int location; // random location in the array
	
	//initialize the array with random numbers
	for(int i=0;i<n;i++)
	{
		a[i]=rand()%2;
	}
	
	srand (time(NULL)); //set the random seed
	while(energy(a,n)!=0)//end when energy is 0
	{
		location = rand()%n;//change random item in array
		if(a[location] == 0)
			a[location] = 1;
		else
			a[location] = 0;
	}
	cout << "end of simulation\n[";
	for(int j=0;j<n;j++)
		cout << a[j] << ",";
	cout << "]\n";
}

int energy(int*a, int n)
{
	int sum;//energy state of array
	sum = 0;
	for(int i=0;i<n-1;i++)
	{
		if(a[i] != a[(i+1)%n])
			sum++;
	}
	return sum;
}
