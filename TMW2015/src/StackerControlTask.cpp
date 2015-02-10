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
	openLoopSpeed = 0;
	liftClosedLoop = false;
	controlRange = 2000;
	homing = false;
	squeezed = false;
	extended = false;
	homed = false;
	prevCycleTime = GetClock();
	homeStartTime = GetClock();
	squeezeStartTime = GetClock();
}

StackerControlTask::~StackerControlTask() {

}

void StackerControlTask::Run() {

//	cout << "StackerCycleTime: " << GetClock() - prevCycleTime << endl;
	prevCycleTime = GetClock();

	if(homing) {
		if(Robot::stacker->home->Get() && !homed)
			if (Robot::stacker->liftFrontRight->GetPosition() > 20000)
				Robot::stacker->liftFrontRight->Set(-.6);
			else
				Robot::stacker->liftFrontRight->Set(-.3);
		else {
			Robot::stacker->liftFrontRight->SetPosition(0);
			homed = true;
			Robot::stacker->liftFrontRight->Set(0);
		}

		if (/*ADD POSTION IN HERE*/(Robot::pdp->GetCurrent(9) < 10 && !squeezed) || GetClock() - homeStartTime < 1.5)
			Robot::stacker->squeeze->Set(.5);
		else {
			Robot::stacker->squeeze->Set(0);
			squeezed = true;
		}


		if (homed && squeezed && Robot::stacker->dart->GetPosition() < 70) {
			homing = false;
			i = 0;
		}
	}
	else {
		if(liftClosedLoop) {
			int error = GetError();
			if (error > controlRange)
				Robot::stacker->liftFrontRight->Set(liftPositionSpeeds[i]);
			else if (error > 0)
				Robot::stacker->liftFrontRight->Set(liftPositionSpeeds[i]*error/controlRange);
			else if (error > -controlRange)
				Robot::stacker->liftFrontRight->Set(.2*error/controlRange);
			else if (error < -controlRange)
				Robot::stacker->liftFrontRight->Set(-.2);
			else
				Robot::stacker->liftFrontRight->Set(0);
		}
		else {
			Robot::stacker->liftFrontRight->Set(openLoopSpeed);
		}
	}

	if (i == 1 && fabs(GetError()) < 1000) {
		if ((Robot::pdp->GetCurrent(9) < 10 && !squeezed) || GetClock() - squeezeStartTime < 1.5)
			Robot::stacker->squeeze->Set(-0.5);
		else {
			Robot::stacker->squeeze->Set(0);
			squeezed = true;
		}
	}

}

void StackerControlTask::IncLiftPosition() {

	liftClosedLoop = true;

	if(i < 6)
		i++;

	SetLiftPosition(i);

	if (i == 1) {
		squeezed = false;
		squeezeStartTime = GetClock();
	}
}

void StackerControlTask::DecLiftPoistion() {

	liftClosedLoop = true;

	if(i > 0)
		i--;
}
void StackerControlTask::SetLiftPosition(int arrayPosition) {

	liftClosedLoop = true;
	Robot::stacker->liftFrontRight->Set(liftPositions[arrayPosition]);
}

void StackerControlTask::LiftOpenLoop(float Speed) {

	liftClosedLoop = false;
	homing = false;
	openLoopSpeed = Speed;
}

bool StackerControlTask::GetLiftClosedLoop() {
	return liftClosedLoop;
}

void StackerControlTask::Home() {
	homing = true;
	squeezed = false;
	homed = false;
	homeStartTime = GetClock();
}

int StackerControlTask::GetError() {
	return liftPositions[i] - Robot::stacker->liftFrontRight->GetPosition();
}

bool StackerControlTask::GetHoming() {
	return homing;
}

int StackerControlTask::Geti() {
	return i;
}
