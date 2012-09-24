#include "SZ_World.h"
#include "windows.h"
#include "map/SZ_FunctionBase.h"
#include <random>



static float planecoords[] ={ 1.0, 0.0, 0.0,
							  0.0, 0.0, 0.0,
							  0.0, 0.0, 1.0,
							  1.0, 0.0, 1.0
};

static float planeNormalcoords[] ={ 0.0, -1.0, 0.0,
									0.0, -1.0, 0.0,
									0.0, -1.0, 0.0,
									0.0, -1.0, 0.0
};

static float modi = 1.0f;
static float planeTexcoords[] ={ 1.0/modi, 0.0,
								 0.0, 0.0,
								 0.0, 1.0/modi,
								 1.0/modi, 1.0/modi
};



void SZ_World_Utils::RenderNodes()
{
	for (int i=0;i<NODES_AMOUNT;i++)
	{
		gNodes[i]->draw();
	}	
}

void SZ_World_Utils::RenderMap()
{
	// uncomment for loading the bitmap every render frame
	//if (!bitmap.read("textures/floor.bmp")) { printf("not loaded \n");}  
	float var_height = 4.0f;	 //ori:5.0			//height modifier, size of the blocks+
	float var_size = 1;								//size modifier of the map

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
    //glEnable(GL_BLEND);

	//draw the walls of the map according to the bitmap file.
	glBegin(GL_QUADS);
	for (int j=0;j<=(WORLD_Z-1);j++)
		{
			for (int i=0;i<=(WORLD_X-1);i++)
			{
				{		
					int h = var_height;
					// RGB = XYZ
					NxVec3 cell =   NxVec3(bitmap.getRGBXY(1,i,j),bitmap.getRGBXY(2,i,j),bitmap.getRGBXY(3,i,j));

					if ( !((cell.x == 0) && (cell.y == 255) && (cell.z==0)) )	//only draw a block if it's not 0 255 0
					{
						// colours of my neighbouring cells, saved as NxVec3 RGB
						NxVec3 cell_right =   NxVec3(bitmap.getRGBXY(1,i+1,j  ),bitmap.getRGBXY(2,i+1,j  ),bitmap.getRGBXY(3,i+1,j  ));
						NxVec3 cell_bottom =  NxVec3(bitmap.getRGBXY(1,i  ,j+1),bitmap.getRGBXY(2,i  ,j+1),bitmap.getRGBXY(3,i  ,j+1));
						NxVec3 cell_left =    NxVec3(bitmap.getRGBXY(1,i-1,j  ),bitmap.getRGBXY(2,i-1,j  ),bitmap.getRGBXY(3,i-1,j  ));
						NxVec3 cell_top =     NxVec3(bitmap.getRGBXY(1,i,j-1  ),bitmap.getRGBXY(2,i,j-1  ),bitmap.getRGBXY(3,i,j-1  ));
						// define what sort of block i am
						bool TOP_LEFT		=false;
						bool TOP_RIGHT		=false;
						bool BOTTOM_LEFT	=false;	
						bool BOTTOM_RIGHT	=false;
						bool NORMAL_BLOCK	=false;

						if ( ((cell_right.y == 255) && (cell_bottom.y==255))) { BOTTOM_RIGHT = true; }  //     _|
						if ( ((cell_left.y == 255) && (cell_bottom.y==255))) {	BOTTOM_LEFT = true; }   //  |_
						if ( ((cell_top.y == 255) && (cell_left.y==255))   ) {  TOP_LEFT = true; }      //     ¯|
						if ( ((cell_top.y == 255) && (cell_right.y==255))  ) {  TOP_RIGHT = true; }     //  |¯
						if ( (TOP_LEFT + TOP_RIGHT + BOTTOM_LEFT + BOTTOM_RIGHT) == 0 ) { NORMAL_BLOCK = true; } // normal block, a square
					
						glColor4f(cell.x/255.0,cell.y/255.0,cell.z/255.0,1);		//set me to the colour of the pixel in the bitmap

						// do a normal wall block. check neighbouring cells to remove any unrequired walls.
						if (NORMAL_BLOCK)
						{
							// ceiling
							glNormal3f(0,1,0);
							glVertex3f((i*var_size),h,(j*var_size));
							glVertex3f((i*var_size),h,(j*var_size)+var_size);
							glVertex3f((i*var_size)+var_size,h,(j*var_size)+var_size);
							glVertex3f((i*var_size)+var_size,h,(j*var_size));	
							
							// walls left
							if ( (cell_left.y == 255) )	//only make a wall if my next tile is low <|| (left)
							{
								glNormal3f(-1,0,0);
								glVertex3f((i*var_size),0,(j*var_size)+var_size);
								glVertex3f((i*var_size),h,(j*var_size)+var_size);
								glVertex3f((i*var_size),h,(j*var_size));
								glVertex3f((i*var_size),0,(j*var_size));	
							}
							// walls bottom
							if ( cell_bottom.y == 255 )	//only make a wall if my next tile is low V (bottom)
							{
								glNormal3f(0,0,1);
								glVertex3f((i*var_size),h,(j*var_size)+var_size);
								glVertex3f((i*var_size),0,(j*var_size)+var_size);
								glVertex3f((i*var_size)+var_size,0,(j*var_size)+var_size);
								glVertex3f((i*var_size)+var_size,h,(j*var_size)+var_size);
							}
							// walls right
							if ( cell_right.y == 255 )	//only make a wall if my next tile is low ||> (right)
							{
								glNormal3f(1,0,0);
								glVertex3f((i*var_size)+var_size,h,(j*var_size)+var_size);
								glVertex3f((i*var_size)+var_size,0,(j*var_size)+var_size);
								glVertex3f((i*var_size)+var_size,0,(j*var_size));
								glVertex3f((i*var_size)+var_size,h,(j*var_size));
							}
							//walls top
							if ( cell_top.y == 255 )	//only make a wall if my next tile is low M (top)
							{
								glNormal3f(0,0,-1);
								glVertex3f((i*var_size),0,(j*var_size));
								glVertex3f((i*var_size),h,(j*var_size));
								glVertex3f((i*var_size)+var_size,h,(j*var_size));
								glVertex3f((i*var_size)+var_size,0,(j*var_size));
							}
						}	// end of NORMAL_BLOCK
					
					// do the corners block
					if (BOTTOM_RIGHT) 
						{		
							// wall
							glNormal3f(0.5,0,1);
							glVertex3f((i*var_size)+var_size,0,(j*var_size));
							glVertex3f((i*var_size)+var_size,h,(j*var_size));
							glVertex3f((i*var_size),h,(j*var_size)+var_size);
							glVertex3f((i*var_size),0,(j*var_size)+var_size);
							// ceiling
							glNormal3f(0,1,0);
							glVertex3f((i*var_size),h,(j*var_size));
							glVertex3f((i*var_size),h,(j*var_size)+var_size);
							glVertex3f((i*var_size)+var_size,h,(j*var_size));
							glVertex3f((i*var_size)+var_size,h,(j*var_size));
						}

					if (BOTTOM_LEFT)
						{
							// wall
							glNormal3f(-1,0,1);
							glVertex3f((i*var_size),h,(j*var_size));
							glVertex3f((i*var_size),0,(j*var_size));
							glVertex3f((i*var_size)+var_size,0,(j*var_size)+var_size);
							glVertex3f((i*var_size)+var_size,h,(j*var_size)+var_size);
							// ceiling
							glNormal3f(0,1,0);
							glVertex3f((i*var_size),h,(j*var_size));
							glVertex3f((i*var_size)+var_size,h,(j*var_size)+var_size);
							glVertex3f((i*var_size)+var_size,h,(j*var_size));
							glVertex3f((i*var_size)+var_size,h,(j*var_size));
						}
				
					
					if (TOP_RIGHT) 
						{
							// wall
							glNormal3f(1,0,-1);
							glVertex3f((i*var_size),0,(j*var_size));
							glVertex3f((i*var_size),h,(j*var_size));
							glVertex3f((i*var_size)+var_size,h,(j*var_size)+var_size);
							glVertex3f((i*var_size)+var_size,0,(j*var_size)+var_size);
							// ceiling
							glNormal3f(0,1,0);
							glVertex3f((i*var_size),h,(j*var_size));
							glVertex3f((i*var_size),h,(j*var_size));
							glVertex3f((i*var_size),h,(j*var_size)+var_size);
							glVertex3f((i*var_size)+var_size,h,(j*var_size)+var_size);
						}
					
					if (TOP_LEFT)
						{
							// wall
							glNormal3f(0,0,-1);
							glVertex3f((i*var_size)+var_size,h,(j*var_size));
							glVertex3f((i*var_size)+var_size,0,(j*var_size));
							glVertex3f((i*var_size),0,(j*var_size)+var_size);
							glVertex3f((i*var_size),h,(j*var_size)+var_size);
							// ceiling
							glNormal3f(0,1,0);
							glVertex3f((i*var_size),h,(j*var_size)+var_size);
							glVertex3f((i*var_size),h,(j*var_size)+var_size);
							glVertex3f((i*var_size)+var_size,h,(j*var_size)+var_size);
							glVertex3f((i*var_size)+var_size,h,(j*var_size));	
						}
			
					}// end of (if not green) section
				}
			}
		}
	glEnd();	

	//glDisable(GL_BLEND);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
}


void SZ_World_Utils::updatePath(int whichCell, SZ_PathfindingList *whichPath)
{	
	// draws the tiles which are in the path recordlist.
	for (int i=0; i<whichPath->length(); i++)
	{	
		if (whichPath->NodesRecord[i]!=NULL)
		{	
			int neighbournode;
			neighbournode = whichPath->NodesRecord[i]->gNode->whichConnection(path.NodesRecord[i]->node_number); //				
			whichPath->NodesRecord[i]->gNode->pathTo[whichCell] = neighbournode;			
		}
	}	
}

void SZ_World_Utils::makePath()
{
	/*
	for (int i=0;i<NODES_AMOUNT;i++)
	{
		if (gNodes[i]!=NULL)
		{
			gNodes[i]->pathTo[337] = 2;
		}
	}
*/
/*		
	for (int j=0;j<NODES_AMOUNT;j++)
	{		
		if (gNodes[j]!=NULL)
			{
		
			findPath(337,j);
			for (int i=0; i<path.length(); i++)
			{	
				if (path.NodesRecord[i]!=NULL)
				{
					
					int neighbournode;
					neighbournode = path.NodesRecord[i]->gNode->whichConnection(path.NodesRecord[i]->node_number); 

					path.NodesRecord[i]->gNode->pathTo[337] = neighbournode;
					//path.NodesRecord[i]->gNode->pathTo[535] = path.NodesRecord[i]->connection;
				}
			}
			
		}
		
	}*/

	//for (int i=0;i<

	for (int j=0;j<NODES_AMOUNT;j++)
	{		
		if (gNodes[j]!=NULL)
			{
			
			for (int k=0;k<NODES_AMOUNT;k++)		// originall k = 0
			{
				//int k=41;
				if (gNodes[k]!=NULL)
				{
					//printf("attempting findpath %i %i - done - \n",k,j);
					findPath(k,j);
					//printf("findpath %i %i - done - \n",k,j);
					for (int i=0; i<path.length(); i++)
					{	
						if (path.NodesRecord[i]!=NULL)
						{		

							int neighbournode;
							neighbournode = path.NodesRecord[i]->gNode->whichConnection(path.NodesRecord[i]->node_number); 

							path.NodesRecord[i]->gNode->pathTo[k] = neighbournode;
							bitmappath.setRGBXY(neighbournode,0,0,path.NodesRecord[i]->gNode->number,k);

							//printf("n %i, j %i, k %i \n", neighbournode,path.NodesRecord[i]->gNode->number,k);
							// J/X = node K/Y = pathTo 
				
			
							//path.NodesRecord[i]->gNode->pathTo[535] = path.NodesRecord[i]->connection;
						}
					}
				}
			}
			
		}
		printf("%i out of %i done \n",j,NODES_AMOUNT);
		
	}

	bitmappath.save("textures/pathmap.bmp",1200,1200,3,bitmappath.dataRGB);
	
/*
		// used to store the information for the pathfinding nodes
	errno_t err; 
	if( (err = fopen_s( &stream4paths, "include\\crowd\\map\\SZ_Paths.cpp", "w+" )) != 0 )
		printf( "SZ_Paths.h: The file was not opened\n" ); 
	else       printf( "SZ_Paths.h: The file was opened\n" );

	
	fprintf( stream4paths," #include \"SZ_World.h\" \n");
	fprintf( stream4paths,"void SZ_World_Utils::loadPath() \n");
	fprintf( stream4paths,"{ \n");
	
	



	for (int i=0;i<NODES_AMOUNT;i++)
	{
		if (gNodes[i]!=NULL)
		{
			//for (int k=0;k<100;k++)
			{
				int k = 337;
				if (gNodes[k]!=NULL)
				{
					//printf("gNodes[%i]->pathTo[%i]=%i \n",i,k, gNodes[i]->pathTo[337]);
					fprintf(stream4paths,"    gNodes[%i]->pathTo[%i]=%i; \n",i,k, gNodes[i]->pathTo[k]);
				}
			}
		}
	}
	
	

	fprintf( stream4paths,"} \n");
	
	fclose (stream4paths);
	*/
	
	

	
	
	
/*
	findPath(535,41);
	for (int i=0; i<path.length(); i++)
	{	
		if (path.NodesRecord[i]!=NULL)
		{
			int neighbournode;
			neighbournode = path.NodesRecord[i]->gNode->whichConnection(path.NodesRecord[i]->node_number);
			path.NodesRecord[i]->gNode->pathTo[535] = neighbournode;
		}
	}
	*/
	
}

void SZ_World_Utils::Render()
{
	// draws the tiles which are in the path recordlist.
	//glEnable(GL_BLEND);
	//glDisable(GL_LIGHTING);


	// draw the cells from pList (cycles through the nodes to find existing paths)
/*
	for (int i=0; i<this->pList.length(); i++)
	{	
		if (pList.NodesRecord[i]!=NULL)
		{
			int nodetmp = pList.NodesRecord[i]->gNode->number;

			//transform node number to XY (XZ) coordindates
			int X = (nodetmp % WORLD_X_5X5);	   // transform the tile value into the top left X corner
			int Z = (nodetmp-X)/WORLD_X_5X5;    //  transform the tile value into the top left Z corner
			
			glColor4f(1,0,0,0.1);
			glPushMatrix();
			glTranslatef(0,0.05,0);	
			
			glLineWidth(10);
			glColor4f(1,0,0,0.8);
			glTranslatef(0,0.05,0);
				glBegin(GL_POLYGON);
					glVertex3f(X*WORLD_TILE+WORLD_TILE,0,Z*WORLD_TILE);
					glVertex3f(X*WORLD_TILE,0,Z*WORLD_TILE);
					glVertex3f(X*WORLD_TILE,0,Z*WORLD_TILE+WORLD_TILE);
					glVertex3f(X*WORLD_TILE+WORLD_TILE,0,Z*WORLD_TILE+WORLD_TILE);
				glEnd();
			glEnd();
			glPopMatrix();

		}
	}
*/
	
	

/*
	// draws the tiles which are in the path recordlist.
	for (int i=0; i<path.length(); i++)
	{	
		if (path.NodesRecord[i]!=NULL)
		{
			int nodetmp = path.NodesRecord[i]->gNode->number;
			//transform node number to XY (XZ) coordindates
			int ex2 = (nodetmp % WORLD_X_5X5);	// transform the tile value into the top left X corner
			int ez2 = (nodetmp-ex2)/WORLD_X_5X5;   // transform the tile value into the top left Y corner

			glColor4f(1,0,0,0.1);

			glPushMatrix();
			glTranslatef(0,0.05,0);
	

			int neighbournode;
			neighbournode = path.NodesRecord[i]->gNode->whichConnection(path.NodesRecord[i]->node_number); //


			 
			//path.NodesRecord[i]->gNode->pathTo[k] = neighbournode;
			
			glLineWidth(10);
			glColor4f(1,0,0,0.8);
			glTranslatef(0,0.05,0);
			glBegin(GL_LINES);
				glVertex3f(ex2*WORLD_TILE	+WORLD_TILE	/2.0,0,ez2*WORLD_TILE	+WORLD_TILE	/2.0);
				glVertex3f(
					(ex2*WORLD_TILE+WORLD_TILE	/2.0)+gNodes[nodetmp]->connection[neighbournode]->cost.x,
					0,
					(ez2*WORLD_TILE+WORLD_TILE/2.0)+  gNodes[nodetmp]->connection[neighbournode]->cost.z)
					;
				//gNodes[current->connection]
				//current = closed.find(gNodes[current->connection]);
			glEnd();
			glLineWidth(1);
			

			glPopMatrix();
		}
	}

*/
}

/*-----------------------------------------------------------
  SZ_World_Utils::MakePhysX(): takes the bitmap file and creates
  PhysX blocks.
  -----------------------------------------------------------*/

void SZ_World_Utils::MakePhysX(){

	// store the loaded bitmap file in a temp variable as we are going to modify the currently loaded file.
	Image::Bmp bitmaptmp;
	bitmaptmp = bitmap;

	for (int j=0;j<=(WORLD_Z-1);j+=5)
		{
			for (int i=0;i<=(WORLD_X-1);i+=5)
			{
				{	
					NxVec3 cell =   NxVec3(bitmap.getRGBXY(1,i,j),bitmap.getRGBXY(2,i,j),bitmap.getRGBXY(3,i,j));
					if ( !((cell.x == 0) && (cell.y == 255) && (cell.z==0)) )	//if a block if it's not 0 255 0
						{

						bool green = false;
									
						// tmpy returns the total length in pixels of a Y line. therefore if devided per 5, it
						// the amount of 5x5 block in a Y line. 
						// i.e Y/5 returns 3, therefore the current line has got 3 blocks in Y.

						green = false;
						int tmpy = 0;
						while ( ( (green == false) && (tmpy+j<bitmap.getHeight())) )
							{
								

								NxVec3 tmp_cell =   NxVec3(bitmap.getRGBXY(1,i,j+tmpy),bitmap.getRGBXY(2,i,j+tmpy),bitmap.getRGBXY(3,i,j+tmpy));

								if ( ((tmp_cell.x == 0) && (tmp_cell.y == 255) && (tmp_cell.z==0))  )	
								{
									green = true;
								} 
								

								if ( !((tmp_cell.x == 0) && (tmp_cell.y == 255) && (tmp_cell.z==0)) )	
								{
										bitmap.setRGBXY(0,255,0,i,j+tmpy);

								}

								tmpy++;					
							}

					
						//now check the next line to the current one.						
						green = false;
						int tmpy2 = 0;
						while ( ( (green == false) && (tmpy2+j<bitmap.getHeight())) && (i+5<bitmap.getWidth()) )
							{
								

								NxVec3 tmp_cell =   NxVec3(bitmap.getRGBXY(1,i+5,j+tmpy2),bitmap.getRGBXY(2,i+5,j+tmpy2),bitmap.getRGBXY(3,i+5,j+tmpy2));

								if ( ((tmp_cell.x == 0) && (tmp_cell.y == 255) && (tmp_cell.z==0))  )	
								{
									green = true;
								} 
								
								
								if ( !((tmp_cell.x == 0) && (tmp_cell.y == 255) && (tmp_cell.z==0)) )	
								{
										bitmap.setRGBXY(0,255,0,i+5,j+tmpy2);
			
								}

								tmpy2++;					
							}

						//if the current line is as long as the next line, that means a block can be created.
						//otherwise 
					
						if (tmpy == tmpy2)
						{
							NxActorDesc actorDesc;
							NxBoxShapeDesc boxDesc;
							boxDesc.dimensions = NxVec3(2.5*2,2.5,2.5*(tmpy/5));
							boxDesc.localPose.t = NxVec3(i+2.5+2.5,2.5,j+(tmpy/5)*2.5);
								boxDesc.mass = 1000.0;
							actorDesc.shapes.pushBack(&boxDesc);
							wScene->createActor(actorDesc);

						} else 
													
							{	//if there's no 2X block, create one line, then the next line.					
								{	
									NxActorDesc actorDesc;
									NxBoxShapeDesc boxDesc;
									boxDesc.dimensions = NxVec3(2.5,2.5,2.5*(tmpy/5));
									boxDesc.localPose.t = NxVec3(i+2.5,2.5,j+(tmpy/5)*2.5);
										boxDesc.mass = 1000.0;
									actorDesc.shapes.pushBack(&boxDesc);
									wScene->createActor(actorDesc);
								}
								if (tmpy2>1)
								{
									NxActorDesc actorDesc;
									NxBoxShapeDesc boxDesc;
									boxDesc.dimensions = NxVec3(2.5,2.5,2.5*(tmpy2/5));
									boxDesc.localPose.t = NxVec3(i+7.5,2.5,j+(tmpy2/5)*2.5);
									boxDesc.mass = 1000.0;
									actorDesc.shapes.pushBack(&boxDesc);
									wScene->createActor(actorDesc);
								}
							  i=i+5;			//ignore the next line, as it has already been made.
							}
							
					}
				}
			}
	}
	bitmap = bitmaptmp; // restore the bitmap file.


	{
		NxVec3 position = NxVec3(WORLD_X/2.0,2.5,-2);
		NxVec3 size		= NxVec3(WORLD_X/2.0,2.5,2);
		NxActorDesc actorDesc;
		NxBoxShapeDesc boxDesc;	
		boxDesc.localPose.t = NxVec3(0,0,0);
		boxDesc.dimensions.set(size);		
		actorDesc.shapes.pushBack(&boxDesc);
		actorDesc.globalPose.t = position;	
		wScene->createActor(actorDesc);
	}
	{
		NxVec3 position = NxVec3(WORLD_X/2.0,2.5,+2+WORLD_Z);
		NxVec3 size		= NxVec3(WORLD_X/2.0,2.5,2);
		NxActorDesc actorDesc;
		NxBoxShapeDesc boxDesc;	
		boxDesc.localPose.t = NxVec3(0,0,0);
		boxDesc.dimensions.set(size);		
		actorDesc.shapes.pushBack(&boxDesc);
		actorDesc.globalPose.t = position;	
		wScene->createActor(actorDesc);
	}

	{
		NxVec3 position = NxVec3(-2,2.5,WORLD_Z/2.0);
		NxVec3 size		= NxVec3(2,2.5,WORLD_Z/2.0);
		NxActorDesc actorDesc;
		NxBoxShapeDesc boxDesc;	
		boxDesc.localPose.t = NxVec3(0,0,0);
		boxDesc.dimensions.set(size);		
		actorDesc.shapes.pushBack(&boxDesc);
		actorDesc.globalPose.t = position;	
		wScene->createActor(actorDesc);
	}

	{
		NxVec3 position = NxVec3(WORLD_X+2,2.5,WORLD_Z/2.0);
		NxVec3 size		= NxVec3(2,2.5,WORLD_Z/2.0);
		NxActorDesc actorDesc;
		NxBoxShapeDesc boxDesc;	
		boxDesc.localPose.t = NxVec3(0,0,0);
		boxDesc.dimensions.set(size);		
		actorDesc.shapes.pushBack(&boxDesc);
		actorDesc.globalPose.t = position;	
		wScene->createActor(actorDesc);
	}



}

void SZ_World_Utils::MakeNodes()
{
	// first pass: make all the nodes in the world
	for (int j=0;j<(WORLD_Z_5X5);j++)
		{
			for (int i=0;i<(WORLD_X_5X5);i++)
			{
					int nodeatXY =   (WORLD_X_5X5 * (j)) + (i);		// cycle through each of the 1200 nodes (0-1199). nodeatXY=current node
					
					NxVec3 cell =   NxVec3(bitmap.getRGBXY(1,i*5,j*5),bitmap.getRGBXY(2,i*5,j*5),bitmap.getRGBXY(3,i*5,j*5));					
				
					// if the cell (the first pixel of a block of five) is empty, that means it's a cell on which people can walk (floor)
					// therefore, we add a node there.
					if ( ((cell.x == 0) && (cell.y == 255) && (cell.z==0)) )	//only put a node if it's a walkable area
					{
						gNodes[nodeatXY] = new SZ_Node();
						gNodes[nodeatXY]->number = nodeatXY;					


					} else { 
						gNodes[nodeatXY] = NULL;  // Otherwise this node is null 
							} 
			}
		}



	// second pass: connect all the nodes in the world with each other
	for (int j=0;j<(WORLD_Z_5X5);j++)
		{
			for (int i=0;i<(WORLD_X_5X5);i++)
			{
					int nodeatXY =   (WORLD_X_5X5 * (j)) + (i);		// cycle through each of the 1200 nodes (0-1199). nodeatXY=curretn node
					NxVec3 cell =   NxVec3(bitmap.getRGBXY(1,i*5,j*5),bitmap.getRGBXY(2,i*5,j*5),bitmap.getRGBXY(3,i*5,j*5));
					
					NxVec3 temp = NxVec3(1,(i*5),(j*5));

					// if the cell (the first pixel of a block of five) is empty, that means it's a cell on which people can walk (floor)
					// therefore, we add a node there.
					if ( ((cell.x == 0) && (cell.y == 255) && (cell.z==0)) )	//only draw a block if it's not 0 255 0
					{

						//gNodesCounter[tmp_counter] = nodeatXY;
						//printf("gonodes%i %i \n",tmp_counter,gNodesCounter[tmp_counter]);
						//tmp_counter++;						
			
						//cells neigbouring the current cell
						NxVec3 cell_around[8];

						temp =  NxVec3(1,(i*5),(j*5)) + NxVec3(0,-5,0);
						cell_around[0]  =  NxVec3(bitmap.getRGBXY(temp.x,temp.y,temp.z),bitmap.getRGBXY(temp.x+1,temp.y,temp.z),bitmap.getRGBXY(temp.x+2,temp.y,temp.z));	//W
						if (((cell_around[0].x == 0) && (cell_around[0].y == 255) && (cell_around[0].z==0)) )
						{	
							int temp2 = (WORLD_X_5X5 * (j-1)) + (i);	
							gNodes[nodeatXY]->connection[0]->toNodeAddress = gNodes[nodeatXY-1];
							gNodes[nodeatXY]->connection[0]->cost = NxVec3(-5,0,0);
						}

						temp = NxVec3(1,(i*5),(j*5))  + NxVec3(0,-5,5);
						cell_around[1] =  NxVec3(bitmap.getRGBXY(temp.x,temp.y,temp.z),bitmap.getRGBXY(temp.x+1,temp.y,temp.z),bitmap.getRGBXY(temp.x+2,temp.y,temp.z));	//SW
						if (((cell_around[1].x == 0) && (cell_around[1].y == 255) && (cell_around[1].z==0)) )
						{	
							int temp2 = (WORLD_X_5X5 * (j-1)) + (i+1);	
							gNodes[nodeatXY]->connection[1]->toNodeAddress = gNodes[nodeatXY+39];
							gNodes[nodeatXY]->connection[1]->cost = NxVec3(-5,0,5);
						}


						temp = NxVec3(1,(i*5),(j*5)) + NxVec3(0,0,5);
						cell_around[2]  =  NxVec3(bitmap.getRGBXY(temp.x,temp.y,temp.z),bitmap.getRGBXY(temp.x+1,temp.y,temp.z),bitmap.getRGBXY(temp.x+2,temp.y,temp.z));	//S
						if (((cell_around[2].x == 0) && (cell_around[2].y == 255) && (cell_around[2].z==0)) )
						{	
							int temp2 = (WORLD_X_5X5 * (j)) + (i+1);	
							gNodes[nodeatXY]->connection[2]->toNodeAddress =  gNodes[nodeatXY+40];
							gNodes[nodeatXY]->connection[2]->cost = NxVec3(0,0,5);
						}


						temp = NxVec3(1,(i*5),(j*5))  + NxVec3(0,5,5);
						cell_around[3] =  NxVec3(bitmap.getRGBXY(temp.x,temp.y,temp.z),bitmap.getRGBXY(temp.x+1,temp.y,temp.z),bitmap.getRGBXY(temp.x+2,temp.y,temp.z));	//SE
						if (((cell_around[3].x == 0) && (cell_around[3].y == 255) && (cell_around[3].z==0)) )
						{	
							int temp2 = (WORLD_X_5X5 * (j+1)) + (i+1);	
							gNodes[nodeatXY]->connection[3]->toNodeAddress =  gNodes[nodeatXY+41];
							gNodes[nodeatXY]->connection[3]->cost =  NxVec3(5,0,5);
						}

						temp = NxVec3(1,(i*5),(j*5))  + NxVec3(0,5,0);
						cell_around[4]  =  NxVec3(bitmap.getRGBXY(temp.x,temp.y,temp.z),bitmap.getRGBXY(temp.x+1,temp.y,temp.z),bitmap.getRGBXY(temp.x+2,temp.y,temp.z));	//E
						if (((cell_around[4].x == 0) && (cell_around[4].y == 255) && (cell_around[4].z==0)) )
						{	
							int temp2 = (WORLD_X_5X5 * (j+1)) + (i);	
							gNodes[nodeatXY]->connection[4]->toNodeAddress =  gNodes[nodeatXY+1];
							gNodes[nodeatXY]->connection[4]->cost =  NxVec3(5,0,0);
						}

						temp = NxVec3(1,(i*5),(j*5))  + NxVec3(0,5,-5);
						cell_around[5] =  NxVec3(bitmap.getRGBXY(temp.x,temp.y,temp.z),bitmap.getRGBXY(temp.x+1,temp.y,temp.z),bitmap.getRGBXY(temp.x+2,temp.y,temp.z));	//NE
						if (((cell_around[5].x == 0) && (cell_around[5].y == 255) && (cell_around[5].z==0)) )
						{	
							int temp2 = (WORLD_X_5X5 * (j+1)) + (i-1);	
							gNodes[nodeatXY]->connection[5]->toNodeAddress =  gNodes[nodeatXY-39];
							gNodes[nodeatXY]->connection[5]->cost =  NxVec3(5,0,-5);
						}
						
						temp = NxVec3(1,(i*5),(j*5))  + NxVec3(0,0,-5);
						cell_around[6]  =  NxVec3(bitmap.getRGBXY(temp.x,temp.y,temp.z),bitmap.getRGBXY(temp.x+1,temp.y,temp.z),bitmap.getRGBXY(temp.x+2,temp.y,temp.z));	//N
						if (((cell_around[6].x == 0) && (cell_around[6].y == 255) && (cell_around[6].z==0)) )
						{	
							int temp2 = (WORLD_X_5X5 * (j)) + (i-1);	
							gNodes[nodeatXY]->connection[6]->toNodeAddress =  gNodes[nodeatXY-40];
							gNodes[nodeatXY]->connection[6]->cost =  NxVec3(0,0,-5);
						}						

						temp = NxVec3(1,(i*5),(j*5))  + NxVec3(0,-5,-5);
						cell_around[7] =  NxVec3(bitmap.getRGBXY(temp.x,temp.y,temp.z),bitmap.getRGBXY(temp.x+1,temp.y,temp.z),bitmap.getRGBXY(temp.x+2,temp.y,temp.z));	//NW
						if (((cell_around[7].x == 0) && (cell_around[7].y == 255) && (cell_around[7].z==0)) )
						{	
							int temp2 = (WORLD_X_5X5 * (j-1)) + (i-1);	
							gNodes[nodeatXY]->connection[7]->toNodeAddress = gNodes[nodeatXY-41];
							gNodes[nodeatXY]->connection[7]->cost =  NxVec3(-5,0,-5);
						}
						
						// cost modifier						
						for (int l=0;l<8;l++)
						{
							gNodes[nodeatXY]->connection[l]->cost *=0.45;
						}

						// remove undesired diagonals.

						
						if ( gNodes[nodeatXY]->connection[6]->toNodeAddress==NULL )
						{	
							gNodes[nodeatXY]->connection[7]->toNodeAddress = NULL;
							gNodes[nodeatXY]->connection[7]->cost = NxVec3(0,0,0);
							gNodes[nodeatXY]->connection[5]->toNodeAddress = NULL;
							gNodes[nodeatXY]->connection[5]->cost = NxVec3(0,0,0);
						}

						if ( gNodes[nodeatXY]->connection[4]->toNodeAddress==NULL)
						{	
							gNodes[nodeatXY]->connection[5]->toNodeAddress = NULL;
							gNodes[nodeatXY]->connection[5]->cost = NxVec3(0,0,0);
							gNodes[nodeatXY]->connection[3]->toNodeAddress = NULL;
							gNodes[nodeatXY]->connection[3]->cost = NxVec3(0,0,0);
						}
					
						if ( gNodes[nodeatXY]->connection[2]->toNodeAddress==NULL )
						{	
							gNodes[nodeatXY]->connection[1]->toNodeAddress = NULL;
							gNodes[nodeatXY]->connection[1]->cost = NxVec3(0,0,0);
							gNodes[nodeatXY]->connection[3]->toNodeAddress = NULL;
							gNodes[nodeatXY]->connection[3]->cost = NxVec3(0,0,0);
						}
						

						if ( gNodes[nodeatXY]->connection[0]->toNodeAddress==NULL )
						{	
							gNodes[nodeatXY]->connection[7]->toNodeAddress = NULL;
							gNodes[nodeatXY]->connection[7]->cost = NxVec3(0,0,0);
							gNodes[nodeatXY]->connection[1]->toNodeAddress = NULL;
							gNodes[nodeatXY]->connection[1]->cost = NxVec3(0,0,0);
						}
						

						

					} else { gNodes[nodeatXY] = NULL; } //otherwise this node is null
			}
	}


	
}

SZ_World_Utils::SZ_World_Utils()
{
	render = RENDER;
}


SZ_World_Utils::~SZ_World_Utils()
{
	//printf("count : %i \n",SZ_Node::nodeIndexCnt);
	// delete all the existing nodes.
	for (int i=0;i<SZ_Node::nodeIndexCnt;i++)
	{
		if (gNodes[i]!=NULL)
		delete gNodes[i];
		gNodes[i]=NULL;
	}
	//delete []gNodesCounter;



}

void SZ_World_Utils::Init()
{

}


void SZ_World_Utils::RenderGrid(int size, NxVec3 color, int linewidth)
{
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LIGHTING);

	glColor4f(color.x,color.y,color.z,0.3);

	glLineWidth(linewidth);

	glPushMatrix();
	glRotatef(90,1,0,0);
	for (int i=0;i<=WORLD_X/size;i++)						//draw the cols
	{
		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,WORLD_Z,0);
		glEnd();
		glTranslatef(size,0,0);
	}

	glPopMatrix();
	glPushMatrix();
	glRotatef(90,1,0,0);
	for (int i=0;i<=WORLD_Z/size;i++)						//draw the rows
	{
		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(WORLD_X,0,0);
		glEnd();
		glTranslatef(0,size,0);
	}
	glPopMatrix();

	//glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
}


void SZ_World_Utils::RenderPlane()
{
	glDisable(GL_LIGHTING);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();

	glColor4f(0,0,0.5,0.1);

	glScalef(WORLD_X,1,WORLD_Z);

	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), planecoords);
	glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat), planeNormalcoords);
	//glTexCoordPointer(2,GL_FLOAT, 2*sizeof(float),planeTexcoords);

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texture_array[0]);
	glDrawArrays(GL_QUADS, 0, 4);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
}


void SZ_World_Utils::LoadTexBitmap(const char *nameoftex, int position_array, unsigned int whichArray[], bool alpha, Image::Bmp &bitmap)
{
	if (alpha)
	{
		//Image::Bmp bitmap;
		if (!bitmap.read(nameoftex, true)) { printf("not loaded \n"); } 
		//bitmap.printSelf();
		
		
		glGenTextures(1, &whichArray[position_array]);	
		
		glBindTexture(GL_TEXTURE_2D,   whichArray[position_array]);



		GLfloat border_color[]={1,1,1,1};
		GLfloat env_color[]={1,1,0,1};
		glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,border_color);
		glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,env_color);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
		
			
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST  ); 
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_NEAREST);
		
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		
		
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, bitmap.getWidth(), bitmap.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.dataRGBA);
		
		
	}
	
	if (!alpha)
	{
		//Image::Bmp bitmap;
		if (!bitmap.read(nameoftex)) { printf("not loaded \n");} 
		//bitmap.printSelf();
		glGenTextures(1, &whichArray[position_array]);	
		glBindTexture(GL_TEXTURE_2D,   whichArray[position_array]);
		

		GLfloat border_color[]={1,1,1,1};
		GLfloat env_color[]={1,1,0,1};
		glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,border_color);
		glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,env_color);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);



		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST  ); 
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_NEAREST);
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, bitmap.getWidth(), bitmap.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.dataRGB);
		
	}

}

SZ_World::SZ_World()
{
}

SZ_World::~SZ_World()
{
	for (int i=0;i<NUM_PERSON;i++)
	{
		if (ppPersons[i]!=NULL)
		{
			delete ppPersons[i];
			ppPersons[i] = NULL;
		}
	}
	Utils.nList.empty();
	Utils.pList.empty();

}


void SZ_World::fiveMinutesUpdateCycle()
{
	//printf("5 minutes update \n");
	PlanesDepart.update();
	PlanesLanding.update();
}



// updates the world. use this function to update the behaviour of all entities such as persons
void SZ_World::Update()
{
	//printf("updatecycle - ");
	//printf("ppiteration %i \n", ppUpdateManager.ppIteration);
	ppUpdateManager.ppPointer = NULL;

	if (ppUpdateManager.ppPointer != NULL)
	{

		//Persons[ppUpdateManager.ppIteration]->behaviour_actions();
		//Persons[ppUpdateManager.ppIteration]->behaviour_navigation();
	}

	
	Utils.vVectorList.empty(); // delete all the objects of drawing lines
	
	Utils.iterations++;

	//printf("camera %i \n", gCamera.RecticleCell);

	for (int i=0;i<ppAmount;i++)
	{
		if (ppPersons[i] == NULL) {continue;}

		if (ppUpdateManager.ppPointer == NULL)
		{
			//if (Utils.iterations % 10 == 0)
			{
				ppUpdateManager.ppPointer   = ppPersons[ppUpdateManager.ppIteration];
				//printf("ppiteration %i \n", ppUpdateManager.ppIteration);
				if (ppUpdateManager.ppPointer != NULL)
				{	
					ppUpdateManager.ppPointer->SZ_ActorSprite::changeTile();
				}
				
				ppUpdateManager.ppIteration += 1;
				if (ppUpdateManager.ppIteration >= ppAmount)
				{
					//printf("set back to null \n");
					ppUpdateManager.ppPointer   = NULL;
					ppUpdateManager.ppIteration = 0;
				}
			}

 		}

		//if (i == 139) {continue;} 
		//if (i == 140) {continue;}
		//if (i == 141) {
			//printf("currentcell %i pos %f %f %s \n", Persons[141]->currentCell, Persons[141]->getPosition().x, Persons[141]->getPosition().z, Persons[i]->actions.modeName());
			//this->Utils.vVectorList.newVector(Persons[141]->getPosition(),NxVec3(0,30,0),NxVec3(1,0,0));
			//ppSubstract(Persons[141]);
			//continue;
		//}
		ppPersons[i]->behaviour_actions();
		//if (i == 141) { printf("after action \n"); }
		ppPersons[i]->behaviour_navigation();
		//if (i == 141) { printf("after navi \n"); }
		


#if 0
		if (Persons[i]->currentCell != Persons[i]->actions.aim_targetCell)
		{
			if (Utils.gNodes[Persons[i]->currentCell]!=NULL)
			{	
			NxVec3 force;
			NxVec3 repulsion;	
			force = NxVec3(0,0,0);



			// force to go towards the desired cell		
			if (Utils.gNodes[Persons[i]->currentCell]->connection[Utils.gNodes[Persons[i]->currentCell]->pathTo[Persons[i]->actions.aim_targetCell]]!=NULL)
			{
				force += sbGetLayerNavigation(Persons[i]->currentCell,Persons[i]->actions.aim_targetCell);
				//force += Utils.gNodes[Persons[i]->currentCell]->connection[Utils.gNodes[Persons[i]->currentCell]->pathTo[Persons[i]->actions.aim_targetCell]]->cost;	
			}

			#if 0  // follow the leader			
			if (((SZ_MscGroup*)(Persons[i]->mscGroup))->PersonRecord[0] != Persons[i])
				{
					Persons[i]->actions.aim_targetCell = ((SZ_MscGroup*)(Persons[i]->mscGroup))->PersonRecord[0]->currentCell;
					if (Persons[i]->currentCell == ((SZ_MscGroup*)(Persons[i]->mscGroup))->PersonRecord[0]->currentCell)
					{
						repulsion +=  ( Persons[i]->lookAt(((SZ_MscGroup*)(Persons[i]->mscGroup))->PersonRecord[0]->getPosition() )) / (Persons[i]->getPosition() - ((SZ_MscGroup*)(Persons[i]->mscGroup))->PersonRecord[0]->getPosition() ).normalize();
						//force = Persons[i]->lookAt(((SZ_MscGroup*)(Persons[i]->mscGroup))->PersonRecord[0]->getPosition());
					}
				}
			#endif // end follow the leader


			#if 0 //  p163  aka Helbing Social Forces	note02 august: causes random crashes apparently.				
					
				for (int j=0;j<Persons[i]->visibleTiles.amount;j++)		// go through all tiles visible by this person
				{

					int cell_no = Utils.gNodes[Persons[i]->visibleTiles.intRecord[j]]->number; // fetch the number of the cell that is being iterated through
					
					
					for (int k=0; k< Utils.gNodes[cell_no]->PersonList.length(); k++) // go through all people on the current iterating cell.
					{
						
						if ( Utils.gNodes[cell_no]->PersonList.PersonRecord[k] != Persons[i] )
						{
							if ( Utils.gNodes[cell_no]->PersonList.PersonRecord[k]->getDirection().dot(Persons[i]->getDirection()) < -0.4 ) // opposing direction
							{
								repulsion +=  50*( Persons[i]->lookAt(Utils.gNodes[cell_no]->PersonList.PersonRecord[k]->getPosition()) ) / (Persons[i]->lookAt(Utils.gNodes[cell_no]->PersonList.PersonRecord[k]->getPosition())).normalize();
							}	
						}						
					}				
				}				
				
				// cell that the person is currently standing on.
				int cell_no = Persons[i]->currentCell; // fetch the number of the cell that is being iterated through
				for (int k=0; k<Utils.gNodes[cell_no]->PersonList.length(); k++) // go through all people on the current iterating cell.
				{
					
					if ( Utils.gNodes[cell_no]->PersonList.PersonRecord[k] != Persons[i] ) // make sure i don't count myself in
					{
						//printf("I have %i on my cell \n", Utils.gNodes[cell_no]->PersonList.length());
						if ( Utils.gNodes[cell_no]->PersonList.PersonRecord[k]->getDirection().dot(Persons[i]->getDirection()) < -0.3 ) // opposing direction
						{
							repulsion += Persons[i]->getDirection().cross(NxVec3(0,1,0))*(Utils.gNodes[cell_no]->PersonList.length());
						}	
					}
				}

	
					repulsion.normalize();

					force +=  (repulsion);
					//Persons[i]->tmpRepulsion = repulsion;
					//printf("force : %4.2f %4.2f %4.2f \n", repulsion.x, repulsion.y, repulsion.z );
					

			#endif




			#if 0	// behaviour of people on the 27 May. get to the cell that is visible that has got the highest amount of people walking in the same direction as i am.
				// obstacle avoidance code.
				NxVec3 totalforce;

				int highest_numPer = 0; // amount of people
				int highest_cellNo = 0; // which cell has got the highest amount of people

				for (int j=0;j<Persons[i]->visibleTiles.amount;j++)		// go through all tiles visible by this person
				{
					int cell_no = Utils.gNodes[Persons[i]->visibleTiles.intRecord[j]]->number; // fetch the number of the cell that is being iterated through
					
					int people_same_dir = 0; // count the amount of people walking in the same direction
					for (int k=0; k< Utils.gNodes[cell_no]->PersonList.length(); k++) // go through all people on the current iterating cell.
					{
						if ( Utils.gNodes[cell_no]->PersonList.PersonRecord[k]->getDirection().dot(Persons[i]->getDirection()) > 0.5 )
						{
							people_same_dir++;
						}
					}
					

					if ( people_same_dir > highest_numPer )
					{
						highest_numPer = people_same_dir;
						highest_cellNo = cell_no;
					}

					//if (Utils.gNodes[Persons[i]->visibleTiles.intRecord[j]]->PersonList.amount>10)
					{
					//	force += (Utils.gNodes[Persons[i]->currentCell]->connection[Utils.gNodes[Persons[i]->currentCell]->pathTo[cell_no]]->cost)*0.5;	
					
					}
					
					
					for (int j=0; j< Utils.gNodes[cell_no]->PersonList.length(); j++)
					{
						
						if (Utils.gNodes[cell_no]->PersonList.PersonRecord[j]!=NULL)
						{
				
						
							//totalforce += (force.cross(NxVec3(0,1,0)));
							//totalforce.normalize();
					
							/*
							NxVec3 avoidanceForce;
							NxVec3 offset = Persons[i]->lookAt(Utils.gNodes[cell_no]->PersonList.PersonRecord[j]->getPosition());
							avoidanceForce = offset;
							if (avoidanceForce.dot(this->Persons[i]->getDirection())>0.4)
							{
								NxVec3 sideforce = (Persons[i]->getDirection().cross(NxVec3(0,0,1)));
								avoidanceForce = (offset.magnitude()) * (sideforce);
							
								totalforce -= (avoidanceForce);
								totalforce.normalize();							
							}
							*/
													
						}
					}
					
				}

				if ((Persons[i]->visibleTilesPersonNum>10) && (highest_cellNo!=0))
				{
					force += (Utils.gNodes[Persons[i]->currentCell]->connection[Utils.gNodes[Persons[i]->currentCell]->pathTo[highest_cellNo]]->cost)*0.9;	
				}

				totalforce.normalize();
		
				//force += (totalforce);

				bool addforce = true;
				for (int j=0;j<Persons[i]->visibleTiles.amount;j++)
				{
					if (Utils.gNodes[Persons[i]->visibleTiles.intRecord[j]]->PersonList.amount>5)
					{				
						//force = force / 1.5f;
						//addforce = false;				

						//this->Utils.findPath(Persons[i]->actions.aim_targetCell,Persons[i]->currentCell);
						//this->Utils.updatePath(Persons[i]->actions.aim_targetCell,&Utils.path);
					} 
				} 
			#endif // behaviour of people, 27th May


			//if (addforce)
			{
				force.normalize();
				force *= 5.0;

				
			#if 0 // slow down the more people are in front of me	
				//if (Utils.gNodes[Persons[i]->currentCell]->locale!=NULL)
				{
					//force = force * (Persons[i]->visibleTilesPersonNum /10.0f);
					NxVec3 aimcellforce = Utils.gNodes[Persons[i]->currentCell]->connection[Utils.gNodes[Persons[i]->currentCell]->pathTo[Persons[i]->actions.aim_targetCell]]->cost;
					//if (force.dot(Utils.gNodes[Persons[i]->currentCell]->DirectionList*Utils.gNodes[Persons[i]->currentCell]->PersonList.length()) > 0.3)
					if (aimcellforce.dot(Utils.gNodes[Persons[i]->currentCell]->DirectionList*Utils.gNodes[Persons[i]->currentCell]->PersonList.length()) > 0.3)
					{
						force += force*Utils.gNodes[Persons[i]->currentCell]->PersonList.length()/20.0f;
						//force += Persons[i]->getDirection().cross(NxVec3(0,1,0))*(Utils.gNodes[cell_no]->PersonList.length()/10.0f);
					}
					if (aimcellforce.dot(Utils.gNodes[Persons[i]->currentCell]->DirectionList*Utils.gNodes[Persons[i]->currentCell]->PersonList.length()) < 0.3)
					{
						//force += Persons[i]->getDirection().cross(NxVec3(0,1,0))*(Utils.gNodes[Player.currentCell]->PersonList.length()/10.0f);
						force *= 0.1;
					}
		
				}
 			
			#endif

				/*
				if (((SZ_MscGroup*)(Persons[i]->mscGroup))->PersonRecord[0] != Persons[i])
				{
					if (Persons[i]->currentCell == ((SZ_MscGroup*)(Persons[i]->mscGroup))->PersonRecord[0]->currentCell)
					{
						force *= 0;
					}
				}
				if (((SZ_MscGroup*)(Persons[i]->mscGroup))->PersonRecord[0] == Persons[i])
					{
						force /= 1.0f;
					}
				*/
				force.multiply(gDeltaTime, force);
				force.multiply(30, force);
		
				ppPersons[i]->applyForceAction(force);

	

				force.normalize();				
				//Persons[i]->setDirection(Persons[i]->lookAt(force+Persons[i]->entity->getGlobalPosition()));
			}
		
			}
		}
#endif

		//if (i==100) { printf("exists since: %i - in cell since %i \n", Persons[i]->TLiving, Persons[i]->TInCell); }

		//forceful quitting hack
		if ( (ppPersons[i]->currentCell >= 18)  )
		{
			if (ppPersons[i]->currentCell <= 21)
			{			
				
				ppPersons[i]->actions.currentAction = ppPersons[i]->actions.amQuitting;
			}

		}

		

			ppPersons[i]->update();

		//printf(" %i \n", i);
 		
		
		switch (ppPersons[i]->actions.currentAction)
		{
			case ppPersons[i]->actions.amGoingTarget:
				{
					//if (Persons[i]->entity->getLinearVelocity().magnitude()>0.9)
					ppPersons[i]->setDirection(ppPersons[i]->lookAt(ppPersons[i]->entity->getLinearVelocity()+ppPersons[i]->entity->getGlobalPosition()));
					break;
				}
				

		}	


		
	

		// if this person has got the person_quitting action, delete it from the world.
		if (ppPersons[i]->actions.currentAction == ppPersons[i]->actions.amQuitting)
		{
			//printf("quitting \n");
			this->ppSubstract(ppPersons[i]);
		}


		//printf("")
	}

	service.update();
	//Player.update(); 
	gCamera.Update();
	gMouse.Update();




	//if ( !((SZ_FunctionBase*)(Utils.gNodes[Player.currentCell]->locale)==NULL) )
	{
		//printf("Player tile: %i event %i \n", Player.currentCell, ((SZ_FunctionBase*)(Utils.gNodes[Player.currentCell]->locale))->eventnumber);
	}

	gDatalogger.update();

	TimerFunc(5*60, &SZ_World::fiveMinutesUpdateCycle);	// every five minutes

	//TimerFunc(60, &SZ_World::PlanesDepart.update);
	//TimerFunc(60, &SZ_World::PlanesLanding.update);


	/*
	printf("Person %i : %f %f %f - %i:%i:%i\n", 100, 
		Persons[100]->getPosition().x, Persons[100]->getPosition().y, Persons[100]->getPosition().z,
		gTimeKeeper.theTime()->tm_hour, gTimeKeeper.theTime()->tm_min, gTimeKeeper.theTime()->tm_sec);
		*/

	


	//TimerFunc(300, &SZ_World::nothing);
	//TimerFunc(60, &SZ_World::nothing);

	/*
	if (gTimeKeeper.theTime()->tm_min == 55)
	{
		bPause = true;
		printf("iteration %i \n", Utils.iterations);
	}
	*/



}


void SZ_World::TimerFunc(unsigned int secs, void ( SZ_World::*func)() )
{
	if (gTimeKeeper.getIterations() % gTimeKeeper.getFrequency() == 0)
	{
		if	(gTimeKeeper.theTimeInSecs() % secs == 0 ) 
		{
			(this->*func)();
		}
	}
	//gTimeKeeper.iterations++;
}



void SZ_World_Utils::findPath(int end, int start)
{
	// invert start and end, otherwise the resulting path is from the END to START
	int tmp;
	tmp = start;
	start = end;
	end = tmp;
	
	open.empty();   // empty all lists.
	path.empty(); 
	closed.empty(); 		

	// initialise the record for the start node
	SZ_NodeRecord *startRecord;
	startRecord = new SZ_NodeRecord();
	startRecord->gNode = gNodes[start];
	startRecord->connection = NULL;
	startRecord->costSoFar = 0;

	//initlialise the open and closed list
	open.add(startRecord);
	SZ_NodeRecord *current = new SZ_NodeRecord();

	// iterate through processing each node
	while (open.length()>0)
	{
		// find the smallest element in the open list	
		current = open.smallestElement();

		// if it is the goal node, then terminate
		if (current->gNode->number == end) { break; }

		// otherwise get its outgoing connections
		for (int i=0; i< current->gNode->amountConnections; i++)
		{		
			// get the cost for the end node
			SZ_Node* endNode;
			float endNodeCost = 0;

			endNode = (SZ_Node*)(current->gNode->connection[i]->toNodeAddress);

			if ( (endNode==NULL)  ) { continue; }
			endNodeCost = current->costSoFar + current->gNode->connection[i]->cost.magnitudeSquared();
			
			// added the next line to include if a tile is busy
			//endNodeCost += endNode->PersonList.amount*10;
			//if (((SZ_FunctionBase*)(endNode->Function))!=NULL) { endNodeCost *=10;  }

			
			// skip if the node is closed
			if (closed.contains(endNode)) { continue; }

			
			// .. or if it is open and we've found a worse route
			if (open.contains(endNode))
			{
				// Here we find the record in the open list
				// corresponding to the endNote.
				SZ_NodeRecord *endNodeRecord = open.find(endNode);
				if (endNodeRecord->costSoFar <= endNodeCost) 
				{ 
					continue;
				}

			 }
			// otherwise we know we've got an unvisited node, so make record for it
			else
				{
					SZ_NodeRecord *endNodeRecord;
					endNodeRecord = new SZ_NodeRecord();
					endNodeRecord->gNode = endNode;

					// We're here if we need to update the node
					// Update the cost and connection
					endNodeRecord->costSoFar = endNodeCost;					
					endNodeRecord->node_number = current->gNode->number;
					endNodeRecord->connection = i;

					// and add it to the open list
					if (!(open.contains(endNode)))
						{
							open.add(endNodeRecord);
						}
				}
		}
		// We've finished looking at the connections for the current node, so add it to the closed
		// list and remove it from the open list.
		closed.add(current);
		open.substract(current);
	}
	
	while (current->gNode->number!=start)
	{

			closed.substract(current);			// remove from close list, to make sure it tries not to be deleted twice
			open.substract(current);			// same from open list.
			path.add(current);					// add to path list to store the path to take.
			current = closed.find(gNodes[current->node_number]);		
	}
}



void SZ_World_Utils::findPathWeighted(int end, int start)
{

	// invert start and end, otherwise the resulting path is from the END to START
	int tmp;
	tmp = start;
	start = end;
	end = tmp;
	
	open.empty();   // empty all lists.
	path.empty(); 
	closed.empty(); 		

	// initialise the record for the start node
	SZ_NodeRecord *startRecord;
	startRecord = new SZ_NodeRecord();
	startRecord->gNode = gNodes[start];
	startRecord->connection = NULL;
	startRecord->costSoFar = 0;

	//initlialise the open and closed list
	open.add(startRecord);
	SZ_NodeRecord *current = new SZ_NodeRecord();

	// iterate through processing each node
	while (open.length()>0)
	{
		// find the smallest element in the open list	
		current = open.smallestElement();

		// if it is the goal node, then terminate
		if (current->gNode->number == end) { break; }

		// otherwise get its outgoing connections
		//for (int i=0; i< current->gNode->amountConnections; i++)

		//get only the required outgoing connections.
		//int whichConnection = current->gNode->whichConnection(current->gNode->pathTo[end]);
		//printf("%i \n", whichConnection);
	
		//for (int i=0; i< current->gNode->amountConnections; i++)
		for (int k=-1; k<=1; k++)
		{	
			
			
			int i = current->gNode->pathTo[end] + k;

			if (i==-3)		{ i = 5;} 
			if (i==-2)		{ i = 6;} 
			if (i==-1)		{ i = 7;} 
			if (i== 8)		{ i = 0;}
			if (i== 9)		{ i = 1;}
			if (i== 10)		{ i = 2;}
			

			// get the cost for the end node
			SZ_Node* endNode;
			float endNodeCost = 0;			

			endNode = (SZ_Node*)(current->gNode->connection[i]->toNodeAddress);


			if ( (endNode==NULL)  ) { continue; }
			endNodeCost = current->costSoFar + current->gNode->connection[i]->cost.magnitudeSquared();
			
			// added the next line to include if a tile is busy

			//for (int m=0; m<current->gNode->PersonList.length(); m++) // go through all people on the current iterating cell.
				{
					//endNodeCost += current->gNode->PersonList.amount*10;
					//float dot = current->gNode->connection[i]->cost.dot( current->gNode->PersonList.PersonRecord[m]->getDirection());
					//if (dot<=0) { endNodeCost++; }
					//float dot = current->gNode->connection[i]->cost.dot( endNode->PersonList.PersonRecord[m]->getDirection() );
					//if (dot<=0.1) { endNodeCost *=10;}
				}
			
			//endNodeCost += endNode->PersonList.amount*10;
			//if (((SZ_FunctionBase*)(endNode->Function))!=NULL) { endNodeCost *=10;  }
					
			// skip if the node is closed
			if (closed.contains(endNode)) { continue; }

			
			// .. or if it is open and we've found a worse route
			if (open.contains(endNode))
			{
				// Here we find the record in the open list
				// corresponding to the endNote.
				SZ_NodeRecord *endNodeRecord = open.find(endNode);
				if (endNodeRecord->costSoFar <= endNodeCost) 
				{ 
					continue;
				}

			 }
			// otherwise we know we've got an unvisited node, so make record for it
			else
				{
					SZ_NodeRecord *endNodeRecord;
					endNodeRecord = new SZ_NodeRecord();
					endNodeRecord->gNode = endNode;

					// We're here if we need to update the node
					// Update the cost and connection
					endNodeRecord->costSoFar = endNodeCost;					
					endNodeRecord->node_number = current->gNode->number;
					endNodeRecord->connection = i;

					// and add it to the open list
					if (!(open.contains(endNode)))
						{
							open.add(endNodeRecord);
						}
				}
		}
		// We've finished looking at the connections for the current node, so add it to the closed
		// list and remove it from the open list.
		closed.add(current);
		open.substract(current);
	}
	
	while (current->gNode->number!=start)
	{
			closed.substract(current);			// remove from close list, to make sure it tries not to be deleted twice
			open.substract(current);			// same from open list.
			path.add(current);					// add to path list to store the path to take.
			current = closed.find(gNodes[current->node_number]);		
	}
}






void SZ_World::processKeys()
{
	for (int i = 0; i < MAX_KEYS; i++)
		{				
			if ((!Utils.gKeysSpecial[i]) && (!Utils.gKeys[i]) ) { continue; }
			if ( Utils.gKeysSpecial[i])	{ sendKeySpecial(i); }
			if ( Utils.gKeys[i])		{ sendKey(i); }
		}
}



void SZ_World::sendKeySpecial(int key)
{
	//Player.processKeysSpecial(key, gDeltaTime);
}

void SZ_World::sendKey(int key)
{
	//Player.processKeys(key, gDeltaTime);
	gCamera.ProcessKeys(key, gDeltaTime);
	
}

void SZ_World::KeyboardDownCall(int key,int x,int y)
{
	Utils.gKeys[key] = true;
	//Player.keyboardDownCall(key,x,y);
	gDatalogger.processLoggerKeys(key, gDeltaTime);

	
}
void SZ_World::KeyboardUpCall(int key,int x,int y)
{
	Utils.gKeys[key] = false;
	//Player.keyboardUpCall(key,x,y);

	//transform player value to tile value
	//int g = (Player.entity->getGlobalPosition().x / WORLD_TILE);
	//int g2 = (Player.entity->getGlobalPosition().z / WORLD_TILE );
	//int randomnode = g+(g2*WORLD_X_5X5);
	
	switch (key)
	{

		//case 'v':
			{
							
				//Utils.path.empty();
				//this->Utils.findPathFrom(40,0,799,Utils.gNodes[Player.currentCell],&Utils.pList,&Player);
				//int target = (Utils.pList.NodesRecord[Utils.pList.length()-1]->gNode->number);
				//this->Utils.findPathWeighted(799,this->Player.currentCell);								
				//this->Utils.updatePath(Player.currentCell,&Utils.path);				
				//this->Utils.findPathFrom(40,0,799,Utils.gNodes[Player.currentCell],&Utils.pList,&Player);

				
				break;
			}
		case '1':
			{
				/*
				int counter[20];
				for (int i=0; i<=20; i++)
				{
					counter[i] = 0;
				}
				for (int i=0; i<150; i++)
				{
					int poisson = PoissonRandomNumber(0.86);
					counter[poisson]++;
					//printf(" poisson %i: %i \n", poisson, counter[poisson] );

				}

				for (int i=0; i<=10; i++)
				{
					printf(" %4.2i: %4.2i ", i, counter[i]);
					//printf("")
					for (int k = 0 ; k < counter[i]; k++)
					{
						printf("*");
					}
					printf("\n");
				}
				*/

				ppEmpty();
				
				//printf("current: %i - to go to 799 take connection %i \n", Player.currentCell, Utils.gNodes[Player.currentCell]->pathTo[799]);
				//printf("empty list \n");
				//Utils.vVectorList.empty();
				//for (int k=-1; k<2; k++)
					{						
						//int i = Utils.gNodes[Player.currentCell]->pathTo[799] + k;
						//if (i<0)		{ i = 8 + k; }
						//if (i>7)		{ i = 0 + k; }
						//printf("neighbour %i = %i\n",k, i );
					}
				break;
			}


		// Spawn 25 people on the cell the camera is pointing at.
		case CONTROL_PPL_SPAWN: 
			{
				for (int i=0; i<GENERATION_MAX_PERSON_PER_TILE; i++)
				{
					ppSpawn(gCamera.RecticleCell);
				}
				break;
			}
		// Delete all the people standing on the selected cell
		case CONTROL_PPL_REMOVE:	
			{
					int amountppl = Utils.gNodes[gCamera.RecticleCell]->PersonList.amount;
					for (int i=0; i<amountppl; i++)
					{
						Utils.gNodes[gCamera.RecticleCell]->PersonList.PersonRecord[i]->actions.currentAction = ppPersons[i]->actions.amQuitting;
					}
					break;
			}
		
		// event/location control: delete selected event 
		case CONTROL_EVENT_REMOVE:
			{
				printf(" --> pressed 'delete selected event' key \n");
				// (1) empty the current tile i am standing on, and save
				for (int i=0; i<Utils.bitmapplaces.getWidth();i++)
				{
					Utils.bitmapplaces.setRGBXY(0,255,0,i,gCamera.RecticleCell);
				}
				Utils.bitmapplaces.save("textures/places.bmp",24,1200,3,Utils.bitmapplaces.dataRGB);
				
				
				//12 oct, not working fine anymore
				// (2) reload and remove all the other event nodes
				Utils.loadPlaces();
				service.empty();

				// (3) recreate all events node
				service.init(this);

				break;
			}
		// shows data for the current node
		case CONTROL_EVENT_SHOW_DATA:
			{
				printf(" --> pressed 'display data on selected node' key \n");
				printf("=== Node Data: Node %i; population %i \n ", gCamera.RecticleCell, sbGetLayerOccupancy(gCamera.RecticleCell)->amount);
				// Inform the user of already existing data.
				if ((Utils.bitmapplaces.getRGBXY(2,0,gCamera.RecticleCell)) != 255)
				{						
					printf("=== Node %i contains exisiting time data on pixel #0 (type of place) :",gCamera.RecticleCell);						
					for (int j = 0; j<1; j++)
					{
						if ((Utils.bitmapplaces.getRGBXY(2,j,gCamera.RecticleCell)) != 255)
						{
							for (int k = 1; k<=3 ; k++)
							{				
								char cCh = (char)(Utils.bitmapplaces.getRGBXY(k,j,gCamera.RecticleCell)); // convert int to ascii
								printf("%c", cCh);	
								//printf("%i", (Utils.bitmapplaces.getRGBXY(k,j,gCamera.RecticleCell)), cCh);	
							}
						}
					}
					printf("\n");

					printf("=== Node %i contains exisiting time data on pixel #1 (time required) :",gCamera.RecticleCell);						
					for (int j = 1; j<2; j++)
					{
						if ((Utils.bitmapplaces.getRGBXY(2,j,gCamera.RecticleCell)) != 255)
						{
							for (int k = 1; k<=3 ; k++)
							{				
								char cCh = (char)(Utils.bitmapplaces.getRGBXY(k,j,gCamera.RecticleCell)); // convert int to ascii
								printf("%c", cCh);	
								//printf("%i", (Utils.bitmapplaces.getRGBXY(k,j,gCamera.RecticleCell)), cCh);	
							}
						}
					}
					printf("\n");
					
					//int cost_time = ((SZ_FunctionBase*)(Utils.gNodes[Persons[100]->currentCell]->locale))->timecost;
					int cost_time = ((SZ_FunctionBase*)(Utils.gNodes[gCamera.RecticleCell]->functions))->timecost;
					printf("=== Node %i contains exisiting time: (time required) : %i\n", gCamera.RecticleCell, cost_time);	
				}
				/*
				printf(" --> pressed 'load data for events' key \n");
				Utils.loadPlaces();
				Events.empty();				
				Events.init(this);
				*/

				/*

			for (int i=0;i<NODES_AMOUNT;i++) // go through each node
				{
					if (Utils.gNodes[i]==NULL) // if the node is non existant, no information will ever be stored about it.
					{
						continue;
					}
					
					if (Utils.bitmapplaces.getRGBXY(2,0,i) != 255)
					{
						printf("node %i contains information \n",i);
						for (int j = 0; j<Utils.bitmapplaces.getWidth(); j++)
						{
							for (int k = 1; k<=3 ; k++)
							{
								//printf("%i ",Utils.bitmapplaces.getRGBXY(k,j,i));
								
								char cCh = (char)(Utils.bitmapplaces.getRGBXY(k,j,i)); // convert int to ascii
								printf("%c", cCh);
							}
						}
						printf("\n");
					}
				}
				*/
			break; // break the case 
			}

		// inputs data for nodes and saves information in bitmapplaces
		case CONTROL_EVENT_INPUT_DATA:
			{
				printf(" --> pressed 'input data on selected node' key \n");


				// Inform the user of already existing data.
				if ((Utils.bitmapplaces.getRGBXY(2,0,gCamera.RecticleCell)) != 255)
				{						
					printf("=== Node %i contains exisiting time data on pixel #0 :",gCamera.RecticleCell);						
					for (int j = 0; j<1; j++)
					{
						if ((Utils.bitmapplaces.getRGBXY(2,j,gCamera.RecticleCell)) != 255)
						{
							for (int k = 1; k<=3 ; k++)
							{				
								char cCh = (char)(Utils.bitmapplaces.getRGBXY(k,j,gCamera.RecticleCell)); // convert int to ascii
								printf("%c", cCh);	
							}
						}
					}
					printf("\n");
				}

				// input the data, maximum 60 characters
				char str[72];		
				do { 
					printf("=== Node Data Places Input: Input Node %i detail: ", gCamera.RecticleCell);
					
					do { scanf("%[^\t\n]",str); } while ((getchar()!=(int)'\n')); } 		// repeat until input character = enter		
				while (strlen(str)>60) ;			// repeat the whole until amount of characters is less than 60	
			
				
				if (strlen(str)>0){

					// empty the current Y row of every X
					for (int i=0; i<Utils.bitmapplaces.getWidth();i++)
					{
						Utils.bitmapplaces.setRGBXY(0,255,0,i,gCamera.RecticleCell);
					}
					Utils.bitmapplaces.save("textures/places.bmp",24,1200,3,Utils.bitmapplaces.dataRGB);

					// convert input data to ascii and store within the bitmapplaces bitmap file.
					int pixel = 0;
					int i=0;
					int RGB=0;
					do {
						if (RGB==0) { Utils.bitmapplaces.setRGBXY(int(str[i]),Utils.bitmapplaces.getRGBXY(2,pixel,gCamera.RecticleCell),Utils.bitmapplaces.getRGBXY(3,pixel,gCamera.RecticleCell),pixel,gCamera.RecticleCell); }
						if (RGB==1) { Utils.bitmapplaces.setRGBXY(Utils.bitmapplaces.getRGBXY(1,pixel,gCamera.RecticleCell),int(str[i]),Utils.bitmapplaces.getRGBXY(3,pixel,gCamera.RecticleCell),pixel,gCamera.RecticleCell); }
						if (RGB==2) { Utils.bitmapplaces.setRGBXY(Utils.bitmapplaces.getRGBXY(1,pixel,gCamera.RecticleCell),Utils.bitmapplaces.getRGBXY(2,pixel,gCamera.RecticleCell),int(str[i]),pixel,gCamera.RecticleCell); }
						i++;
						RGB++;
						if (i % 3 == 0)
						{ 
							pixel++;
							RGB=0;
						}
					} while (str[i]!='\0'); // repeat until the next character equals to *nothing set*

					// out saved information for customer service purposes.
					if (Utils.bitmapplaces.save("textures/places.bmp",24,1200,3,Utils.bitmapplaces.dataRGB)) 
					{	printf("=== Node Data Places Input: Node %i - %s - saved \n",gCamera.RecticleCell, str); }
					else
					{   printf("=== Node Data Places Input: Node %i - %s - NOT saved, Error \n",gCamera.RecticleCell,str);
					}

					Utils.loadPlaces();
					service.empty();				
					service.init(this);

				} else { 
					printf("=== Node Data Places Input: No changes made. \n"); 
				}				

				break;
			}

			//input time data for selected node:
			case CONTROL_EVENT_INPUT_TIME:
			{
				printf(" --> pressed 'input time data on selected node' key \n");

				// Inform the user of already existing data.
				if ((Utils.bitmapplaces.getRGBXY(2,1,gCamera.RecticleCell)) != 255)
				{	
					
					printf("=== Node %i contains exisiting time data on pixel #1: ",gCamera.RecticleCell);						
					for (int j = 1; j<2; j++)
					{
						if ((Utils.bitmapplaces.getRGBXY(2,j,gCamera.RecticleCell)) != 255)
						{

							for (int k = 1; k<=3 ; k++)
							{				
								char cCh = (char)(Utils.bitmapplaces.getRGBXY(k,j,gCamera.RecticleCell)); // convert int to ascii
								printf("%c", cCh);	
							}
						}
					}
					printf("\n");
					
				}

				// input the data, maximum 60 characters
				char str[72];		
				do { 
					printf("=== Node Data Places Input: Input Node %i time data: ", gCamera.RecticleCell);
					
					do { scanf("%[^\t\n]",str); } while ((getchar()!=(int)'\n')); } 		// repeat until input character = enter		
				while (strlen(str)>60) ;			// repeat the whole until amount of characters is less than 60	
			
				
				if (strlen(str)>0){

					// empty the current Y row of every X
					//for (int i=1; i<Utils.bitmapplaces.getWidth();i++)
					//{
						Utils.bitmapplaces.setRGBXY(0,255,0,1,gCamera.RecticleCell);
					//}
					Utils.bitmapplaces.save("textures/places.bmp",24,1200,3,Utils.bitmapplaces.dataRGB);
	
					
					// convert input data to ascii and store within the bitmapplaces bitmap file.
					int pixel = 1;
					int i=0;
					int RGB=0;
					do {
						if (RGB==0) { Utils.bitmapplaces.setRGBXY(int(str[i]),Utils.bitmapplaces.getRGBXY(2,pixel,gCamera.RecticleCell),Utils.bitmapplaces.getRGBXY(3,pixel,gCamera.RecticleCell),pixel,gCamera.RecticleCell); }
						if (RGB==1) { Utils.bitmapplaces.setRGBXY(Utils.bitmapplaces.getRGBXY(1,pixel,gCamera.RecticleCell),int(str[i]),Utils.bitmapplaces.getRGBXY(3,pixel,gCamera.RecticleCell),pixel,gCamera.RecticleCell); }
						if (RGB==2) { Utils.bitmapplaces.setRGBXY(Utils.bitmapplaces.getRGBXY(1,pixel,gCamera.RecticleCell),Utils.bitmapplaces.getRGBXY(2,pixel,gCamera.RecticleCell),int(str[i]),pixel,gCamera.RecticleCell); }
						i++;
						RGB++;
						if (i % 3 == 0)
						{ 
							pixel++;
							RGB=0;
						}
					} while (str[i]!='\0'); // repeat until the next character equals to *nothing set*

					// out saved information for customer service purposes.
					if (Utils.bitmapplaces.save("textures/places.bmp",24,1200,3,Utils.bitmapplaces.dataRGB)) 
					{	printf("=== Node Time Data Places Input: Node %i - %s - saved \n",gCamera.RecticleCell, str); }
					else
					{   printf("=== Node Data Places Input: Node %i - %s - NOT saved, Error \n",gCamera.RecticleCell,str);
					}

					// 12 oct, not working anymore.
					Utils.loadPlaces();	
					service.empty();				
					service.init(this);

				} else { 
					printf("=== Node Data Places Input: No changes made. \n"); 
				}				

				break;
			}
		// makes paths; calculates through dijkstra the paths.
		case CONTROL_MAKE_DIJKSTRA: { 
			printf(" --> pressed 'Compute Dikstra paths' key \n");
				this->service.modifyNode();
				this->Utils.makePath(); 
			printf("=== Dijkstra computed. Please restart the application\n");
			break; }
		// loads the external path file manually
		//case 'l': { printf("loadpath! \n"); this->Utils.loadPath(); break; }

	    // lists all the nodes if they contain individuals
	  /*
		case 'j': {
					for (int i = 0;i<NODES_AMOUNT;i++)
					{
						if (this->Utils.gNodes[i]!=NULL)
						{
							if (Utils.gNodes[i]->PersonList.amount>0)
							printf("nodes %i has %i persons \n",i, Utils.gNodes[i]->PersonList.amount);
						}

					}
				    break;
				  }
	  */
	}
}
void SZ_World::KeyboardSpecialDownCall(int key,int x,int y)
{
	//Player.keyboardSpecialDownCall(key,x,y);
	Utils.gKeysSpecial[key] = true;
}
void SZ_World::KeyboardSpecialUpCall(int key,int x,int y)
{
	Utils.gKeysSpecial[key] = false;
	//Player.keyboardSpecialUpCall(key,x,y);

	switch (key)
	{
	case GLUT_KEY_F1:
		{
			if (gCamera.projection == gCamera.cpPerspective )
			{
				gCamera.setPositionForward(NxVec3(100,100,150), NxVec3(0, -0.85, -0.50));	// persp top down
			}

			if (gCamera.projection == gCamera.cpOrthoGraphic)
			{
				gCamera.setPositionForward(NxVec3( (WORLD_X)/2.0, 10, (WORLD_Z)/2.0), NxVec3(0.00f, -0.99, 0.01) );
				gCamera.projectionOrthoZoom = -gCamera.projectionOrthoZoom;
			}
			//printf("f1 pressed");
			
			//ogCamera.Pos =  ;
			
			//gCamera.setPositionForward(NxVec3(100,100,150), NxVec3(0.00f, -0.99, 0.01));	// orthographic top down

			
			break;
		}
	case GLUT_KEY_F2:
		{
			gCamera.setPositionForward(NxVec3(80,20,140), NxVec3(0.45, -0.70, -0.60));
			break;
		}
	case GLUT_KEY_F3:
		{
			gCamera.setPositionForward(NxVec3(110,9.50,35), NxVec3(-0.75, -0.55, 0.32));
			break;
		}
	case GLUT_KEY_F11:
		{
			gCamera.projection_selectNextMode();
			printf("%s \n", gCamera.projection_modeName());
		}
	}
}

void SZ_World::Init(NxScene *whichScene, NxPhysicsSDK *whichSDK)
{
	// initialise the world variables
	wScene = whichScene;
	wSDK = whichSDK;
	Utils.wScene = whichScene;
	Utils.wSDK = whichSDK;	
	bPause = false;

	Utils.Init();

	// loads the physical layer
	Utils.bitmap.read("textures/floor.bmp", false);
	printf("...floor.bmp file loaded \n");
	
	Utils.MakePhysX();  // make the PhysX world out of the physical layer
	Utils.MakeNodes();  // create the nodes on top of the physical layer
	printf("...Nodes initialised \n");	

	// load path layer
	Utils.loadPath();			// load paths layer
	Utils.loadPlaces();			// load places layer
	Utils.loadPlaces_Icon();	// loads icons

	// load character & font texture files
	Utils.LoadTexBitmap("textures/char_heads.bmp",2,Utils.texture_array,false, Utils.bitmapHeads);		
	printf("...char_heads.bmp file loaded \n");
	Utils.LoadTexBitmap("textures/font1.bmp",3,Utils.texture_array,false,Utils.bitmapFont1);
	printf("...font1.bmp file loaded \n");
	Utils.LoadTexBitmap("textures/user_interface.bmp",4,Utils.texture_array,true, Utils.bitmapUserInterface);	// load the texture used for individuals/ui
	printf("...user_interface.bmp \n");
	
	Sleep(0.1f);

	// load the map information layer & initialise the event/place list
	service.init(this);
	printf("...services initialised \n");	

	// init camera and player
	gCamera.Init(wScene);
	gCamera.mode = gCamera.cmFree;
	gCamera.world = this;	
	//Player.init(wScene, Utils.texture_array, this);	
	//Player.model->setTexture(19,19,19);
	//Player.update();
	//Player.changeTile();
	printf("...Camera & Player entities initialised \n");

	// initialise the time object
	gTimeKeeper.init();	
	printf("...Timekeeper initialised \n");
	ppEmpty();			// make sure there is no population in the world

	// generate all the nodes
	for (int j=0;j<NODES_AMOUNT-1;j++)
	{		
		if (Utils.gNodes[j]!=NULL)
			{
				SZ_NodeRecord* tmpRecord;
				tmpRecord = new SZ_NodeRecord();
				tmpRecord->gNode = Utils.gNodes[j];	
				Utils.gNodesListOrdered.add(tmpRecord);
				tmpRecord->gNode->setWorld(this);
			}		
	}
	printf("...Nodes initialised \n");	

	// initialize the crowd. Not used anymore as crowd is generated dynamically.
	for (int i=0; i<0; i++)
	{
		// create a new Person.
		SZ_Person* tmpPerson = new SZ_Person();
		tmpPerson->init(wScene,Utils.texture_array,this);
		tmpPerson->putOnCell();
		// give this person a specific Function/place to go to
		//int tmp_rand_event = rand()% Events.amount; //returns a value from 0 to the amount of events on the map
		//tmpPerson->actions.aim_targetCell = Events.FunctionBaseList[tmp_rand_event]->currentCell;
		ppAdd(tmpPerson);		// add the person to the list of people.
	}

	// PLANE DEPARTING SCHEDULE
	// initialize the planes departing.
	PlanesDepart.onCreate(this, Utils.texture_array);
	PlanesDepart.setSchedule(SZ_Schedule::apDepartTable);
		PlanesDepart.ppAddExit(Utils.gNodes[service.findSpecificEvent(EXIT_01_PIXEL)->currentCell]);
		PlanesDepart.ppAddExit(Utils.gNodes[service.findSpecificEvent(EXIT_02_PIXEL)->currentCell]);
		PlanesDepart.ppAddExit(Utils.gNodes[service.findSpecificEvent(EXIT_03_PIXEL)->currentCell]);
		PlanesDepart.ppAddExit(Utils.gNodes[service.findSpecificEvent(EXIT_04_PIXEL)->currentCell]);
		PlanesDepart.ppAddExit(Utils.gNodes[service.findSpecificEvent(EXIT_05_PIXEL)->currentCell]);
		PlanesDepart.ppAddSecurity(Utils.gNodes[215]); // hardcoded security node
		PlanesDepart.ppAddSecurity(Utils.gNodes[217]); // hardcoded security node
		PlanesDepart.ppAddSecurity(Utils.gNodes[219]); // hardcoded security node
		PlanesDepart.ppAddSecurity(Utils.gNodes[221]); // hardcoded security node
		PlanesDepart.ppAddDeparts(Utils.gNodes[service.findSpecificEvent(DEP_01_PIXEL)->currentCell]);
		PlanesDepart.ppAddDeparts(Utils.gNodes[service.findSpecificEvent(DEP_02_PIXEL)->currentCell]);
		PlanesDepart.ppAddDeparts(Utils.gNodes[service.findSpecificEvent(DEP_03_PIXEL)->currentCell]);
		PlanesDepart.ppAddDeparts(Utils.gNodes[service.findSpecificEvent(DEP_04_PIXEL)->currentCell]);

	PlanesDepart.setPosition(NxVec3(91.5f,3.5,120.1f));
	//PlanesDepart.setPosition(NxVec3(93.5f,3,120.1f));
	int tmpHour, tmpMin;
	tmpHour = PLANE_FIRST_DEPART_HOUR;
	tmpMin =  PLANE_FIRST_DEPART_MINUTE;
	for (int i=0; i<65; i++)	// 4 planes per hour, for 20 hours 20*4 = 80
	{
		PlanesDepart.newPlane(tmpHour,tmpMin,SZ_Airplane::apWaiting, NULL);
		tmpMin = tmpMin + 15; if (tmpMin>=60) { tmpMin = 0; tmpHour++;}	// plane departing every 15minutes
		if (tmpHour>=25) { tmpHour = 0; }
	}


	// PLANE LANDING SCHEDULE
	// initialize the planes landing.
	PlanesLanding.onCreate(this, Utils.texture_array);
	PlanesLanding.setSchedule(SZ_Schedule::apLandingTable);
		PlanesLanding.ppAddGate(Utils.gNodes[service.findSpecificEvent(ARRIVAL_01_PIXEL)->currentCell]);	
		PlanesLanding.ppAddGate(Utils.gNodes[service.findSpecificEvent(ARRIVAL_02_PIXEL)->currentCell]);
		PlanesLanding.ppAddGate(Utils.gNodes[service.findSpecificEvent(ARRIVAL_03_PIXEL)->currentCell]);
		PlanesLanding.ppAddGate(Utils.gNodes[service.findSpecificEvent(ARRIVAL_04_PIXEL)->currentCell]);
		PlanesLanding.ppAddGate(Utils.gNodes[service.findSpecificEvent(ARRIVAL_05_PIXEL)->currentCell]);
		PlanesLanding.ppAddExit(Utils.gNodes[service.findSpecificEvent(EXIT_01_PIXEL)->currentCell]);
		PlanesLanding.ppAddExit(Utils.gNodes[service.findSpecificEvent(EXIT_02_PIXEL)->currentCell]);
		PlanesLanding.ppAddExit(Utils.gNodes[service.findSpecificEvent(EXIT_03_PIXEL)->currentCell]);
		PlanesLanding.ppAddExit(Utils.gNodes[service.findSpecificEvent(EXIT_04_PIXEL)->currentCell]);
		PlanesLanding.ppAddExit(Utils.gNodes[service.findSpecificEvent(EXIT_05_PIXEL)->currentCell]);
	PlanesLanding.setPosition(NxVec3(100,3.5,120.1f));
	//PlanesLanding.setPosition(NxVec3(100.1,3,120.1f));
	tmpHour = PLANE_FIRST_LAND_HOUR;
	tmpMin =  PLANE_FIRST_DEPART_MINUTE;
	for (int i=0; i<65; i++)	// 4 planes per hour, for 20 hours 20*4 = 80
	{
		PlanesLanding.newPlane(tmpHour,tmpMin,SZ_Airplane::apExpected, NULL);
		tmpMin = tmpMin + 15; if (tmpMin>=60) { tmpMin = 3; tmpHour++;}	// plane departing every 15minutes
		if (tmpHour>=24) { tmpHour = 0;}
	}

	printf("...Planes initialised \n");	

	Sleep(0.1f); // fixes crash on load bug.
	Sleep(1.0f); // fixes crash on load bug.

	gMouse.init(this);

	ppUpdateManager.tmp_world = this;
	
	for (int i = 0; i < MAX_SOCIAL_LINKS ; i++)
	{
		ggGroupsTotalCount[i] = 0; 
	}

	ggGroupsAmount = 0;
	gDatalogger.init(this, "data200.txt");

}

//transforms the value of a cell to a nxvec (top left)
NxVec3 SZ_World_Utils::cellToPos(int whichCell)
{
	//int ex = (whichCell % WORLD_X_5X5);	   // transform the tile value into the top left X corner
	//int ey = (whichCell-ex)/WORLD_X_5X5;    //  transform the tile value into the top left Z corner

	/*
	int X = (tmp_world->Utils.gNodes[visibleTiles.intRecord[j]]->number % WORLD_X_5X5)   ;	   // transform the tile value into the top left X corner
	int Z = ((tmp_world->Utils.gNodes[visibleTiles.intRecord[j]]->number-X)/WORLD_X_5X5);    //  transform the tile value into the top left Z corner
	X = X * WORLD_TILE	+WORLD_TILE /2.0;
	Z = Z * WORLD_TILE	+WORLD_TILE /2.0;
	*/


	float X = ((whichCell % WORLD_X_5X5));// * WORLD_TILE;//	+ (WORLD_TILE/2.0f) ;	   // transform the tile value into the top left X corner
	float Z = ((whichCell-X)/WORLD_X_5X5);// * WORLD_TILE;//	+ (WORLD_TILE/2.0f);    //  transform the tile value into the top left Z corner
	X = X * WORLD_TILE	+WORLD_TILE /2.0;
	Z = Z * WORLD_TILE	+WORLD_TILE /2.0;

	//int ex = (whichCell) % (WORLD_TILE);	  // transform the tile value into the top left X corner
	//int ey = ((whichCell) -ex)/WORLD_TILE;   // transform the tile value into the top left Y corner

	return NxVec3(X,0, Z);
}

// find target cell using the pre-calculated paths layer.
void SZ_World_Utils::findPathFrom(int mDepth, int cDepth, int end, SZ_Node *whichNode, SZ_PathfindingList *pList, SZ_ActorSprite *whichActorSprite)
{
	if (cDepth == 0)
	{
		pList->empty();
	}

	if (mDepth != cDepth)
	{
		cDepth++;
		if (whichNode !=NULL)
		{
			if ( (whichNode->number != end) && (whichNode->pathTo[end]!=-1) && (end>0))
			{
				//printf("**\n");
				//printf("to go to %i take connection %i \n", end, whichNode->pathTo[end]);
				
				SZ_Node *tmpNode = ((SZ_Node*)(whichNode->connection[whichNode->pathTo[end]]->toNodeAddress));			
		
				
				SZ_NodeRecord *tmpRecord;
				tmpRecord = new SZ_NodeRecord();
				tmpRecord->gNode = tmpNode;

				//printf("currently on %i, have to go to %i \n", whichActorSprite->currentCell, tmpNode->number);			
	
				if (!(pList->contains(tmpNode))) { pList->add(tmpRecord); }
				findPathFrom(mDepth, cDepth, end, tmpNode, pList, whichActorSprite);
			}
		}
	}
	
}

// finds my neighbour of depth M, of a given node, result stored in nList, of which actorsprit
// breath first search
void SZ_World_Utils::findNeighbour(int mDepth, int cDepth, SZ_Node *whichNode, SZ_PathfindingList *nList, SZ_ActorSprite *whichActorSprite, bool dotproduct)
{
	
	//if (whichActorSprite == NULL) { return; }
	if (mDepth != cDepth)
	{	
		cDepth++;
		if (whichNode!=NULL)
		{
			
			for (int i=0;i<8;i++)
			{
				
				if (whichNode->connection[i]->toNodeAddress!=NULL)
				{

					// dot product = true, only checks neighbours outgoing from SZ_ActorSprite's directionl					
					if (dotproduct)
					{
						float dotforward = whichActorSprite->getDirection().dot(whichNode->connection[i]->cost);
						if (dotforward>0.1)
						{
							SZ_Node* tmpNode = ((SZ_Node*)(whichNode->connection[i]->toNodeAddress));								
							SZ_NodeRecord* tmpRecord;
							tmpRecord = new SZ_NodeRecord();
							tmpRecord->gNode = tmpNode;					
							if (!(nList->contains(tmpNode))) { nList->add(tmpRecord); } else { delete tmpRecord; }
							findNeighbour(mDepth,cDepth,tmpNode,nList, whichActorSprite, dotproduct);	
						}
					}

					// dot product = false; check ALL connections outgoing from given SZ_Node
					if (!dotproduct)
					{					
							SZ_Node* tmpNode = ((SZ_Node*)(whichNode->connection[i]->toNodeAddress));								
							SZ_NodeRecord* tmpRecord;
							tmpRecord = new SZ_NodeRecord();
							tmpRecord->gNode = tmpNode;					
							if (!(nList->contains(tmpNode))) { nList->add(tmpRecord); } else { delete tmpRecord; }
							findNeighbour(mDepth,cDepth,tmpNode,nList, whichActorSprite, dotproduct);
					}
				}
			}
		}
	}
	
}


void SZ_World::Draw()
{	
	if ((Utils.render==0)) {return;}

	//glPushMatrix();		
	glEnable(GL_BLEND);


	
	gCamera.draw();		// draw camera recticle
	//Utils.Render();		// draws cell from pList and recordList (disused)
	//Utils.RenderNodes();// draws each cell (+connections)
	Utils.RenderMap();	// draws the 3d layout based on the physical layer

	glEnable(GL_COLOR_MATERIAL);
	//glDisable(GL_LIGHTING);

	
	// draw the grid
	glTranslatef(0,0.001,0);
	Utils.RenderGrid(1 ,NxVec3(0.1,0.1,0.1),1);
	glTranslatef(0,0.001,0);
	Utils.RenderGrid(5,NxVec3(0.2,0.2,0.2),2);
	glTranslatef(0,0.001,0);
	Utils.RenderGrid(20,NxVec3(0.85,0.0,0.3),3);
	glTranslatef(0,0.001,0);
	glTranslatef(0,0.1,0);


	service.drawNeighbour();	// draws the range of the service
	service.drawQuad();			// draws the 3d logo of the service
	
	PlanesDepart.draw();
	PlanesLanding.draw();


	if (gCamera.mode != gCamera.cmFirstPerson) 
		{
			//Player.drawNeighbour();
			//Player.draw(); 
		}	

	// draws the population of the world onto the world
	for (int i=0;i<ppAmount;i++)
	{
		ppPersons[i]->draw();	
		//Persons[i]->drawNeighbour();
	}
	//glPopMatrix();


	//glRotatef()

	Utils.vVectorList.draw();

	gDatalogger.draw();


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	//glTranslatef(-1,1,0);
	//glScalef(2,2,2);
	glPushMatrix();

	glEnable(GL_BLEND);
	//glTranslatef(-0.5,-0.5,0);
	glRotatef(90,1,0,0);	

	// user interface 
	/*
		SZ_Sprite *sprite  = new SZ_Sprite(2.01f,2.01f,0,0,1200,800,0);	// sizex, sizez, texture: start u, start v, width, length, frames total
		sprite->SetTexture(4);
		sprite->init(Utils.texture_array);
		sprite->draw();
		delete sprite;
	*/
	

	//glPointSize(100);

	// 0,0 = top left corner. 1,1 = bottom right corner
	//glBegin(GL_POINTS);
	//glVertex3f(-1, 0, 0);
	//glVertex3f( 0, 0, 0);
	//glVertex3f( 0, 1, 0);
	//glVertex3f( 0,-1, 0);
	//glVertex3f( 1, -1, 0);
	//glEnd();



	/*


	glBegin(GL_QUADS);
	glVertex3d( -0.5,-0.5, 0 );
	glVertex3d(  0.5,-0.5, 0 );
	glVertex3d(  0.5, 0.5, 0 );
	glVertex3d( -0.5, 0.5, 0 );
	glEnd();
	*/
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_BLEND);
}




void SZ_World::ppEmpty()
{
	for (int i = 0;i<NUM_PERSON;i++)
	{
		if (ppPersons[i]!=NULL)
		{
			ppPersons[i]->actions.currentAction = ppPersons[i]->actions.amQuitting;
			//delete Persons[i];
			//Persons[i]=NULL;
		}
	}
	//ppAmount=0;
}

int SZ_World::ppLength()
{
	return ppAmount;
}


bool SZ_World::ppContains(SZ_Person* whichPerson)
{
	bool found = false;
	int counter = 0;
	while ( (counter<ppAmount)  )
	{
		
		if (ppPersons[counter]==whichPerson)
		{
			found = true;
			counter = ppAmount;
		}
		counter++;

	}
	return found;
}

void SZ_World::ppAdd(SZ_Person* whichPerson)
{
		if (ppAmount <NUM_PERSON)
		{
			{
				ppPersons[ppAmount] = whichPerson;
				ppAmount++;
			}
		
		}
		else { delete whichPerson; printf("Person not created \n"); }
}

void SZ_World::ppSubstract(SZ_Person* whichPerson)
{	
	//if (whichPerson == NULL) { return; }

	// remove it from the cell it is standing on
	if (Utils.gNodes[whichPerson->currentCell]!=NULL)	// make sure I remove from a cell that actually exists
	{	
		Utils.gNodes[whichPerson->currentCell]->PersonList.substract(whichPerson);
	}


	bool found = false;
	int counter = 0;
	while ( (counter<ppAmount)  )
	{
		
		if (ppPersons[counter]==whichPerson)
		{
			//delete NodesRecord[counter];
			ppAmount--;

			ppPersons[counter]= ppPersons[ppAmount];		
			ppPersons[ppAmount] = NULL;
			counter=ppAmount;

			whichPerson->releasePerson(whichPerson);
			delete whichPerson;
			whichPerson = NULL;
			
		}
		counter++;
	}
}



SZ_Person* SZ_World::ppSpawn( int whichCell )
{
	if (ppAmount >= NUM_PERSON ) { return NULL; }

	if (whichCell<=NODES_AMOUNT)
	{
		if (Utils.gNodes[whichCell]!=NULL)
		{			
			// create a new Person.
			SZ_Person* tmpPerson = new SZ_Person();
			tmpPerson->init(wScene,Utils.texture_array,this);
			tmpPerson->putOnCell(whichCell);
								
			ppAdd(tmpPerson);	
			return tmpPerson;			
		}
	}
	return NULL;
}

int SZ_World::sbGetLayerPhysical( int whichNode )
{
	if (Utils.gNodes[whichNode] == NULL)
	{
		return 0;
	} else	
	{
		return 1;
	}
}

NxVec3 SZ_World::sbGetLayerNavigation( int whichNode, int targetNode )
{
	NxVec3 tmp_output;
	tmp_output = Utils.gNodes[whichNode]->connection[Utils.gNodes[whichNode]->pathTo[targetNode]]->cost;
	
	
	return tmp_output;
}

SZ_PersonList* SZ_World::sbGetLayerOccupancy( int whichNode )
{
	return &Utils.gNodes[whichNode]->PersonList;
}

SZ_NumberList* SZ_World::sbGetLayerAreaSearch( SZ_Person* whichPerson )
{
	return &whichPerson->visibleTiles;
}

SZ_FunctionBase* SZ_World::sbGetLayerFunction( int whichNode )
{
	return (SZ_FunctionBase*)(Utils.gNodes[whichNode]->functions);
}


