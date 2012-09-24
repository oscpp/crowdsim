#include "SZ_NumberList.h"



SZ_NumberList::SZ_NumberList()
{
	for (int i = 0;i<max_number_int;i++)
	{
		intRecord[i]=-1;
	}
	amount = 0;
}

//empties the pathfinding list from all its entries
SZ_NumberList::~SZ_NumberList()
{
	for (int i = 0;i<max_number_int;i++)
	{
		if (intRecord[i]!=NULL)
		{
			//delete intRecord[i];
		}
	}
}

void SZ_NumberList::empty()
{
	for (int i = 0;i<max_number_int;i++)
	{
		intRecord[i]=-1;
	}
	amount=0;
	/*
	//if (amount>0)
	{
		for (int i = 0;i<max_number_int;i++)
		{
			//if (intRecord[i]!=-1)
			{
				//delete intRecord[i];
				//this->substract(intRecord[i]);
				intRecord[i]=-1;
				//this->substract(intRecord[i]);
			
			}
		}
	}
	*/

	amount=0;
}

int SZ_NumberList::length2()
{
	int amount2 = 0;
	for (int i = 0;i<max_number_int;i++)
	{
		if (intRecord[i]!=-1)
		{
			amount2 ++;
		}
	}
	return amount2;
}


int SZ_NumberList::length()
{
	return amount;
}


bool SZ_NumberList::contains(int whichInt)
{
	bool found = false;
	if (amount>0)
	{
		int counter = 0;
		while ( (counter<amount)  )
		{
			
			if (intRecord[counter]==whichInt)
			{
				found = true;
				counter = amount;
			}
			counter++;

		}
	}
	return found;
}



void SZ_NumberList::add(int whichInt)
{
		if (amount <max_number_int)
		{
			{
				intRecord[amount] = whichInt;
				amount++;
			}
		
		}
}

void SZ_NumberList::substract(int whichInt)
{	
	bool found = false;
	int counter = 0;
	while ( (counter<amount)  )
	{
		
		if (intRecord[counter]==whichInt)
		{
			amount--;

			intRecord[counter]= intRecord[amount];		
			intRecord[amount] = -1;
			counter=amount;

		}
		counter++;

	}

}

SZ_NumberList & SZ_NumberList::operator=( const SZ_NumberList &copyNumberList )
{
	empty();
	amount = copyNumberList.amount;
	for (int i=0; i<copyNumberList.amount; i++)
	{
		intRecord[i] = copyNumberList.intRecord[i];
	}
	return *this;
}
