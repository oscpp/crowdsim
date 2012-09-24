#ifndef SZPERSON_H
#define SZPERSON_H

#include "SZ_ActorSprite.h"
#include "./map/SZ_PrAction.h"
#include "./group/SZ_SocialConnectionIntraList.h"


class SZ_Person: public SZ_ActorSprite
{

	public:
		 SZ_Person();
		~SZ_Person();
		void update();

		static int personIndexCnt;				// the total amount of exiting persons
		int personIndex;						// the number of this person.

		NxVec3 force;							// Force being applied onto the given individual

		void applyForceAction(NxVec3 force);

		SZ_PrAction actions;


		
		void onCreate();	// called after the initialization of the sprite and making of PhysX
		void draw();
		void changeTile();	// called when i switch from one cell to another
		void releasePerson(SZ_Person* &self); // release this person's entity (physX) and model. still need to delete it though.
		void putOnCell();	// put this person on a random cell, not overlapping others.
		void putOnCell(int whichCell);		

		//  GG: used for group purposes
		int ggMemberCount; 
		SZ_SocialConnectionIntraList* ggIntraList;

		// all behavioural related stuff of individuals
		void	behaviour_actions();	// behaviour of agent related to actions to take
		void	behaviour_navigation();// agent navigational behaviour
		NxVec3	behaviour_seek(NxVec3 target); 
		NxVec3  behaviour_flee(NxVec3 target);
};

#endif

