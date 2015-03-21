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
	holdPosition = Robot::stacker->liftFrontRight->GetPosition();
	dropPos = false;
	pickupComplete = false;
	pickupStage1Complete = false;
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
			liftState = ClosedLoop;
			SetOutput(0);
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

		if (error > controlRange) {
			if(i>2)
				SetOutput(ramp);
			else
				SetOutput(speed*ramp);
		}
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
		SetOutput(openLoopSpeed);
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
			if(releaseLiftStartPosition - Robot::stacker->liftFrontRight->GetPosition() < 12000)
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

	case ContainerPickup:
			if (error > controlRange)
				SetOutput(1);
			else if (error > 0)
				SetOutput(speed*error/controlRange);
			else if (error > -controlRange)
				SetOutput(.3*error/controlRange);
			else if (error < -controlRange)
				SetOutput(-.3);
			else
				SetOutput(0);

			if(fabs(error) < 500) {
				if(i == 5)
					pickupStage1Complete = true;
				if(i == 11) {
					pickupComplete = true;
					pickupStage1Complete = false;
				}
			}

		break;
	}
}

void StackerControlTask::IncLiftPosition() {

	liftState = ClosedLoop;
	releaseStage1Complete = false;
	dropPos = false;
	if(i < 8)
		i++;
	incStartTime = GetClock();

	if (i == 1) {
		Robot::squeezeControl->Close(true);
	}
}

void StackerControlTask::DecLiftPoistion() {

	liftState = ClosedLoop;
	releaseStage1Complete = false;
	dropPos = false;
	if(i > 0)
		i--;
}
void StackerControlTask::SetLiftPosition(int arrayPosition) {

	liftState = ClosedLoop;
	releaseStage1Complete = false;
	dropPos = false;
	i = arrayPosition;
}

void StackerControlTask::LiftOpenLoop(float Speed) {
	liftState = OpenLoop;
	openLoopSpeed = Speed;
	dropPos = false;
	if(fabs(Speed) > .1) {
		holdPosition = Robot::stacker->liftFrontRight->GetPosition();
	}

}
bool StackerControlTask::GetLiftClosedLoop() {
	return liftState == ClosedLoop;
}

void StackerControlTask::Home() {
	if(i != 0 || Robot::stacker->liftFrontRight->GetPosition() > 10000) {
		liftState = Homing;
		homeStartTime = GetClock();
		homed = false;
		dropPos = false;
		releaseStage1Complete = false;
		i = 0;
	}
	Robot::squeezeControl->Home();
}

void StackerControlTask::Release() {
	liftState = Releasing;
	dropPos = false;
	if(!releaseStage1Complete) {
		releaseState = DartOut;
		releaseLiftStartPosition = Robot::stacker->liftFrontRight->GetPosition();
	}
	else
		releaseState = ReleaseContainer;
}

void StackerControlTask::PickupContainer() {
	liftState = ContainerPickup;
	dropPos = false;
	if(!pickupStage1Complete) {
		Robot::stacker->dart->SetControlMode(CANSpeedController::kPosition);
		Robot::stacker->dart->Set(49);
		dartClosedLoop = true;
		i = 5;
		pickupComplete = false;
	}
	else {
		i = 11;
	}
}

int StackerControlTask::GetError() {
	int error = liftPositions[i] - Robot::stacker->liftFrontRight->GetPosition() - 1000;
	if(dropPos)
		error -= 3500;
	return error;
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
	dropPos = false;
}

void StackerControlTask::SetDropPos(bool value) {
	dropPos = value;
}

bool StackerControlTask::GetDropPos() {
	return dropPos;
}
