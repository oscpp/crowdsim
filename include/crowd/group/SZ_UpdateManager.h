#ifndef SZUPDATEMANAGER
#define SZUPDATEMANAGER

#include "SZ_Person.h"
#include "SZ_Config.h"
#include "..\..\general_utils.h"
#include "SZ_Sprite.h"

class SZ_World;

// an instance of this class points towards a given sz_person; the pointer changes to the next person at a given updatecycle.
// it is used to update an instance of a single sz_person once every updatecycles instead of all of them at once.
// on every updatecycle, person pointer goes n+1 until ppAmount is reached, then restart at 0
class SZ_UpdateManager
{
	private:
	public:
		 SZ_UpdateManager();
		~SZ_UpdateManager();
		SZ_Person* ppPointer;			// pointer towards a given person
		int	ppIteration;				// person I am currently iterating through
		SZ_World* tmp_world;					// pointer towards world i belong to.
		void update();					//
		void init( SZ_World* parent);	// initialize the object and sets world it belongs to
};

#endif