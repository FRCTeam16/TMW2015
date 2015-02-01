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

private:
	int liftPositions[7] = {0, 24700, 45700, 66411, 87071, 107840, 115200};
	float liftPositionSpeeds[7] = {.3, .3, .4, .6, .6, .6, .6};
	int i;
	bool liftClosedLoop;
	float gain;
	float openLoopSpeed;
	int controlRange;

};

#endif /* SRC_StackerControlTask_H_ */
