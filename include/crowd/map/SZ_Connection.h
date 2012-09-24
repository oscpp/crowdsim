#ifndef SZCONNECTION_H
#define SZCONNECTION_H

#include "..\SZ_Config.h"
#include "..\..\..\general_utils.h"


class SZ_Connection
{
	public:
		SZ_Connection();
		NxVec3 cost;
		float getCost();

		void* toNodeAddress;	// pointer towards a given node.
  
};


#endif