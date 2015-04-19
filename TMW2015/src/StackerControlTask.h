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
	void Reset();
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
	void SetDropPos(bool value);
	bool GetDropPos();
	void PickupContainer();

private:
//	int liftPositions[10] = {0, 19500, 20858, 41000, 62000, 83000, 104000, 112125, 67400, 116806};  //competition
//	int liftPositions[12] = {0, 19500, 24000, 44000, 61000, 78200, 96000, 113500, 113500, 61000, 116806, 52378}; // Competition bot settings
//	float liftPositionSpeeds[12] =     {.1, .5, .5, .6, 1.0, 1.0, 1.0, 1.0, 1.0, .8, .85, .5};
//	float liftPositionSpeeds[11] =     {.1, .5, .5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, .8, .85};
//	float autoliftPositions[12] =      {22700, 43000, 43000, 61000, 61000, 61000, 70000, 70000, 61000, 61000, 61000, 61000}; // Midwest style 3 tote auto positions
	float liftPositionSpeeds[12] =     {.1,   .5,    .6,    .7,    .7,    .7,    .8,    .85,   .85,    .8,     .85,    .5}; // SAVED BEFORE CHANGING ON 4/17/2015
//	int liftPositions[12] =            {0, 34281, 24000, 41500, 59500, 77000, 94000, 111500, 111500, 59000, 114806, 50378}; //Practice bot settings
	int liftPositions[12] =            {0, 34281, 18600, 36600, 54300, 71800, 89300, 106750, 109000, 59000, 114806, 50378}; //for the raised lift
	float autoliftPositions[12] =      {0, 34281, 18600, 36600, 54300, 71800, 89300, 106750, 109000, 59000, 114806, 50378}; //for the raised lift old positions: {0, 34281, 24000, 42500, 59500, 77000, 94000, 97000, 97000, 113500, 114806, 50378};
	float autoLiftPositionSpeeds[12] = {.1,    .7,    .7,    .5,   .7,    .7,     .7,    1.0,   1.0,   1.0,   1.0,   1.0};
	bool autoSpeed;
	typedef enum {Homing, ClosedLoop, OpenLoop, Releasing, ContainerUp, ContainerPickup} LiftState;
	LiftState liftState;
	typedef enum {DartOut, Down, Vert, ReleaseContainer, WaitForOpen, Back, Up} ReleaseState;
	typedef enum {ConveyorToFirst, ConveyorToSecond}PickupState;
	int dartStartRelease = 500;
	int dartVert = 588;
	int dartEndRelease = 800;
	ReleaseState releaseState;
	bool releaseStage1Complete;
	bool dropPos;
	int holdPosition;
	int i;
	float openLoopSpeed;
	bool dartClosedLoop;
	int controlRange;
	int releaseLiftStartPosition;
	bool homed;
	bool pickupStage1Complete;
	bool pickupComplete;
	double prevCycleTime;
	double homeStartTime;
	double smokeStartTime;
	double incStartTime;
	bool liftSmoked;
	void SetOutput(float output);
};

#endif /* SRC_StackerControlTask_H_ */
