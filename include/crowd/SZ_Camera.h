#ifndef SZCAMERA_H
#define SZCAMERA_H

/*-----------------------------------------------------------
  Simulation Header File. more comments in the cpp file
  -----------------------------------------------------------*/

#include "..\..\general_utils.h"
#include "SZ_Config.h"

class SZ_World;

/*-----------------------------------------------------------
  camera class
  -----------------------------------------------------------*/
class SZ_Camera
{
	private:
	public:
		SZ_Camera();
		void Init(NxScene*);
		void ProcessKeys(int key, float delta);		
		void MotionCallBack(int x, int y);
		void Update();
		void reset();
		void setPositionForward(NxVec3 position, NxVec3 Forward);
		float  AspectRatio;

		NxActor* entity;			//stores the physX stuff of the area.
		NxVec3 Pos;
		NxVec3 Forward;
		NxVec3 Right;
		NxVec3 Up;
		NxReal Speed;
		NxVec3 RecticlePoint;	// where the camera is pointing at in the world.
		int	   RecticleCell;	// Cell the camera is pointing at in the world.


		enum cameraProjection
		{
			cpStartMode, cpOrthoGraphic, cpPerspective, cpEndMode
		};
		float projectionOrthoZoom;
		float projectionPerspZoom;
		cameraProjection projection;
		void  projection_selectNextMode();
		cameraProjection projection_successorMode(const cameraProjection cp) const;
		char* projection_modeName();

		enum cameraMode
		{
			cmStartMode, cmFree, cmFirstPerson, cmOffsetPOV, cmEndMode
		};

		NxActor* actorToTrack;		
		cameraMode mode;

		// select next camera mode
        void selectNextMode (void);

        // the mode that comes after the given mode (used by selectNextMode)
        cameraMode successorMode (const cameraMode cm) const;

		char* modeName();
		void draw();	 // draws the recticle.
		void* world;	 // pointer towards the world i belong to.
		SZ_World* world_test;
		int currentCell; // which tile is currently under the camera.
		void printCurrentCellInfo(); // prints all the info about the current chosen cell
		SZ_Camera* address;			//pointer to self
};


#endif