#ifndef SZEVENTBASE_H
#define SZEVENTBASE_H

//#include "SZ_Config.h"
#include "SZ_PathfindingList.h"
//#include "SZ_World.h"
//#include "general_utils.h"

class SZ_FunctionBase
{
	public:
		 SZ_FunctionBase();
		~SZ_FunctionBase();

		int range;
		int currentCell;

		NxVec3 position;
		bool drawrange();	// do i also draw the range of this event?


		// used for drawing the logo that displays localities
		float quadShopWidth;	// width of logo
		float quadShopHeight;	// height of logo
		float quadOffsetFloor;	// Offset from floor
		NxVec3 quadA,quadB;		// Used for quad always facing camera.
		NxVec3 quadX, quadT;	// Used for constrained quad
		GLint quadBaseHeight, quadBaseWidth; 

		GLint texWidth,texHeight;
		int texArray;

		int row, index;				// used to store the row/index of this event relative to the bitmap file.
		int eventnumber;			// used to store the number of this event relative to the bitmap file.
		int timecost;				// stores the time required to fulfill the action on this cell (0=no time)

		int u,v;					// offset from the texture top left
		int w,h;					// width and height of the used texture
		int FCountMax;				// Count to this value for frame change
		int FCount;					// Frame Counter
		int NFrames;				// number of animation frames

		// used for the texture
		SZ_PathfindingList nList;	// list of all the nodes this event is in range		

		void *world;		// pointer to the world i belong to
		void init(int whichTile, void* world, int range);
		void drawQuad();		// draw the quad of this event
		void drawNeighbour();	// draw the neighbouring range tiles of this event
		void update();			// updates this event
		

};

#endif