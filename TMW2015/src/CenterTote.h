/*
 * CenterTote.h
 *
 *  Created on: Jan 31, 2015
 *      Author: Programmer
 */
#include "jankyTask.h"
#include "WPILib.h"
#include "Robot.h"

#ifndef CENTERTOTE_H_
#define CENTERTOTE_H_

class CenterTote : public JankyTask{
public:
	CenterTote();
	virtual ~CenterTote();
	virtual void Run();
};

#endif /* CENTERTOTE_H_ */
