#include "..\..\..\general_utils.h"
#include "..\SZ_Config.h"
#include "SZ_WorldPersonList.h"


SZ_WorldPersonList::SZ_WorldPersonList()
{
	amount = 0;
}

SZ_WorldPersonList::~SZ_WorldPersonList()
{
}

void SZ_WorldPersonList::empty()
{
	for (int i = 0;i<NUM_PERSON-1;i++)
	{
		//if (PersonRecord[i]!=NULL)
		{
			//delete PersonRecord[i];
			PersonRecord[i]=NULL;
			//this->substract(NodesRecord[i]);

		}
	}

	amount=0;
}

int SZ_WorldPersonList::length2()
{
	int amount2 = 0;
	for (int i = 0;i<NUM_PERSON-1;i++)
	{
		if (PersonRecord[i]!=NULL)
		{
			amount2 ++;
		}
	}
	return amount2;
}


int SZ_WorldPersonList::length()
{
	return amount;
}


bool SZ_WorldPersonList::contains(SZ_Person* whichPerson)
{
	bool found = false;
	int counter = 0;
	while ( (counter<amount)  )
	{
		
		if (PersonRecord[counter]==whichPerson)
		{
			found = true;
			counter = amount;
		}
		counter++;

	}
	return found;
}

void SZ_WorldPersonList::add(SZ_Person* whichPerson)
{
		if (amount <NUM_PERSON-1)
		{
			{
				PersonRecord[amount] = whichPerson;
				amount++;
			}
		
		}
		else { delete whichPerson; printf("Person not created \n"); }
}

void SZ_WorldPersonList::substract(SZ_Person* whichPerson)
{	
	bool found = false;
	int counter = 0;
	while ( (counter<amount)  )
	{
		
		if (PersonRecord[counter]==whichPerson)
		{
			//delete NodesRecord[counter];
			amount--;

			PersonRecord[counter]= PersonRecord[amount];		
			PersonRecord[amount] = NULL;
			counter=amount;
		}
		counter++;
	}
}

