#ifndef SZSPRITEACTOR_H
#define SZSPRITEACTOR_H

#include "..\..\general_utils.h"

#include "SZ_Config.h"
//#include "SZ_Sprite.h"
#include "SZ_3DModel.h"
#include "SZ_NumberList.h"

class SZ_World;


class SZ_ActorSprite
{
	private:

	public:
		 SZ_ActorSprite();
		~SZ_ActorSprite();

		//SZ_Sprite *sprite;
		SZ_3DModel *model;
		void *world;		// pointer to the world i belong to
		void *mscGroup;		// pointer to the MSc approach group I belong to.
		
		NxActor* entity;	// pointer to NxActor
		NxScene* wScene;    // pointer to wScene
		NxMaterial *CushionMaterial;
	    NxMaterialDesc materialDesc;

		SZ_World* tmp_world;

		// tile and neighbour stuff
		int oldTile;					// tile no I was on.
		int currentCell;				// tile no I am currently on.
		SZ_NumberList	visibleTiles;	// list of tiles I can currently see. filled up with data every changetile
		//SZ_NumberList   pathTiles;      // list of tiles I will walk onto to reach my aim_targetCell. 
		int visibleTilesPersonNum;		// number of persons I can currently see (based on visibleTiles)
		virtual void changeTile();		// what happens when this actor changes tile?
		void drawNeighbour();			// draws the visible neighbouring tiles.


		// how long do I exist (augmented by 1 every world update)
		int iterations;


		void draw();
		virtual void init(NxScene *whichScene,unsigned int texture_array[], void *world);  // creates an actorsprite with standard texture and standard size.
		virtual void onCreate();		// called after the initialization (init and makephysx)
		void makePhysX();				
		
		virtual void update();
				
		GLfloat vertices[72*3];
		GLfloat indices[72];
		GLfloat texcoord[216*2];
		GLfloat normals[72*2];

		inline bool isEntityNull() { if (entity==NULL) {return true;} else {return false;}; };

		// keyboard functions
		virtual void keyboardDownCall(int key,int x,int y);
		virtual void keyboardUpCall(int key,int x,int y);
		virtual void keyboardSpecialDownCall(int key,int x,int y);
		virtual void keyboardSpecialUpCall(int key,int x,int y);
		virtual void processKeys(int gKeys, float delta);
		virtual void processKeysSpecial(int gKeysSpecial, float delta);

		NxVec3 getPosition();						// get the globalposition of the entity
		NxVec3 getDirection();						// Returns in which direction the individual is currently facing
		NxVec3 lookAt(NxVec3 whereto);				// returns a vector from->to whereto
		void rotatef(float angle, NxVec3 xyz);		// rotates individual angle around axis xyz
		void setDirection(NxVec3 target);			

		NxVec3 size; //stores size for the model.

		// temporary 
		NxVec3 tmpRepulsion;
};

#endif