#ifndef SZSPRITE_H
#define SZSPRITE_H

#include "..\..\general_utils.h"
#include "SZ_Config.h"


class SZ_Sprite
{

	private:

		int u,v;					// offset from the texture top left
		int w,h;					// width and height of the used texture
		int FCountMax;				// Count to this value for frame change
		int FCount;					// Frame Counter
		int NFrames;				// number of animation frames
		float sizeX, sizeY;			// size of the polygon that the sprite is being drawn on.
		float planeTexcoords[8];
		float planecoords[12];
		float planeNormalcoords[12];

	public:
		 SZ_Sprite();
		 SZ_Sprite(float sizeX, float sizeY, int u, int v, int w, int h, int NFrames);
		~SZ_Sprite();

		float offsetx,offsety;		// offset of the 2dquad.
		void setOffset(float x, float y);
		void setUVSprite(int u, int v);
		void setWHSprite(int w, int h);
		void setFrames(int Frames);
		void init(unsigned int texture_array[]);
		void draw();
		void animateSprite(float Count);
		float SizeX() { return sizeX; }
		float SizeY() { return sizeY; }
		void SetTexture(int whichTexture) { this->whichTexture = whichTexture; }

		int whichTexture;			// int value of the texture in the texture_array for this Sprite
		unsigned int *textureArray; // pointer to which texture array is used.
		GLint texWidth, texHeight;  // height and width of the whole loaded texture

};

#endif