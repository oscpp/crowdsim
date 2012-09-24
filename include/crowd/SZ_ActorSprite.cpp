#include "SZ_ActorSprite.h"
#include "SZ_World.h"
#include "GLFontRenderer.h"


// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// vertex coords array
/*
GLfloat vertices[] = {  1,1,1,    -1,1,1,   -1,-1,1,    1,-1,1,        // v0-v1-v2-v3
                        1,1,-1,    1,1,1,    1,-1,1,    1,-1,-1,    // v0-v3-v4-v5
                        1,1,-1,   -1,1,-1,  -1,1,1,     1,1,1,        // v0-v5-v6-v1
                       -1,1,1,    -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
                       -1,-1,-1,  1,-1,-1,   1,-1,1,   -1,-1,1,    // v7-v4-v3-v2
                       -1,1,-1,   1,1,-1,1,  -1,-1,    -1,-1,-1,   // v4-v7-v6-v5

					    // 0.5 = shoulder, 
					    1+0.5, 1-2  ,1,    -1-0.5, 1-2, 1,  -1-0.5,-1-2.3, 1,    1+0.5,-1-2.3, 1,        // v0-v1-v2-v3 front
                        1+0.5, 1-2   ,-1,     1+0.5, 1-2, 1,   1+0.5,-1-2.3, 1,    1+0.5,-1-2.3,-1,				 // v0-v3-v4-v5  right
                        1+0.5, 1-2   ,-1,    -1-0.5, 1-2,-1,  -1-0.5, 1-2, 1,    1+0.5, 1-2, 1,           // v0-v5-v6-v1  top
                       -1-0.5, 1-2   ,1,    -1-0.5, 1-2,-1,  -1-0.5,-1-2.3,-1,   -1-0.5,-1-2.3, 1,      // v1-v6-v7-v2   left 
                       -1-.05,-1-2.3 ,-1,    1+0.5 ,-1-2.3,-1,   1+0.5,-1-2.3, 1,   -1-0.5,-1-2.3, 1,			// v7-v4-v3-v2   bottom
                       -1-0.5, 1-2   ,-1,    1 +0.5, 1-2,-1,   1+0.5,-1-2.3,-1,   -1-0.5,-1-2.3,-1,      // v4-v7-v6-v5   behind
			 
						1,1-4.3,1,    -1,1-4.3,1,   -1,-1-4.3,1,    1,-1-4.3,1,        // v0-v1-v2-v4.3
                        1,1-4.3,-1,    1,1-4.3,1,    1,-1-4.3,1,    1,-1-4.3,-1,    // v0-v4.3-v4-v5
                        1,1-4.3,-1,   -1,1-4.3,-1,  -1,1-4.3,1,     1,1-4.3,1,        // v0-v5-v6-v1
                       -1,1-4.3,1,    -1,1-4.3,-1,  -1,-1-4.3,-1,   -1,-1-4.3,1,    // v1-v6-v7-v2
                       -1,-1-4.3,-1,  1,-1-4.3,-1,   1,-1-4.3,1,    -1,-1-4.3,1,    // v7-v4-v4.3-v2
                       -1,1-4.3,-1,   1,1-4.3,-1,    1,-1-4.3,-1,   -1,-1-4.3,-1  // v4-v7-v6-v5
};  
*/
						

// normal array




SZ_ActorSprite::SZ_ActorSprite()
{
	entity = NULL;
	//sprite = NULL;
	model = NULL;
	visibleTilesPersonNum = 0;
	iterations = 0;	
	
}

SZ_ActorSprite::~SZ_ActorSprite()
{
	if (model!=NULL)
	{
		delete model;
		model = NULL;
	}
}

void SZ_ActorSprite::processKeys(int gKeys, float delta)
{
}

void SZ_ActorSprite::processKeysSpecial(int gKeysSpecial, float delta)
{	
}

void SZ_ActorSprite::keyboardDownCall(int key,int x,int y)
{
}
void SZ_ActorSprite::keyboardUpCall(int key,int x,int y)
{
}
void SZ_ActorSprite::keyboardSpecialDownCall(int key,int x,int y)
{
}
void SZ_ActorSprite::keyboardSpecialUpCall(int key,int x,int y)
{
}

void SZ_ActorSprite::makePhysX()
{
	NxMaterial *CushionMaterial = NULL; //global
	NxMaterialDesc materialDesc;
	materialDesc.restitution = 1.0f;		// original 0
	materialDesc.staticFriction = 1.0f;
	materialDesc.dynamicFriction = 1.0f;
	CushionMaterial=wScene->createMaterial(materialDesc);
	
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	NxBoxShapeDesc boxDesc;

	
	NxVec3 modelSize = NxVec3(frandom02(0.20,0.30),frandom02(0.20,0.30),frandom02(0.20,0.30));
	model->passSize(modelSize);			// size of the 3d model
	size = NxVec3(0.25,0.25,0.20);						// size of the bounding box

	boxDesc.dimensions = NxVec3(1/3.0f,0.5,1/3.0f);			// dimensions of the box of the person
	boxDesc.dimensions = NxVec3(size.x,1,size.z);					// always from the origin (i.e. 1 = 1m to the left o
	boxDesc.localPose.M.setColumn(3,NxVec3(0,1,0));			// rotate on the y axis
	boxDesc.localPose.t = NxVec3(0,1,0);
	boxDesc.materialIndex = CushionMaterial ->getMaterialIndex();
	//boxDesc.mass = 1000000;

	actorDesc.shapes.pushBack(&boxDesc);
	actorDesc.body = &bodyDesc;
	//volume is based on the size of the 3dmodel
	//float volume = size.x * size.y * size.z;
	//actorDesc.density = 0.2+volume*2000.0f; 
	actorDesc.density = 10000.0f;



	NxVec3 testangle;

	


	entity = wScene->createActor(actorDesc);
	
	entity->setLinearDamping(10);   // 3 // 20
	entity->setAngularDamping(10);  // 3 // 20



	entity->raiseBodyFlag(NX_BF_FROZEN_POS_Y);
	entity->raiseBodyFlag(NX_BF_FROZEN_ROT_Z);
	entity->raiseBodyFlag(NX_BF_FROZEN_ROT_X);
	entity->raiseBodyFlag(NX_BF_FROZEN_ROT_Y);
	
	entity->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
	entity->setMass(10000);	
}

void SZ_ActorSprite::onCreate()
{
}

void SZ_ActorSprite::init(NxScene *whichScene, unsigned int texture_array[], void *world)
{
	//sprite  = new SZ_Sprite(1,1,96,0,32,32,8);	// sizex, sizez, texture: start u, start v, width, length, frames total
	model = new SZ_3DModel();

	//size = NxVec3((rand()%100+1)/1000.0f,(rand()%100+1)/1000.0f,(rand()%100+1)/1000.0f);
	//size = NxVec3((rand()%1+1)/10.0f,(rand()%1+1)/10.0f,(rand()%1+1)/10.0f);



	wScene = whichScene;
	makePhysX();
	entity->setGlobalPosition(NxVec3(0,0,0));
	this->world = world;
	model->init(texture_array,world, this);

	this->onCreate();

	oldTile = 0;
	currentCell = 0;

	tmp_world = ((SZ_World*)(world));
}

// draws the actor on screen.
void SZ_ActorSprite::draw()
{	
	//return;
	//if (currentCell<0) {return;}
	//if (this==NULL) {return;}

	//glColor4f(1,1,1,1);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	
	glBindTexture(GL_TEXTURE_2D, model->textureArray[model->whichTexture]);
	glEnable(GL_TEXTURE_2D);
	//glDisable(GL_LIGHTING);
	glPushMatrix();
		model->draw();	
	glPopMatrix();	

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	
	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glColor4f(1,1,1,1);





	//((SZ_World*)(world))->Utils.findNeighbour(3,0,((SZ_World*)(world))->Utils.gNodes[currentCell],&((SZ_World*)(world))->Utils.nList,this);
	
	/*
	for (int j=0;j<visibleTiles.amount;j++)
		{
			((SZ_World*)(world))->Utils.gNodes[visibleTiles.intRecord[j]]->draw();;
		}
		*/

/*

	for (int j=0; j< ((SZ_World*)(world))->Utils.gNodes[this->currentCell]->PersonList.length(); j++)
	{

		NxVec3 avoidanceForce;
		NxVec3 offset = lookAt(((SZ_World*)(world))->Utils.gNodes[currentCell]->PersonList.PersonRecord[j]->getPosition());


		//avoidanceForce = avoidanceForce.cross(getDirection()) + getPosition();


		if (avoidanceForce.dot(getDirection())<0)
		{
	
		NxVec3 sideforce = getDirection().cross(NxVec3(0,1,0));
		//avoidanceForce = avoidanceForce.magnitude() * sideforce;
		//avoidanceForce = offset + getPosition();
		avoidanceForce = (sideforce * offset.magnitude()) + getPosition();

		glLineWidth(10);
	
		glBegin(GL_LINES);
			//glVertex3f(getPosition().x,getPosition().y,getPosition().z);
			//glVertex3f(((SZ_World*)(world))->Utils.gNodes[this->currentCell]->PersonList.PersonRecord[j]->getPosition().x,
			//		   ((SZ_World*)(world))->Utils.gNodes[this->currentCell]->PersonList.PersonRecord[j]->getPosition().y,
			//		   ((SZ_World*)(world))->Utils.gNodes[this->currentCell]->PersonList.PersonRecord[j]->getPosition().z);
			glVertex3f(getPosition().x,getPosition().y,getPosition().z);
			glVertex3f(avoidanceForce.x,avoidanceForce.y,avoidanceForce.z);
		glEnd();
		}
		
	}
	*/
}

// lookAt(whereto) returns a normalised vector from this sprite towards whereto
NxVec3 SZ_ActorSprite::lookAt(NxVec3 whereto)
{	
	NxVec3 L1 = whereto;
	NxVec3 p = this->entity->getGlobalPosition();
	L1 = L1 -p;
	L1.y = 0;		// flat 2D surface
	L1.normalize();
	return L1;
}

NxVec3 SZ_ActorSprite::getPosition()
{
	entity->wakeUp();
	return entity->getGlobalPosition();
}

// turns the actor towards a specified vector in his local actorspace.
void SZ_ActorSprite::setDirection(NxVec3 target)
{
	NxMat33 m = entity->getGlobalOrientation();
	m.setColumn(2, target);
	entity->setGlobalOrientation(m);
}

NxVec3 SZ_ActorSprite::getDirection()
{
	NxMat33 m = entity->getGlobalOrientation();
	return m.getColumn(2);
}

// rotates this actor for angle around axis xyz
void SZ_ActorSprite::rotatef(float angle, NxVec3 xyz)
{
	NxQuat quat(angle, xyz);
	quat.multiply(entity->getGlobalOrientationQuat(),quat);
	entity->setGlobalOrientationQuat(quat);

}

// updates the cator
void SZ_ActorSprite::update()
{
	/*
	// Turn the head in Y direction towards the camera
	model->head_matrix = entity->getGlobalOrientation();	
	NxVec3 L1;
	L1 = tmp_world->gCamera.RecticlePoint;
	NxVec3 p = entity->getGlobalPosition();
	L1 = L1 - p ;

	NxVec3 L2;
	L2 = getDirection();

	NxVec3 L3;
	L3 = L2;

	//2d cross product - find if the recticle is left or right of the person
	L3.x = L2.z;
	L3.z = -L2.x;

	float angle;
	angle = NxMath::acos( L1.dot(L2) / (L1.magnitude()*L2.magnitude()));
	NxMat33 tmp_head_matrix;
	if (L3.dot(L1)<0)
	{
		tmp_head_matrix.rotY(-angle);
	}
	if (L3.dot(L1)>0)
	{
		tmp_head_matrix.rotY(angle);
	}
	model->head_matrix = model->head_matrix*tmp_head_matrix;
	*/
	


	/*
	NxVec3 axis, binormal, normal;
	axis = tmp_world->gCamera.RecticlePoint;
	NxVec3 p = entity->getGlobalPosition();
	axis = axis - p ;
	axis.y = 0;
	axis = getDirection();
	axis.y = 0;
	axis.normalize();
	axis = NxVec3(0,1,0);
	normal = NxVec3(0,0,0);
	binormal = NxVec3(0,0,0);
	//binormal.cross(normal, axis);
	*/

	//NxVec3 axis(0,1,0);
	//axis.normalize();
	//NxVec3 binormal, normal;
	

	
	//normal = NxVec3(100,100,4301);
	//NxNormalToTangents(axis,binormal,normal);
	//NxMat33 orient(normal,axis,binormal);
	//model->head_matrix.
	/*
	NxVec3 Forward(0,0,0);
	NxVec3 Right;

	
	Forward = tmp_world->gCamera.RecticlePoint;
	NxVec3 p = entity->getGlobalPosition();
	Forward = Forward - p ;

	NxVec3 Up(0,1,0);
	Forward.normalize();
	Right.cross(Forward,Up);

	NxVec3 binormal, normal;
	NxNormalToTangents(Forward,Right,Up);
	NxMat33 orient(Right,Forward,Up);

	

	
	model->head_matrix = orient; //* entity->getGlobalOrientation();
	*/



	if (entity == NULL) { return;}
	// update the iterations count
	iterations++;
	//which tile i am on;
	int g  = (entity->getGlobalPosition().x / WORLD_TILE);
	int g2 = (entity->getGlobalPosition().z / WORLD_TILE );
	
	currentCell = g+(g2*WORLD_X_5X5);

	if (currentCell<0) {return;}

	//printf("g %i, g2, currentCell %i \n",g,g2, currentCell);
	//make sure I am currently on the map.
	if (currentCell > NODES_AMOUNT )
	{
		currentCell = oldTile;
	}



	//if the player changes tile, trigger the event changetile
	if (currentCell!=oldTile)
	{
		//printf("Changetile! %i to %i \n", oldTile, currentCell);
		
		this->changeTile();
		oldTile=currentCell;
	}


	/*
	visibleTilesPersonNum = 0;
	//if (((SZ_World*)(world))->Utils.iterations>1)
	{
		
		for (int j=0;j<visibleTiles.amount;j++)
		{
			if (((SZ_World*)(world))->Utils.gNodes[visibleTiles.intRecord[j]]!= NULL)
			{
				visibleTilesPersonNum += ((SZ_World*)(world))->Utils.gNodes[visibleTiles.intRecord[j]]->PersonList.amount;	
			}
		}
	}
	*/

 if (currentCell<0) {
	 printf("change tile on node :%i iterations %i, %4.2f %4.2f oldtile %i \n", currentCell, iterations, getPosition().x, getPosition().z, oldTile); 
 }


}



// this function is called every time an actorsprite goes from one tile to another.
void SZ_ActorSprite::changeTile()
{
	
	//if (currentCell<0) {return;}
	//if (((SZ_World*)(world))->Utils.gNodes[currentCell]==NULL) {return;}
	if (iterations<10) {return;}

	// visible node update code
	visibleTiles.empty();  // empty the list of neighbouring visible nodes
	((SZ_World*)(world))->Utils.nList.empty();
	// find me all the neighbouring tiles. save neighbouring tiles into nList
	//if (currentCell<0) {return;}

	/*
	SZ_Node* tmpNode = ((SZ_World*)(world))->Utils.gNodes[currentCell];						
	SZ_NodeRecord* tmpRecord;
	tmpRecord = new SZ_NodeRecord();
	tmpRecord->gNode = tmpNode;	
	//((SZ_World*)(world))->Utils.nList.add(tmpRecord);
	
	if (!(((SZ_World*)(world))->Utils.nList.contains(tmpNode)))
	
	{ ((SZ_World*)(world))->Utils.nList.add(tmpRecord); } else { delete tmpRecord; }
	*/

	((SZ_World*)(world))->Utils.findNeighbour(MAX_PERSON_VIEW_DISTANCE,0,((SZ_World*)(world))->Utils.gNodes[currentCell],&((SZ_World*)(world))->Utils.nList,this, true);



	/*
	if (this->iterations>3)
	{
		((SZ_World*)(world))->Utils.findPathFrom(3,0,aim_targetCell, ((SZ_World*)(world))->Utils.gNodes[currentCell],&((SZ_World*)(world))->Utils.pList,this);
	}
	*/
	
	
	// used for the total amount of characters inside the visible !this! character node
	// int tmp_total_visible = 0;  // -1, as 'self' is also counted. 

	// go through every member of the list, and add them into visibleTiles

	//printf("world amount: %i \n", ((SZ_World*)(world))->Utils.nList.amount);
	
	visibleTiles.add(currentCell); // add the cell this person is currently in
	for (int j=0;j<((SZ_World*)(world))->Utils.nList.amount;j++)
		{
			visibleTiles.add(((SZ_World*)(world))->Utils.nList.NodesRecord[j]->gNode->number);

			// added to sum up the total of visible people
			// tmp_total_visible += ((SZ_World*)(world))->Utils.gNodes[(((SZ_World*)(world))->Utils.nList.NodesRecord[j]->gNode->number)]->PersonList.amount;
		}
	//if (((SZ_World*)(world))->Utils.nList.NodesRecord[currentCell]->gNode->number == NULL) { printf("error \n"); }
	

	// this->visibleTilesPersonNum = tmp_total_visible;
	
	
	//if (iterations > 50)
	{
			//if (this->visibleTilesPersonNum >= 30)
			{
				/*

				this->Utils.findPathFrom(10,0,799,Utils.gNodes[Player.currentCell],&Utils.path,&Player);
				int target = (Utils.path.NodesRecord[Utils.path.length()-1]->gNode->number);
				this->Utils.findPathWeighted(target,this->Player.currentCell);								
				this->Utils.updatePath(target,&Utils.pList);
				*/

				//((SZ_World*)(world))->Utils.findPathFrom(3,0,this->aim_targetCell,((SZ_World*)(world))->Utils.gNodes[currentCell],&(((SZ_World*)(world))->Utils.pList),this);
				
				
				//int target = (((SZ_World*)(world))->Utils.path.NodesRecord[((SZ_World*)(world))->Utils.pList.length()-1]->gNode->number);
				//printf("target : %i \n", );
				//if (((SZ_World*)(world))->Utils.pList.length()>0)
				{
					//int cell   = ((SZ_World*)(world))->Utils.pList.length()-1;
					//int target = ((SZ_World*)(world))->Utils.pList.NodesRecord[cell]->gNode->number;

					//printf("target %i cell %i \n", target, cell);
					//((SZ_World*)(world))->Utils.findPathWeighted(target,this->currentCell);	
					//((SZ_World*)(world))->Utils.updatePath(this->currentCell,&(((SZ_World*)(world))->Utils.path));
				}
				

				//((SZ_World*)(world))->Utils.findPathWeighted(target,this->currentCell);				
				
				//((SZ_World*)(world))->Utils.updatePath(this->currentCell,&(((SZ_World*)(world))->Utils.path));

				//((SZ_World*)(world))->Utils.findPathWeighted(this->aim_targetCell,this->currentCell);								
				//((SZ_World*)(world))->Utils.updatePath(this->currentCell,&(((SZ_World*)(world))->Utils.path));
			}
	}





}


void SZ_ActorSprite::drawNeighbour()
{
	for (int j=0;j<visibleTiles.amount;j++)
		{
			// transform node number to XZ coordinates
			int X = (visibleTiles.intRecord[j] % WORLD_X_5X5);	  // transform the tile value into the top left X corner
			int Z = (visibleTiles.intRecord[j]-X)/WORLD_X_5X5;    // transform the tile value into the top left Y corner1

			glEnable(GL_BLEND);
			//glColor4f(0,0.1,0.5,0.15);
			glColor4f(1,0,0,0.3);		
			glBegin(GL_POLYGON);
				glVertex3f(X*WORLD_TILE+WORLD_TILE,0,Z*WORLD_TILE);
				glVertex3f(X*WORLD_TILE,0,Z*WORLD_TILE);
				glVertex3f(X*WORLD_TILE,0,Z*WORLD_TILE+WORLD_TILE);
				glVertex3f(X*WORLD_TILE+WORLD_TILE,0,Z*WORLD_TILE+WORLD_TILE);
			glEnd();	
			glColor4f(1,1,1,1);
			//glDisable(GL_BLEND);
		}
}