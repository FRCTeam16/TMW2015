/*
 * SqueezeControlTask.h
 *
 *  Created on: Jan 28, 2015
 *      Author: Programmer
 */
#include "JankySlowTask.h"
#include "WPILib.h"
#include "Robot.h"

#ifndef SRC_SqueezeControlTask_H_
#define SRC_SqueezeControlTask_H_

class SqueezeControlTask: public JankySlowTask
{
public:
	SqueezeControlTask();
	virtual ~SqueezeControlTask();
	virtual void Run();
	void Open();
	void Close(bool wait);
	void Home();
	void SetOpenLoop(float output);
	bool GetSqueezed();
	bool GetOpened();

private:
	typedef enum {WaitForLift, Opening, Closing, Homing, Hold, OpenLoop} SqueezerState;
	SqueezerState squeezerState;
	bool squeezed;
	bool opened;
	float openLoopOutput;
	float startTime;
	int currentStop;
	float containerTouchTime;
	bool containerTouched;
};

#endif /* SRC_SqueezeControlTask_H_ */
