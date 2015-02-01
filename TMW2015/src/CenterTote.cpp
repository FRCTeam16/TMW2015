/*
 * CenterTote.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: Programmer
 */

#include "CenterTote.h"

CenterTote::CenterTote() {
	// TODO Auto-generated constructor stub

}

CenterTote::~CenterTote() {
	// TODO Auto-generated destructor stub
}

void CenterTote::Run() {
	float x = 0;
	if(Robot::driveBase->toteNarrowLeft->Get() && !Robot::driveBase->toteNarrowRight->Get())
		x = .2;
	if(!Robot::driveBase->toteNarrowLeft->Get() && Robot::driveBase->toteNarrowRight->Get())
			x = -.2;
	Robot::driveBase->Crab(Robot::oi->getJoystickTwist(),-Robot::oi->getJoystickY(),x,true);
}
