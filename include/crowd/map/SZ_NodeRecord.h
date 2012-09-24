#ifndef SZNODERECORD_H
#define SZNODERECORD_H

//#include "general_utils.h"
#include "SZ_Config.h"
#include "SZ_Node.h"

class SZ_NodeRecord
{
	public:
		 SZ_NodeRecord();
		~SZ_NodeRecord();
		SZ_Node* gNode;
		int connection;	  // stores which connection this record is coming from
		int node_number;  // stored the numerical value of the current node.
		float costSoFar;
};

#endif