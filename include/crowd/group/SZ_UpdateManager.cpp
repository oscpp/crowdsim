#include "SZ_World.h"
#include "SZ_UpdateManager.h"

SZ_UpdateManager::SZ_UpdateManager()
{
	ppIteration = 0;
	ppPointer = NULL;
}

SZ_UpdateManager::~SZ_UpdateManager()
{

}

void SZ_UpdateManager::update()
{
	
}

void SZ_UpdateManager::init( SZ_World* parent )
{
	tmp_world = parent;
}
