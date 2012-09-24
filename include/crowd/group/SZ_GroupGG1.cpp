#include "group/SZ_GroupGG1.h"
#include "SZ_World.h"


int SZ_GroupGG1::ggGroupIndexCnt = 0;

SZ_GroupGG1::SZ_GroupGG1()
{
	ggGroupIndexCnt ++;
}

SZ_GroupGG1::~SZ_GroupGG1()
{
}

void SZ_GroupGG1::init()
{
	// there needs to be people within this group
	if (this->ggGroupMembers.amount == 0)
	{
		printf("SZ_GroupGG1: Error Nobody in this group. \n");
		return;
	}
}

void SZ_GroupGG1::draw()
{

}

