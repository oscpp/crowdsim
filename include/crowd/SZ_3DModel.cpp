#include "SZ_3DModel.h"
#include "SZ_ActorSprite.h"
#include "SZ_World.h"

SZ_3DModel::SZ_3DModel()
{
	whichTexture = 2;		// default texture used = 2
}

SZ_3DModel::~SZ_3DModel()
{
}


void SZ_3DModel::draw()
{

	// draw the body 
	NxMat34 pose = parent->entity->getGlobalPose();
	SetupGLMatrix(pose.t, pose.M);
	// draw body+legs
	glNormalPointer(GL_FLOAT, 0, body_normals);
	glTexCoordPointer(2,GL_FLOAT, 2*sizeof(GL_FLOAT),body_texcoord);
    glVertexPointer(3, GL_FLOAT, 0, body_vertices);

	glScalef(size.x,size.y,size.z);
	glTranslatef(0,4.6,0); // move the models slightly above the ground	
	glDrawElements(GL_QUADS, 40, GL_UNSIGNED_BYTE, body_indices); 

	SetupGLMatrix(NxVec3(0,0,0), head_matrix33);
	glNormalPointer(GL_FLOAT, 0, head_normals);
	glTexCoordPointer(2,GL_FLOAT, 2*sizeof(GL_FLOAT),head_texcoord);
	glVertexPointer(3, GL_FLOAT, 0, head_vertices);
	glDrawElements(GL_QUADS, 20, GL_UNSIGNED_BYTE, head_indices); 
}


void SZ_3DModel::passSize(NxVec3 size)
{
	this->size = size;
}

float SZ_3DModel::getSize()
{
	return this->size.magnitude();
}


// initialises the model with all the verticles, normals, texture UV values etc.
void SZ_3DModel::init(unsigned int texture_array[], void *world, SZ_ActorSprite* myParent)
{
	parent = myParent;
	this->textureArray = texture_array;
	GLfloat tmp_bodyvertices[] = {  

	// 0.5 = shoulder, 
	1+0.5, 1-2  ,1,    -1-0.5, 1-2, 1,  -1-0.5,-1-2.3, 1,    1+0.5,-1-2.3, 1,        // v0-v1-v2-v3 front
	1+0.5, 1-2   ,-1,     1+0.5, 1-2, 1,   1+0.5,-1-2.3, 1,    1+0.5,-1-2.3,-1,				 // v0-v3-v4-v5  right
	1+0.5, 1-2   ,-1,    -1-0.5, 1-2,-1,  -1-0.5, 1-2, 1,    1+0.5, 1-2, 1,           // v0-v5-v6-v1  top
   -1-0.5, 1-2   ,1,    -1-0.5, 1-2,-1,  -1-0.5,-1-2.3,-1,   -1-0.5,-1-2.3, 1,      // v1-v6-v7-v2   left 
   //-1-.05,-1-2.3 ,-1,    1+0.5 ,-1-2.3,-1,   1+0.5,-1-2.3, 1,   -1-0.5,-1-2.3, 1,			// v7-v4-v3-v2   bottom
   -1-0.5, 1-2   ,-1,    1 +0.5, 1-2,-1,   1+0.5,-1-2.3,-1,   -1-0.5,-1-2.3,-1,      // v4-v7-v6-v5   behind

    // legs
	1,1-4.3,1,    -1,1-4.3,1,   -1,-1-4.3,1,    1,-1-4.3,1,        // v0-v1-v2-v4.3
	1,1-4.3,-1,    1,1-4.3,1,    1,-1-4.3,1,    1,-1-4.3,-1,    // v0-v4.3-v4-v5
	1,1-4.3,-1,   -1,1-4.3,-1,  -1,1-4.3,1,     1,1-4.3,1,        // v0-v5-v6-v1
   -1,1-4.3,1,    -1,1-4.3,-1,  -1,-1-4.3,-1,   -1,-1-4.3,1,    // v1-v6-v7-v2
  // -1,-1-4.3,-1,  1,-1-4.3,-1,   1,-1-4.3,1,    -1,-1-4.3,1,    // v7-v4-v4.3-v2
   -1,1-4.3,-1,   1,1-4.3,-1,    1,-1-4.3,-1,   -1,-1-4.3,-1  // v4-v7-v6-v5
					};  

	GLfloat tmp_headvertices[] = {
		//head
	 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,  // v0-v1-v1-v3 
	 1, 1,-1,   1, 1, 1,   1,-1, 1,   1,-1,-1,  // v0-v3-v4-v5
	 1, 1,-1,  -1, 1,-1,  -1, 1, 1,   1, 1, 1,  // v0-v5-v6-v1
	-1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1,  // v1-v6-v7-v1
	//-1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1,  // v7-v4-v3-v1
	-1, 1,-1,   1, 1,-1,   1,-1,-1,  -1,-1,-1   // v4-v7-v6-v5
	};

	GLfloat tmp_headnormals[] = {
	//head
	 0, 0, 1,   0, 0, 1,   0, 0, 1,  0, 0, 1,
	 1, 0, 0,   1, 0, 0,   1, 0, 0,  1, 0, 0,            
	 0, 1, 0,   0, 1, 0,   0, 1, 0,  0, 1, 0,             
	-1, 0, 0,  -1, 0, 0,  -1, 0, 0, -1, 0, 0,     
	 //0,-1, 0,   0,-1, 0,   0,-1, 0,  0,-1, 0,     
	 0, 0,-1,   0, 0,-1,   0, 0,-1,  0, 0,-1
	};

	GLubyte tmp_headindices[] = {
	0,1,2,3,4,5,6,7,8,9,10,11,
	12,13,14,15,16,17,18,19,
	20 //,21,22,23,24
	};

	GLfloat tmp_bodynormals[] = {
	 0,0,1,  0,0,1,  0,0,1,  0,0,1,             // v0-v1-v2-v3
	 1,0,0,  1,0,0,  1,0,0, 1,0,0,              // v0-v3-v4-v5
	 0,1,0,  0,1,0,  0,1,0, 0,1,0,              // v0-v5-v6-v1
	 -1,0,0,  -1,0,0, -1,0,0,  -1,0,0,          // v1-v6-v7-v2
	 //0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,         // v7-v4-v3-v2
	 0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1,

	 0,0,1,  0,0,1,  0,0,1,  0,0,1,             // v0-v1-v2-v3
	 1,0,0,  1,0,0,  1,0,0, 1,0,0,              // v0-v3-v4-v5
	 0,1,0,  0,1,0,  0,1,0, 0,1,0,              // v0-v5-v6-v1
	 -1,0,0,  -1,0,0, -1,0,0,  -1,0,0,          // v1-v6-v7-v2
	// 0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,         // v7-v4-v3-v2
	 0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1
	};

	// indices for the body.
	GLubyte tmp_bodyindices[] = {
	0,1,2,3,4,5,6,7,8,9,10,11,
	12,13,14,15,16,17,18,19,
	20,21,22,23,24,25,26,27,
	28,29,30,31,32,33,34,35,
	36,37,38,39 //,40,41,42,43,
	//44,45,46,47
	};

	// texture information
	glBindTexture(GL_TEXTURE_2D, texture_array[whichTexture]);	
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);   // Returns the width of TEXTURE_2D and stores it in texWidth
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight); // Returns the heigh of TEXTURE_2D and sotres it in texHeigh

	// set the default texture.
	setTexture(rand()%8,rand()%8,rand()%8);
	//setTexture(2,2,2);		

	memcpy(this->body_vertices,tmp_bodyvertices,40*3*sizeof(GLfloat));
	memcpy(this->body_normals, tmp_bodynormals,40*3*sizeof(GLfloat));
	memcpy(this->body_indices, tmp_bodyindices,40*sizeof(GLfloat));


	memcpy(this->head_vertices,tmp_headvertices,20*3*sizeof(GLfloat));
	memcpy(this->head_normals, tmp_headnormals,20*3*sizeof(GLfloat));
	memcpy(this->head_indices, tmp_headindices,20*sizeof(GLfloat));

	head_matrix33.id();
}

void SZ_3DModel::setTexture(int face, int body, int legs)
{
	GLfloat w = 32;
	GLfloat h = 32;

	GLfloat tmp_headtexUV[] = { 	
	0,0 +(face*32),   // front
	32,0+(face*32),   // right
	96,0+(face*32),   // top
	32,0+(face*32),   // left
	//32,0+(face*32),   // bottom
	64,0+(face*32),   // behind
	};

	GLfloat tmp_headtexcoord[] = {	
	//front
	(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[0]),
	(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[1]),
	(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[0]),
	(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[1]),
	(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[0]),
	(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[1]),
	(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[0]),
	(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[1]),

	//right
	(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[2]),
	(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[3]),
	(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[2]),
	(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[3]),
	(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[2]),
	(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[3]),
	(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[2]),
	(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[3]),

	//top
	(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[4]),
	(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[5]),
	(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[4]),
	(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[5]),
	(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[4]),
	(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[5]),
	(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[4]),
	(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[5]),

	//left
	(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[6]),
	(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[7]),
	(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[6]),
	(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[7]),
	(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[6]),	
	(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[7]),
	(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[6]),
	(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[7]),


	
	//bottom
	(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[8]),
	(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[9]),
	(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[8]),
	(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[9]),
	(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[8]),
	(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[9]),
	(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[8]),
	(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[9]),
	


	/*
	//behind
	(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[10]),
	(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[11]),
	(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[10]),
	(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[11]),
	(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[10]),
	(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[11]),
	(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_headtexUV[10]),
	(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_headtexUV[11]),
	*/
	};

	// used to set the UV information for the textures (in pixelformat)
	GLfloat tmp_bodytexUV[] = { 	//head
								/*
								0,0+(face*32),   // front
								32,0+(face*32),   // right
								96,0+(face*32),   // top
								32,0+(face*32),   // left
								32,0+(face*32),   // bottom
								64,0+(face*32),   // behind
								*/
								

								//body
								128,0+(body*32),   // front
								160,0+(body*32),   // right
								224,0+(body*32),   // top
								160,0+(body*32),   // left
								//128,0+(body*32),   // bottom
								192,0+(body*32),   // behind

								//legs
								256,0+(legs*32),   // front
								288,0+(legs*32),   // right
								352,0+(legs*32),   // top
								288,0+(legs*32),   // left
								//256,0+(legs*32),   // bottom
								320,0+(legs*32),   // behind
						  };


	GLfloat tmp_bodytexcoord[] = {	
		
							//body
								//front
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[0]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[1]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[0]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[1]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[0]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[1]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[0]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[1]),

								//right
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[2]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[3]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[2]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[3]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[2]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[3]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[2]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[3]),

								//top
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[4]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[5]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[4]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[5]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[4]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[5]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[4]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[5]),

								//left
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[6]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[7]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[6]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[7]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[6]),	
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[7]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[6]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[7]),
							
								
								//bottom
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[8]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[9]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[8]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[9]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[8]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[9]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[8]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[9]),
								
								
								
								//behind
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[10]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[11]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[10]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[11]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[10]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[11]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[10]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[11]),
									

							//legs
								//front
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[12]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[13]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[12]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[13]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[12]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[13]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[12]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[13]),

								//right
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[14]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[15]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[14]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[15]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[14]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[15]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[14]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[15]),

								//top
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[16]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[17]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[16]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[17]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[16]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[17]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[16]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[17]),

								//left
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[18]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[19]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[18]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[19]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[18]),	
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[19]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[18]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[19]),

					
								/*
								//bottom
								
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[20]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[21]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[20]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[21]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[20]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[21]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[20]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[21]),
								
								
								//behind
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[22]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[23]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[22]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[23]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[22]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[23]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[22]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[23]),
								*/
								/*
							//legs
								//front
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[24]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[25]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[24]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[25]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[24]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[25]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[24]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[25]),

								//right
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[26]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[27]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[26]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[27]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[26]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[27]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[26]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[27]),

								//top
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[28]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[29]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[28]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[29]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[28]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[29]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[28]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[29]),

								//left
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[30]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[31]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[30]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[31]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[30]),	
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[31]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[30]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[31]),


								
								//bottom
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[32]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[33]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[32]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[33]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[32]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[33]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[32]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[33]),
								
								
								//behind
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[34]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[35]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[34]),
								(0*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[35]),
								(0*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[34]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[35]),
								(1*(1.0/float(texWidth) *w))+(1.0/float(texWidth) *tmp_bodytexUV[34]),
								(1*(1.0/float(texHeight)*h))+(1.0/float(texHeight)*tmp_bodytexUV[35]),
								*/
								};

	memcpy(this->body_texcoord,tmp_bodytexcoord,(216-48)*2*sizeof(GLfloat)); // edited 25/07/12 to remove heads
	memcpy(this->head_texcoord,tmp_headtexcoord,(40)*2*sizeof(GLfloat)); // edited 25/07/12 heads are separate
}



void SZ_3DModel::head_setDirection( NxVec3 direction )
{
	// Turn the head in Y direction towards the camera
	head_matrix33 = parent->entity->getGlobalOrientation();	
	NxVec3 L1;
	L1 = direction;
	NxVec3 p = parent->entity->getGlobalPosition();
	L1 = L1 - p ;

	NxVec3 L2;
	L2 = parent->getDirection();
	NxVec3 L3;
	L3 = L2;

	//2d cross product with forward - vector left of person
	L3.x = L2.z;
	L3.z = -L2.x;

	NxReal angle;
	angle = NxMath::acos( L1.dot(L2) / (L1.magnitude()*L2.magnitude()));
	angle = NxMath::radToDeg(angle);

	if (angle > 90) { head_matrix33.id(); return; }

	if (L3.dot(L1)<0) { angle = -angle; }
	//NxQuat head_quat(head_matrix);
	NxQuat	quat( angle, NxVec3(0,1,0));
	head_matrix33.fromQuat(quat);
}

void SZ_3DModel::head_setIdentity()
{
	head_matrix33.id();
}
