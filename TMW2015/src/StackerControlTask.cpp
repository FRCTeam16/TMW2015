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
	homed = false;
	prevCycleTime = GetClock();
	homeStartTime = GetClock();
}

StackerControlTask::~StackerControlTask() {

}

void StackerControlTask::Run() {

//	cout << "StackerCycleTime: " << GetClock() - prevCycleTime << endl;
	prevCycleTime = GetClock();

	if(homing) {
		if(Robot::stacker->home->Get() && !homed)
			if (Robot::stacker->liftFrontRight->GetPosition() > 20000)
				SetOutput(-.6);
			else
				SetOutput(-.4);
		else {
			Robot::stacker->liftFrontRight->SetPosition(0);
			homed = true;
			SetOutput(0);
		}

		if (homed && Robot::squeezeControl->GetOpened() && (Robot::stacker->dart->GetPosition() < 100 || !Robot::stacker->dart->GetReverseLimitOK())) {
			homing = false;
			i = 0;
		}
	}
	else {
		if(liftClosedLoop) {
			int error = GetError();
			if (error > controlRange)
				SetOutput(liftPositionSpeeds[i]);
			else if (error > 0)
				SetOutput(liftPositionSpeeds[i]*error/controlRange);
			else if (error > -controlRange)
				SetOutput(.2*error/controlRange);
			else if (error < -controlRange)
				SetOutput(-.2);
			else
				SetOutput(0);
		}
		else {
			SetOutput(openLoopSpeed);
		}
	}

}

void StackerControlTask::IncLiftPosition() {

	liftClosedLoop = true;

	if(i < 6)
		i++;

	SetLiftPosition(i);

	if (i == 1) {
		Robot::squeezeControl->Close(true);
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
	homed = false;
	homeStartTime = GetClock();
	Robot::squeezeControl->Open();
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

void StackerControlTask::SetOutput(float output) {
	Robot::stacker->liftFrontRight->Set(12.5/Robot::stacker->liftFrontRight->GetBusVoltage()*output);
}
