#include "SZ_World.h"
#include "SZ_Node.h"
#include "SZ_FunctionBase.h"

int SZ_Node::nodeIndexCnt = 0;

SZ_Node::SZ_Node()
{
	 //index = nodeIndexCnt++; 
	 for (int i=0;i<amountConnections;i++)
	 {
		 connection[i] = new SZ_Connection();
		 //connection[i]->fromNodeAddress = this;
		 connection[i]->toNodeAddress = NULL;
		 connection[i]->cost = NxVec3(0,0,0);
	 }

	for (int i=0;i<NODES_AMOUNT;i++)
	{
		pathTo[i] = -1;		// initliase to -1 as NULL sets it to ZERO. 
							// when using paths, make sure to compare to -1. if -1, it's not set.
	}

	this->PersonList.empty();
	this->functions = NULL;
}


SZ_Node::~SZ_Node()
{
    for (int i=0;i<amountConnections;i++)
	{
		if (connection[i]!=NULL)
		{
		delete connection[i];
		connection[i] = NULL;
		}
	}
}



/*
int SZ_Node::getNodeXZ(int x, int z)
{
	int i = (WORLD_Z * (z)) + (x);
	return i;
}*/


void SZ_Node::draw(NxVec3 colour)
{
	if (this!=NULL)
	{
		glColor4f(colour.x,colour.y,colour.z,0.5f);
		//transform node number to XY (XZ) coordindates
		int X = (number % WORLD_X_5X5);	  // transform the tile value into the top left X corner
		int Z = (number-X)/WORLD_X_5X5;   // transform the tile value into the top left Y corner
		
		
		// draw the outcoming connections of the current node
		glBegin(GL_LINES);
		for (int i=0;i<amountConnections;i++)
		{
			if (this->connection!=NULL)
			{
				glVertex3f(X*WORLD_TILE	+WORLD_TILE	/2.0,0,Z*WORLD_TILE	+WORLD_TILE	/2.0);
				glVertex3f((X*WORLD_TILE+WORLD_TILE	/2.0)+connection[i]->cost.x,0,(Z*WORLD_TILE+WORLD_TILE/2.0)+connection[i]->cost.z);	
			}
		}
		glEnd();
	}
}
void SZ_Node::draw()
{
	if (this==NULL) { return; }		// do not do anything if this node points to null
	
	//glEnable(GL_BLEND);
	glColor4f(0,0.5,0,0.3);
	glLineWidth(1);
	glColor4f(0,0.5,0,0.3);
	//transform node number to XY (XZ) coordindates
	int X = (number % WORLD_X_5X5);	  // transform the tile value into the top left X corner
	int Z = (number-X)/WORLD_X_5X5;   // transform the tile value into the top left Y corner
	
	/*
	// draw cell population density	
	glColor4f(1,0,0,PersonList.amount/25.0f); // 25.0
	glBegin(GL_POLYGON);
	glVertex3f(X*WORLD_TILE+WORLD_TILE,0,Z*WORLD_TILE);
	glVertex3f(X*WORLD_TILE,0,Z*WORLD_TILE);
	glVertex3f(X*WORLD_TILE,0,Z*WORLD_TILE+WORLD_TILE);
	glVertex3f(X*WORLD_TILE+WORLD_TILE,0,Z*WORLD_TILE+WORLD_TILE);
	glEnd();
	*/
	
	
	/*
	//draw red rectangle.
	glColor4f(0.9,0.1,0,1);
	glLineWidth(4);
	glBegin(GL_LINE_LOOP);
	glVertex3f(X*WORLD_TILE				,0.04f,Z*WORLD_TILE);
	glVertex3f(X*WORLD_TILE + WORLD_TILE,0.04f,Z*WORLD_TILE);
	glVertex3f(X*WORLD_TILE	+ WORLD_TILE,0.04f,Z*WORLD_TILE+WORLD_TILE);
	glVertex3f(X*WORLD_TILE				,0.04f,Z*WORLD_TILE+WORLD_TILE);
	glEnd();
	*/
	/*
	glEnable(GL_BLEND);
	glColor4f(1,0.5,0,1.0);
	// draw connections from node
	glBegin(GL_LINES);
	for (int i=0;i<amountConnections;i++)
	{
		if (this->connection!=NULL)
		{
			glVertex3f(X*WORLD_TILE	+WORLD_TILE	/2.0,0,Z*WORLD_TILE	+WORLD_TILE	/2.0);
			glVertex3f((X*WORLD_TILE+WORLD_TILE	/2.0)+connection[i]->cost.x,0,(Z*WORLD_TILE+WORLD_TILE/2.0)+connection[i]->cost.z);	
		}
	}
	glEnd();
	*/
	
	/*
	if (pathTo[this->world->gCamera.RecticleCell]!=-1)
	{
		NxVec3 start = NxVec3(X*WORLD_TILE	+WORLD_TILE	/2.0,0,Z*WORLD_TILE	+WORLD_TILE	/2.0);
		NxVec3 ending= NxVec3(connection[pathTo[world->gCamera.RecticleCell]]->cost.x,0,connection[pathTo[world->gCamera.RecticleCell]]->cost.z);	

		world->Utils.vVectorList.newVector(start,ending,NxVec3(1,0,0));
		/*
		glBegin(GL_LINES);

		glVertex3f(X*WORLD_TILE	+WORLD_TILE	/2.0,0,Z*WORLD_TILE	+WORLD_TILE	/2.0);
		//glVertex3f((X*WORLD_TILE+WORLD_TILE	/2.0)+connection[i]->cost.x,0,(Z*WORLD_TILE+WORLD_TILE/2.0)+connection[i]->cost.z);	
		glVertex3f((X*WORLD_TILE+WORLD_TILE	/2.0)+connection[pathTo[world->gCamera.RecticleCell]]->cost.x,0,(Z*WORLD_TILE+WORLD_TILE/2.0)+connection[pathTo[world->gCamera.RecticleCell]]->cost.z);	

		glEnd();
	
	}
		*/


	/*
	// draw the averaged and normalized direction of all the people walking on this cell
	if (PersonList.length()>5)
	{
		glLineWidth(5);
		glColor4f(1.0,1.0,0.0,1.0);
		DirectionList.normalize();
		DirectionList = DirectionList*PersonList.length()/10.0f;

		NxVec3 c = NxVec3(DirectionList.x,0,DirectionList.z);
		c = c.cross(NxVec3(0,1,0));
		//c.normalize();
	
		glBegin(GL_LINES);
				glVertex3f( X*WORLD_TILE+WORLD_TILE	/2.0,				  3,Z*WORLD_TILE	+WORLD_TILE	/2.0);
				glVertex3f((X*WORLD_TILE+WORLD_TILE	/2.0)+DirectionList.x,3,(Z*WORLD_TILE+WORLD_TILE/2.0)+DirectionList.z);	
				glVertex3f((X*WORLD_TILE+WORLD_TILE	/2.0)+DirectionList.x,3,(Z*WORLD_TILE+WORLD_TILE/2.0)+DirectionList.z);	
				glVertex3f((X*WORLD_TILE+WORLD_TILE	/2.0)-c.x,3,(Z*WORLD_TILE+WORLD_TILE/2.0)-c.z);	
				glVertex3f((X*WORLD_TILE+WORLD_TILE	/2.0)+DirectionList.x,3,(Z*WORLD_TILE+WORLD_TILE/2.0)+DirectionList.z);	
				glVertex3f((X*WORLD_TILE+WORLD_TILE	/2.0)+c.x,3,(Z*WORLD_TILE+WORLD_TILE/2.0)+c.z);	
		glEnd();
		glLineWidth(1);
	}
	*/	
	//glDisable(GL_BLEND);
}

int SZ_Node::whichNeighbour(int numConnection)
{
	if (numConnection > amountConnections)
	{
		return -1;
	}
	SZ_Node* NodeToCheck;
	NodeToCheck = (SZ_Node*)(connection[numConnection]->toNodeAddress);

	if (NodeToCheck != NULL )
	{
		return NodeToCheck->number;
	} else { return -1; }

}

int SZ_Node::whichConnection(int nodeNumber)
{
	int nodenum = 0;
    for (int i=0;i<amountConnections;i++)
	{
		SZ_Node* currentNode;
		currentNode = (SZ_Node*)(connection[i]->toNodeAddress);
		if (currentNode != NULL)
		{
			if (currentNode->number == nodeNumber) { nodenum = i; }
		}
	}
	
	return nodenum;
}

void SZ_Node::setWorld( SZ_World *world )
{
	this->world = world;
}
