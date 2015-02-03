/*
 * StackerControlTask.h
 *
 *  Created on: Jan 28, 2015
 *      Author: Programmer
 */
#include "jankyTask.h"
#include "WPILib.h"
#include "Robot.h"

#ifndef SRC_StackerControlTask_H_
#define SRC_StackerControlTask_H_

class StackerControlTask: public JankyTask
{
public:
	StackerControlTask();
	virtual ~StackerControlTask();
	virtual void Run();
	void IncLiftPosition();
	void DecLiftPoistion();
	void LiftOpenLoop(float ouput);
	void SetLiftPosition(int arrayPosition);
	bool GetLiftClosedLoop();
	void Home();

private:
	//int liftPositions[7] = {0, 21625, 42362, 63000, 83625, 104775, 112125};
	int liftPositions[7] = {0, 20858, 41200, 62310, 83123, 103797, 112125};
	float liftPositionSpeeds[7] = {.1, .4, .45, .55, .65, .7, .7};
	int i;
	bool liftClosedLoop;
	float openLoopSpeed;
	int controlRange;
	bool homing;
};

#endif /* SRC_StackerControlTask_H_ */
