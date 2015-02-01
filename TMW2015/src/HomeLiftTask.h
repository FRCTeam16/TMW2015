/*
 * HomeLiftTask.h
 *
 *  Created on: Jan 28, 2015
 *      Author: Programmer
 */
#include "jankyTask.h"
#include "WPILib.h"
#include "Robot.h"

#ifndef SRC_HomeLiftTask_H_
#define SRC_HomeLiftTask_H_

class HomeLiftTask: public JankyTask
{
public:
	HomeLiftTask();
	virtual ~HomeLiftTask();
	virtual void Run();
private:
	int i;

};

#endif /* SRC_HomeLiftTask_H_ */
