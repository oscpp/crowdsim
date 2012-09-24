#ifndef SZMOUSE_H
#define SZMOUSE_H

#include ".\..\..\general_utils.h"
#include ".\SZ_Config.h"

class SZ_World;

class SZ_Mouse
{
private:
	void onLeftMouse();
	void onRightMouse();
	void onMiddleMouse();

	int mx, my;
public:
	 SZ_Mouse();
	~SZ_Mouse();
	SZ_World* world;

	void init(void *world);
	void Update();		//called every frame, checks the state of mouse buttons

	int gMouseButton[MAX_KEYS];

	void* tmp_person;	// used in left mouse click, to store a pointer towards the last created person

	// GLUT CALLBACKS

	void MotionCallback(int x, int y);
	void MousePassiveCallback(int x, int y);
	void MouseCallback(int button, int state, int x, int y);

};

#endif