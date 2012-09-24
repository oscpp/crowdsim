#ifndef SZPATHFINDINGLIST_H
#define SZPATHFINDINGLIST_H

#include "..\..\..\general_utils.h"
#include "..\SZ_Config.h"
#include "SZ_NodeRecord.h"

class SZ_PathfindingList
{
	public:
		 SZ_PathfindingList();
		~SZ_PathfindingList();

		int amount;	// amount of NodeRecord in this Pathfindinglist

		SZ_NodeRecord* NodesRecord[NODES_AMOUNT];		// stores all the node records

		SZ_NodeRecord* smallestElement();				// returns record with the smallest cost so far
		bool contains(SZ_Node* whichNode);				// true if this list contains specified node
		SZ_NodeRecord* find(SZ_Node* whichNode);		// 
		void add(SZ_NodeRecord* whichNodeRecord);
		void substract(SZ_NodeRecord* whichNodeRecord);
		void empty();
		int length();  // returns the amount of noderecords in this instance 
		int length2(); // returns the amount of noderecords in this instance by manually counting them 


};

#endif