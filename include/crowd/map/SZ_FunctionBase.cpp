#include "SZ_FunctionBase.h"
#include "..\SZ_World.h"
#include "SZ_Node.h"

// from World Utils, still related to Function base
// function from world utils, loads the file that contains all the content about all nodes. i.e. if a node is an exit or
void SZ_World_Utils::loadPlaces()
{
	if (!bitmapplaces.read("textures/places.bmp", false)) { printf("places.bmp not loaded \n"); }	// load the map
	//bitmappath.printSelf();
	printf("...Places file loaded \n");
}

// function from world utils, loads the bitmap file that loads all the icons content
void SZ_World_Utils::loadPlaces_Icon()
{
	LoadTexBitmap("textures/places_icons.bmp",1,texture_array,true, bitmapplacesIcons);	// load the texture used for individuals/ui
	printf("...Icons file loaded \n");
}

// Function Base
SZ_FunctionBase::SZ_FunctionBase()
{
}

SZ_FunctionBase::~SZ_FunctionBase()
{
}

void SZ_FunctionBase::init(int whichTile, void* world, int range)
{	
	//index = 1;
	//row = 3;

	quadShopWidth  = LOCAL_QUAD_SIZE;
	quadShopHeight = LOCAL_QUAD_SIZE;
	quadOffsetFloor = LOCAL_QUAD_POS_Y;

	//(((SZ_World*)(world))->Utils.bitmapplacesIcons.printSelf());
	texWidth = (((SZ_World*)(world))->Utils.bitmapplacesIcons.getWidth());
	texHeight = (((SZ_World*)(world))->Utils.bitmapplacesIcons.getHeight());

	this->currentCell = whichTile;
	this->world = world;
	this->range = range;

	int X,Z;
	X = (currentCell % WORLD_X_5X5); 	  // transform the tile value into the top left X corner
	Z = (currentCell-X)/WORLD_X_5X5;      // transform the tile value into the top left Y corner
	position.x = (X*WORLD_TILE	+WORLD_TILE	/2.0);
	position.z = (Z*WORLD_TILE	+WORLD_TILE	/2.0);

	// find all the neighbouring tiles surrounding this specific Functionbase and store it into nList
	((SZ_World*)(world))->Utils.findNeighbour(range,0,((SZ_World*)(world))->Utils.gNodes[currentCell],&this->nList,NULL,false);

	
	SZ_Node* tmpNode = ((SZ_Node*)(((SZ_World*)(world))->Utils.gNodes[currentCell]));	
	SZ_NodeRecord* tmpRecord;
	tmpRecord = new SZ_NodeRecord();
	tmpRecord->gNode = tmpNode;
	nList.add(tmpRecord);

	texArray = 1;	// which texture array am I using?
}

// updates the base, used for the quad to always face the camera.
void SZ_FunctionBase::update()
{	
	NxVec3 quadZ = ( ((SZ_World*)(world))->gCamera.Pos - this->position ) / ((((SZ_World*)(world))->gCamera.Pos - this->position).magnitude());
	quadB = ( quadZ.cross(((SZ_World*)(world))->gCamera.Right) ) / ( quadZ.cross( ((SZ_World*)(world))->gCamera.Right).magnitude());
	quadA = quadB.cross(quadZ);
	quadX = NxVec3( this->position.z - ((SZ_World*)(world))->gCamera.Pos.z, 0, ((SZ_World*)(world))->gCamera.Pos.x - this->position.x);
	quadT = NxVec3( quadX / quadX.magnitude());
}

void SZ_FunctionBase::drawNeighbour()
{
	int X,Z;
	X = (currentCell % WORLD_X_5X5); 	  // transform the tile value into the top left X corner
	Z = (currentCell-X)/WORLD_X_5X5;      // transform the tile value into the top left Y corner

	// draw the neighbouring tiles
	for (int j=0;j<nList.amount;j++)
		{
			// transform node number to XZ coordinateswwwd
			X = (nList.NodesRecord[j]->gNode->number % WORLD_X_5X5); 	  // transform the tile value into the top left X corner
			Z = (nList.NodesRecord[j]->gNode->number-X)/WORLD_X_5X5;      // transform the tile value into the top left Y corner1

			glEnable(GL_BLEND);
			glColor4f(0,0.1,0.5,0.15);
					
			glBegin(GL_POLYGON);
				glVertex3f(X*WORLD_TILE+WORLD_TILE,0,Z*WORLD_TILE);
				glVertex3f(X*WORLD_TILE,0,Z*WORLD_TILE);
				glVertex3f(X*WORLD_TILE,0,Z*WORLD_TILE+WORLD_TILE);
				glVertex3f(X*WORLD_TILE+WORLD_TILE,0,Z*WORLD_TILE+WORLD_TILE);
			glEnd();		
		}
}

void SZ_FunctionBase::drawQuad()
{		
	u = (index*64)-64;	// index
	v = (row*64)-64;	// row

	h = 64; // 	
	w = 64; //
	
	int X,Z;
	X = (currentCell % WORLD_X_5X5); 	  // transform the tile value into the top left X corner
	Z = (currentCell-X)/WORLD_X_5X5;      // transform the tile value into the top left Y corner

	glColor3f(1,1,1);
  	GLboolean lighting_enabled;
	glGetBooleanv( GL_LIGHTING, &lighting_enabled );
	glDisable( GL_LIGHTING );

	glAlphaFunc(GL_GREATER, 0.1);
	glEnable(GL_ALPHA_TEST);	
  	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, ((SZ_World*)(world))->Utils.texture_array[texArray]);	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	NxVec3 quadTemp = (quadShopWidth)*quadT; 
	
	glBegin(GL_QUADS);
  		NxVec3 Q1,Q2,Q3,Q4;
		NxVec3 temp2 = NxVec3(0,quadShopHeight,0);
		Q1 = position + quadTemp + temp2;
		Q2 = position - quadTemp + temp2;
		Q3 = position - quadTemp - temp2;
		Q4 = position + quadTemp - temp2;

		Q1.y += quadOffsetFloor;
		Q2.y += quadOffsetFloor;
		Q3.y += quadOffsetFloor;
		Q4.y += quadOffsetFloor;
	 
		glTexCoord2f(1.0f*(1.0/float(texWidth)*w)+(1.0/float(texWidth)*u), 1.0f*(1.0/float(texHeight)*h)+(1.0/float(texHeight)*v));
		glVertex3f(Q3.x,Q3.y,Q3.z);

		glTexCoord2f(1.0f*(1.0/float(texWidth)*w)+(1.0/float(texWidth)*u), 0.0f*(1.0/float(texHeight)*h)+(1.0/float(texHeight)*v));
		glVertex3f(Q2.x,Q2.y,Q2.z);

		glTexCoord2f(0.0f*(1.0/float(texWidth)*w)+(1.0/float(texWidth)*u), 0.0f*(1.0/float(texHeight)*h)+(1.0/float(texHeight)*v));
		glVertex3f(Q1.x,Q1.y,Q1.z);

		glTexCoord2f(0.0f*(1.0/float(texWidth)*w)+(1.0/float(texWidth)*u), 1.0f*(1.0/float(texHeight)*h)+(1.0/float(texHeight)*v));
		glVertex3f(Q4.x,Q4.y,Q4.z);
	glEnd();
	
	
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);
	if( lighting_enabled ) glEnable( GL_LIGHTING );
}