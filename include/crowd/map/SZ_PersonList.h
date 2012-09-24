#ifndef SZPERSONRECORD_H
#define SZPERSONRECORD_H

#include "..\..\..\general_utils.h"
#include "..\SZ_Config.h"
#include "..\SZ_Person.h"
#include "..\SZ_Person.h"



class SZ_PersonList
{
	public:
		 SZ_PersonList();
		~SZ_PersonList();

		int amount;	// amount of persons in this person list
		SZ_Person* PersonRecord[MAX_PERSON_NODE];	// stores all the Person records

		//SZ_Person* smallestElement();				// returns record with the smallest cost so far
		bool contains(SZ_Person* whichPerson);		// true if this list contains specified person
		SZ_Person* find(SZ_Person* whichPerson);	// 
		void add(SZ_Person* whichPerson);
		void substract(SZ_Person* whichPerson);
		void empty();
		int length();  // returns the amount of persons in this instance 
		int length2(); // returns the amount of persons in this instance by manually counting them 
};

#endif