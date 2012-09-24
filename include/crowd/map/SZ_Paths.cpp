// this file uses SZ_World.h however it includes the function that loads the paths/mapplaces from a bitmap file.

#include "..\SZ_World.h"

void SZ_World_Utils::loadPath()
{
	if (!bitmappath.read("textures/pathmap.bmp", false)) { printf("pathmap.bmp not loaded \n"); }	// load the map
	//bitmappath.printSelf();
	printf("...Path file loaded \n");
	
	for (int i=0;i<NODES_AMOUNT-1;i++)
	{
		for (int j=0;j<NODES_AMOUNT-1;j++)
		{
			if (gNodes[i]!=NULL)
			{
				NxVec3 cell =   NxVec3(bitmappath.getRGBXY(1,i,j),bitmappath.getRGBXY(2,i,j),bitmappath.getRGBXY(3,i,j));
				if ( !((cell.x == 0) && (cell.y == 255) && (cell.z==0)) )	//if a block if it's not 0 255 0
				{
					gNodes[i]->pathTo[j] = this->bitmappath.getRGBXY(1,i,j);
				}
			}
		}
	}
}

/*
							path.NodesRecord[i]->gNode->pathTo[k] = neighbournode;
							bitmappath.setRGBXY(neighbournode,0,0,j,k);
							*/