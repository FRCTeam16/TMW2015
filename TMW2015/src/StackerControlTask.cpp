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
	controlRange = 2000;
	homed = false;
	liftSmoked = false;
	prevCycleTime = GetClock();
	homeStartTime = GetClock();
	smokeStartTime = GetClock();
	releaseLiftStartPosition = 0;
	releaseDartStartPosition = 0;
	liftState = OpenLoop;
	releaseState = Down;
	dartClosedLoop = true;
	incStartTime = GetClock();
	autoSpeed = false;
}

StackerControlTask::~StackerControlTask() {

}

void StackerControlTask::Run() {

	int error = GetError();
	float speed = 0;

	float ramp = 0;

	switch(liftState) {

	case Homing:
		if(Robot::stacker->home->Get() && !homed)
			if (Robot::stacker->liftFrontRight->GetPosition() > 20000)
				SetOutput(-.3);
			else
				SetOutput(-.4);
		else {
			Robot::stacker->liftFrontRight->SetPosition(0);
			homed = true;
			SetOutput(0);
		}

		if (homed && Robot::squeezeControl->GetOpened() && (Robot::stacker->dart->GetPosition() < 100 || !Robot::stacker->dart->GetReverseLimitOK())) {
			liftState = ClosedLoop;
		}
		break;

	case ClosedLoop:
		if(autoSpeed) {
			speed = autoLiftPositionSpeeds[i];
			ramp = (GetClock() - incStartTime)*.5;
			if(ramp > 1)
				ramp = 1;
		}
		else {
			speed = liftPositionSpeeds[i];
			ramp = (GetClock() - incStartTime)*2;
			if(ramp > 1)
				ramp = 1;

		}

		if (error > controlRange)
			SetOutput(speed*ramp);
		else if (error > 0)
			SetOutput(speed*error/controlRange);
//		else if (error > -controlRange/2)
//			SetOutput(0);
		else if (error > -controlRange)
			SetOutput(.3*error/controlRange);
		else if (error < -controlRange)
			SetOutput(-.3);
		else
			SetOutput(0);

		if(i == 1 && Robot::squeezeControl->GetSqueezed())
			i++;

		break;

	case OpenLoop:
		SetOutput(openLoopSpeed);
		break;

	case Releasing:
		switch (releaseState) {

		case Down:
			if(releaseLiftStartPosition - Robot::stacker->liftFrontRight->GetPosition() < 21000)
				Robot::stacker->liftFrontRight->Set(-.2);
			else {
				Robot::stacker->liftFrontRight->Set(0);
				dartClosedLoop = true;
				Robot::stacker->dart->SetControlMode(CANSpeedController::kPosition);
				Robot::stacker->dart->Set(releaseDartStartPosition + 200);
				releaseState = Back;
			}
			break;

		case Back:
			if (fabs(Robot::stacker->dart->GetClosedLoopError()) < 2) {
					releaseState = ReleaseContainer;
					Robot::squeezeControl->Open();
			}
			break;

		case ReleaseContainer:
			if (Robot::squeezeControl->GetOpened())
				releaseState = Up;
			break;

		case Up:
			i = 7;
			liftState = ClosedLoop;
			releaseState = Down;
			break;
		}

		break;

	case ContainerUp:
		if(GetClock() - incStartTime < 3) {
			error = 116806 - Robot::stacker->liftFrontRight->GetPosition();
			speed = .75;
		}
		else
//			error = top - - Robot::stacker->liftFrontRight->GetPosition();

		if (error > controlRange)
			SetOutput(speed);
		else if (error > 0)
			SetOutput(speed*error/controlRange);
		else if (error > -controlRange)
			SetOutput(.3*error/controlRange);
		else if (error < -controlRange)
			SetOutput(-.3);
		else
			SetOutput(0);
		break;
	}
}

void StackerControlTask::IncLiftPosition() {

	liftState = ClosedLoop;

	if(i < 7)
		i++;
	incStartTime = GetClock();

	if (i == 1) {
		Robot::squeezeControl->Close(true);
	}
}

void StackerControlTask::DecLiftPoistion() {

	liftState = ClosedLoop;

	if(i > 0)
		i--;
}
void StackerControlTask::SetLiftPosition(int arrayPosition) {

	liftState = ClosedLoop;
	i = arrayPosition;
}

void StackerControlTask::LiftOpenLoop(float Speed) {

	liftState = OpenLoop;
	openLoopSpeed = Speed;
}

bool StackerControlTask::GetLiftClosedLoop() {
	return liftState == ClosedLoop;
}

void StackerControlTask::Home() {
	liftState = Homing;
	homeStartTime = GetClock();
	Robot::squeezeControl->Open();
	homed = false;
	i = 0;
}

void StackerControlTask::Release() {
	liftState = Releasing;
	releaseState = Down;
	releaseLiftStartPosition = Robot::stacker->liftFrontRight->GetPosition();
	releaseDartStartPosition = Robot::stacker->dart->GetPosition();
}

int StackerControlTask::GetError() {
	return liftPositions[i] + 1000 - Robot::stacker->liftFrontRight->GetPosition();
	//added 1000 to positions for practice bot
}

bool StackerControlTask::GetHoming() {
	return liftState == Homing;
}

int StackerControlTask::Geti() {
	return i;
}

bool StackerControlTask::GetSmoked() {
	return liftSmoked;
}

void StackerControlTask::SetOutput(float output) {
	if(Robot::stacker->liftFrontRight->GetOutputCurrent() < 40 && Robot::stacker->liftFrontLeft->GetOutputCurrent() < 40 )
		smokeStartTime = GetClock();

//	if (GetClock() - smokeStartTime > 2)
//		liftSmoked = true;

	if(!liftSmoked)
		Robot::stacker->liftFrontRight->Set(12.5/Robot::stacker->liftFrontRight->GetBusVoltage()*output);
	else
		Robot::stacker->liftFrontRight->Set(0);
}

void StackerControlTask::SetDartClosedLoop(bool ClosedLoop) {
	dartClosedLoop = ClosedLoop;
}

bool StackerControlTask::GetDartClosedLoop() {
	return dartClosedLoop;
}

void StackerControlTask::SetAutoSpeed(bool AutoSpeed) {
	autoSpeed = AutoSpeed;
}

void StackerControlTask::LiftContainer() {
	incStartTime = GetClock();
	liftState = ContainerUp;
}
