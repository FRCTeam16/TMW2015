/*
 * StackerControlTask.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: Programmer
 */

#include "StackerControlTask.h"
using namespace std;

StackerControlTask::StackerControlTask() {
	i = 0;
	gain = 2.0;
	openLoopSpeed = 0;
	liftClosedLoop = false;
	controlRange = 2000;
}

StackerControlTask::~StackerControlTask() {

}

void StackerControlTask::Run() {
	int error = liftPositions[i] - Robot::stacker->liftFrontRight->GetPosition();

	if(liftClosedLoop) {
		if (error > controlRange) {
			Robot::stacker->liftFrontRight->Set(liftPositionSpeeds[i]);
		}
		else if (error > 0)
			Robot::stacker->liftFrontRight->Set(liftPositionSpeeds[i]*error/controlRange);
		else if (error > -controlRange)
			Robot::stacker->liftFrontRight->Set(-.3*error/controlRange);
		else if (error < -controlRange)
			Robot::stacker->liftFrontRight->Set(-.3);
		else
			Robot::stacker->liftFrontRight->Set(0);
	}
	else {
		Robot::stacker->liftFrontRight->Set(openLoopSpeed);
	}
	//cout << "ClosedLoop: " << liftClosedLoop << " || i: " << i << " || liftPostions[i]: " << liftPositions[i] << " || Error: " << error << endl;

}

void StackerControlTask::IncLiftPosition() {

	liftClosedLoop = true;

	if(i < 6)
		i++;

	SetLiftPosition(i);
}

void StackerControlTask::DecLiftPoistion() {

	liftClosedLoop = true;

	if(i > 0)
		i--;
}
void StackerControlTask::SetLiftPosition(int arrayPosition) {

		liftClosedLoop = true;

//	Robot::stacker->liftFrontRight->SetPID(liftP[arrayPosition], liftI[arrayPosition], liftD[arrayPosition]);
//	Robot::stacker->liftFrontRight->SetCloseLoopRampRate(liftRampRate[arrayPosition]);
//	Robot::stacker->liftFrontRight->SetIzone(liftIZone[arrayPosition]);
	Robot::stacker->liftFrontRight->Set(liftPositions[arrayPosition]);
}

void StackerControlTask::LiftOpenLoop(float Speed) {

	liftClosedLoop = false;

	openLoopSpeed = Speed;
}

bool StackerControlTask::GetLiftClosedLoop() {
	return liftClosedLoop;
}
