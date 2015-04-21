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
	squeezerState = Off;
	startTime = GetClock();
	currentStop = 0;
	containerTouchTime = 0;
	containerTouched = false;
	openLoopOutput = 0;
	homed = false;
}

SqueezeControlTask::~SqueezeControlTask() {

}

void SqueezeControlTask::Run() {

	switch (squeezerState) {

	case Opening:
		if (Robot::stacker->squeeze->GetOutputCurrent() > 10 && GetPosition() > -1300)
			currentStop++;
		else
			currentStop = 0;

		if (GetPosition() > 11700 || currentStop > 5 || GetClock() - startTime > 3.0) { // check for stop
			Robot::stacker->squeeze->Set(0);
			opened = true;
			squeezerState = Off;
		}
		else if (GetPosition() > 4000){ // check for slow down
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
			squeezerState = Off;
			Robot::stacker->squeezePosition->Reset();
			homed = true;
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

		if (containerTouched > 0 && GetClock() - containerTouchTime > .5) {
			squeezed = true;
			squeezerState = Hold;
		}
		else if (GetPosition() < -12300) {
			squeezed = true;
			squeezerState = Off;
		}
		else{
			Robot::stacker->squeeze->Set(1.0); // just run
		}
		break;

	case WaitForLift:
		if(fabs(Robot::stackerControl->GetError()) < 1000) {
			squeezerState = Closing;
			startTime = GetClock();
		}
			break;

	case Off:
		Robot::stacker->squeeze->Set(0);
		break;

	case Hold:
		Robot::stacker->squeeze->Set(.1);
		break;

	case OpenLoop:
		Robot::stacker->squeeze->Set(openLoopOutput);
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

void SqueezeControlTask::SetOpenLoop(float output) {
	squeezerState = OpenLoop;
	openLoopOutput = output;
}

bool SqueezeControlTask::GetSqueezed() {
	return squeezed;
}

bool SqueezeControlTask::GetOpened () {
	return opened;
}

int SqueezeControlTask::GetPosition() {
	int position = Robot::stacker->squeezePosition->GetRaw();
	if(homed)
		position += 13700;
}
