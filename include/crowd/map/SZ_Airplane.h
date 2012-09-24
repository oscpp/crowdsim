#ifndef SZAIRPLANEH
#define SZAIRPLANEH

#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "SZ_ConfigTime.h"
#include "SZ_PersonList.h"

class SZ_Schedule;
class SZ_Node;

class SZ_Airplane
{

	private:
		struct tm tmStart_param;		// specifies time when the plane is scheduled
		struct tm *time;				// structure that returns the simulated time
		time_t delay;					// delay in seconds
		char* tmpDeplayDisplay;			// used for delay displaying.
		SZ_Schedule* mySchedule;		// pointer to which schedule I belong to.
		SZ_Node*	gate;				// pointer towards a SZ_Node from which the plane departs/lands. used for spawning/despawning people.

	public:
		SZ_Airplane();
		~SZ_Airplane();

		//actions of a plane, which returns to the schedule.
		void actionTimeRunning(SZ_Airplane* whichPlane, int timeleft);
		void actionLanding(SZ_Airplane* whichPlane);
		void actionDeparting(SZ_Airplane* whichPlane);
		void actionBoarding(SZ_Airplane* whichPlane);
		void actionDelayed(SZ_Airplane* whichPlane);
		void actionLanded(SZ_Airplane* whichPlane);

		enum apStatus
		{
			apStartMode, apWaiting, apLanding, apDeparting, apBoarding, apCancel, apDelayed, apLanded, apExpected, apEndMode
		};

		void setMySchedule(SZ_Schedule* whichSchedule);	// sets which schedule (depart or landing) this plane belongs to
		SZ_Schedule* getMySchedule();					// returns a pointer towards which schedule this plane belongs to

		void nextDay();	// sets the airplane for the next day and resets its status.
		void setSchedule(int hour, int min);
		void selectNextStatus();				// choses the next status. 
		void selectStatus(const apStatus ap);	// selects a specific status
		void setDelay(time_t secs);				// sets the delay of the plane in secs
		struct tm *getSchedule();				// returns a tm structure which contains the plane schedule.
		int TotalPassengerAmount;				// total amount of passengers this plane can carry.
		SZ_PersonList passengers;				// list of pointers to people who are taking this plane
		apStatus successorMode(const apStatus ap) const;
		apStatus getPlaneStatus();
		apStatus planeStatus;
		char* statusName();						// returns a char array of the status of this plane
		time_t getDelay();						// gets the delay of the plane in secs
		const char* city;						// stores where this plane is heading. 
		void setGate(SZ_Node* whichNode);		// sets gate
		SZ_Node* getGate();						// returns a pointer towards a SZ_Node from which the plane departs/lands.
};
#endif

