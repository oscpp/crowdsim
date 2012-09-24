#ifndef SZSOCIALCONNECTIONINTRA
#define SZSOCIALCONNECTIONINTRA

#include "SZ_Config.h"
#include "scene.h"

class SZ_SocialConnectionIntraList;
class SZ_Person;

class SZ_SocialConnectionIntra
{
	private:
	public:
		 SZ_SocialConnectionIntra();
		~SZ_SocialConnectionIntra();
		
		SZ_SocialConnectionIntraList* parent;
		NxVec3 getToPersonVector();	// returns a position vector to toPerson* from *parent;
		SZ_Person* toPerson;		// who am I pointing to (connection to)
		NxVec3 toPDA;				// PDA values of this connection

		void draw();

};

#endif