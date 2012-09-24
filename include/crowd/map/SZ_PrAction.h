#ifndef SZPRACTION_H
#define SZPRACTION_H

#include "SZ_NumberList.h"

// used to store actions/aims this particular entity is doing.

class SZ_PrAction
{
	private:
	public:
	 SZ_PrAction();
	~SZ_PrAction();

	

	//SZ_PrAction operator = (SZ_PrAction);
	SZ_PrAction &operator=(const SZ_PrAction &copyaction); //the automatically added assignment op

	enum actionMode
	{
		amStartMode, amGoingTarget, amWaiting, amQuitting, amGroup1, amEndMode
	};
	
	// used for aims and routes
	int aim_targetCell;					// current cell i need to visit.
	SZ_NumberList aim_targetCellList;	// list of aims (cells to visit) i have got to accomplish.

	// target and aims, from one cell to another
	void aim_GenerateListAims(int amount); // generate a list of aims. listsize = amount
	void aim_SetNextAim();				   // change to the next aim in the list. FIFO. if listsize = 0 --> change action to PERSON_QUITTING

	void update();
	void* world;		// which world do I belong to?
	void* person;		// which person do I belong to?
	actionMode oldAction;
	actionMode currentAction;
	void selectNextAction();
	void selectAction(const actionMode cm);
	actionMode successorMode(const actionMode cm) const;
	char* modeName();

	int TLiving;		// time in seconds since I am living.
	int TInCell;		// time in seconds spent in current cell.
	int TamWaiting;		// time in seconds spent waiting. (only if PERSON_ACTION_WAITING)
	int TamGoingTarget; // time in seconds spent walking towards target (if PERSON_WALKING_TARGET)

	// related to augment time spend for each actions.
	void incrementTLiving(int amount);			// augment TLiving by amount secs.
	void incrementTInCell(int amount);			// augment TInCell by amount secs.
	void incrementTWaiting(int amount);			// augment TWaiting by amount secs.
	void incrementTWalkingTarget(int amount);	// augment TWalkingTarget by amount secs.

};

#endif