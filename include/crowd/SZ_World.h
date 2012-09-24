#ifndef SZWORLD_H
#define SZWORLD_H

#include "Bmp.h"
#include "..\..\general_utils.h"

#include "SZ_Config.h"
#include "SZ_Utils.h"
#include "SZ_Camera.h"
#include "SZ_Player.h"
#include "SZ_Person.h"
#include "SZ_ActorSprite.h"
#include "SZ_Sprite.h"
#include "SZ_Mouse.h"
#include "map/SZ_Node.h"
#include "map/SZ_NodeRecord.h"
#include "map/SZ_PathfindingList.h"
#include "map/SZ_FunctionBase.h"
#include "map/SZ_FunctionBaseList.h"
#include "map/SZ_WorldPersonList.h"
#include "map/SZ_ForceVectorList.h"
#include "group/SZ_GroupGG1.h"
#include "SZ_TimeKeeper.h"
#include "map/SZ_Schedule.h"
#include "group/SZ_UpdateManager.h"
#include "map/SZ_Logger.h"
#include <random>




class SZ_World_Utils
{
	public:
		 SZ_World_Utils();
		~SZ_World_Utils();
		void Init();
		
		void RenderPlane();	// renders a 1x1 plane
		void Render();      // used to render non specific stuff, i.e. for debugging purposes.
		void RenderNodes(); // renders all the nodes in the world.
		void RenderMap();	// renders the 2d bitmap file into a fine 3d world.
		void RenderGrid(int size, NxVec3 color, int linewidth);  // renders a grid with parameters

		void LoadTexBitmap(const char *nameoftex, int position_array, unsigned int whichArray[], bool alpha, Image::Bmp &bitmap);
		void MakePhysX();
		void MakeNodes();
		void loadPlaces_Icon();

		// textures general
		Image::Bmp bitmap;		            // this stores the map data
		Image::Bmp bitmappath;				// this stores the path map data.
		Image::Bmp bitmapplaces;			// stores all the places data
		Image::Bmp bitmapUI;				// stores the UI data.
		Image::Bmp bitmapplacesIcons;		// stores all the icons for places.
		Image::Bmp bitmapFont1;				// stores the first pixelated font. used for the board.
		Image::Bmp bitmapUserInterface;		// User Interface Bitmap

		//textures for characters
		Image::Bmp bitmapHeads;				// stores all the heads

		int iterations; // how long, in ticks, has this world been existing
		bool render;	// is the world being rendered?

		NxScene* wScene;
		NxPhysicsSDK* wSDK;
		NxActor* entity;
		unsigned int texture_array[99];		// array where all the textures are stored

		SZ_Node* gNodes[WORLD_SURFACE_5X5-1]; // one node for each 5x5 block cell. contains all the informations from the layers.
		
		// Keyboard and mouse stuff
		NxVec3 Mouse2dCoord;
		NxVec3 MouseWinCoord;
		int gKeys[MAX_KEYS];
		int gKeysSpecial[MAX_KEYS];
		int gMouseButton[MAX_KEYS];

		// Pathfinding stuff
		//SZ_PathfindingList pPathfindingList;
		SZ_PathfindingList open;
		SZ_PathfindingList closed;
		SZ_PathfindingList path;
		SZ_PathfindingList nList; // neighbourlist		
		SZ_PathfindingList pList; // short term path list

		// ForceVector list
		SZ_ForceVectorList vVectorList;	// list of vectors to draw

		// stuff from scene.cpp
		int scenecpp_timer;
		int scenecpp_timebase;
		int scenecpp_iterations;
		int scenecpp_frame;





		// Node sorting list
		SZ_PathfindingList gNodesListOrdered; // all nodes, nicely ordered from 0 to amount of nodes.

		void findPath(int end, int start);  // finds the SPP from tile START to tile END
		void makePath();					// calculate specified paths, store them in a file
		void loadPath();					// loads the paths from SZ_Paths.cpp
		void updatePath(int whichCell, SZ_PathfindingList *whichPath);     // updates all connections to a specific cell using the nodes from whichPath 
		void findNeighbour(int mDepth, int cDepth, SZ_Node *whichNode, SZ_PathfindingList *nList, SZ_ActorSprite *whichActorSprite, bool dotproduct);

		void findPathWeighted(int end, int start);  // finds the SPP from tile START to tile END, using the path layer, occupancy layer etc.
		void  findPathFrom(int mDepth, int cDepth, int end, SZ_Node *whichNode, SZ_PathfindingList *pList, SZ_ActorSprite *whichActorSprite);
		NxVec3 cellToPos(int whichCell);
		// FILE * stream4paths;				// save to file, used for.
		void loadPlaces();					// loads the places from the external bitmap file
};

class SZ_World
{
	public:
		SZ_World();	
		~SZ_World();
		void Init(NxScene* whichScene, NxPhysicsSDK* whichSDK);
		void Draw();					
		void Update();

		void fiveMinutesUpdateCycle();	// called every five minutes of simulated worldtime	
		void TimerFunc(unsigned int secs,  void (SZ_World::*func)() ); // start a function from this object every secs SIM seconds

		// Keyboard functions		
		void KeyboardDownCall(int key,int x,int y);
		void KeyboardUpCall(int key,int x,int y);
		void KeyboardSpecialDownCall(int key,int x,int y);
		void KeyboardSpecialUpCall(int key,int x,int y);	

		void processKeys();
		void sendKey(int key);
		void sendKeySpecial(int key);
		
		NxScene* wScene;
		NxPhysicsSDK* wSDK;

		SZ_World_Utils Utils;
		SZ_Player Player;
		
		SZ_Camera gCamera;						// the camera controlled by the player
		SZ_Mouse  gMouse;						// the mouse object
		SZ_FunctionBaseList service;			// stores all the places/events	

		SZ_TimeKeeper	gTimeKeeper;			// ticking time of this world.

		SZ_Logger gDatalogger;					// datalogger

		SZ_Schedule PlanesDepart;				// stores all the planes departing.
		SZ_Schedule	PlanesLanding;				// stores all the planes landing.

		float gDeltaTime;
		bool bPause;

		// SB: functions and parameters related to the spatial database approach
		int				 sbGetLayerPhysical(int whichNode);				
		NxVec3			 sbGetLayerNavigation(int whichNode, int targetNode);
		SZ_PersonList*	 sbGetLayerOccupancy(int whichNode);
		SZ_NumberList*	 sbGetLayerAreaSearch(SZ_Person* whichPerson);
		SZ_FunctionBase* sbGetLayerFunction(int whichNode);
	

		// PP: following variables take care of the world's population
		SZ_Person*  ppPersons[NUM_PERSON];				// pointer towards all the persons in the world
		int			ppAmount;							// amount of persons in this world
		bool		ppContains(SZ_Person* whichPerson);	// true if this list contains specified person
		SZ_Person*	ppFind(SZ_Person* whichPerson);	 
		void		ppAdd(SZ_Person* whichPerson);
		void		ppSubstract(SZ_Person* whichPerson);
		void		ppEmpty();
		int			ppLength();							// returns the amount of noderecords in this instance 
		SZ_Person*  ppSpawn(int whichCell);				// spawns a person on whichcell. Returns a pointer towards the person that has been created.
		SZ_UpdateManager ppUpdateManager;				// used as a debugging tool to update people's visiblity list sequentially.

		// GG: Functions and code related to Groups
		SZ_PersonList ggTmpListPerson;				// used to cycle through all people in the world.
		SZ_GroupGG1 ggGroups[MAX_GROUP_GG1];		// pointer towards all the groups (lists of people) within the world
		int ggGroupsAmount;							// amount of existing groups within the world			
		int	ggGroupsTotalCount[MAX_SOCIAL_LINKS];	// used to store throughout the runtime the frequency of groups

		
	

};

#endif