/*
 * CenterToteTask.h
 *
 *  Created on: Jan 28, 2015
 *      Author: Programmer
 */
#include "jankyTask.h"
#include "WPILib.h"
#include "Robot.h"

#ifndef SRC_CenterToteTask_H_
#define SRC_CenterToteTask_H_

class CenterToteTask: public JankyTask
{
public:
	CenterToteTask();
	virtual void Run();
	void Reset();

private:
	float rgain;
	float xgain;
	float rrad;
	bool turnright;
	bool turnleft;
	bool picked;
	int i;
	int j;
	int inPosCount;
};

#endif /* SRC_CenterToteTask_H_ */
