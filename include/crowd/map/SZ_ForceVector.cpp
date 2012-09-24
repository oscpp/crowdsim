#include "SZ_ForceVector.h"
#include "SZ_World.h"
#include "SZ_ConfigTime.h"

SZ_ForceVector::SZ_ForceVector()
{

}

SZ_ForceVector::~SZ_ForceVector()
{

}

void SZ_ForceVector::draw()
{	
	glColor3f(colour3f.x,colour3f.y,colour3f.z);
	glVertex3f(position.x, position.y, position.z);
	glVertex3f(position.x+vector3f.x, position.y+vector3f.y, position.z+vector3f.z);	
	
}

