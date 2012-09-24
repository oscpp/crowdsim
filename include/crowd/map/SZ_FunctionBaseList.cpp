#include "SZ_FunctionBaseList.h"
#include "..\SZ_World.h"



SZ_FunctionBaseList::SZ_FunctionBaseList()
{
	amount = 0;
}

// Empties the pathfinding list from all its entries
SZ_FunctionBaseList::~SZ_FunctionBaseList()
{
	if (amount>0)
	{
		for (int i = 0; i<amount;i++)
		{
			if (FunctionBaseList[i]!=NULL)
			{
				//delete FunctionBaseList[i];
				FunctionBaseList[i]=NULL;			
			}
		}
	}
	amount=0;
}

void SZ_FunctionBaseList::drawNeighbour()
{
	for (int i = 0; i<amount;i++)
	{				
		if (FunctionBaseList[i]!=NULL)
		{
			FunctionBaseList[i]->drawNeighbour();
		}
		
	}
}

void SZ_FunctionBaseList::drawQuad()
{
	for (int i = 0; i<amount;i++)
	{				
		if (FunctionBaseList[i]!=NULL)
		{
			FunctionBaseList[i]->drawQuad();
		}
		
	}
}

void SZ_FunctionBaseList::update()
{
	for (int i = 0; i<amount;i++)
	{
		if (FunctionBaseList[i]!=NULL)
		{
			FunctionBaseList[i]->update();
		}
	}
}

// modifies the connection of each node
void SZ_FunctionBaseList::modifyNode()
{

	for (int i = 0; i<amount;i++)		// go through all events
	{				
		if (FunctionBaseList[i]!=NULL)  // make sure the event exist
		{
			
			{				
				SZ_Node* currentNode = (((SZ_World*)world)->Utils.gNodes[FunctionBaseList[i]->currentCell]);
				printf("Node %i has an event \n", currentNode->number);

				// find all surrounding nodes
				
				for (int j=0; j<currentNode->amountConnections;j++)
				{
					if ( currentNode->connection[j]->toNodeAddress!=NULL)
					{
						SZ_Node* tmpNode = ((SZ_Node*)(currentNode->connection[j]->toNodeAddress));
						printf("node %i has got %i as neighbour \n",currentNode->number,tmpNode->number);
						tmpNode->connection[tmpNode->whichConnection(currentNode->number)]->cost *= 10.0f;
					}
				}

				/*
				for (int j=0;j< currentNode->amountConnections;j++)
				{
					if ( currentNode->connection[j]!=NULL)
					{
						
						// Goto all neighbour nodes
						

						
						SZ_Node* tmpNode	 = ((SZ_Node*)((currentNode->connection[j]->toNodeAddress)));

						printf("connection %i on node %i leads to %i \n",tmpNode->whichConnection(currentNode->number),currentNode->number,tmpNode->number);

						;
						*/


/*
						// find connection to currentNode
						for (int k=0;k< tmpNode->amountConnections;k++)
						{
							if ( tmpNode->connection[k]->toNodeAddress !=NULL )
							{
								SZ_Node* tmpNodeToAddress = ((SZ_Node*)((tmpNode->connection[k]->toNodeAddress)));
								if (tmpNodeToAddress == currentNode) { printf("connection %i \n",k); }
							}
						}



					}
					
				}*/
			}
		}
	}
}

void SZ_FunctionBaseList::init(void *world)
{
	this->world = world;
	
	// get stored information about each node from places.bmp
	for (int i=0;i<NODES_AMOUNT;i++) // go through each node
		{
			if (((SZ_World*)world)->Utils.gNodes[i]==NULL) // if the node is non existant, no information will ever be stored about it.
			{
				continue;
			}	


	
				char cChtime[5];
				for (int j = 1; j<2; j++)
					{
						if ((((SZ_World*)world)->Utils.bitmapplaces.getRGBXY(2,j,i)) != 255)
						{
							for (int k = 1; k<=3 ; k++)
							{				
								cChtime[k-1] = (char)(((SZ_World*)world)->Utils.bitmapplaces.getRGBXY(k,j,i)); // convert int to ascii
							}
						}
					}
				cChtime[4] = '\n';
				int time = atoi(cChtime);
				//printf("time: %i - ", time);



			// recall: always saves as ASCII characters
			if (((SZ_World*)world)->Utils.bitmapplaces.getRGBXY(2,0,i) != 255)
			{
				//printf("node %i contains information: ",i);						
				for (int j = 0; j<((SZ_World*)world)->Utils.bitmapplaces.getWidth(); j++)
				{
					for (int k = 1; k<=3 ; k++)
					{
						//printf("%i ",Utils.bitmapplaces.getRGBXY(k,j,i));						
						char cCh = (char)(((SZ_World*)world)->Utils.bitmapplaces.getRGBXY(k,j,i)); // convert int to ascii
						//printf("%c", cCh);					
					}
				}
				//printf("\n");
				
				// get row
				char cCh[2];
				cCh[0] = (char)(((SZ_World*)world)->Utils.bitmapplaces.getRGBXY(1,0,i)); 
				cCh[1] = '\n';	// atoi doesn't like strings that do not finish in a null terminator
				int row = atoi(cCh);
					
				// get index
				char cCh2[3];
				cCh2[0] = (char)(((SZ_World*)world)->Utils.bitmapplaces.getRGBXY(2,0,i)); 
				cCh2[1] = (char)(((SZ_World*)world)->Utils.bitmapplaces.getRGBXY(3,0,i));
				cCh2[2] = '\n';	// atoi does not like strings that do not finish in a null terminator
				int index = atoi(cCh2);

				// get index and rownumber
				char cCh3[4];
				cCh3[0] =	(char)(((SZ_World*)world)->Utils.bitmapplaces.getRGBXY(1,0,i)); 
				cCh3[1] =	(char)(((SZ_World*)world)->Utils.bitmapplaces.getRGBXY(2,0,i)); 
				cCh3[2] =	(char)(((SZ_World*)world)->Utils.bitmapplaces.getRGBXY(3,0,i));
				cCh3[3] = '\n';	// atoi does not like strings that do not finish in a null terminator
				int eventnumber = atoi(cCh3);

				/*
				printf("row: %i \n",row);
				printf("index %i \n",index);
				printf("\n");
				*/

				// create a new object 
				SZ_FunctionBase* tmpBase;
				tmpBase = new SZ_FunctionBase();

				// now initialise the Function using size
				tmpBase->init(i,world,0);	
				tmpBase->row = row;
				tmpBase->index = index;
				tmpBase->eventnumber = eventnumber;
				tmpBase->timecost = time;

				add(tmpBase);

				((SZ_World*)world)->Utils.gNodes[i]->functions = tmpBase;
			}			
		}
	//printf(" amount of Functionbase: %i \n", this->amount);
}

void SZ_FunctionBaseList::empty()
{
	for (int i = 0;i<LOCALBASELIST_AMOUNT-1;i++)
		{
			if (FunctionBaseList[i]!=NULL)
			{
				delete FunctionBaseList[i];
				FunctionBaseList[i]=NULL;			
			}
		}
	amount=0;
}

int SZ_FunctionBaseList::length2()
{
	int amount2 = 0;
	for (int i = 0;i<LOCALBASELIST_AMOUNT-1;i++)
	{
		if (FunctionBaseList[i]!=NULL)
		{
			amount2 ++;
		}
	}
	return amount2;
}


int SZ_FunctionBaseList::length()
{
	return amount;
}


void SZ_FunctionBaseList::add(SZ_FunctionBase* whichLocalBase)
{
		if (amount <LOCALBASELIST_AMOUNT)
		{
			
			//if (find(whichNodeRecord) == false)	// check if we do not already have this record in our list.
			{
				//pParticle = new SZ_Shop();
				//pParticle->createShop(this->Position, this->Size, this->Influence,theScene,theSDK);
			
				//pParticle->ShopNumber = amount;
					
				//for (int i=0;i<TOTAL_AMOUNT_ACTIONS;i++)
				//{
				//	pParticle->offer_action[i] = this->offer_action[i];
				//}
				FunctionBaseList[amount] = whichLocalBase;
				amount++;
			}
		
		}
}

void SZ_FunctionBaseList::substract(SZ_FunctionBase* whichLocalBase)
{	
	bool found = false;
	int counter = 0;
	while ( (counter<amount)  )
	{		
		if (FunctionBaseList[counter]==whichLocalBase)
		{
			//delete NodesRecord[counter];
			amount--;
			FunctionBaseList[counter]= FunctionBaseList[amount];		
			FunctionBaseList[amount] = NULL;
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

SZ_FunctionBase* SZ_FunctionBaseList::findSpecificEvent( int whichEvent )
{	
	bool found = false;
	int counter = 0;
	while ( (counter<amount)  )
	{

		if (FunctionBaseList[counter]->eventnumber==whichEvent)
		{
			found = true;
			return FunctionBaseList[counter];
			counter = amount;
		}
		counter++;
	}
	return NULL;
}
