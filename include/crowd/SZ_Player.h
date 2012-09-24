#ifndef SZPLAYER_H
#define SZPLAYER_H

#include "SZ_ActorSprite.h"

class SZ_Player: public SZ_ActorSprite
{
	public:
	 SZ_Player();
	~SZ_Player();

	void update();
	void onCreate();	// called after the initialisation of the sprite and making of PhysX
	void init(NxScene *whichScene,unsigned int texture_array[], void *world);  // creates an actorsprite with standard texture and standard size.

	//keyboard functions
	void keyboardDownCall(int key,int x,int y);
	void keyboardUpCall(int key,int x,int y);
	void keyboardSpecialDownCall(int key,int x,int y);
	void keyboardSpecialUpCall(int key,int x,int y);
	void processKeys(int gKeys, float delta);
	void processKeysSpecial(int gKeysSpecial, float delta);


};

#endif

