#include "SZ_Person.h"
#include "SZ_World.h"
#include "SZ_ConfigTime.h"
#include "./group/SZ_GroupGG1.h"


int SZ_Person::personIndexCnt = 0;

SZ_Person::SZ_Person()
{
	ggIntraList		= new SZ_SocialConnectionIntraList;
	ggMemberCount	= 0;
}

SZ_Person::~SZ_Person()
{
	SZ_ActorSprite::~SZ_ActorSprite();
	if (entity!=NULL)
	{
		//entity->getScene().releaseActor(*entity);
		entity = NULL;
	}

	if (ggIntraList!=NULL) { delete ggIntraList; ggIntraList = NULL; }
}

void SZ_Person::releasePerson(SZ_Person* &self)
{
	if (this == NULL) { return; }
	if (model!=NULL)
	{
		delete model;
		model = NULL;
	}
	if (entity!=NULL)
	{
		//if (wScene !=NULL)
		wScene->releaseActor(*entity);
	}
	//self = NULL;
}

void SZ_Person::update()
{	

	SZ_ActorSprite::update();
	actions.update();			// update all action related stuff.

	/*
	if (currentCell<0)
	{
		//tmp_world->ppSubstract(this);
		//changeTile();
		//update();
		//tmp_world->Utils.vVectorList.newVector(this->getPosition(),NxVec3(0,30,0),NxVec3(0,0,1));

		printf("end update: node :%i iterations %i, %4.2f %4.2f \n", currentCell, iterations, getPosition().x, getPosition().z);
		printf(" action : %s \n", actions.modeName());
	}
	*/
}


void SZ_Person::putOnCell(int whichCell)
{
	changeTile();
	int counter = 0;
	int newCell = whichCell;
	bool foundCell = false;

	// make sure the tile has got less than GENERATION_MAX_PERSON_PER_TILE ppl ( default: 25 )
	// if yes, spawn the chaps on a neighbouring tile
	do 
	{
		if (( ((SZ_World*)(world))->Utils.gNodes[whichCell]->PersonList.amount) >= GENERATION_MAX_PERSON_PER_TILE	)
		{

			if ( ((SZ_World*)(world))->Utils.gNodes[newCell]->whichNeighbour(counter) != -1)
			{
				whichCell = ((SZ_World*)(world))->Utils.gNodes[newCell]->whichNeighbour(counter);
			}
		} 
		counter++;
	} while( (counter<8) );
	

	
	entity->raiseActorFlag(NX_AF_DISABLE_COLLISION);				
		int X = (whichCell % WORLD_X_5X5);	  // transform the tile value into the top left X corner
		int Z = (whichCell-X)/WORLD_X_5X5;    // transform the tile value into the top left Y corner

		entity->setGlobalPosition(NxVec3((X*WORLD_TILE)		,0.10,(Z*WORLD_TILE)	 	));

		// post init requirements.
		update();
		changeTile();
		visibleTiles.empty();

		int ex = ((((SZ_World*)(world))->Utils.gNodes[currentCell]->PersonList.amount-1) % (WORLD_TILE));	  // transform the tile value into the top left X corner
		int ey = ((((SZ_World*)(world))->Utils.gNodes[currentCell]->PersonList.amount-1) -ex)/WORLD_TILE;   // transform the tile value into the top left Y corner
		
		entity->setGlobalPosition(getPosition()+NxVec3(ex+0.5,0,ey+0.5));
		update();
		changeTile();
		visibleTiles.empty();
	entity->clearActorFlag(NX_AF_DISABLE_COLLISION);
}



void SZ_Person::putOnCell()
{

		// randomly put the person on a random cell make sure its not overallping with other already exisiting people		
		int tmp_randnode;
		tmp_randnode = rand() % ( ((SZ_World*)(world))->Utils.gNodesListOrdered.length() );
				
		int X = (((SZ_World*)(world))->Utils.gNodesListOrdered.NodesRecord[tmp_randnode]->gNode->number % WORLD_X_5X5);	  // transform the tile value into the top left X corner
		int Z = (((SZ_World*)(world))->Utils.gNodesListOrdered.NodesRecord[tmp_randnode]->gNode->number-X)/WORLD_X_5X5;   // transform the tile value into the top left Y corner

		entity->setGlobalPosition(NxVec3((X*WORLD_TILE)		,0.10,(Z*WORLD_TILE)	 	));

		// post init requirements.
		update();
		changeTile();
		visibleTiles.empty();

		int ex = ((((SZ_World*)(world))->Utils.gNodes[currentCell]->PersonList.amount*2) % (WORLD_TILE));	  // transform the tile value into the top left X corner
		int ey = ((((SZ_World*)(world))->Utils.gNodes[currentCell]->PersonList.amount*2) -ex)/WORLD_TILE;   // transform the tile value into the top left Y corner
		
		entity->setGlobalPosition(getPosition()+NxVec3(ex,0,ey));
		update();
		changeTile();
		visibleTiles.empty();
}

void SZ_Person::changeTile()
{
	
	// update the nodes, to know which person is standing on which node
	// add the person to the node i am currently standing on.
	if (((SZ_World*)(world))->Utils.gNodes[currentCell]!=NULL) 
	{
		((SZ_World*)(world))->Utils.gNodes[currentCell]->PersonList.add(this);
	}

	// remove me from the node i was standing on.
	if  (((SZ_World*)(world))->Utils.gNodes[oldTile]!=NULL)
	{
		((SZ_World*)(world))->Utils.gNodes[oldTile]->PersonList.substract(this);
	}


		
		actions.TInCell = 0;
	
		SZ_ActorSprite::changeTile();
}

void SZ_Person::applyForceAction(NxVec3 force)
{
	switch (actions.currentAction)
	{
		case actions.amWaiting:	
			entity->setLinearVelocity(NxVec3(0,0,0));
			entity->setAngularVelocity(NxVec3(0,0,0));
		break;		

		case actions.amGoingTarget:
			//entity->setLinearVelocity(force*10);
			if (force.magnitude() > 2) { force.normalize(); force = force * 2; }	// max force
			entity->addForce(force,NX_VELOCITY_CHANGE, true);
			//entity->addForce(force*5,NX_ACCELERATION, true);
			//entity->addForce(force*8,NX_FORCE, true);
			//entity->addForce(force*5, NX_FORCE, true);
			
		break;
	}	
}


// What happens to a crowd member when it spawns? generate it's list of aims.
void SZ_Person::onCreate()
{
	//this->sprite->setRGBA((int)frandom02(0,255), (int)frandom02(0,255), (int)frandom02(0,255), 255);
	this->entity->setGlobalPosition(NxVec3(7.5,0.1,35));
	actions.currentAction = actions.amGoingTarget; 

	actions.person = this;
	
	actions.oldAction	  = actions.currentAction;

	actions.TLiving = 0;
	actions.TInCell = 0;
	actions.TamWaiting = 0;
	actions.TamGoingTarget = 0;	

	actions.world  = world;
	actions.person = this;
 
	this->actions.aim_GenerateListAims(2);	// generate the aim list randomly

	personIndexCnt ++;
	personIndex = personIndexCnt;

	ggIntraList->parent = this;
}


void SZ_Person::behaviour_actions()
{
	
	if (tmp_world->Utils.gNodes[currentCell]!=NULL)
	{
		tmp_world->Utils.gNodes[currentCell]->DirectionList += getDirection();
	}


	if ( (currentCell!= actions.aim_targetCell) && (actions.currentAction == actions.amWaiting) )
	{
		actions.currentAction = actions.amGoingTarget;
	}

	// if the person reaches the node it wants to go to, wait there for x seconds then proceed to the next aim
	if (currentCell == actions.aim_targetCell)
	{
		int cost_time = 0;
		actions.currentAction = actions.amWaiting;
		//((SZ_World*)(world))->Utils.vVectorList.newVector(this->getPosition(),NxVec3(0,10,0),NxVec3(1,0,0));
		cost_time = ((SZ_FunctionBase*)(tmp_world->Utils.gNodes[currentCell]->functions))->timecost;
		//cost_time = cost_time * (FUNCTION_TIME_COST_MULTI - (tmp_world->sbGetLayerOccupancy(currentCell))->amount*2) ;
		

		
		if (actions.TamWaiting > cost_time)
		{
			int groupMembersOnSameCell = 0;
			for (int i = 0; i < (tmp_world->sbGetLayerOccupancy(currentCell)->amount); i++)
			{
				for ( int k = 0; k < ggIntraList->amount; k++)
				{
					if ( ggIntraList->SocialIntraRecord[k]->toPerson == tmp_world->sbGetLayerOccupancy(currentCell)->PersonRecord[i])
					{
						groupMembersOnSameCell++;		

					}
				}
				
				if (ggIntraList->amount == 0)
				{
					// for people with no groups, no point of waiting for others
					actions.currentAction = actions.amGoingTarget;
					actions.aim_SetNextAim();
				}

			}

			
			// friend waiting

				//printf("group members on same cell %i \n",groupMembersOnSameCell);
				if (groupMembersOnSameCell != ggIntraList->amount )
				{
					//printf("waiting for friends \n");
					actions.TamWaiting = 0;
					
				}

				if (groupMembersOnSameCell == ggIntraList->amount )
				{
					//printf("all friends there, let's roll \n");
						for ( int k = 0; k < ggIntraList->amount; k++)
						{			
							actions.currentAction = actions.amGoingTarget;
							actions.aim_SetNextAim();
							ggIntraList->SocialIntraRecord[k]->toPerson->actions = this->actions;
						}
				}

				if ( (tmp_world->sbGetLayerOccupancy(currentCell)->amount)>25) // dont wait if its too busy
				{
						for ( int k = 0; k < ggIntraList->amount; k++)
						{			
							actions.currentAction = actions.amGoingTarget;
							actions.aim_SetNextAim();
							ggIntraList->SocialIntraRecord[k]->toPerson->actions = this->actions;
						}
				}
			

			

			//entity is most likely to be put to sleep by physx after a short while
			if (entity->isSleeping()) {  entity->wakeUp(); }
		}
		
	}
}

void SZ_Person::behaviour_navigation()
{

	if ((currentCell>NODES_AMOUNT) && (currentCell<0)) { return; }	// make sure this person is on a valid cell

	//tmp_world->Utils.vVectorList.newVector(getPosition(),NxVec3(0,ggMemberCount,0),NxVec3(1,0,0));


	int  friendsFrontMe = 1; // total of friends in front of me (positive dot product)
	bool head_move = false;	 // will this person move its head this updatecycle?

	// forces acting on a person when navigating through the world (action == going to target)
	if (actions.currentAction == actions.amGoingTarget)
	{
		// (1) get the force towards aim
		if (currentCell != actions.aim_targetCell)
		{

				if (tmp_world->Utils.gNodes[currentCell]!=NULL)
				{	
					NxVec3 aim_force_cell(0,0,0);				// force towards the desired goal cell
					//aim_force_cell = NxVec3(0,0,0);		
		 
					// force to go towards the desired cell		
					if (tmp_world->Utils.gNodes[currentCell]->connection[tmp_world->Utils.gNodes[currentCell]->pathTo[actions.aim_targetCell]]!=NULL)
					{
 						aim_force_cell += tmp_world->sbGetLayerNavigation(currentCell,actions.aim_targetCell);
					}  

					// (2) get the crowd member avoidance force

					NxVec3 totalavoidance = NxVec3(0,0,0);

					if (ggMemberCount <= 1)	// only the first member of the group gets the force applied
					{
						// go through all nodes visible by this person	
						for (int j=0;j<tmp_world->sbGetLayerAreaSearch(this)->amount;j++)		
						{		
							
							//get the current node number of iterating visible node of the person
							if (tmp_world->Utils.gNodes[tmp_world->sbGetLayerAreaSearch(this)->intRecord[j]]==NULL) {continue;}				
							int currentNodeNum = tmp_world->Utils.gNodes[tmp_world->sbGetLayerAreaSearch(this)->intRecord[j]]->number;
						
							//tmp_world->Utils.vVectorList.newVector(tmp_world->Utils.cellToPos(currentNodeNum), NxVec3(0,1,0), NxVec3(1,0,0));
							// turn head towards camera if visible
							//if (tmp_world->gCamera.RecticleCell == currentNodeNum)
							{	
							//	head_move = true;
							}	

							// debug mode, count the camera reticle as a person
							if (RENDER_CAMERA_RECTILE)
							{
								if (tmp_world->gCamera.RecticleCell == currentNodeNum)
								{
									NxVec3 dji = NxVec3(0,0,0);
									//if (tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]!=NULL)
									{
										dji = 
											getPosition() - tmp_world->gCamera.RecticlePoint ;
										//tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->getPosition()+
										//(tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->entity->getLinearVelocity()*10)
										//-getPosition();
									}			

									//tmp_world->Utils.vVectorList.newVector(getPosition(),dji,NxVec3(1,0,0));

									NxVec3 djicrossvi = NxVec3(0,0,0);
									djicrossvi = dji;
									djicrossvi = djicrossvi.cross(getDirection());
									djicrossvi = djicrossvi.cross(dji);
									if (djicrossvi.magnitude()!=0)
									{
										djicrossvi.normalize();
									}								

									NxVec3 tj;
									tj = djicrossvi;

									float wdi = (dji.magnitude() - (MAX_PERSON_VIEW_DISTANCE*WORLD_TILE))*(dji.magnitude() - (MAX_PERSON_VIEW_DISTANCE*WORLD_TILE));
									float woi = 2.4f;

									//if (aim_force_cell.dot(tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->getDirection()) < 0) { 

									//	woi = 1.2f;
									//}

									tj = tj*woi*wdi;
									totalavoidance += (tj);		/// avoidance from camera reticle

									//tmp_world->Utils.vVectorList.newVector(getPosition(),totalavoidance,NxVec3(0,0,1));
								}

									
							}
							// end debug mode, camera as reticle
			
						
							//go through all persons on the current iterating node
							for (int k=0; k<tmp_world->sbGetLayerOccupancy(currentNodeNum)->amount; k++)
							{	
								//printf("going through person on cell %i \n", tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->currentCell);
								if (tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k] != this) 
								{
									
									NxVec3 dji = NxVec3(0,0,0);
									if (tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]!=NULL)
									{
										dji = 
											getPosition() - tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->getPosition()+
											(tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->entity->getLinearVelocity());	// prediction of position adjustement
											
									}			

									//tmp_world->Utils.vVectorList.newVector(getPosition(),dji,NxVec3(1,0,0));
									
									NxVec3 djicrossvi = NxVec3(0,0,0);
									djicrossvi = dji;
									djicrossvi = djicrossvi.cross(getDirection());
									djicrossvi = djicrossvi.cross(dji);
									if (djicrossvi.magnitude()!=0)
									{
										djicrossvi.normalize();
									}								

									NxVec3 tj;
									tj = djicrossvi;

									float wdi = (dji.magnitude() - (MAX_PERSON_VIEW_DISTANCE*WORLD_TILE))*(dji.magnitude() - (MAX_PERSON_VIEW_DISTANCE*WORLD_TILE));
									float woi = 0.0024f;
									
									if (aim_force_cell.dot(tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->getDirection()) < 0) { 

										woi = 0.0012f;
									}

									tj = tj*woi*wdi;
									
									//printf("tj %f %f \n", tj.x, tj.z);
									//tj.normalize();

									

									// group: check if person is in social group. If yes, do not avoid him
									for (int l = 0; l< this->ggIntraList->amount; l++)
									{
										//printf("got %i friends \n", ggIntraList->amount);
										
										if ( tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k] == ggIntraList->SocialIntraRecord[l]->toPerson)
										{
											//centerMass += ggIntraList->SocialIntraRecord[l]->toPerson->getPosition();
											//printf("found a friend! \n");
											totalavoidance = NxVec3(0,0,0);
											tj = NxVec3(0,0,0);
											if (getDirection().dot(ggIntraList->SocialIntraRecord[l]->getToPersonVector())>0.0f)
											{
												friendsFrontMe++;
											}

											//tj = this->behaviour_seek(tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->getPosition());
											//tmp_world->Utils.vVectorList.newVector(tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->getPosition(), ggIntraList[j].parent->getPosition()-tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->getPosition(), NxVec3(0,0,1));
										}
										
									}
									
									
									
									totalavoidance += (tj);		// avoidance of people	
									
								}		// end avoid other people
							
						

					


				

							//visibleTilesPersonNum += tmp_world->sbGetLayerOccupancy(currentNodeNum)->amount; 

							//totalavoidance *= this->visibleTilesPersonNum;
							//totalavoidance.y = 0;	
							//totalavoidance = totalavoidance  / (tmp_world->sbGetLayerOccupancy(currentNodeNum)->amount+0.01f); // normalise avoidance vector
							//totalavoidance /= 1000
							//totalavoidance.normalize();
							}



					//totalavoidance.normalize();

					//tmp_world->Utils.vVectorList.newVector(getPosition(), aim_force_cell, NxVec3(1,0,0));
					//tmp_world->Utils.vVectorList.newVector(getPosition(), totalavoidance, NxVec3(0,1,0));

					//printf("totalavoidance %f %f \n", totalavoidance.x, totalavoidance.z);
					
					
					// (end) apply the sum of all the forces onto agent
					
					

					//tmp_world->Utils.vVectorList.newVector(getPosition(), force, NxVec3(0,0,1));

					/*
						if (totalavoidance.magnitudeSquared() > 6.0)
					{
						totalavoidance.normalize();
					}
					*/
		
						}

						// end go through all nodes visible by this person
					} // end only first member

					force = NxVec3(0,0,0);
					force = aim_force_cell*tmp_world->gDatalogger.tmp_forceSelfAimAttract+ totalavoidance*tmp_world->gDatalogger.tmp_forceOthersAvoid; //+ (this->getDirection());														// sum of the force
					force = force *  ( ( MAX_PERSON_PER_CELL - tmp_world->sbGetLayerOccupancy(this->currentCell)->amount ) / MAX_PERSON_PER_CELL ) ;	// force reduced with density

					// (b) groups: instead of having each individual a force acting on them, use the same force as the first person of the group.				
					for (int l = 0; l< this->ggIntraList->amount; l++)
					{
						ggIntraList->SocialIntraRecord[l]->toPerson->force += force;

					}

					
					NxVec3 groupForce = NxVec3(0,0,0);
						
					
					for (int l = 0; l< this->ggIntraList->amount; l++)
					{
						groupForce += (ggIntraList->SocialIntraRecord[l]->getToPersonVector()) ;
						//tmp_world->Utils.vVectorList.newVector(getPosition(), (ggIntraList->SocialIntraRecord[l]->getToPersonVector()) ,NxVec3(0,0,1));			
					}

					if (ggIntraList->amount != 0 ) 
					{
						
						groupForce	= groupForce / ( (float)(ggIntraList->amount)+1.0f );	// vector to center of mass of the group
						
						//groupForce  = getPosition()+groupForce ;
						//groupForce  = groupForce - (getDirection());

						
						//tmp_world->Utils.vVectorList.newVector(getPosition(), groupForce ,NxVec3(0,0,1));
						
						//model->head_setDirection(getPosition()+groupForce);

						//groupForce =   groupForce - getPosition() ;
						//groupForce.normalize();

						//tmp_world->Utils.vVectorList.newVector(getPosition(), groupForce ,NxVec3(0,0,1));

						//if (groupForce.dot(getDirection()) > 0.7)
						

							NxVec3 dji = NxVec3(0,0,0);
							//if (tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]!=NULL)
							{
								dji = 
									groupForce + getDirection() ;
								//tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->getPosition()+
								//(tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->entity->getLinearVelocity()*10)
								//-getPosition();
							}			

							//tmp_world->Utils.vVectorList.newVector(getPosition(),dji,NxVec3(1,0,0));

							NxVec3 djicrossvi = NxVec3(0,0,0);
							djicrossvi = dji;
							djicrossvi = djicrossvi.cross(getDirection());
							djicrossvi = djicrossvi.cross(dji);
							if (djicrossvi.magnitude()!=0)
							{
								djicrossvi.normalize();
							}								

							NxVec3 tj;
							tj = djicrossvi;

							float wdi = (dji.magnitude() - (MAX_PERSON_VIEW_DISTANCE*WORLD_TILE))*(dji.magnitude() - (MAX_PERSON_VIEW_DISTANCE*WORLD_TILE));
							float woi = 0.024f;

							//if (aim_force_cell.dot(tmp_world->sbGetLayerOccupancy(currentNodeNum)->PersonRecord[k]->getDirection()) < 0) { 

							//	woi = 1.2f;
							//}

							tj = tj*woi*wdi;

							//tmp_world->Utils.vVectorList.newVector(getPosition()+groupForce, NxVec3(0,5,0), NxVec3(0,1,0));

		

							force = force + ((groupForce)	*tmp_world->gDatalogger.tmp_forceGroupCenterAttract	);	// force towards the center of the group
							totalavoidance += (tj)			*tmp_world->gDatalogger.tmp_forceGroupCenterAvoid;			// force avoiding the center

							//tmp_world->Utils.vVectorList.newVector(getPosition(),totalavoidance,NxVec3(1,0,0));

							force = force + (totalavoidance);

					}
					

					

					

				
				

			}

			applyForceAction(force);
		}
	}

	/*
	NxVec3 centerMass;
	//centerMass = this->getPosition();
	// go through every person within my list of known people
	for (int l = 0; l< this->ggIntraList->amount; l++)
	{
		centerMass += ggIntraList->SocialIntraRecord[l]->getToPersonVector();
	}
	centerMass = centerMass / (ggIntraList->amount +1);

	if (ggIntraList->amount != 0) 
	{ 
		//tmp_world->Utils.vVectorList.newVector(getPosition()+centerMass, NxVec3 (0,10,0) ,NxVec3(1,0,0));
		//tmp_world->Utils.vVectorList.newVector(getPosition(), centerMass,NxVec3(1,0,0));
	}
	*/

	//turn head if conditions met
	//if (head_move == true )
	{
		
		//model->head_setDirection(tmp_world->gCamera.Pos);
	}// else
	{
	//	model->head_setIdentity();
	}


	//centerMass = centerMass / (ggIntraList->amount);
	


	//aim_force_cell += ;
	
	

	//NxVec3 forcecell = lookAt(tmp_world->gCamera.RecticlePoint+entity->getGlobalPosition());
	//forcecell.normalize();
	//tmp_world->Utils.vVectorList.newVector(getPosition(),forcecell,NxVec3(10,0,0));

	//setDirection(forcecell);

	//Persons[i]->setDirection(Persons[i]->lookAt(Persons[i]->entity->getLinearVelocity()+Persons[i]->entity->getGlobalPosition()));
	
	
	//forcecell.normalize();
	//setDirection(forcecell);

}




NxVec3 SZ_Person::behaviour_seek( NxVec3 target )
{
	NxVec3 steering;

	steering = target - this->getPosition();
	steering.normalize();

	steering = steering - entity->getLinearVelocity();

	return steering;
}

NxVec3 SZ_Person::behaviour_flee( NxVec3 target )
{
	NxVec3 steering;

	steering = this->getPosition() - target;
	steering.normalize();

	steering = steering - entity->getLinearVelocity();

	return steering;
}

void SZ_Person::draw()
{
	SZ_ActorSprite::draw();
	//if (RENDER_SOCIAL_CONNECTIONS)	
		ggIntraList->draw();	 // draw connections between individuals within the same group
}
