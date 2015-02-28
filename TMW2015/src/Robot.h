// RobotBuilder Version: 1.5
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef _ROBOT_H
#define _ROBOT_H

#include "WPILib.h"
#include "Commands/Command.h"
#include "RobotMap.h"
#include "LiveWindow/LiveWindow.h"
#include "RawCConstants.h"
#include "StackerControlTask.h"
#include "SqueezeControlTask.h"
#include "CenterToteTask.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "Subsystems/Arduino.h"
#include "Subsystems/DriveBase.h"
#include "Subsystems/Grabber.h"
#include "Subsystems/Stacker.h"


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "OI.h"

class Robot : public IterativeRobot {
public:
//	Command *autonomousCommand;
	static OI *oi;
	LiveWindow *lw;
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	static DriveBase* driveBase;
	static Stacker* stacker;
	static Grabber* grabber;
	static Arduino* arduino;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	virtual void RobotInit();
	virtual void DisabledInit();
	virtual void DisabledPeriodic();
	virtual void AutonomousInit();
	virtual void AutonomousPeriodic();
	virtual void TeleopInit();
	virtual void TeleopPeriodic();
	virtual void TestPeriodic();
	static PowerDistributionPanel *pdp;
	static StackerControlTask *stackerControl;
	static SqueezeControlTask *squeezeControl;

private:
	RAWCConstants* File;
	double FLOffset;
	double FROffset;
	double RLOffset;
	double RROffset;
	bool prevTrigger;
	Preferences* Prefs;
	int dartVert = 503;
	int dartBumpPos = 649;
	int dartPos2 = 333;
	int dartFullBack = 920;
	int dartContPickPos = 90;
	bool dartPitchControl = false;
	double pickupStartTime = GetClock();
	int pitchControlCounter = 0;
	bool pressed5 = false;
	bool pressed7 = false;
	bool pressed2 = false;
	bool liftUsingJoystick = false;
	int autoCurrentStop = 0;
	typedef enum {ThreeToteStacker, ContainerGrabber, ToteAndContainer, DoNothing, DriveForward}AutoProgram;
	typedef enum {IndexUp, WaitForLift, WaitForLiftCoast, DriveForward15, PickThirdTote, DriveToAutoZone, DropTotes, BackAwayFromTotes, ForwardToTote, Turn90, CrabToWall, ExtendGrabber, RetractGrabber, DriveOverBump, DriveAwayFromBumpWithCont, FinishTurn, End}AutoStep;
	AutoStep autoStep;
	AutoProgram autoProgram;
	bool autoStepComplete = false;
	int autoStepi = 0;
	vector<AutoStep> genericAutoProgram;
	SendableChooser* autoChooser;
	float autoStepTime = GetClock();
	float robotAngle = 0;
	float autoleftUSDistanceToWall = 80;
	int onTargetCounter = 0;
	void SystemSMDB();
	void DriveBaseSMDB();
	void StackerSMDB();
	void SendArduinoOutputs();
	bool ReadyForPickup();
	CenterToteTask *centerTote;
};
#endif
