#ifndef NEIGHBOR_H
#define NEIGHBOR_H

class neighbor//node for a linked list
{
	public:
		neighbor(){data = -1;next=0;}
		~neighbor(){}
		int data;
		neighbor* next;
	private:
};
#endif
