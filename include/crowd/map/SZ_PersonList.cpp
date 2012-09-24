#include "..\..\..\general_utils.h"
#include "..\SZ_Config.h"
#include "SZ_PersonList.h"


SZ_PersonList::SZ_PersonList()
{
	amount = 0;
	for (int i = 0;i<MAX_PERSON_NODE;i++)
	{
			PersonRecord[i]=NULL;
	}
}

SZ_PersonList::~SZ_PersonList()
{
}

void SZ_PersonList::empty()
{
	for (int i = 0;i<MAX_PERSON_NODE-1;i++)
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

int SZ_PersonList::length2()
{
	int amount2 = 0;
	for (int i = 0;i<MAX_PERSON_NODE-1;i++)
	{
		if (PersonRecord[i]!=NULL)
		{
			amount2 ++;
		}
	}
	return amount2;
}


int SZ_PersonList::length()
{
	return amount;
}


bool SZ_PersonList::contains(SZ_Person* whichPerson)
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

void SZ_PersonList::add(SZ_Person* whichPerson)
{
		if (amount <MAX_PERSON_NODE-1)
		{
			{			
				PersonRecord[amount] = whichPerson;				
				amount++;
			}
		
		}
}

void SZ_PersonList::substract(SZ_Person* whichPerson)
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

/*
SZ_Record* SZ_PathfindingList::find(SZ_Node* whichNode)
{

	bool found = false;
	int counter = 0;
	while ( (counter<amount)  )
	{
		
		if (NodesRecord[counter]->gNode==whichNode)
		{
			found = true;
			return NodesRecord[counter];
			counter = amount;
		}
		counter++;
	}

	return NULL;

}*/


/*
		 SZ_PersonList();
		~SZ_PersonList();

		int amount;	// amount of NodeRecord in this Pathfindinglist

		SZ_Person* PersonRecord[MAX_PERSON_NODE];	// stores all the Person records

		SZ_Person* smallestElement();				// returns record with the smallest cost so far
		bool contains(SZ_Person* whichPerson);		// true if this list contains specified person
		SZ_Person* find(SZ_Person* whichPerson);	// 
		void add(SZ_Person* whichPerson);
		void substract(SZ_Person* whichPerson);
		void empty();
		int length();  // returns the amount of noderecords in this instance 
		int length2(); // returns the amount of noderecords in this instance by manually counting them 
		*/