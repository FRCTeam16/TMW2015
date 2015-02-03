// RobotBuilder Version: 1.5
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.
#include "Robot.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION
DriveBase* Robot::driveBase = 0;
Stacker* Robot::stacker = 0;
Grabber* Robot::grabber = 0;
OI* Robot::oi = 0;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION

void Robot::RobotInit() {
	RobotMap::init();
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
	driveBase = new DriveBase();
	stacker = new Stacker();
	grabber = new Grabber();

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
	// This MUST be here. If the OI creates Commands (which it very likely
	// will), constructing it during the construction of CommandBase (from
	// which commands extend), subsystems are not guaranteed to be
	// yet. Thus, their requires() statements may grab null pointers. Bad
	// news. Don't move it.
	oi = new OI();
	lw = LiveWindow::GetInstance();

	// instantiate the command used for the autonomous period
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=AUTONOMOUS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=AUTONOMOUS
	File = RAWCConstants::getInstance();
	cout << "Past RAWCConstants" << endl;
	driveBase->SetWheelbase(19.5/2, 28.0, 19.5/2);
	FLOffset = File->getValueForKey("FLOff");
	FROffset = File->getValueForKey("FROff");
	RLOffset = File->getValueForKey("RLOff");
	RROffset = File->getValueForKey("RROff");
	driveBase->SetOffsets(FLOffset, FROffset, RLOffset, RROffset);

	driveBase->frontLeft->Enable();
	driveBase->frontRight->Enable();
	driveBase->rearLeft->Enable();
	driveBase->rearRight->Enable();

	pdp=new PowerDistributionPanel();

	pressed5 = false;
	pressed7 = false;
	liftUsingJoystick = false;

	dartPos1 = 460;
	dartPos2 = 333;
	dartClosedLoop = true;
//	homeLift = new HomeLiftTask();
	stackerControl = new StackerControlTask();
	stackerControl->Start();

	prefs->GetInstance();
//	prefs->PutDouble("LiftSpeed", .6);

	autoChooser = new SendableChooser();
	autoChooser->AddDefault("01. 3TotePickup", (void*)ThreeTotePickup);
	SmartDashboard::PutData("Autonomous Chooser", autoChooser);
  }

void Robot::DisabledInit(){

}

void Robot::DisabledPeriodic() {
	Scheduler::GetInstance()->Run();
	unsigned char USDistance[2];
//	uint8_t USDistanceLower = 0;
	unsigned char USRegister = 0x51;
	SmartDashboard::PutBoolean("USWriteSucess",driveBase->UltrasonicFrontLeft->Write(USRegister, 1));
	Wait(.100);
	SmartDashboard::PutBoolean("USUpperReadSucces",driveBase->UltrasonicFrontLeftRead->ReadOnly( 2, USDistance));
//	SmartDashboard::PutBoolean("USLowerReadSucces", driveBase->UltrasonicFrontRight->ReadOnly(1, (uint8_t *)&USDistanceLower));
	SmartDashboard::PutNumber("UltrasonicDistanceUpper", USDistance[0]);
	SmartDashboard::PutNumber("UltrasonicDistanceLower", USDistance[1]);
	SmartDashboard::PutNumber("ScaledJoystick",-oi->getGamePad()->GetRawAxis(1));
}

void Robot::AutonomousInit() {
    autoProgram = static_cast<AutoProgram>((int)(autoChooser->GetSelected()));
	genericAutoProgram.clear();
	switch(autoProgram) {

	case ThreeTotePickup:
		genericAutoProgram.push_back(IndexUp);
		genericAutoProgram.push_back(Diveforward);
		genericAutoProgram.push_back(End);
	}
	autoStep = genericAutoProgram[autoStepIncrementer];
}

void Robot::AutonomousPeriodic() {
	Scheduler::GetInstance()->Run();
}

void Robot::TeleopInit() {

}

void Robot::TeleopPeriodic() {
	Scheduler::GetInstance()->Run();


	/*******************TESTING*********************/




	/**************DRIVEBASE************************/

	//Resets gyro to zero when crab starts
	if (!prevTrigger && oi->getDriverRight()->GetRawButton(1)) {
		driveBase->imu->ZeroYaw();
	}

	prevTrigger = oi->getDriverRight()->GetRawButton(1);

	if(oi->getDriverLeft()->GetRawButton(1) && oi->getDriverRight()->GetMagnitude() < .1)
	{
		driveBase->Lock();
	}
	else if(oi->getDriverRight()->GetRawButton(1))
	{
		driveBase->Crab(oi->getJoystickTwist(),-oi->getJoystickY(),oi->getJoystickX(),true);
	}
	else
	{
		driveBase->Steer(oi->getLeftJoystickXRadians(),oi->getJoystickY(),0.5);
	}



	/**************STACKER************************/

	if((oi->getGamePad()->GetRawButton(6) || oi->getGamePad()->GetRawButton(8))) {

		if (!dartClosedLoop) {
			dartClosedLoop = true;
			stacker->dart->SetControlMode(CANSpeedController::kPosition);
		}

		if(oi->getGamePad()->GetRawButton(6))
			stacker->dart->Set(dartPos1);

		else if(oi->getGamePad()->GetRawButton(8))
			stacker->dart->Set(dartPos2);
	}

	else if (fabs(oi->getGamePad()->GetRawAxis(3)) > .1) {
		if (dartClosedLoop) {
			dartClosedLoop = false;
			stacker->dart->SetControlMode(CANSpeedController::kPercentVbus);
		}
		stacker->dart->Set(oi->getGamePad()->GetRawAxis(3));
	}

	else if(!dartClosedLoop)
		stacker->dart->Set(0);




	if(oi->getGamePad()->GetRawButton(5)) {
		if (!pressed5) {
		stackerControl->IncLiftPosition();
		pressed5 = true;
		}
	}
	else
		pressed5 = false;

	if (oi->getGamePad()->GetRawButton(7)) {
		if (!pressed7) {
			stackerControl->DecLiftPoistion();
			pressed7 = true;
			}
	}
	else
		pressed7 = false;

	if (oi->getGamePad()->GetPOV(0) == 180)
		stackerControl->Home();

	if (fabs(oi->getGamePad()->GetRawAxis(1)) > .1) {
		stackerControl->LiftOpenLoop(-oi->getScaledJoystick(oi->getGamePad()->GetRawAxis(1),3));
		liftUsingJoystick = true;
	}
	else if(liftUsingJoystick) {
		stackerControl->LiftOpenLoop(0);
		liftUsingJoystick = false;
	}

/*
	if(oi->getGamePad()->GetRawButton(4))
		stacker->liftFrontRight->Set(.6);
	else if (oi->getGamePad()->GetRawButton(2))
		stacker->liftFrontRight->Set(-.6);
	else
		stacker->liftFrontRight->Set(0);
*/

	stacker->squeeze->Set(oi->getGamePad()->GetRawAxis(0)/2);




	/**************GRABBER************************/

	grabber->extension->Set(oi->getGamePad()->GetRawAxis(2));


	/**************SMARTDASHBOARD************************/

	SmartDashboard::PutNumber("IMUYaw",driveBase->imu->GetYaw());
	SmartDashboard::PutNumber("DartPosition",stacker->dart->GetAnalogIn());
	SmartDashboard::PutNumber("DartForwardPosition", stacker->dart->GetForwardLimitOK());
	SmartDashboard::PutNumber("DartReversePosition", stacker->dart->GetReverseLimitOK());
	SmartDashboard::PutNumber("LitPosition",stacker->liftFrontLeft->GetEncPosition());
	SmartDashboard::PutNumber("FrontLeftSteerCurrent",driveBase->frontRightSteer->GetOutputCurrent());
	SmartDashboard::PutNumber("FrontLeftSteerCurrentPDP",pdp->GetCurrent(10));
	SmartDashboard::PutNumber("FrontRightSteerCurentPDP",pdp->GetCurrent(5));
	SmartDashboard::PutNumber("FrontLeftDriveCurrentPDP",pdp->GetCurrent(14));
	SmartDashboard::PutNumber("FrontRightDriveCurrentPDP",pdp->GetCurrent(1));
	SmartDashboard::PutNumber("frontleftvolt",driveBase->frontLeftPos->GetAverageVoltage());
	SmartDashboard::PutNumber("rearleftvolt",driveBase->rearLeftPos->GetAverageVoltage());
	SmartDashboard::PutNumber("frontrightvolt",driveBase->frontRightPos->GetAverageVoltage());
//	SmartDashboard::PutNumber("LiftPositions[i]", liftPositions[i]);

}

void Robot::TestPeriodic() {
	lw->Run();
}

START_ROBOT_CLASS(Robot);

