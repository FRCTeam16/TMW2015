/*
 * CenterToteTask.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: Programmer
 */

#include "CenterToteTask.h"
using namespace std;

CenterToteTask::CenterToteTask() {
	xgain=.003;
	rgain=-.001;
	rrad=1;
	turnright=true;
	turnleft=true;
	picked = false;
	dropped = false;
	i=0;
	j=0;
	inPosCount = 0;
	outCounter = 0;
	crookedCounter = 0;
	resetCounter = 0;
}

void CenterToteTask::Run() {
	if(Robot::driveBase->toteNarrowLeft->Get() ^ Robot::driveBase->toteNarrowRight->Get())
		crookedCounter++;
	else
		crookedCounter = 0;


		if(crookedCounter > 200) {
//			outCounter = 5;
		}

		if(outCounter) {
			Robot::stacker->suckerLeft->Set(.5);
			Robot::stacker->suckerLeft->Set(.5);
			outCounter--;
		}
		else {
			//run the sucker(s) if the prox isn't made, hold a 20% if it is made
			if(Robot::driveBase->toteNarrowLeft->Get())
				Robot::stacker->suckerLeft->Set(-1.0);
			else
				Robot::stacker->suckerLeft->Set(-.2);
			if(Robot::driveBase->toteNarrowRight->Get())
				Robot::stacker->suckerRight->Set(-1.0);
			else
				Robot::stacker->suckerRight->Set(-.2);

			//if both proxes are made, pickup
			if(!picked && !Robot::driveBase->toteNarrowLeft->Get() && !Robot::driveBase->toteNarrowRight->Get()) {
				inPosCount++;
			}
			else {
				inPosCount = 0;
			}
		}


	if(!picked && inPosCount > 100) {
		Robot::stackerControl->IncLiftPosition();
		picked = true;
		}


	//Reset if picked and dropped and all sensors aren't made
	if(picked && Robot::driveBase->toteNarrowLeft->Get() && Robot::driveBase->toteNarrowRight->Get())
		resetCounter++;
	else
		resetCounter = 0;

	if(resetCounter > 100)
		Reset();

/*	if(!dropped) { //Move conveyor to drop position if not there
		Robot::stackerControl->SetDropPos(true);
		dropped = true;
	}
	else if(fabs(Robot::stackerControl->GetError()) < 500) { //Wait for conveyor to move to drop position and then start centering

		if(Robot::driveBase->toteWideLeft->Get() && Robot::driveBase->toteWideRight->Get() && Robot::driveBase->toteNarrowLeft->Get() && Robot::driveBase->toteNarrowRight->Get()) {
			inPosCount++;
		}
		else
			inPosCount = 0;

		if(picked)
			Robot::driveBase->Crab(0,.15,0,false);

		if(!picked && inPosCount > 1) {
			Robot::stackerControl->IncLiftPosition();
			picked = true;
			}

		else if(Robot::driveBase->toteWideLeft->Get() ^ Robot::driveBase->toteWideRight->Get()) {
			if(Robot::driveBase->toteWideLeft->Get()){
				if(turnright) {
					j=100;
					turnright=false;
				}
				j++;
				rrad = .4;
				turnleft=true;
			}

			else if (Robot::driveBase->toteWideRight->Get()) {
				if(turnleft) {
					j=100;
					turnleft=false;
				}
				j++;
				rrad=2.725;
				turnright=true;
			}

			Robot::driveBase->Steer(rrad,j*rgain,1.2);
		}
		else if(!Robot::driveBase->toteNarrowLeft->Get() ^ !Robot::driveBase->toteNarrowRight->Get()){

			if(!Robot::driveBase->toteNarrowLeft->Get()) {
				if(i>0)
					i=-100;
				i--;
			}
			else if (!Robot::driveBase->toteNarrowRight->Get()) {
				if(i<0)
					i=100;
				i++;
			}
			else
				i=0;
			Robot::driveBase->Crab(0,0,i*xgain, false);
		}
		else
			Robot::driveBase->Crab(0,.15,0,false);
	}
	else
		Robot::driveBase->Crab(0,.2,0,false);
*/
}

void CenterToteTask::Reset() {
	i=0;
	j=0;
	turnright=true;
	turnleft=true;
	picked = false;
	dropped = false;
}
