#ifndef SZMSCGROUP_H
#define SZMSCGROUP_H

#include "Bmp.h"
#include "..\..\general_utils.h"

#include "SZ_WorldMap.h"
#include "SZ_Config.h"
#include "SZ_ActorSprite.h"
#include "map\SZ_PersonList.h"


class SZ_GroupGG1
{
	public:
		 SZ_GroupGG1();
		~SZ_GroupGG1();

		static int ggGroupIndexCnt;					// the total amount of exiting groups.
		NxVec3	colour;

		NxVec3	CenterMass(SZ_Person* whichPerson);	// returns the perceived center of mass of a given person.
		void	draw();
		
		SZ_PersonList ggGroupMembers;

		// initialize the group actions. first person in the group is the leader
		void init();

};


#endif