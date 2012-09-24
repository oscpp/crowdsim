#ifndef SZSCHEDULEH
#define SZSCHEDULEH

#include "SZ_Airplane.h"
#include "SZ_Config.h"
#include "..\..\general_utils.h"
#include "SZ_Sprite.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

class SZ_World;
class SZ_Node;

// contains the schedule of the airplanes in this simulated world.
// also contains further information about the world to generate/handle the schedule of individuals' SZ_PrAction

class SZ_Schedule 
{
	private:
		SZ_Sprite *boardText[32*8];
		NxVec3 position;

		static const int ppTotalppNodes = 10;
		int ppGatesLength;
		int ppExitLength;
		int ppSecurityLength;
		int ppDepartLength;
		SZ_Node* ppGates[ppTotalppNodes];	 // Pointers to the nodes to which planes are arriving or departing, depending on ScheduleType
		SZ_Node* ppExits[ppTotalppNodes];	 // Pointer to nodes containing exits
		SZ_Node* ppSecurity[ppTotalppNodes]; // Pointer to nodes containing security checkins
		SZ_Node* ppDeparts[ppTotalppNodes];  // Pointer to nodes of departure

	public:
		SZ_Schedule();
		~SZ_Schedule();

		void ppAddGate(SZ_Node* whichNode);		// Adds a pointer to the list of landing/departures gates.
		void ppAddExit(SZ_Node* whichNode);		// Adds a pointer to the list of exits in the world
		void ppAddSecurity(SZ_Node* whichNode); // Adds a pointer to security checkins.
		void ppAddDeparts(SZ_Node* whichNode);  // Adds a pointer to departure nodes.

		enum apScheduleType
		{
			apDepartTable, apLandingTable
		};

		void onCreate(void *world, unsigned int texture_array[]);	// reads all the destinations from cities.txt. Max 256 cities
		void update();
		void draw();
		void setTextOnSprite(int row, int col, char whichChar);
		void setSchedule(apScheduleType typeSchedule);
		void newPlane(int tmpHour,int tmpMin, SZ_Airplane::apStatus, const char* city);
		void setPosition(NxVec3 newPos);
		NxVec3 getPosition();
		bool contains(SZ_Airplane* whichPlane);				// true if this list contains specified node
		void add(SZ_Airplane* whichPlane);					// add a specified plane to the list.
		void substract(SZ_Airplane* whichPlane);			// remove a specified plane to the list
		void empty();
		int	length();  // returns the amount of noderecords in this instance

		SZ_World *world;	// world i belong to
		apScheduleType MySchedule;
		apScheduleType getSchedule();
		std::string cities[MAX_CITIES];	// array containing cities
		int citiesNum;					// amount of cities in array.
		static const int max_number_plane = MAX_PLANES;
		int amountPlanes;	// amount of ints in this numberlist
		SZ_Airplane* planeRecord[max_number_plane];		// stores all the airplane records

		void actionLanding(SZ_Airplane* whichPlane);
		void actionDeparting(SZ_Airplane* whichPlane);
		void actionBoarding(SZ_Airplane* whichPlane);
		void actionDelayed(SZ_Airplane* whichPlane);
		void actionLanded(SZ_Airplane* whichPlane);
		void actionTimeRunning( SZ_Airplane* whichPlane, int timeleft );
};

#endif