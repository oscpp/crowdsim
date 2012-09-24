#include "SZ_World.h"


SZ_Logger::SZ_Logger()
{
	nodesAmount = 0;

	tmp_forceGroupCenterAttract		=	1.2f;
	tmp_forceGroupCenterAvoid		=	0.3f;
	tmp_forceSelfAimAttract			=	0.5f;
	tmp_forceOthersAvoid			=	0.8f;
	tmp_forceSelfCellDensity		=	1.0f;
}

SZ_Logger::~SZ_Logger()
{

}

void SZ_Logger::init( SZ_World* tmp_world, const char* filename )
{
	errno_t err; 
	if( (err = fopen_s (&stream2, filename, "w+" )) != 0 )
		printf("...SZ_Logger: The file %s was not opened.\n", filename ); 
	else       printf( "...SZ_Logger: %s is used for logging.\n", filename );

	this->tmp_world = tmp_world;

	add(tmp_world->Utils.gNodes[1016]);
}

void SZ_Logger::update()
{
	if (tmp_world->Utils.scenecpp_timer - tmp_world->Utils.scenecpp_timebase > 5000 )
	
	//if (iterations < 100)
	{
		fprintf(stream2, "%i; 0.83; \n", tmp_world->Utils.iterations);

		int total_spawned = 0;
		for (int i=1; i<MAX_SOCIAL_LINKS; i++)
		{
			if (tmp_world->ggGroupsTotalCount[i]!=0)
			{
				//printf("%i; %i \n", i, tmp_world->ggGroupsTotalCount[i]);
				fprintf(stream2, "%i; %i \n", i, tmp_world->ggGroupsTotalCount[i]);		

				total_spawned += tmp_world->ggGroupsTotalCount[i] * i ;
			}
		}

		//fprintf(stream2, "%i; \n \n", total_spawned);

		//printf("total spawned %i \n \n", total_spawned);
		
		/*
		fprintf( stream2,"%4.2f;",	tmp_world->Utils.scenecpp_frame*1000.0/(tmp_world->Utils.scenecpp_timer-tmp_world->Utils.scenecpp_timebase));
		fprintf( stream2,"%i;",		tmp_world->ppAmount);
		fprintf( stream2,"%i;",		tmp_world->Utils.scenecpp_iterations);
		

		// go through each node this logger has got
		for (int i = 0; i<nodesAmount;i++)
		{
			if (NodeList[i]!=NULL)
			{
				int currentNodeNum = NodeList[i]->number;	// current iterating node
				for (int k=0; k<tmp_world->sbGetLayerOccupancy(currentNodeNum)->amount; k++)	// go through each person on the cell/node
				{	
				}
				
				//fprintf( stream2,"%i; ", tmp_world->sbGetLayerOccupancy(NodeList[i]->number));	// get number of people on node
				//tmp_world->sbGetLayerOccupancy(NodeList[i]->number);
			}
		}
		*/
		fprintf( stream2, "\n");

	}
	

}

void SZ_Logger::draw()
{
	for (int i = 0; i<nodesAmount;i++)
	{
		if (NodeList[i]!=NULL)
		{
			int currentNodeNum = NodeList[i]->number;

			int X = (currentNodeNum % WORLD_X_5X5);	  // transform the tile value into the top left X corner
			int Z = (currentNodeNum-X)/WORLD_X_5X5;   // transform the tile value into the top left Y corner

			glColor4f(0.1,0.1,0.9,0.5);
			glLineWidth(4);
			glBegin(GL_LINE_LOOP);
			glVertex3f(X*WORLD_TILE				,0.04f,Z*WORLD_TILE);
			glVertex3f(X*WORLD_TILE + WORLD_TILE,0.04f,Z*WORLD_TILE);
			glVertex3f(X*WORLD_TILE	+ WORLD_TILE,0.04f,Z*WORLD_TILE+WORLD_TILE);
			glVertex3f(X*WORLD_TILE				,0.04f,Z*WORLD_TILE+WORLD_TILE);
			glEnd();
		}
	}
}

void SZ_Logger::add( SZ_Node* whichNode )
{
	if (nodesAmount <LOCALBASELIST_AMOUNT)
	{
		NodeList[nodesAmount] = whichNode;
		nodesAmount++;
	}
}

void SZ_Logger::processLoggerKeys( int keys, float delta )
{
	switch (keys)
	{
		case ',': { 
					switch (ForceWeightSelected)
					{
						case fwGroupCenterAttract:  {  tmp_forceGroupCenterAttract	+=0.01f;  printf("Group, Center Attract: %f \n", tmp_forceGroupCenterAttract); break; }
						case fwGroupCenterAvoid:	{  tmp_forceGroupCenterAvoid	+=0.01f;  printf("Group, Center Avoid  : %f \n", tmp_forceGroupCenterAvoid); break; }
						case fwSelfAimAttract:		{  tmp_forceSelfAimAttract		+=0.01f;  printf("Self,  Force Attract : %f \n", tmp_forceSelfAimAttract); break; }
						case fwOthersAvoid:			{  tmp_forceOthersAvoid			+=0.01f;  printf("Others,Force Avoid	 : %f \n", tmp_forceOthersAvoid);  break; }
					}
					break;
				  }
		case '.': {
					switch (ForceWeightSelected)
					{
						case fwGroupCenterAttract:  {  tmp_forceGroupCenterAttract	-=0.01f;  printf("Group, Center Attract: %f \n", tmp_forceGroupCenterAttract); break; }
						case fwGroupCenterAvoid:	{  tmp_forceGroupCenterAvoid	-=0.01f;  printf("Group, Center Avoid  : %f \n", tmp_forceGroupCenterAvoid); break; }
						case fwSelfAimAttract:		{  tmp_forceSelfAimAttract		-=0.01f;  printf("Self,  Force Attract : %f \n", tmp_forceSelfAimAttract); break; }
						case fwOthersAvoid:			{  tmp_forceOthersAvoid			-=0.01f;  printf("Others,Force Avoid	 : %f \n", tmp_forceOthersAvoid);  break; }
					}
					break;
				  }
		//case 'v': {	 tmp_forceGroupCenterAttract	+=0.1f; printf("Group, Center Attract: %f \n", tmp_forceGroupCenterAttract);	break;}
		//case 'b': {	 tmp_forceGroupCenterAvoid		+=0.1f;	printf("Group, Center Avoid  : %f \n", tmp_forceGroupCenterAvoid);   break;}
		//case 'n': {	 tmp_forceSelfAimAttract		+=0.1f; printf("Self,  Force Attract : %f \n", tmp_forceSelfAimAttract);	break;}
		//case 'm': {	 tmp_forceOthersAvoid			+=0.1f;	printf("Others,Force Avoid	 : %f \n", tmp_forceOthersAvoid); break;}
		case 'm': { ForceWeightNext(); printf("Selected: %s \n", ForceWeightPrint()); break; }
	}
}

void SZ_Logger::ForceWeightNext()
{
	ForceWeightSelected = ForceWeightSuccessorMode(ForceWeightSelected);
	if (ForceWeightSelected >= fwEnd) ForceWeightSelected = ForceWeightSuccessorMode(fwStart);
}

SZ_Logger::forceWeight SZ_Logger::ForceWeightSuccessorMode( const forceWeight fw )
{
		return (forceWeight) (((int)fw)+1);
}

char* SZ_Logger::ForceWeightPrint()
{
	switch (ForceWeightSelected)
	{
		case fwGroupCenterAttract:  { return "fwGroupCenterAttract."; break; }
		case fwGroupCenterAvoid:	{ return "fwGroupCenterAvoid."; break; }
		case fwSelfAimAttract:		{ return "fwSelfAimAttract."; break; }
		case fwOthersAvoid:			{ return "fwOthersAvoid."; break; }
		case fwSelfCellDensity:     { return "fwSelfCellDensity."; break; }

		default:						return "Error: char* SZ_Logger::projection_modeName(): Unknown"; break;
	}
}
