#include "SZ_PathfindingList.h"


SZ_PathfindingList::SZ_PathfindingList()
{
	amount = 0;
}

//empties the pathfinding list from all its entries
SZ_PathfindingList::~SZ_PathfindingList()
{
	for (int i = 0;i<NODES_AMOUNT;i++)
	{
		if (NodesRecord[i]!=NULL)
		{
			delete NodesRecord[i];
		}
	}
}

void SZ_PathfindingList::empty()
{
	if (amount>0)
	{
		//for (int i = 0;i<NODES_AMOUNT;i++)
		for (int i = 0;i<length();i++)
		{
			if (NodesRecord[i]!=NULL)
			{
				delete NodesRecord[i];
				NodesRecord[i]=NULL;
				//this->substract(NodesRecord[i]);
			
			}
		}
	}

	amount=0;
}

int SZ_PathfindingList::length2()
{
	int amount2 = 0;
	for (int i = 0;i<NODES_AMOUNT-1;i++)
	{
		if (NodesRecord[i]!=NULL)
		{
			amount2 ++;
		}
	}
	return amount2;
}


int SZ_PathfindingList::length()
{
	return amount;
}

SZ_NodeRecord* SZ_PathfindingList::smallestElement()
{	
	float smallest = INT_MAX;
	int smallestindex =0;
	for (int i = 0;i<amount;i++)
	{
		if (NodesRecord[i]!=NULL)
		{
			if (this->NodesRecord[i]->costSoFar<smallest)
			{
				smallest = NodesRecord[i]->costSoFar;
				smallestindex = i;
			}
		}

	}
	return NodesRecord[smallestindex];
}


bool SZ_PathfindingList::contains(SZ_Node* whichNode)
{
	bool found = false;
	if (amount>0)
	{
		int counter = 0;
		while ( (counter<amount)  )
		{
			
			if (NodesRecord[counter]->gNode==whichNode)
			{
				found = true;
				counter = amount;
			}
			counter++;

		}
	}
	return found;
}


SZ_NodeRecord* SZ_PathfindingList::find(SZ_Node* whichNode)
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

}


void SZ_PathfindingList::add(SZ_NodeRecord* whichNodeRecord)
{
	if (amount <NODES_AMOUNT)
		{
			NodesRecord[amount] = whichNodeRecord;
			amount++;		
		}
}

void SZ_PathfindingList::substract(SZ_NodeRecord* whichNodeRecord)
{	
	bool found = false;
	int counter = 0;
	while ( (counter<amount)  )
	{
		
		if (NodesRecord[counter]==whichNodeRecord)
		{
			//delete NodesRecord[counter];
			amount--;
			NodesRecord[counter]= NodesRecord[amount];		
			NodesRecord[amount] = NULL;
			counter=amount;

		}
		counter++;

	}

	

	/*
	for (int i = 0;i<NODES_AMOUNT;i++)
	{
		if (NodesRecord[i]!=NULL)
		{
			printf("nodes info %i %i \n",i,NodesRecord[i]->gNode->number);
		}

	}
*/

	

/*
		if ((amount > 0 ) && ( amount!=whichParticle) )    //if is not deleting the last particle
		{	
			if( pParticle != NULL)
			{
				delete particles[whichParticle];
				particles[whichParticle] = particles[amount];
				particles[whichParticle] = NULL;
				amount--;
			}
		}
	else if ((amount > 0 ) && ( amount==whichParticle) ) //if it's the last particle
		{			
			delete particles[amount];
			particles[amount]=NULL;
			amount--;
		}
*/
}