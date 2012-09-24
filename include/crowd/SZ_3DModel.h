#ifndef SZ3DMODEL_H
#define SZ3DMODEL_H

#include "..\..\general_utils.h"

#include "SZ_Config.h"

class SZ_ActorSprite;

class SZ_3DModel
{
	private:
		GLfloat body_vertices[64*3];
		GLfloat body_indices[64];
		GLfloat body_texcoord[200*2];
		GLfloat body_normals[64*3];


		GLfloat head_vertices[68*3];
		GLfloat head_indices[68];
		GLfloat head_texcoord[208*2];
		GLfloat head_normals[68*3];

		SZ_ActorSprite* parent;		// pointer towards the actorsprite i belong to
		void *world;				// pointer to the world i belong to
		GLint texWidth;
		GLint texHeight;

		NxVec3 size;				// stores size;		
	public:
		 SZ_3DModel();
		~SZ_3DModel();
		 NxMat33 head_matrix33;
		 int whichTexture;			 // int value of the texture in the texture_array for this model
		 unsigned int *textureArray; // pointer to which texture array is used.

		void  draw();				 // draws the model; body and head
		void  setTexture(int face,int body, int legs);
		void  init(unsigned int texture_array[], void *world, SZ_ActorSprite* myParent);
		void  passSize(NxVec3 size);	// passes size
		float getSize();
		void  head_setDirection(NxVec3 direction); // sets the direction of the head in a given direction (sets head_matrix33)
		void  head_setIdentity();

};

#endif