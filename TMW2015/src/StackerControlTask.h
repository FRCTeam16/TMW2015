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
	void SetAutoSpeed(bool AutoSpeed);
	void LiftContainer();

private:
//	int liftPositions[8] = {0, 20000, 20858, 41600, 62000, 83123, 103797, 112125};  //Competition
	int liftPositions[10] = {0, 19500, 20858, 41750, 62800, 83944, 104706, 112125, 67400, 116806};  //Practice
	//float liftPositionSpeeds[10] = {.1, .4, .4, .5, .6, .7, .75, .75, .7, .75}; Remmed on 2/28/07 after new lift
	float liftPositionSpeeds[10] =     {.1, .5, .5, .6, .7, .8, .85, .85, .8, .85};
	float autoLiftPositionSpeeds[10] = {.1, .4, .4, .5, .5, .7, .75, .75, .7, .75};
	bool autoSpeed;
	typedef enum {Homing, ClosedLoop, OpenLoop, Releasing, ContainerUp} LiftState;
	LiftState liftState;
	typedef enum {DartOut, Down, Vert, ReleaseContainer, WaitForOpen, Back, Up} ReleaseState;
	int dartStartRelease = 400;
	int dartVert = 500;
	int dartEndRelease = 800;
	ReleaseState releaseState;
	bool releaseStage1Complete;
	int holdPosition;
	int i;
	float openLoopSpeed;
	bool dartClosedLoop;
	int controlRange;
	int releaseLiftStartPosition;
	bool homed;
	double prevCycleTime;
	double homeStartTime;
	double smokeStartTime;
	double incStartTime;
	bool liftSmoked;
	void SetOutput(float output);
};

#endif /* SRC_StackerControlTask_H_ */
