#ifndef SZWORLDPERSONRECORD_H
#define SZWORLDPERSONRECORD_H

#include "..\..\..\general_utils.h"
#include "..\SZ_Config.h"
#include "..\SZ_Person.h"



class SZ_WorldPersonList
{
	public:
		 SZ_WorldPersonList();
		~SZ_WorldPersonList();

		int amount;	// amount of NodeRecord in this Pathfindinglist

		SZ_Person* PersonRecord[NUM_PERSON];	// stores all the Person records

		//SZ_Person* smallestElement();				// returns record with the smallest cost so far
		bool contains(SZ_Person* whichPerson);		// true if this list contains specified person
		SZ_Person* find(SZ_Person* whichPerson);	// 
		void add(SZ_Person* whichPerson);
		void substract(SZ_Person* whichPerson);
		void empty();
		int length();  // returns the amount of noderecords in this instance 
		int length2(); // returns the amount of noderecords in this instance by manually counting them 

};

#endif