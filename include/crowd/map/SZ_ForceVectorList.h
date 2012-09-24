#ifndef SZFORCEVETORLIST_H
#define SZFORCEVETORLIST_H

#include "..\..\..\general_utils.h"
#include "..\SZ_Config.h"
#include "SZ_ForceVector.h"

class SZ_ForceVectorList
{
public:
	SZ_ForceVectorList();
	~SZ_ForceVectorList();
	

	int amount; // amount of forcevectors in this object
	SZ_ForceVector* ForceRecord[MAX_FORCE_AMOUNT*2];		// stores all the node records

	void draw();
	void newVector(NxVec3 start, NxVec3 vector3f, NxVec3 colour3f);

	void add(SZ_ForceVector* whichForce);
	void substract(SZ_ForceVector* whichForce);
	void empty();
	int length();  // returns the amount of noderecords in this instance 
	
};

#endif