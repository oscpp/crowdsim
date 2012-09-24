#ifndef SZCONFIGTIME_H
#define SZCONFIGTIME_H

/*-----------------------------------------------------------
  Timekeeping measures default values
  -----------------------------------------------------------*/	
#define TIME_STEPSIZE		(15)						// size of a timestep in seconds. Always use values that will add up to 60.
#define TIME_FREQUENCY		(10)						// frequency (over iterations) of augmenting the time
#define TIME_START_DAY		(1)							// 1-365 day of year
#define TIME_START_HOUR		(7)							// hour 0-23
#define TIME_START_MIN		(0)							// minute 0-59

/*-----------------------------------------------------------
  Time that an action on a location (checkin) requires (in secs)
  -----------------------------------------------------------*/	

/*-----------------------------------------------------------
  Airplane times
  -----------------------------------------------------------*/	
#define PLANE_TIME_WAIT_TO_BOARD	(30*60)			// secs required for a plane to switch from BOARDING to DEPARTED
#define PLANE_TIME_WAITING_TO_LANDING	(30*60)			// secs required for a plane to switch from LANDING to LANDED
#define PLANE_TIME_BOARD_TO_DEPART	(0)				// secs from board to depart, scheduled time

#define PLANE_FIRST_DEPART_HOUR		(7)		// first plane departs at 7AM...
#define PLANE_FIRST_DEPART_MINUTE	(10)	// ... 7:10AM

#define PLANE_FIRST_LAND_HOUR		(7)		// first plane lands at 7AM...
#define PLANE_FIRST_LAND_MINUTE		(15)	// ... 7:15AM

#define FUNCTION_TIME_COST_MULTI	(60)

#endif