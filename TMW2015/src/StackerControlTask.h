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
	void Release();
	bool GetLiftClosedLoop();
	void Home();
	int GetError();
	bool GetHoming();
	int Geti();
	bool GetSmoked();
	void SetDartClosedLoop(bool ClosedLoop);
	bool GetDartClosedLoop();

private:
	//int liftPositions[7] = {0, 21625, 42362, 63000, 83625, 104775, 112125};
	int liftPositions[8] = {0, 20000, 20858, 41600, 62000, 83123, 103797, 112125};
	float liftPositionSpeeds[8] = {.1, .4, .4, .5, .6, .7, .75, .75};
	typedef enum {Homing, ClosedLoop, OpenLoop, Releasing} LiftState;
	LiftState liftState;
	typedef enum {Down, Back, ReleaseContainer, Up} ReleaseState;
	ReleaseState releaseState;
	int i;
	float openLoopSpeed;
	bool dartClosedLoop;
	int controlRange;
	int releaseDartStartPosition;
	int releaseLiftStartPosition;
	bool homed;
	float prevCycleTime;
	float homeStartTime;
	float smokeStartTime;
	bool liftSmoked;
	void SetOutput(float output);
};

#endif /* SRC_StackerControlTask_H_ */
