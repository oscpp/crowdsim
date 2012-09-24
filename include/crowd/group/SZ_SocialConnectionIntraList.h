#ifndef SZSOCIALCONNECTIONINTRALIST
#define SZSOCIALCONNECTIONINTRALIST

#include "SZ_SocialConnectionIntra.h"

#include "..\..\..\general_utils.h"
#include "..\SZ_Config.h"

class SZ_Person;

class SZ_SocialConnectionIntraList
{
	private:
	public:
		 SZ_SocialConnectionIntraList();
		~SZ_SocialConnectionIntraList();
		
		NxVec3 colour;
		SZ_SocialConnectionIntra* SocialIntraRecord[MAX_SOCIAL_LINKS]; 
		SZ_Person* parent;
		int amount;	// amount of social connections in this list
		
		void NewSocialConnectionIntra(SZ_Person* toWhichPerson);	// 1:1 creates a social connection towards toWhichPerson. toWhichPerson creates a connection back to this
		void NewSocialConnectionIntraALL(SZ_Person* toWhichPerson); // 1:* similar as above, but additionally creates a connection towards ALL connections of toWhichPerson.
		void deleteSocialConnectionIntra();
		void substract(SZ_Person* whichPerson);
		void init(SZ_Person* parent);	// connects this to parent person
		void draw();					// draws all the social connections in this list
	
};

#endif