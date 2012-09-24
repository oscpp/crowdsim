#include "SZ_Camera.h"
#include "SZ_World.h"

/*-----------------------------------------------------------
  SZ_Camera class members
  -----------------------------------------------------------*/
SZ_Camera::SZ_Camera()
{
	
	AspectRatio = 1.0;
	Pos = NxVec3(3,20,24.5);
	//Forward = NxVec3(-0.05,-0.5,-0.8);

	Right = NxVec3(1,0,0);
	Up = NxVec3(0,1,0);
	Speed = CAM_SPEED;
	mode = cmFree;
	actorToTrack = NULL;

	projection = cpPerspective;
	
}

// resets the camera to it's starting position;
void SZ_Camera::reset()
{
	AspectRatio = 1.0;
	Pos = NxVec3(3,20,24.5);
	Pos = NxVec3(100,15,140); 
	//Forward = NxVec3(-1, -0.30, -0.95);
	//Forward = NxVec3(0.7f,-0.3f, 0.6f);
	Forward = NxVec3(0.0, -0.40, -1.0);

	//Right = NxVec3(1,0,0);
	//Up = NxVec3(0,1,0);
	Speed = CAM_SPEED;
	mode = cmFree;
	actorToTrack = NULL;

	projectionPerspZoom =  1;
	projectionOrthoZoom = 10;
}

void SZ_Camera::Update()
{
	//this->entity->setGlobalPosition(Pos);	//position of the physX object = position of camera
	
	switch (mode)
	{
		case cmFree:
			break;
		case cmOffsetPOV:
				if (actorToTrack==NULL) { mode=cmFree; break; }
				Pos			= actorToTrack->getGlobalPosition();
				Pos.y		+= 15; 
				Forward		= actorToTrack->getGlobalOrientation().getColumn(2);
				Forward.y	= (-0.9);
				Forward.x	*= 0.3;
				Forward.z	*= 0.3;
			break;
		case cmFirstPerson:
				if (actorToTrack==NULL) { mode=cmFree; break; }
				Pos			= actorToTrack->getGlobalPosition()+NxVec3(0,0,0);
				Pos.y		+= 2.0; // 0.3 				

				Forward		= actorToTrack->getGlobalOrientation().getColumn(2);
				Forward.y	= (-0.9); // -0.9
				Forward.x	*= 5.0;
				Forward.z	*= 5.0;				
			break;
	}
}

void SZ_Camera::draw()
{	

	//printf("gCamera Position %4.2f %4.2f %4.2f \n", Pos.x, Pos.y, Pos.z);
	//printf("gCamera Looking: %4.2f %4.2f %4.2f \n", Forward.x, Forward.y, Forward.z);


	if (RENDER_CAMERA_RECTILE == 0) { return; }	// don't draw the rectile if option set.
	
	NxVec3 N = NxVec3(0,1,0);
	NxVec3 S = NxVec3(Pos.x, Pos.y, Pos.z);
	NxVec3 V = Forward;
	double t = (-(N.dot(S)))  / N.dot(V);	// no distance from origin, as plane is on origin.

	NxVec3 coll = S + (t*V);
	if ( (coll.x<0) || (coll.z<0)) { return; }
	if ( (coll.x>(WORLD_X)) || (coll.z>(WORLD_Z)) ) { return; }
	RecticlePoint = coll;

	// transform collision point to cell
	int g = ((coll.x) / WORLD_TILE);
	int g2 = ((coll.z) / WORLD_TILE );
	int randomnode = g+(g2*WORLD_X_5X5);

	RecticleCell = randomnode;
	if ((((SZ_World*)(world))->Utils.gNodes[randomnode]) == NULL) { return; }
	int X = (randomnode % WORLD_X_5X5);	  // transform the tile value into the top left X corner
	int Z = (randomnode-X)/WORLD_X_5X5;   // transform the tile value into the top left Y corner		



		int g3 = Pos.x / WORLD_TILE;
		int g4 = Pos.z / WORLD_TILE ;
		currentCell = g3+(g4*WORLD_X_5X5);


	
	glColor4f(0.9,0.1,0,1);
	glLineWidth(4);
	glBegin(GL_LINE_LOOP);
			glVertex3f(X*WORLD_TILE				,0.04f,Z*WORLD_TILE);
			glVertex3f(X*WORLD_TILE + WORLD_TILE,0.04f,Z*WORLD_TILE);
			glVertex3f(X*WORLD_TILE	+ WORLD_TILE,0.04f,Z*WORLD_TILE+WORLD_TILE);
			glVertex3f(X*WORLD_TILE				,0.04f,Z*WORLD_TILE+WORLD_TILE);
	glEnd();

	// draw reticle at point of contact with plane
	const float a = 0.2f;
    const float b = 0.5f;

	glPushMatrix();
	glTranslatef(coll.x,0,coll.z);
	glRotatef(90,1,0,0);
	glColor4f(0,0,0,1);
	glDisable(GL_LIGHTING);

	glLineWidth(2);
	glColor4f(1,1,1,1);
	glBegin(GL_LINES);
		glVertex3f(a,0,0);
		glVertex3f(b,0,0);
		glVertex3f(0,a,0);
		glVertex3f(0,b,0);
		glVertex3f(-a,0,0);
		glVertex3f(-b,0,0);
		glVertex3f(0,-a,0);
		glVertex3f(0,-b,0);
	glEnd();
	glPopMatrix();
}

//cmFree, cmOffsetPOV, cmFirstPerson
char* SZ_Camera::modeName()
{
    switch (mode)
    {
    case  cmFree:               return "free view";                break;
    case  cmOffsetPOV:			return "Fixed distance offset"; break;
    case  cmFirstPerson:		return "first person";    break;
    default:                    return "?";
    }
}


void SZ_Camera::ProcessKeys(int keys, float delta)
{

	if (delta <= 0.005 ) { delta = 0.03; } // delta change, for instance simulation is paused.

	{	
		switch (keys)
		{
			case CAM_FORWARD:		{ Pos += Forward*Speed*delta; MotionCallBack(0,0); break;  }
			case CAM_BACKWARD:		{ Pos -= Forward*Speed*delta; MotionCallBack(0,0); break; }
			case CAM_STRAFE_LEFT:	{ Pos -= Right*Speed*delta;   MotionCallBack(0,0); break; }
			case CAM_STRAFE_BACK:	{ Pos += Right*Speed*delta;   MotionCallBack(0,0); break; }
			case CAM_DOWN:			{ Pos -= NxVec3(0,1,0)*Speed*delta; MotionCallBack(0,0); break; }
			case CAM_UP:			{ Pos += NxVec3(0,1,0)*Speed*delta; MotionCallBack(0,0); break; }
			case CAM_STATS:			{ this->printCurrentCellInfo(); break;}
			case CAM_ZOOM_IN:		{ printf("in %f %f\n", projectionOrthoZoom, projectionPerspZoom );  projectionOrthoZoom+=0.05f; projectionPerspZoom+=0.50f;  break; }
			case CAM_ZOOM_OUT:		{ printf("out %f %f\n", projectionOrthoZoom, projectionPerspZoom ); projectionOrthoZoom-=0.05f; projectionPerspZoom-=0.50f; break; }
		}
	}
}


void SZ_Camera::MotionCallBack(int x, int y)
{

	switch (mode)
	{
		case cmFree:
			Forward.normalize();
			Right.cross(Forward,NxVec3(0,1,0));
			NxQuat qx(NxPiF32 * x * 20.0f / 180.0f, NxVec3(0,1,0));
			qx.rotate(Forward);
			NxQuat qy(NxPiF32 * y * 20.0f / 180.0f, Right);
			qy.rotate(Forward);
		break;
	}
}

void 
SZ_Camera::selectNextMode (void)
{
    mode = successorMode (mode);
    if (mode >= cmEndMode) mode = successorMode (cmStartMode);
}


// ----------------------------------------------------------------------------
// cycles through the various camera modes
SZ_Camera::cameraMode SZ_Camera::successorMode (const cameraMode cm) const
{
    return (cameraMode)(((int)cm) + 1);
}


void SZ_Camera::Init(NxScene* whichScene) //creates a Area using certain parameters
{
	this->reset();
}

void SZ_Camera::printCurrentCellInfo()
{
	if (((SZ_World*)(world))->Utils.gNodes[currentCell] == NULL) { return; }
	int pplamount = ((SZ_World*)(world))->Utils.gNodes[currentCell]->PersonList.amount;

	printf("currentCell : %i ", currentCell);

	
	for (int i=0; i< ((SZ_World*)(world))->Utils.gNodes[currentCell]->amountConnections; i++)
	{	
		//((SZ_World*)(world))->Utils.gNodes[currentCell]->connection[i]->
	}
	//((SZ_World*)(world))->Utils.gNodes[currentCell]->
	//printf(" - timecost: %i \n",((SZ_FunctionBase*)(((SZ_World*)(world))->Utils.gNodes[currentCell]->functions))->timecost*100);
	//printf("pathto : %i \n",((SZ_World*)(world))->Utils.gNodes[currentCell]->pathTo[927]);
	//printf("cost of 6 : %f %f \n",((SZ_World*)(world))->Utils.gNodes[currentCell]->connection[6]->cost.x, ((SZ_World*)(world))->Utils.gNodes[currentCell]->connection[6]->cost.z);

	/*
	for (int i=0; i<pplamount; i++)
	{
	
		printf("action : %s %i\n",((SZ_World*)(world))->Utils.gNodes[currentCell]->PersonList.PersonRecord[i]->actions.modeName(), ((SZ_World*)(world))->Utils.gNodes[currentCell]->PersonList.PersonRecord[i]->actions.aim_targetCell);
		printf("targetcell : %i \n",((SZ_World*)(world))->Utils.gNodes[currentCell]->PersonList.PersonRecord[i]->actions.aim_targetCell);
		printf("TinCell: %i \n",((SZ_World*)(world))->Utils.gNodes[currentCell]->PersonList.PersonRecord[i]->actions.TInCell);
		printf("Twaiting: %i \n \n",((SZ_World*)(world))->Utils.gNodes[currentCell]->PersonList.PersonRecord[i]->actions.TamWaiting);


		
		//(tmp_world->Utils.gNodes[currentCell]->functions))->timecost*100


	}*/
	//this->currentCell
}

void SZ_Camera::setPositionForward( NxVec3 position, NxVec3 Forward )
{
	this->Forward	= Forward;
	this->Pos		= position;
}

void SZ_Camera::projection_selectNextMode()
{
	projection =   projection_successorMode(projection);
	if (projection >= cpEndMode) projection = projection_successorMode(cpStartMode);
}

SZ_Camera::cameraProjection SZ_Camera::projection_successorMode( const cameraProjection cp ) const
{

	return (cameraProjection)(((int)cp) + 1);
}

char* SZ_Camera::projection_modeName()
{
	switch (projection)
	{
	case  cpOrthoGraphic:               return "Orthographic";					break;
	case  cpPerspective:				return "Perspective";					break;
	default:							return "Error: char* SZ_Camera::projection_modeName(): Unknown"; break;
	}
}
