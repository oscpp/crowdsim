#ifndef SZNODE_H
#define SZNODE_H

#include "..\SZ_Config.h"
#include "SZ_Connection.h"
#include "SZ_PersonList.h"

#include <stdio.h>
#include "Stream.h"


#include "..\..\..\general_utils.h"


class SZ_Node
{
	private:
		
	public:
		SZ_Node();
		~SZ_Node();

		void setWorld(SZ_World *world);

		static const int amountConnections = 8;
		void draw();		// draws the node, with all connections.
		void draw(NxVec3 colour);

		void* functions;		// pointer towards a SZ_FunctionBase (for instance, to say that this place is a checkpoint)

		static int nodeIndexCnt;				// the amount of existing nodes
		//int index;								// index of the node in the existing nodes
		int number;								// current number of all possible nodes (in our case between 0-1199)
		int pathTo[NODES_AMOUNT];				// stores the next tile number a person needs to travel to if it wants to reach tile [array_index] NAVIGATION LAYER
		//int getNodeXZ(int x, int z);
		//int entity;
		SZ_Connection *connection[amountConnections];
		
		int whichNeighbour(int connection);		// inputs a connection value, returns the node number that is connected to that connection.
		int whichConnection(int nodeNumber);	// inputs a node number, and checks if that node is one of my neighbour. if yes, return which connection no.

		SZ_PersonList  PersonList;		//keeps track of the people inside this node OCCUPANCY LAYER
		NxVec3		   DirectionList;	//average direction of all the people in the cell DIRECTION LAYER
		SZ_World *world;	// world i belong to
};

#endif
