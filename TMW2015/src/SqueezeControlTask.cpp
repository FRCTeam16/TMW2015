/*
 * SqueezeControlTask.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: Programmer
 */

#include "SqueezeControlTask.h"
using namespace std;

SqueezeControlTask::SqueezeControlTask() {
	squeezed = false;
	opened = false;
	squeezerState = Hold;
	startTime = GetClock();
	currentStop = 0;
	containerTouchTime = 0;
	containerTouched = false;
}

SqueezeControlTask::~SqueezeControlTask() {

}

void SqueezeControlTask::Run() {

	switch (squeezerState) {

	case Opening:
		if (Robot::stacker->squeeze->GetOutputCurrent() > 10 && Robot::stacker->squeezePosition->GetRaw() > -15000)
			currentStop++;
		else
			currentStop = 0;

		if (Robot::stacker->squeezePosition->GetRaw() > -2000 || currentStop > 5 || GetClock() - startTime > 3.0) { // check for stop
			Robot::stacker->squeeze->Set(0);
			opened = true;
			squeezerState = Hold;
		}
		else if (Robot::stacker->squeezePosition->GetRaw() > -4000){ // check for slow down
			Robot::stacker->squeeze->Set(-.5);
		}
		else { // just run
			Robot::stacker->squeeze->Set(-1.0);
		}
		break;

	case Homing:
		if (Robot::stacker->squeeze->GetOutputCurrent() > .75)
			currentStop++;
		else
			currentStop = 0;

		if (currentStop > 3 || GetClock() - startTime > 3.0) { // check for stop
			Robot::stacker->squeeze->Set(0);
			opened = true;
			squeezerState = Hold;
			Robot::stacker->squeezePosition->Reset();
		}
		else { //run
			Robot::stacker->squeeze->Set(-.3);
		}
		break;

	case Closing:
		if (Robot::stacker->squeeze->GetOutputCurrent() > 2.5 && !containerTouched) {
			containerTouchTime = GetClock();
			containerTouched = true;
		}

		if ((containerTouched > 0 && GetClock() - containerTouchTime > 1.0) || Robot::stacker->squeezePosition->GetRaw() < -30000) {
			Robot::stacker->squeeze->Set(0);
			squeezed = true;
			squeezerState = Hold;
		}
		else {
			Robot::stacker->squeeze->Set(1.0); // just run
		}
		break;

	case WaitForLift:
		if(fabs(Robot::stackerControl->GetError()) < 1000) {
			squeezerState = Closing;
			startTime = GetClock();
		}
			break;

	case Hold:
		Robot::stacker->squeeze->Set(0);
		break;
	}
}

void SqueezeControlTask::Open() {
	squeezed = false;
	opened = false;
	startTime = GetClock();
	currentStop = 0;
	squeezerState = Opening;
}

void SqueezeControlTask::Close(bool wait) {
	squeezed = false;
	opened = false;
	currentStop = 0;
	containerTouchTime = 0;
	containerTouched = false;
	if (wait)
		squeezerState = WaitForLift;
	else {
		squeezerState = Closing;
		startTime = GetClock();
	}

}

void SqueezeControlTask::Home() {
	squeezed = false;
	startTime = GetClock();
	currentStop = 0;
	squeezerState = Homing;
}

bool SqueezeControlTask::GetSqueezed() {
	return squeezed;
}

bool SqueezeControlTask::GetOpened () {
	return opened;
}
