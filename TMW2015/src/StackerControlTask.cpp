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
	liftState = OpenLoop;
	releaseState = Down;
	releaseStage1Complete = false;
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
				SetOutput(-.7);
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
			ramp = (GetClock() - incStartTime)*1;
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
		if(fabs(openLoopSpeed) > .1)
			SetOutput(openLoopSpeed);
		else {
			error = holdPosition - Robot::stacker->liftFrontRight->GetPosition();
			speed = .75;
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
		}
		break;

	case Releasing:
		switch (releaseState) {

		case DartOut:
			Robot::stacker->dart->SetControlMode(CANSpeedController::kPosition);
			Robot::stacker->dart->Set(dartStartRelease);
			dartClosedLoop = true;
			if (fabs(Robot::stacker->dart->GetClosedLoopError()) < 2)
				releaseState = Down;
			break;

		case Down:
			if(releaseLiftStartPosition - Robot::stacker->liftFrontRight->GetPosition() < 21000)
				Robot::stacker->liftFrontRight->Set(-.2);
			else {
				Robot::stacker->liftFrontRight->Set(0);
				Robot::stacker->dart->Set(dartVert);
				releaseState = Vert;
			}
			break;

		case Vert:
			if (fabs(Robot::stacker->dart->GetClosedLoopError()) < 2) {
				releaseStage1Complete = true;
			}
			break;

		case ReleaseContainer:
			Robot::squeezeControl->Open();
			releaseState = WaitForOpen;
			break;

		case WaitForOpen:
			if (Robot::squeezeControl->GetOpened()) {
				releaseState = Back;
				Robot::stacker->dart->Set(dartEndRelease);
			}
			break;

		case Back:
			if (fabs(Robot::stacker->dart->GetClosedLoopError()) < 2) {
				releaseState = Up;
			}
			break;

		case Up:
			i = 7;
			liftState = ClosedLoop;
			releaseState = Down;
			releaseStage1Complete = false;
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
	releaseStage1Complete = false;

	if(i < 7)
		i++;
	incStartTime = GetClock();

	if (i == 1) {
		Robot::squeezeControl->Close(true);
	}
}

void StackerControlTask::DecLiftPoistion() {

	liftState = ClosedLoop;
	releaseStage1Complete = false;
	if(i > 0)
		i--;
}
void StackerControlTask::SetLiftPosition(int arrayPosition) {

	liftState = ClosedLoop;
	releaseStage1Complete = false;
	i = arrayPosition;
}

void StackerControlTask::LiftOpenLoop(float Speed) {
	liftState = OpenLoop;
	openLoopSpeed = Speed;

	if(fabs(Speed) > .1) {
		holdPosition = Robot::stacker->liftFrontRight->GetPosition();
	}

}
bool StackerControlTask::GetLiftClosedLoop() {
	return liftState == ClosedLoop;
}

void StackerControlTask::Home() {
	liftState = Homing;
	homeStartTime = GetClock();
	Robot::squeezeControl->Open();
	homed = false;
	releaseStage1Complete = false;
	i = 0;
}

void StackerControlTask::Release() {
	liftState = Releasing;
	if(!releaseStage1Complete) {
		releaseState = DartOut;
		releaseLiftStartPosition = Robot::stacker->liftFrontRight->GetPosition();
	}
	else
		releaseState = ReleaseContainer;
}

int StackerControlTask::GetError() {
	return liftPositions[i] - Robot::stacker->liftFrontRight->GetPosition();
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
