#include "SZ_Schedule.h"
#include "SZ_World.h"
#include "SZ_Node.h"



SZ_Schedule::SZ_Schedule()
{
	//create the sprites required for the board
	for (int i = 0; i<32; i++)
	{
		for (int k = 0; k<8; k++)
		{
			boardText[(i*8)+k] = new SZ_Sprite((1/32.0f)*100,(1/16.0f)*100,16*1,0,16,32,0);	// sizex, sizez, texture: start u, start v, width, length, frames total
			boardText[(i*8)+k]->setOffset(i*3.1,k*5.1);	
		}
	}	
	//sets all the planes to NULL
	for (int i = 0;i<max_number_plane;i++)
	{
		planeRecord[i] = NULL;
	}
	for (int i = 0;i<ppTotalppNodes;i++)
	{
		ppGates[i] = NULL;	//initilise the gates to NULL
	}
	amountPlanes = 0;
	citiesNum = 0;
	amountPlanes = 0;
	position = NxVec3(0,0,0);	//board always created at origin.
}

SZ_Schedule::~SZ_Schedule()
{
	for (int i = 0;i<max_number_plane;i++)
	{
		if (planeRecord[i]!=NULL)
		{
			delete planeRecord[i];
			planeRecord[i]=NULL;
		}
	}

	for (int i = 0; i<32*8; i++)
	{
		if (boardText[i]!=NULL)
		{
			delete boardText[i]; 
			boardText[i] = NULL;
		}
	}
};

void SZ_Schedule::onCreate(void *world, unsigned int texture_array[])
{
	this->world = (SZ_World*)world;
	std::ifstream  data("textures/plane_cities.txt");
	std::string line;

	int counter=0;
	while(std::getline(data,line))
	{	
		std::stringstream  lineStream(line);
		std::string        cell;
		while(std::getline(lineStream,cell,','))
		{
			cities[counter] = cell;
		}
		//printf("%i: %s \n",counter, cities[counter].c_str());
		counter++;		
	}
	data.close();
	citiesNum = counter;

	for (int i = 0; i<32*8; i++)
	{
		boardText[i]->SetTexture(3);
		boardText[i]->init(texture_array);		
	}
}

void SZ_Schedule::update()
{

	//printf("updated \n");
	char* tmpCols = new char[257];
	for (int i=0; i<257;i++)
	{
		tmpCols[i] = '\0';
	}
	
	int planesInBoard = 0;	//how many planes do I have in my board
	for (int i=0; i<amountPlanes; i++)
	{
		int tDiff = difftime(mktime(planeRecord[i]->getSchedule()),mktime(world->gTimeKeeper.theTime()));

		//prepare the array of characters for drawing		
		if (planesInBoard<8)
		{
			if (abs(tDiff)<3600)	// a plane is sometime +/- one hour from current time.
			{
				char* tmpplane = new char[33];		
				
				int num_letters = 0;
				if (MySchedule == SZ_Schedule::apDepartTable)
				{
					num_letters = sprintf(tmpplane,"%0.2i:%0.2i -> %13.13s:%.13s", planeRecord[i]->getSchedule()->tm_hour, planeRecord[i]->getSchedule()->tm_min ,planeRecord[i]->city, planeRecord[i]->statusName());
				}	

				if (MySchedule == SZ_Schedule::apLandingTable)
				{
					num_letters = sprintf(tmpplane,"%8.13s:%13.13s <- %0.2i:%0.2i", 
						planeRecord[i]->statusName(),
						planeRecord[i]->city, 
						planeRecord[i]->getSchedule()->tm_hour, 
						planeRecord[i]->getSchedule()->tm_min
						);
				}					

				// padding spaces.
				if (num_letters<32)
				{	
					for (int k = 0; k<32-num_letters;k++)
					{
						char tmpc[] = " ";
						strncat(tmpplane,tmpc,1);
					}
				}
				strncat(tmpCols,tmpplane, 64);
				delete tmpplane;
				planesInBoard++;
			}
		}
		
	
		//Schedule mangement if planes are departing
		if (MySchedule == SZ_Schedule::apDepartTable)
		{
			// A plane get the status delayed if a delay of geDelay() is above 0
			if (planeRecord[i]->getDelay()>0)
			{
				planeRecord[i]->selectStatus(SZ_Airplane::apDelayed);
			}

			if (planeRecord[i]->getPlaneStatus()!=SZ_Airplane::apCancel)
			{
				// time 1800secs (30min) -> change from BOARDING to DEPARTED
				tDiff = tDiff + planeRecord[i]->getDelay();
				
				planeRecord[i]->actionTimeRunning(planeRecord[i],tDiff);  


				if ( (tDiff<=(PLANE_TIME_WAIT_TO_BOARD)) && (planeRecord[i]->getPlaneStatus())== SZ_Airplane::apDelayed)
				{
					planeRecord[i]->selectStatus(SZ_Airplane::apBoarding);
				}
				
				// time 1800secs (30min) -> change from BOARDING to DEPARTED
				if ( (tDiff<=(PLANE_TIME_WAIT_TO_BOARD)) && (planeRecord[i]->getPlaneStatus())== SZ_Airplane::apWaiting)
				{
					planeRecord[i]->selectStatus(SZ_Airplane::apBoarding);
				}
				
				// time 0 -> change from BOARDING to DEPARTED
				if ( (tDiff<=PLANE_TIME_BOARD_TO_DEPART) && (planeRecord[i]->getPlaneStatus())== SZ_Airplane::apBoarding)
				{
					planeRecord[i]->selectStatus(SZ_Airplane::apDeparting);
				}
			}
		}

		//Schedule mangement if planes are departing
		if (MySchedule == SZ_Schedule::apLandingTable)
		{
			//printf("landing \n");
			// A plane get the status delayed if a delay of geDelay() is above 0
			if (planeRecord[i]->getDelay()>0)
			{
				planeRecord[i]->selectStatus(SZ_Airplane::apDelayed);
			}

			if (planeRecord[i]->getPlaneStatus()!=SZ_Airplane::apCancel)
			{
				// time 1800secs (30min) -> change from EXPECTED to LANDING
				tDiff = tDiff + planeRecord[i]->getDelay();
				if ( (tDiff<=PLANE_TIME_WAITING_TO_LANDING) && (planeRecord[i]->getPlaneStatus())== SZ_Airplane::apExpected)
				{
					planeRecord[i]->selectStatus(SZ_Airplane::apLanding);
				}
				
				// time 0secs (0min) -> change from LANDING to LANDED
				if ( (tDiff<=(0)) && (planeRecord[i]->getPlaneStatus())== SZ_Airplane::apLanding)
				{
					planeRecord[i]->selectStatus(SZ_Airplane::apLanded);
				}
			}
		}
	}

	for (int i = 0; i<32; i++)
	{
		for (int k = 0; k<8; k++)
		{
			//boardText[(i*32)+k]->draw();
			setTextOnSprite(i,k,tmpCols[(k*32)+i]);
			//printf("%c", tmpCols[(k*32)+i]);
		}
	}
	delete[] tmpCols;
}

bool SZ_Schedule::contains( SZ_Airplane* whichPlane )
{
	bool found = false;
	if (amountPlanes>0)
	{
		int counter = 0;
		while ( (counter<amountPlanes)  )
		{

			if (planeRecord[counter]==whichPlane)
			{
				found = true;
				counter = amountPlanes;
			}
			counter++;
		}
	}
	return found;
}

void SZ_Schedule::add( SZ_Airplane* whichPlane )
{
	if (amountPlanes <max_number_plane)
	{
		{
			planeRecord[amountPlanes] = whichPlane;
			amountPlanes++;
		}
	}
}

void SZ_Schedule::substract( SZ_Airplane* whichPlane )
{
	bool found = false;
	int counter = 0;
	while ( (counter<amountPlanes)  )
	{

		if (planeRecord[counter]==whichPlane)
		{
			amountPlanes--;
			planeRecord[counter]= planeRecord[amountPlanes];		
			planeRecord[amountPlanes] = NULL;
			counter=amountPlanes;
		}
		counter++;
	}
}

void SZ_Schedule::empty()
{
	if (amountPlanes>0)
	{
		for (int i = 0;i<length();i++)
		{
			if (planeRecord[i]!=NULL)
			{
				planeRecord[i]=NULL;
			}
		}
	}
	amountPlanes=0;
}

int SZ_Schedule::length()
{
	return amountPlanes;
}

void SZ_Schedule::newPlane( int tmpHour,int tmpMin, SZ_Airplane::apStatus status, const char* city )
{
	SZ_Airplane* tmpPlane = new SZ_Airplane();
	tmpPlane->setSchedule(tmpHour,tmpMin);
	tmpPlane->selectStatus(status);
	tmpPlane->setMySchedule(this);
	
	if (ppGatesLength!=0)
	{
		tmpPlane->setGate(ppGates[rand()%ppGatesLength]);
	}


	if (city==NULL) 
		{ 	
			tmpPlane->city = cities[rand()%citiesNum].c_str(); 
	} else { tmpPlane->city = city; }	
	add(tmpPlane);	
	//printf("citiesnum %i, amountplanes %i \n", citiesNum, amountPlanes);
	//printf("Plane departing at %0.2i:%0.2i to %-15.15s - status: %s", tmpPlane->getSchedule()->tm_hour, tmpPlane->getSchedule()->tm_min ,tmpPlane->city, tmpPlane->statusName());
	//printf("\n");	
}

SZ_Schedule::apScheduleType SZ_Schedule::getSchedule()
{
	return MySchedule;
}

void SZ_Schedule::setSchedule(SZ_Schedule::apScheduleType Schedule)
{
	MySchedule = Schedule;
}

void SZ_Schedule::draw()
{
	glPushMatrix();
	glTranslatef(position.x,position.y,position.z);
	//glScalef(0.085f,0.085f,0.085f);	//original
	glScalef(0.085f,0.085f,0.085f);
	glRotatef(90,1,0,0);
	for (int i = 0; i<8; i++)
	{
		for (int k = 0; k<32; k++)
		{
			boardText[(i*32)+k]->draw();
		}
	}
	glPopMatrix();	
}

void SZ_Schedule::setTextOnSprite(int row, int col, char whichChar )
{
	int asciivalue = int(whichChar)-32;
	int whichSprite = (row*8)+col;
	boardText[whichSprite]->setUVSprite(16*asciivalue,0);
}

NxVec3 SZ_Schedule::getPosition()
{
	return position;
}

void SZ_Schedule::setPosition( NxVec3 newPos )
{
	position = newPos;
}

// Define here what happens when the planes are changing their status.
void SZ_Schedule::actionLanding(SZ_Airplane* whichPlane)
{
	//printf("Plane landing at %0.2i:%0.2i to %-15.15s - status: %s \n", whichPlane->getSchedule()->tm_hour, whichPlane->getSchedule()->tm_min , whichPlane->city, whichPlane->statusName());
	//this->update();
}

void SZ_Schedule::actionDeparting(SZ_Airplane* whichPlane)
{
	//this->update();
}

void SZ_Schedule::actionBoarding(SZ_Airplane* whichPlane)
{
	//this->update();
}

void SZ_Schedule::actionDelayed(SZ_Airplane* whichPlane)
{
	//this->update();
}


// amount of people to spawn who are about to depart
void SZ_Schedule::actionTimeRunning( SZ_Airplane* whichPlane, int timeleft )
{
	if (world->service.amount == 0) { return;}	// if services don't exist, do not create a person.

	if (MySchedule==apDepartTable)
	{
		if (timeleft==60*60)	// an hour before the departure of a plane
		{
			
			//amount of people to spawn who want to depart
			int PERSON_TO_SPAWN_ON_LANDED = GENERATION_MAX_PERSON_PER_TILE*GENERATION_MODIFIER;
						
			int i = 0;
			while (i<PERSON_TO_SPAWN_ON_LANDED)
			{
				SZ_Person* lastPPSpawned = NULL;
				//int amountToSpawn = PoissonRandomNumber(5);
				//int amountToSpawn = 4;
				//int amountToSpawn = PoissonRandomNumber(0.83f);
				int amountToSpawn = PoissonRandomNumber(6);
				

				if (amountToSpawn >= MAX_SOCIAL_LINKS) { amountToSpawn = MAX_SOCIAL_LINKS; }
				world->ggGroupsTotalCount[amountToSpawn]++;
				
				int gateToSpawn = ppExits[rand()%ppExitLength]->number;
				float R,G,B;
				R = (rand()%100) / 100.0f;
				G = (rand()%100) / 100.0f;
				B = (rand()%100) / 100.0f;
				
				for (int k=0; k<amountToSpawn; k++)	// create people connected together (groups)
				{
						
					i++;
					if (world->ppAmount >= NUM_PERSON) { /* printf("max people reached! \n"); */ continue; }
					// spawn x persons, keep their pointers so their schedule can be edited.				
					SZ_Person* tmpPerson = NULL;
					tmpPerson = world->ppSpawn(gateToSpawn);	// make new person
					
					
					if (tmpPerson!=NULL)
					{				
						tmpPerson->actions.aim_targetCellList.empty();					
						tmpPerson->actions.aim_targetCellList.add(ppDeparts[rand()%ppDepartLength]->number);
						tmpPerson->actions.aim_targetCellList.add(ppSecurity[rand()%ppSecurityLength]->number);
						tmpPerson->actions.aim_GenerateListAims(3);										
						tmpPerson->actions.aim_targetCell = tmpPerson->actions.aim_targetCellList.intRecord[tmpPerson->actions.aim_targetCellList.length()-1];
						tmpPerson->ggIntraList->colour = NxVec3(R,G,B);

						tmpPerson->ggMemberCount = 0;

					}

					if (lastPPSpawned != NULL)
					{	
						tmpPerson->ggIntraList->NewSocialConnectionIntraALL(lastPPSpawned);	// connect this person to the previous spawned person
						tmpPerson->actions = lastPPSpawned->actions;


						lastPPSpawned->ggMemberCount++;
						tmpPerson->ggMemberCount = lastPPSpawned->ggMemberCount+1;
					}
					lastPPSpawned =tmpPerson;
		
					
				}
			}
			
		}
		
	}
}

// amount of people to spawn on landed
void SZ_Schedule::actionLanded(SZ_Airplane* whichPlane)
{
	int PERSON_TO_SPAWN_ON_LANDED = GENERATION_MAX_PERSON_PER_TILE-20;
	for (int i=0; i<PERSON_TO_SPAWN_ON_LANDED; i++)
	{
		// spawn x persons, keep their pointers so their schedule can be edited.
		SZ_Person* tmpPerson = NULL;
		tmpPerson = world->ppSpawn(whichPlane->getGate()->number);	// make new person

		if (tmpPerson!=NULL)
		{
			tmpPerson->actions.aim_targetCellList.empty();
			tmpPerson->actions.aim_targetCellList.add(ppExits[rand()%ppExitLength]->number); // make sure his last aim is an exit.
			tmpPerson->actions.aim_GenerateListAims(1);
		}

	}

}

void SZ_Schedule::ppAddGate( SZ_Node* whichNode )
{
	if (ppGatesLength <ppTotalppNodes)
	{
		ppGates[ppGatesLength] = whichNode;
		ppGatesLength++;
	}

}

void SZ_Schedule::ppAddExit( SZ_Node* whichNode )
{
	if (ppExitLength <ppTotalppNodes)
	{
		ppExits[ppExitLength] = whichNode;
		ppExitLength++;
	}
}

void SZ_Schedule::ppAddSecurity( SZ_Node* whichNode )
{
	if (ppSecurityLength <ppTotalppNodes)
	{
		ppSecurity[ppSecurityLength] = whichNode;
		ppSecurityLength++;
	}
}

void SZ_Schedule::ppAddDeparts( SZ_Node* whichNode )
{
	if (ppDepartLength <ppTotalppNodes)
	{
		ppDeparts[ppDepartLength] = whichNode;
		ppDepartLength++;
	}
}
