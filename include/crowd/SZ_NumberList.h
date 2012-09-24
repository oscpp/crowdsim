#ifndef SZNUMBERLIST_H
#define SZNUMBERLIST_H

#include "..\..\general_utils.h"
#include "SZ_Config.h"



class SZ_NumberList
{
	public:
		 SZ_NumberList();
		~SZ_NumberList();

		SZ_NumberList &operator = (const SZ_NumberList &copyNumberList);

		static const int max_number_int = 1024;

		int amount;	// amount of ints in this numberlist

		int intRecord[max_number_int];		// stores all the node records


		bool contains(int whichInt);				// true if this list contains specified node

		void add(int whichInt);
		void substract(int whichInt);
		void empty();
		int length();  // returns the amount of noderecords in this instance 
		int length2(); // returns the amount of noderecords in this instance by manually counting them 


};

#endif