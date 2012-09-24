#include "SZ_SocialConnectionIntra.h"
#include "SZ_Person.h"
#include "SZ_SocialConnectionIntraList.h"


SZ_SocialConnectionIntra::SZ_SocialConnectionIntra()
{
	parent   = NULL;
	toPerson = NULL;
}

SZ_SocialConnectionIntra::~SZ_SocialConnectionIntra()
{

}

void SZ_SocialConnectionIntra::draw()
{
	
	if ((toPerson == NULL) || ( parent == NULL))
	{
		printf("Error: SocialConnectionIntra::draw, parent or toPerson is NULL \n");
		return;
	}

	NxVec3 posA  = this->parent->parent->getPosition();
	NxVec3 posB  = this->toPerson->getPosition();
	NxVec3 color = NxVec3(1,0,0);
	NxVec3 vec = posB - posA;

	NxVec3 t0, t1, t2;
	//NxNormalToTangents(vec, t1, t2);

	t0 = posB - posA;
	t0.normalize();

	// normalised vector towards posb
	NxVec3 posC = getToPersonVector();
	posC.normalize();

	float arrowwidth = 0.00f; // 1.25
	NxVec3 lobe1  = posB - t0*arrowwidth + t2 * arrowwidth;
	NxVec3 lobe2  = posB - t0*arrowwidth - t2 * arrowwidth;

	NxVec3 v3ArrowShape[] = {
		NxVec3(posA), NxVec3(posB),
		NxVec3(posB), NxVec3(lobe1),
		NxVec3(posB), NxVec3(lobe2),
	};


	glLineWidth(3.5f);
	//glColor4f(color.x,color.y,color.z,1.0f);
	glColor4f(parent->colour.x, parent->colour.y, parent->colour.z, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(NxVec3), &v3ArrowShape[0].x);
	glDrawArrays(GL_LINES, 0, sizeof(v3ArrowShape)/sizeof(NxVec3));
	glDisableClientState(GL_VERTEX_ARRAY);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
}

NxVec3 SZ_SocialConnectionIntra::getToPersonVector()
{
	if ((toPerson == NULL) || ( parent == NULL))
	{
		printf("Error: SocialConnectionIntra::toPersonVector, parent or toPerson is NULL \n");
		return NxVec3(0,0,0);
	}

	//if (toPerson == parent->parent ) { return NxVec3(0,0,0); }

	NxVec3 posA  = this->parent->parent->getPosition();
	NxVec3 posB  = this->toPerson->getPosition();

	NxVec3 vec = posB - posA;
	return vec;
}

