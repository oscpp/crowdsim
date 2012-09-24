#include "SZ_PrAction.h"
#include "SZ_World.h"
#include "SZ_Person.h"

/*-----------------------------------------------------------
  SZ_PrAction class members
  -----------------------------------------------------------*/

SZ_PrAction::SZ_PrAction()
{
}

SZ_PrAction::~SZ_PrAction()
{
} 

void SZ_PrAction::incrementTWaiting(int amount)
{
	TamWaiting = TamWaiting + amount;
}

void SZ_PrAction::incrementTWalkingTarget(int amount)
{
	TamGoingTarget = TamGoingTarget + amount;
}

void SZ_PrAction::incrementTLiving(int amount)
{
	TLiving = TLiving + amount;
}

void SZ_PrAction::incrementTInCell(int amount)
{
	TInCell = TInCell + amount;
}

void SZ_PrAction::update()
{
	//if (((SZ_Person*)(person))->iterations<50) { return;}
	if (currentAction <0) { return;}
	SZ_World* tmp_world = ( (SZ_World*)(world));
	//if the player changes actions, trigger changeaction!
	if (currentAction!=oldAction)
	{
		oldAction=currentAction;
		TamWaiting		= 0;
		TamGoingTarget	= 0;
	}

	// the person 'disappears' out of the world, if there are no further aims.
	if (aim_targetCellList.length()==1)
	{
		//if (TInCell>=120)
		{
			if ( ((SZ_Person*)(person))->currentCell == aim_targetCell )
			{
				currentAction = amQuitting;
			}
		}	
	}

	//keep track of the time when this entity started to exist in this world.
	if ( ((SZ_World*)(world))->gTimeKeeper.getIterations() % ((SZ_World*)(world))->gTimeKeeper.getFrequency() == 0)
	{		
		if	(((SZ_World*)(world))->gTimeKeeper.theTimeInSecs() % TIME_STEPSIZE == 0 ) 
		{
			incrementTLiving(TIME_STEPSIZE);
			incrementTInCell(TIME_STEPSIZE);

			/*
			//if doActionTime is set, agent will do a specific action for a certain amount of time
			if (doAction)
			{
				if (doActionTime>=0)
				{
					
					doActionTime	=	doActionTime - TIME_STEPSIZE;
					currentAction	=	doWhichAction;	
				}
				if (doActionTime<=0)
				{
					doActionTime = 0;
					doAction = false;
					currentAction = doThenAction;
				}
			}
			*/
			
			
			switch (currentAction)
			{
				case amWaiting:	
					incrementTWaiting(TIME_STEPSIZE);
				break;		

				case amGoingTarget:
					incrementTWalkingTarget(TIME_STEPSIZE);
				break;
			}	
			
					
		}
	}	

}



char* SZ_PrAction::modeName()
{
	switch (currentAction)
	{
		case amGoingTarget:	return "Going to Target.";	break;
		case amWaiting:		return "Waiting.";		break;
		case amQuitting:	return "Deleting/Leaving from world."; break;
		case amGroup1:		return "Group 1: Intergroup relationship"; break;
		default:			return "?"; break;
	}
}

void SZ_PrAction::aim_GenerateListAims(int amount)
{
	//aim_targetCellList.empty();
	for (int i = 0; i< amount; i++)
	{
		// create a random list of places that is needed to go 
		int randFunction = rand() % ((SZ_World*)(world))->service.amount;
		randFunction = ((SZ_World*)(world))->service.FunctionBaseList[randFunction]->currentCell;
		aim_targetCellList.add(randFunction);
	}
	aim_targetCell = aim_targetCellList.intRecord[aim_targetCellList.length()-1];

}

void SZ_PrAction::aim_SetNextAim()
{
	if (aim_targetCellList.length()-1 != 0)
	{
		aim_targetCellList.substract(aim_targetCell);
		aim_targetCell = aim_targetCellList.intRecord[aim_targetCellList.length()-1];
	}
	//printf("list length %i \n", aim_targetCellList.length());

}

void SZ_PrAction::selectNextAction()
{
    currentAction = successorMode (currentAction);
    if (currentAction >= amEndMode) currentAction = successorMode (amStartMode);
}

SZ_PrAction::actionMode SZ_PrAction::successorMode(const actionMode am) const
{
	  return (actionMode)(((int)am) + 1);
}



SZ_PrAction & SZ_PrAction::operator=( const SZ_PrAction &copyaction )
{
	aim_targetCell	   = copyaction.aim_targetCell;					
	aim_targetCellList = copyaction.aim_targetCellList;	

	
	oldAction			= copyaction.oldAction;
	currentAction		= copyaction.currentAction;
	
	TLiving				= copyaction.TLiving;		
	TInCell				= copyaction.TInCell;
	TamWaiting			= copyaction.TamWaiting;
	TamGoingTarget		= copyaction.TamGoingTarget; 

	return *this;
}
