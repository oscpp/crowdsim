#include "SZ_Sprite.h"



SZ_Sprite::~SZ_Sprite()
{
}

SZ_Sprite::SZ_Sprite(float sizeX, float sizeY, int u, int v, int w, int h, int NFrames)
{	
	// coordinates for the texture, the plane and the normals.
	planeTexcoords[0] = 1; 	planecoords[0]  = 1; 	planeNormalcoords[0]  =0;
	planeTexcoords[1] = 0;	planecoords[1]  = 0; 	planeNormalcoords[1]  =-1;
	planeTexcoords[2] = 0; 	planecoords[2]  = 0; 	planeNormalcoords[2]  =0;
	planeTexcoords[3] = 0; 	planecoords[3]  = 0; 	planeNormalcoords[3]  =-1;
	planeTexcoords[4] = 0; 	planecoords[4]  = 0; 	planeNormalcoords[4]  =0;
	planeTexcoords[5] = 1; 	planecoords[5]  = 0; 	planeNormalcoords[5]  =-1;
	planeTexcoords[6] = 1; 	planecoords[6]  = 0;  	planeNormalcoords[6]  =0;
	planeTexcoords[7] = 1; 	planecoords[7]  = 0; 	planeNormalcoords[7]  =-1;
							planecoords[8]  = 1; 	planeNormalcoords[8]  =0;
							planecoords[9]  = 1; 	planeNormalcoords[9]  =-1;
							planecoords[10] = 0; 	planeNormalcoords[10] =0;
							planecoords[11] = 1; 	planeNormalcoords[11] =-1;
	this->u = u;
	this->v = v;
	this->w = w;
	this->h = h;
	this->NFrames = NFrames;
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->whichTexture = 0;	// default texture in array used used = 0
}

SZ_Sprite::SZ_Sprite()
{
	u = 96;					// offset from the texture top left (on X)   (96)
	v = 0;					// offset from the texture top left (on Y)   (0)

	w = 32;					// width  of the texture                     (32)
	h = 32;                 // height of the texture                     (32)

	// Animation parameters
	//FCountMax = 1;	
	FCount = 0;
	NFrames = 8;

	sizeX = 1;		// default size of the polygon 1x1
	sizeY = 1;

	whichTexture = 0;		// default texture used = 0
	offsetx = 0;
	offsety = 0;
	
}



void SZ_Sprite::setUVSprite(int u, int v)
{
	this->u = u;
	this->v = v;




	// Set the new UV settings
	planeTexcoords[0] = 1.0;
	planeTexcoords[1] = 0.0;
	planeTexcoords[2] = 0.0;
	planeTexcoords[3] = 0.0;
	planeTexcoords[4] = 0.0;
	planeTexcoords[5] = 1.0;
	planeTexcoords[6] = 1.0;
	planeTexcoords[7] = 1.0;

	int i, j;
	for (i=0,j=1; i<8; i+=2,j+=2)
	{
		planeTexcoords[i] = (planeTexcoords[i]*(1.0/float(texWidth) *w))+(1.0/float(texWidth)*u);
		planeTexcoords[j] = (planeTexcoords[j]*(1.0/float(texHeight)*h))+(1.0/float(texWidth)*v);
	}

}
void SZ_Sprite::setWHSprite(int w, int h)
{
	this->w = w;
	this->h = h;
}

void SZ_Sprite::setFrames(int Frames)
{
	this->NFrames = Frames;
}


void SZ_Sprite::init(unsigned int texture_array[])
{ 
	textureArray = texture_array;			// saves which texture array this sprite is using.
	glBindTexture(GL_TEXTURE_2D, texture_array[whichTexture]);	
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);   // Returns the width of TEXTURE_2D and stores it in texWidth
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight); // Returns the heigh of TEXTURE_2D and sotres it in texHeigh

	int i, j;
	for (i=0,j=1; i<8; i+=2,j+=2)	// Move the texture UV to what is specified in the uvwh variables
	{
		planeTexcoords[i] = (planeTexcoords[i]*(1.0/float(texWidth) *w))+(1.0/float(texWidth)*u);
		planeTexcoords[j] = (planeTexcoords[j]*(1.0/float(texHeight)*h))+(1.0/float(texWidth)*v);
	}

	for (i=0,j=2; i<12; i+=3,j+=3)	// Move the texture UV to what is specified in the uvwh variables
	{
	planecoords[i] = (planecoords[i])*sizeX; 
	planecoords[j] = (planecoords[j])*sizeY; 
	}
}

void SZ_Sprite::animateSprite(float Count)
{

	// Calculate the new UV settings according to the current frame
	int FCountMax = 0 - (int)(NFrames * Count);
	if((this->FCount++)%FCountMax == 0)
		{
			this->v += this->h;
			if(this->v > (this->NFrames - 1) * this->h)this->v = 0;
		}
	
	// Set the new UV settings
	planeTexcoords[0] = 1.0;
	planeTexcoords[1] = 0.0;
	planeTexcoords[2] = 0.0;
	planeTexcoords[3] = 0.0;
	planeTexcoords[4] = 0.0;
	planeTexcoords[5] = 1.0;
	planeTexcoords[6] = 1.0;
	planeTexcoords[7] = 1.0;

	int i, j;
	for (i=0,j=1; i<8; i+=2,j+=2)
	{
		planeTexcoords[i] = (planeTexcoords[i]*(1.0/float(texWidth) *w))+(1.0/float(texWidth)*u);
		planeTexcoords[j] = (planeTexcoords[j]*(1.0/float(texHeight)*h))+(1.0/float(texWidth)*v);
	}
}

void SZ_Sprite::draw()
{
	glPushMatrix();
		glDisable(GL_LIGHTING);
		
		glDisable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//glColor4f(R/255.0,G/255.0,B/255.0,A/255.0); // colour modifier of the texture.
		//glColor4f(255,255,255,255.0); // colour modifier of the texture.
 	


		
		glTranslatef(-SizeX()/2.0f,0,-SizeY()/2.0f);

			
		glTranslatef(offsetx,0.001f,offsety);
		
		

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		
		glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), planecoords);
		glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat), planeNormalcoords);
		glTexCoordPointer(2,GL_FLOAT, 2*sizeof(float),planeTexcoords);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureArray[whichTexture]);

		glDrawArrays(GL_QUADS, 0, 4);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisable(GL_TEXTURE_2D);
		//glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);

	glPopMatrix();


}

void SZ_Sprite::setOffset( float x, float y )
{
	offsetx = x;
	offsety = y;
}
