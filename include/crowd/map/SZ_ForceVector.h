#ifndef SZFORCEVETOR_H
#define SZFORCEVETOR_H

#include "..\..\..\general_utils.h"
#include "..\SZ_Config.h"

class SZ_ForceVector
{
public:
	SZ_ForceVector();
	~SZ_ForceVector();

	void draw();
	NxVec3 colour3f;
	NxVec3 vector3f;
	NxVec3 position;
};

#endif