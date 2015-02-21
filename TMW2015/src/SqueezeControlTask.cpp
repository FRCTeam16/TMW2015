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
}

SqueezeControlTask::~SqueezeControlTask() {

}

void SqueezeControlTask::Run() {

	switch (squeezerState) {

	case Opening:
		if (Robot::stacker->squeeze->GetOutputCurrent() > 4 && Robot::stacker->squeezePosition->GetRaw() > -15000)
			currentStop++;
		else
			currentStop = 0;

		if (currentStop > 50 || GetClock() - startTime > 3.0) { // check for stop
			Robot::stacker->squeeze->Set(0);
			opened = true;
			squeezerState = Hold;
			Robot::stacker->squeezePosition->Reset();
		}
		else if (Robot::stacker->squeezePosition->GetRaw() > -5000){ // check for slow down
			Robot::stacker->squeeze->Set(-.50);
		}
		else { // just run
			Robot::stacker->squeeze->Set(-.75);
		}
		break;

	case Closing:
		if (Robot::stacker->squeeze->GetOutputCurrent() < 10)
			currentStop = 0;
		else
			currentStop++;

		if (currentStop > 100 || GetClock() - startTime > 1.5 || (currentStop == 0 && Robot::stacker->squeezePosition->GetRaw() < -50000)) { // check for stop
			Robot::stacker->squeeze->Set(0);
			squeezed = true;
			squeezerState = Hold;
		}
		else {
			Robot::stacker->squeeze->Set(0.75); // just run
		}
		break;

	case WaitForLift:
		if(fabs(Robot::stackerControl->GetError()) < 1000) {
			squeezerState = Closing;
			startTime = GetClock();
		}
			break;

	case Hold:
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
	if (wait)
		squeezerState = WaitForLift;
	else {
		squeezerState = Closing;
		startTime = GetClock();
	}

}

bool SqueezeControlTask::GetSqueezed() {
	return squeezed;
}

bool SqueezeControlTask::GetOpened () {
	return opened;
}
