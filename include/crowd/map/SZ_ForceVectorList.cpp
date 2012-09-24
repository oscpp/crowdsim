#include "SZ_ForceVectorList.h"
#include "SZ_World.h"
#include "SZ_ConfigTime.h"

SZ_ForceVectorList::SZ_ForceVectorList()
{

}

SZ_ForceVectorList::~SZ_ForceVectorList()
{
	
}

void SZ_ForceVectorList::add(SZ_ForceVector* whichForceRecord)
{
	//printf("amount of force %i \n", amount);
	if (amount <MAX_FORCE_AMOUNT-1)
	{
		{
			ForceRecord[amount] = whichForceRecord;
			amount++;
		}
	} 
	else { delete whichForceRecord;}
}


void SZ_ForceVectorList::substract(SZ_ForceVector* whichForce)
{	
	bool found = false;
	int counter = 0;
	while ( (counter<amount)  )
	{

		if (ForceRecord[counter]== whichForce)
		{			

			amount--;				
			ForceRecord[counter]= ForceRecord[amount];		
			delete ForceRecord[amount];
			ForceRecord[amount] = NULL;
			counter=amount;
		}
		counter++;
	}
}

void SZ_ForceVectorList::draw()
{
		
	/*
	glLineWidth(1);
	glPointSize(1);
	glBegin(GL_POINTS);
	for (int i=0;i<amount;i++)
	{
		glColor3f(1,0,0);
		glVertex3f(ForceRecord[i]->position.x, ForceRecord[i]->position.y, ForceRecord[i]->position.z);
		//ForceRecord[i]->draw(); 
	}
	glEnd();
*/

	

	

	
	glDisable(GL_LIGHTING);
	for (int i=0;i<amount;i++)
	{
		NxVec3 posA  = ForceRecord[i]->position;
		NxVec3 posB  = ForceRecord[i]->vector3f+	ForceRecord[i]->position;
		NxVec3 color = ForceRecord[i]->colour3f;
		NxVec3 vec = posB - posA;
		
		NxVec3 t0, t1, t2;
		NxNormalToTangents(vec, t1, t2);

		t0 = posB - posA;
		t0.normalize();

		float arrowwidth = 0.25f;
		//NxVec3 lobe1  = posB - t0*arrowwidth + t1 * arrowwidth;
		//NxVec3 lobe2  = posB - t0*arrowwidth - t1 * arrowwidth;
		NxVec3 lobe3  = posB - t0*arrowwidth + t2 * arrowwidth;
		NxVec3 lobe4  = posB - t0*arrowwidth - t2 * arrowwidth;

		NxVec3 v3ArrowShape[] = {
			NxVec3(posA), NxVec3(posB),
			//NxVec3(posB), NxVec3(lobe1),
			//NxVec3(posB), NxVec3(lobe2),
			NxVec3(posB), NxVec3(lobe3),
			NxVec3(posB), NxVec3(lobe4),
		};

		
		glLineWidth(0.5f);
		glColor4f(color.x,color.y,color.z,1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(NxVec3), &v3ArrowShape[0].x);
		glDrawArrays(GL_LINES, 0, sizeof(v3ArrowShape)/sizeof(NxVec3));
		glDisableClientState(GL_VERTEX_ARRAY);
		glColor4f(1.0f,1.0f,1.0f,1.0f);
	}
	glEnable(GL_LIGHTING);
}

void SZ_ForceVectorList::empty()
{

	for (int i = 0;i<amount;i++)
	{
		if (ForceRecord[i]!=NULL)
		{
			delete ForceRecord[i];
			ForceRecord[i]=NULL;
		}
	}
	amount=0;
}

void SZ_ForceVectorList::newVector( NxVec3 start, NxVec3 vector3f, NxVec3 colour3f)
{
	if (RENDER == 1) // can only create vectors if i am rendering
	{
		SZ_ForceVector *tmp_vector;
		tmp_vector = new SZ_ForceVector();
		tmp_vector->position = start;
		tmp_vector->vector3f = vector3f;
		tmp_vector->colour3f = colour3f;
		this->add(tmp_vector);
	}

}
