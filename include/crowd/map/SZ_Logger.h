#ifndef	SZLOGGER
#define SZLOGGER

#include "..\SZ_Config.h"
#include "SZ_Connection.h"
#include "SZ_PersonList.h"
#include "SZ_Node.h"


#include "..\..\..\general_utils.h"

class SZ_World;

class SZ_Logger
{
	public:
		 SZ_Logger();
		~SZ_Logger();

		// world i belong to
		SZ_World*	tmp_world;

		float tmp_forceGroupCenterAttract ;
		float tmp_forceGroupCenterAvoid;
		float tmp_forceSelfAimAttract;
		float tmp_forceOthersAvoid;
		float tmp_forceSelfCellDensity;

		enum forceWeight
		{
			fwStart, fwGroupCenterAttract, fwGroupCenterAvoid, fwSelfAimAttract, fwOthersAvoid, fwSelfCellDensity, fwEnd
		};

		void  ForceWeightNext();
		forceWeight  ForceWeightSuccessorMode(const forceWeight fw);
		char* ForceWeightPrint();

		forceWeight ForceWeightSelected;

	

		// nodes this logger controls
		SZ_Node* NodeList[12];			// stores all the cells this logger logs
		void add(SZ_Node* whichNode);	
		int nodesAmount;



		FILE * stream2;				//save to file, used for storing in external file

		void init(SZ_World* tmp_world, const char* filename);
		void update();
		void draw();				// draw a rectangle around all the nodes this is currently logging
		void processLoggerKeys(int keys, float delta);
};

#endif

