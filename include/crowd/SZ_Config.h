#ifndef SZCONFIG_H
#define SZCONFIG_H

/*-----------------------------------------------------------
  Camera Controls
  -----------------------------------------------------------*/

#define	CAM_FORWARD				 'w'	
#define	CAM_BACKWARD			 's'
#define	CAM_STRAFE_LEFT			 'a'
#define	CAM_STRAFE_BACK			 'd'
#define	CAM_UP					 'e'
#define	CAM_DOWN				 'q'	
#define CAM_STATS			     'o'
#define CAM_ZOOM_IN				 'r'
#define CAM_ZOOM_OUT			 't'
#define CAM_SPEED				 (30.0f)

/*-----------------------------------------------------------
  Other keyboard controls
  -----------------------------------------------------------*/	

#define CONTROL_PPL_SPAWN			'z'
#define CONTROL_PPL_REMOVE			'x'
#define CONTROL_EVENT_REMOVE		'f'
#define CONTROL_EVENT_SHOW_DATA		'g'
#define CONTROL_EVENT_INPUT_DATA	'h'
#define CONTROL_EVENT_INPUT_TIME	'j'
#define CONTROL_MAKE_DIJKSTRA		'k'

/*-----------------------------------------------------------
  World Controls
  -----------------------------------------------------------*/	

#define WORLD_X					 (200)  //200
#define WORLD_Z					 (150)  //150 (125)

#define WORLD_SURFACE			 (WORLD_X*WORLD_Z)

#define WORLD_TILE				 (5)
#define WORLD_SURFACE_5X5		 (WORLD_SURFACE/ (WORLD_TILE*WORLD_TILE))
#define WORLD_X_5X5				 (WORLD_X/WORLD_TILE)
#define WORLD_Z_5X5				 (WORLD_Z/WORLD_TILE)
 
#define NODES_AMOUNT			 (WORLD_X_5X5*WORLD_Z_5X5)
#define MAX_PERSON_NODE			 (256)

#define MAX_KEYS (256)
//#define MAX_FPS (30)					// set to 0 for no frame limit

#define NUM_PERSON (2500)				    // maximum number of people in this world
#define MAX_PERSON_VIEW_DISTANCE (2)	// distance in cells a person can see ahead (used with findneighbour)

#define GENERATION_MAX_PERSON_PER_TILE	(25)	// on generation, maximum amount of people per tile.
#define MAX_PERSON_PER_CELL				(100.0)	// max amount of individuals on a cell
#define GENERATION_MODIFIER				(15)		// on generation, modifier of amount of people spawned per plane (total num = max * modifier)	
#define MAX_CITIES	(256)				// maximum amount of cities in the external file
#define MAX_PLANES	(256)


#define MAX_FORCE_AMOUNT (100000)			// used for forcevectorlist


/*-----------------------------------------------------------
  Rendering and Drawing Controls
  -----------------------------------------------------------*/	
#define RENDER						(1)					// 1 if render
#define RENDER_CAMERA_RECTILE		(1)					// render camera rectile 
#define RENDER_SOCIAL_CONNECTIONS	(0)					// render social connections between individuals


/*-----------------------------------------------------------
  Group control variables
  -----------------------------------------------------------*/	

#define MAX_SOCIAL_LINKS  (256)
#define MAX_GROUP_GG1	  (2560)
#define MAX_PERSON_GROUP1 (2560)

/*-----------------------------------------------------------
  Event and locations control
  -----------------------------------------------------------*/	
#define LOCALBASELIST_AMOUNT					(100)	// total amount of locations
#define LOCAL_QUAD_SIZE				(1)					// size of a quad of event, for drawing
#define LOCAL_QUAD_POS_Y			(5)					// position above ground of quad, for drawing

// hardcoded location pixel positions in places_icons.bmp
#define ARRIVAL_01_PIXEL					(111)
#define ARRIVAL_02_PIXEL					(112)
#define ARRIVAL_03_PIXEL					(113)
#define ARRIVAL_04_PIXEL					(114)
#define ARRIVAL_05_PIXEL					(115)
#define EXIT_01_PIXEL						(105)
#define EXIT_02_PIXEL						(106)
#define EXIT_03_PIXEL						(107)
#define EXIT_04_PIXEL						(108)
#define EXIT_05_PIXEL						(109)
#define DEP_01_PIXEL						(116)
#define DEP_02_PIXEL						(117)
#define DEP_03_PIXEL						(118)
#define DEP_04_PIXEL						(119)

#endif