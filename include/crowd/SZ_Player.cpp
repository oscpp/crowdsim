#include "SZ_Player.h"
#include "SZ_World.h"


SZ_Player::SZ_Player()
{
}

SZ_Player::~SZ_Player()
{
}

void SZ_Player::onCreate()
{
	entity->setGlobalPosition(NxVec3(7.5,0.1,7.5));
	//sprite->setFrames(8);
}

void SZ_Player::init(NxScene *whichScene, unsigned int texture_array[], void *world)
{
	//sprite  = new SZ_Sprite(1,1,96,0,32,32,8);	// sizex, sizez, texture: start u, start v, width, length, frames total
	model = new SZ_3DModel();

	size = NxVec3(0.1,0.1,0.1);
	model->passSize(size);

	wScene = whichScene;
	makePhysX();
	entity->setGlobalPosition(NxVec3(0,0,0));
	this->world = world;
	//model->init(texture_array,world);

	this->onCreate();

	oldTile = 0;
	currentCell = 0;
	this->changeTile();

}

void SZ_Player::processKeys(int gKeys, float delta)
{
}

void SZ_Player::processKeysSpecial(int gKeysSpecial, float delta)
{
	switch (gKeysSpecial)
			{	
				case GLUT_KEY_UP: { 
					//this->sprite->animateSprite(1);
					entity->addForce(entity->getGlobalOrientation().getColumn(2)*108400*delta, NX_FORCE,true); 
    				break; 
								  }
				case GLUT_KEY_LEFT: { 
			
						this->rotatef(3,NxVec3(0,1,0)); //this->sprite->animateSprite(1);
						//this->changeTile();	// forces a change tile every time the player turns. thus updating his visible neighbours
						break; 
									}
				case GLUT_KEY_RIGHT: { 
		
						this->rotatef(-3,NxVec3(0,1,0)); //this->sprite->animateSprite(1); 
						//this->changeTile(); // forces a change tile every time the player turns. thus updating his visible neighbours
						break; 
						}
			}
}

void SZ_Player::keyboardDownCall(int key,int x,int y)
{
}
void SZ_Player::keyboardUpCall(int key,int x,int y)
{
}
void SZ_Player::keyboardSpecialDownCall(int key,int x,int y)
{
}
void SZ_Player::keyboardSpecialUpCall(int key,int x,int y)
{
	switch (key)
	{
		case GLUT_KEY_UP: { entity->setLinearVelocity(NxVec3(0,0,0)); break; }
	}
}

void SZ_Player::update()
{	
	SZ_ActorSprite::update();
	//entity->setLinearVelocity(NxVec3(0,0,0));

}