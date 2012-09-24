#ifndef SZLOCALLIST_H
#define SZLOCALLIST_H


#include "..\..\..\general_utils.h"
#include "..\SZ_Config.h"
#include "SZ_FunctionBase.h"


class SZ_FunctionBaseList
{
	public:
		 SZ_FunctionBaseList();
		~SZ_FunctionBaseList();

		void* world;
		void init(void *world); // reads data from local places and creates events accordingly.
		int amount;	// amount of NodeRecord in this Pathfindinglist
		SZ_FunctionBase* FunctionBaseList[LOCALBASELIST_AMOUNT];		// stores all the node records

		SZ_FunctionBase* findSpecificEvent(int whichEvent);		// 

		void add(SZ_FunctionBase* whichFunctionBase);
		void substract(SZ_FunctionBase* whichFunctionBase);
		void drawQuad();
		void drawNeighbour();
		void update();
		void modifyNode();
		void empty();  // delete all Function bases.
		int length();  // returns the amount of noderecords in this instance 
		int length2(); // returns the amount of noderecords in this instance by manually counting them 
		
};

#endif