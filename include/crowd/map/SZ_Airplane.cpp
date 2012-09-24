#include "SZ_Airplane.h"
#include "SZ_Schedule.h"
#include "SZ_Node.h"

SZ_Airplane::SZ_Airplane()
{
	planeStatus = apWaiting;
	delay = 0*60;		//in secs
	tmpDeplayDisplay = new char;
}

SZ_Airplane::~SZ_Airplane()
{
	delete tmpDeplayDisplay;
	tmpDeplayDisplay = NULL;
}

void SZ_Airplane::selectNextStatus()
{    
	planeStatus = successorMode (planeStatus);
	if (planeStatus >= apEndMode) planeStatus = successorMode (apStartMode);

}

SZ_Airplane::apStatus SZ_Airplane::successorMode( const apStatus ap ) const
{
	return (apStatus)(((int)ap) + 1);
}

void SZ_Airplane::selectStatus( const apStatus ap )
{
	if (ap != this->planeStatus)
	{
		this->planeStatus = ap;
		switch (ap)
		{
			case apLanding:	  actionLanding(this);   break;
			case apBoarding:  actionBoarding(this);	 break;
			case apDeparting: actionDeparting(this); break;	// AIRBORNE
			case apDelayed:	  actionDelayed(this);	 break; 
			case apLanded:    actionLanded(this);	 break;	// plane has landed and people are leaving plane
		}
	}
	
}

char* SZ_Airplane::statusName()
{
	switch (planeStatus)
	{
		case apExpected:		return  "EXPECTED"; break;
		case apWaiting:			return  "WAITING"; break;
		case apBoarding:		return	"BOARDING";	break;
		case apDelayed:			
			{			
				sprintf(tmpDeplayDisplay,"%d MIN",getDelay()/60);
				return  tmpDeplayDisplay;	break;
			}
		case apDeparting:		return	"AIRBORNE"; break;
		case apLanding:			return	"LANDING"; break;
		case apLanded:			return  "ARRIVED"; break;
		case apCancel:			return  "CANCEL"; break; 
		default:				return  "?"; break;
	}
	
}

void SZ_Airplane::setSchedule( int hour, int min )
{
	tmStart_param.tm_mday = 1;
	tmStart_param.tm_mon  = 0;
	tmStart_param.tm_hour = hour;
	tmStart_param.tm_min  = min;
	tmStart_param.tm_sec  = 0;
	tmStart_param.tm_year = 2011 - 1900;
}


struct tm* SZ_Airplane::getSchedule()
{
	time = &tmStart_param;
	return time;
}

SZ_Airplane::apStatus SZ_Airplane::getPlaneStatus()
{
	return planeStatus;
}

void SZ_Airplane::setDelay( time_t secs )
{
	delay = secs;
}

time_t SZ_Airplane::getDelay()
{
	return delay;
}

void SZ_Airplane::nextDay()
{
	setDelay(0);
	selectStatus(SZ_Airplane::apWaiting);
	tmStart_param.tm_mday++;
}

void SZ_Airplane::actionLanding(SZ_Airplane* whichPlane)
{
	mySchedule->actionLanding(this);
}

void SZ_Airplane::actionDeparting(SZ_Airplane* whichPlane)
{
	mySchedule->actionDeparting(this);
}

void SZ_Airplane::actionBoarding(SZ_Airplane* whichPlane)
{
	mySchedule->actionBoarding(this);
}

void SZ_Airplane::actionDelayed(SZ_Airplane* whichPlane)
{
	mySchedule->actionDelayed(this);
}

void SZ_Airplane::actionLanded(SZ_Airplane* whichPlane)
{
	mySchedule->actionLanded(this);
}

void SZ_Airplane::actionTimeRunning( SZ_Airplane* whichPlane, int timeleft )
{
	mySchedule->actionTimeRunning(this, timeleft);
}


SZ_Schedule* SZ_Airplane::getMySchedule()
{
	return mySchedule;
}

void SZ_Airplane::setMySchedule( SZ_Schedule* whichSchedule )
{
	mySchedule = (SZ_Schedule*)(whichSchedule);
}

void SZ_Airplane::setGate( SZ_Node* whichNode )
{
	gate = whichNode;
}

SZ_Node* SZ_Airplane::getGate()
{
	return gate;
}


