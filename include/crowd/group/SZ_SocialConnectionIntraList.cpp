#include "SZ_SocialConnectionIntraList.h"
#include "SZ_Person.h"

SZ_SocialConnectionIntraList::SZ_SocialConnectionIntraList()
{
	amount = 0;
	parent = NULL; 
}

SZ_SocialConnectionIntraList::~SZ_SocialConnectionIntraList()
{
	deleteSocialConnectionIntra();
}

void SZ_SocialConnectionIntraList::NewSocialConnectionIntra( SZ_Person* toWhichPerson )
{
	if ((toWhichPerson == NULL) || ( parent == NULL))
	{
		printf("Error: SocialConnectionIntraList, parent or toWhichPerson is NULL \n");
	}
	if ( (amount < MAX_SOCIAL_LINKS ) && ( toWhichPerson->ggIntraList->amount < MAX_SOCIAL_LINKS) )
	{
		// first pass: create a connection from the current person to toWhichPerson
		SZ_SocialConnectionIntra* SocialConnectionIntra;
		SocialConnectionIntra = new SZ_SocialConnectionIntra();
		SocialConnectionIntra->toPerson = toWhichPerson;
		SocialConnectionIntra->parent = this;
		SocialIntraRecord[amount] = SocialConnectionIntra;	
		//printf("connection from %i to %i \n", this->parent->personIndex, toWhichPerson->personIndex);
		//printf("parent sz_person of this connection %i \n",  SocialConnectionIntra->parent->parent->personIndex);
		amount++;

		// second pass: create a connection from toWhichPerson back to this.
		SZ_SocialConnectionIntra* SocialConnectionIntraToWhichPerson;
		SocialConnectionIntraToWhichPerson = new SZ_SocialConnectionIntra();
		SocialConnectionIntraToWhichPerson->toPerson = this->parent;
		SocialConnectionIntraToWhichPerson->parent = toWhichPerson->ggIntraList;
		toWhichPerson->ggIntraList->SocialIntraRecord[toWhichPerson->ggIntraList->amount] = SocialConnectionIntraToWhichPerson;
		toWhichPerson->ggIntraList->amount++;
		
	}
	else
	{
		printf("Error: SocialConnectionIntraList, MAX_SOCIAL_LINKS \n");
	}
}

void SZ_SocialConnectionIntraList::init( SZ_Person* parent )
{
	this->parent = parent;
}

void SZ_SocialConnectionIntraList::draw()
{

	
	for (int i = 0; i < amount; i++ )
	{
		//printf(" amount of connections %i \n", amount);
		//printf(" this->parent->personIndex; %i pointing to %i \n", parent->personIndex, SocialIntraRecord[i]->toPerson->personIndex );
		//printf(" this->parent->parent->personIndex; %i \n", SocialIntraRecord[i]->parent->parent->personIndex );
		
		this->SocialIntraRecord[i]->draw();


	}
	
}

void SZ_SocialConnectionIntraList::deleteSocialConnectionIntra()
{
	//printf("The person %i knows %i persons \n ", this->parent->personIndex, this->amount);
	for (int i = 0; i < amount; i++)
	{
		//printf(" this->parent->personIndex; %i pointing to %i \n", parent->personIndex, SocialIntraRecord[i]->toPerson->personIndex );
		SocialIntraRecord[i]->toPerson->ggIntraList->substract(parent);
		//SocialIntraRecord[i]->toPerson->parent->substract(parent);
	}
	//printf("\n");
}

void SZ_SocialConnectionIntraList::substract(SZ_Person* whichPerson)
{	
	//printf("currently working on person %i \n", parent->personIndex);
	bool found = false;
	int counter = 0;
	while ( (counter<amount)  )
	{
		//printf("counter %i amount %i \n",counter, amount);
		if (SocialIntraRecord[counter]->toPerson==whichPerson)
		{
	
			//delete NodesRecord[counter];
			amount--;
			
			delete SocialIntraRecord[counter];
			SocialIntraRecord[counter]= SocialIntraRecord[amount];	
			
			SocialIntraRecord[amount] = NULL;
			
			counter=amount;

		}
		counter++;

	}
}

void SZ_SocialConnectionIntraList::NewSocialConnectionIntraALL( SZ_Person* toWhichPerson )
{
	
	/*
	((SZ_Person*)(tmp_person))->ggIntraList->NewSocialConnectionIntra(tmp_oldperson);


	// attach to group. new person becomes friend with friends of old person
	for (int i=0; i< tmp_oldperson->ggIntraList->amount;i++)  // go through all the social relationship of old person
	{
		SZ_Person* tmp_oldpersonFriend;
		tmp_oldpersonFriend = tmp_oldperson->ggIntraList->SocialIntraRecord[i]->toPerson; // find all the SZ_Persons connected to oldperson
		printf("old person is friend with %i \n", tmp_oldpersonFriend->personIndex);
		if (tmp_oldpersonFriend == tmp_person) { printf("found myself %i \n", tmp_oldpersonFriend->personIndex ); continue; }		// if SZ_Person returns to new created person, ignore
		((SZ_Person*)(tmp_person))->ggIntraList->NewSocialConnectionIntra(tmp_oldpersonFriend);	// connect friend of friend with new person

	}
	*/

	// new person becomes friend with toWhichPerson
	NewSocialConnectionIntra(toWhichPerson);


	// attach to group. new person becomes friend with friends of old person
	for (int i=0; i< toWhichPerson->ggIntraList->amount;i++)  // go through all the social relationship of old person
	{
		SZ_Person* tmp_oldpersonFriend;
		tmp_oldpersonFriend = toWhichPerson->ggIntraList->SocialIntraRecord[i]->toPerson; // find all the SZ_Persons connected to toWhichPerson
		//printf("old person is friend with %i \n", tmp_oldpersonFriend->personIndex);
		if (tmp_oldpersonFriend == parent) { 
			//printf("found myself %i \n", tmp_oldpersonFriend->personIndex ); 
			continue; 
		}		// if SZ_Person returns to new created person, ignore
		NewSocialConnectionIntra(tmp_oldpersonFriend);	// connect friend of friend with new person

	}
}
