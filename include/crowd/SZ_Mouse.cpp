#include "SZ_Mouse.h"
#include "SZ_World.h"
#include "SZ_Person.h"

SZ_Mouse::SZ_Mouse()
{
	tmp_person = NULL;
}

SZ_Mouse::~SZ_Mouse()
{

}


// left mouse HOLD PRESSED
void SZ_Mouse::onLeftMouse()
{
	SZ_Person* tmp_oldperson;
	tmp_oldperson = ((SZ_Person*)(tmp_person));


	tmp_person = world->ppSpawn(world->gCamera.RecticleCell);

	if ( (tmp_oldperson != NULL) && ( tmp_person !=NULL ) )
	{
		printf("\n");
		printf("new person %i \n", ((SZ_Person*)(tmp_person))->personIndex);
		printf("connect old: %i with n:%i \n", tmp_oldperson->personIndex, ((SZ_Person*)(tmp_person))->personIndex);
		printf("old has %i connections \n", tmp_oldperson->ggIntraList->amount);

		((SZ_Person*)(tmp_person))->ggIntraList->NewSocialConnectionIntraALL(tmp_oldperson);
		((SZ_Person*)(tmp_person))->actions = tmp_oldperson->actions;


		/*
		// new person becomes friend with last created person
		((SZ_Person*)(tmp_person))->ggIntraList->NewSocialConnectionIntra(tmp_oldperson);

		
		// attach to group. new person becomes friend with friends of old person
		for (int i=0; i< tmp_oldperson->ggIntraList->amount;i++)  // go through all the social relationship of old person
		{
			SZ_Person* tmp_oldpersonFriend;
			tmp_oldpersonFriend = tmp_oldperson->ggIntraList->SocialIntraRecord[i]->toPerson; // find all the SZ_Persons connected to oldperson
			printf("old person is friend with %i \n", tmp_oldpersonFriend->personIndex);
			if (tmp_oldpersonFriend == tmp_person) { printf("found myself %i \n", tmp_oldpersonFriend->personIndex ); continue; }		// if SZ_Person returns to new created person, ignore
			((SZ_Person*)(tmp_person))->ggIntraList->NewSocialConnectionIntra(tmp_oldpersonFriend);	// connect friend of friend with new person

		}
		*/
		
		
	}
}

// right mouse HOLD PRESSED
void SZ_Mouse::onRightMouse()
{
	//printf("Right \n");
}

// middle mouse HOLD PRESSED
void SZ_Mouse::onMiddleMouse()
{
	//printf(" middle \n");
}

// called every gWorld update cycle
void SZ_Mouse::Update()
{
	
	if (gMouseButton[GLUT_LEFT_BUTTON])  { onLeftMouse();  }
	if (gMouseButton[GLUT_RIGHT_BUTTON]) { onRightMouse(); }
	if (gMouseButton[GLUT_MIDDLE_BUTTON]){ onMiddleMouse();}
}

void SZ_Mouse::init( void *world )
{
	this->world = ((SZ_World*)(world));

}

//When a user presses and releases mouse buttons in the window, each press and each release generates a mouse callback.
void SZ_Mouse::MouseCallback( int button, int state, int x, int y )
{
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW); // hide mouse cursor
	int dx = mx - x;
	int dy = my - y;

	mx = x;
	my = y;

	world->Utils.MouseWinCoord.x = dx;
	world->Utils.MouseWinCoord.y = dy;


	// button hold/press logic
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{		
			gMouseButton[GLUT_LEFT_BUTTON] = true;
		}
		if (state == GLUT_UP)
		{
			gMouseButton[GLUT_LEFT_BUTTON] = false;
			this->tmp_person = NULL;	// on release, forget who i was pointing at (groups)
		}
	}

	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			gMouseButton[GLUT_RIGHT_BUTTON] = true;
		}
		if (state == GLUT_UP)
		{
			gMouseButton[GLUT_RIGHT_BUTTON] = false;
		}
	}

	if (button == GLUT_MIDDLE_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			gMouseButton[GLUT_MIDDLE_BUTTON] = true;
		}
		if (state == GLUT_UP)
		{
			gMouseButton[GLUT_MIDDLE_BUTTON] = false;
		}
	}
}


//  The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed

void SZ_Mouse::MousePassiveCallback( int x, int y )
{		
	int dx = mx - x;
	int dy = my - y;

	mx = x;
	my = y;

	// move camera with no button pressed
	// world->gCamera.MotionCallBack(dx, dy);
	// world->Utils.MouseWinCoord.x = dx;
	// world->Utils.MouseWinCoord.y = dy;


}

// The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed
void SZ_Mouse::MotionCallback( int x, int y )
{
	int dx = mx - x;
	int dy = my - y;

	mx = x;
	my = y;

	if (gMouseButton[GLUT_RIGHT_BUTTON])  
	{ 
		int centerX = (float)glutGet(GLUT_WINDOW_WIDTH) / 2.0;
		int centerY = (float)glutGet(GLUT_WINDOW_HEIGHT) / 2.0;

		//int deltaX = (x - centerX);
		//int deltaY = (y - centerY);

		//mouseX += deltaX / (float)glutGet(GLUT_WINDOW_WIDTH);
		//mouseY -= deltaY / (float)glutGet(GLUT_WINDOW_HEIGHT);

		//glutWarpPointer( centerX, centerY );

		glutSetCursor(GLUT_CURSOR_NONE); // hide mouse cursor
		world->gCamera.MotionCallBack(dx, dy);
		world->Utils.MouseWinCoord.x = dx;
		world->Utils.MouseWinCoord.y = dy;
	}

	
}
