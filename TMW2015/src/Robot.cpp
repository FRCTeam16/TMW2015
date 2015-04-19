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
Arduino* Robot::arduino = 0;
OI* Robot::oi = 0;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION
PowerDistributionPanel* Robot::pdp = 0;
StackerControlTask* Robot::stackerControl = 0;
SqueezeControlTask* Robot::squeezeControl = 0;

void Robot::RobotInit() {
	RobotMap::init();
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
	driveBase = new DriveBase();
	stacker = new Stacker();
	grabber = new Grabber();
	arduino = new Arduino();

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

	stackerControl = new StackerControlTask();
	stackerControl->Start();
	squeezeControl = new SqueezeControlTask();
	squeezeControl->Start();
//	squeezeControl->Pause();
	centerTote = new CenterToteTask();


	autoChooser = new SendableChooser();
	autoChooser->AddDefault("01. ThreeToteStack", (void*)ThreeToteSuckerStacker);
	autoChooser->AddObject("02. ContainerGrabber-Slow, Drive Short, Release", (void*)ContainerGrabberSSR);
	autoChooser->AddObject("03. ContainerGrabber-Slow, Drive Far, Release", (void*)ContainerGrabberSFR);
	autoChooser->AddObject("04. ContainerGrabber-Slow, Drive Short, No Release", (void*)ContainerGrabberSSN);
	autoChooser->AddObject("05. ContainerGrabber-Slow, Drive Far, No Release", (void*)ContainerGrabberSFN);
	autoChooser->AddObject("06. ContainerGrabber-Fast, Drive Short, Release", (void*)ContainerGrabberFSR);
	autoChooser->AddObject("07. ContainerGrabber-Fast, Drive Far, Release", (void*)ContainerGrabberFFR);
	autoChooser->AddObject("08. ContainerGrabber-Fast, Drive Short, No Release", (void*)ContainerGrabberFSN);
	autoChooser->AddObject("09. ContainerGrabber-Fast, Drive Far, No Release", (void*)ContainerGrabberFFN);
	autoChooser->AddObject("10. DriveForward", (void*)DriveForward);
	autoChooser->AddObject("11. Do Nothing", (void*)DoNothing);
	SmartDashboard::PutData("Autonomous Chooser", autoChooser);


	stacker->liftFrontRight->SetPosition(34281);
	stackerControl->SetLiftPosition(1);

  }

void Robot::DisabledInit(){
	stackerControl->SetAutoSpeed(false);
}

void Robot::DisabledPeriodic() {
	Scheduler::GetInstance()->Run();

	SmartDashboard::PutBoolean("LiftSmokedB",!stackerControl->GetSmoked());
	SendArduinoOutputs();

	/*******************SmartDashboard*********************/
	if(oi->getDriverRight()->GetRawButton(7))
		StackerSMDB();
	if(oi->getDriverRight()->GetRawButton(8))
		DriveBaseSMDB();
	if(oi->getDriverRight()->GetRawButton(9))
		SystemSMDB();

	/******************Keeping Talons Alive*****************/
	driveBase->frontLeftSteer->Set(0);
	driveBase->frontRightSteer->Set(0);
	driveBase->rearLeftSteer->Set(0);
	driveBase->rearRightSteer->Set(0);
	stacker->liftFrontRight->Set(0);
	stacker->liftFrontLeft->Set(12);
	stacker->dart->SetControlMode(CANSpeedController::kPosition);
	stacker->dart->Set(stacker->dart->GetPosition());
	stacker->squeeze->Set(0);

//	SmartDashboard::PutNumber("GrabberSpeed", (int)fastChooser->GetSelected());

}

void Robot::AutonomousInit() {
//	if(static_cast<int>((int)(fastChooser->GetSelected())))
//		fast
	autoProgram = static_cast<AutoProgram>((int)(autoChooser->GetSelected()));
    genericAutoProgram.clear();
	switch(autoProgram) {

	case ThreeToteStacker:
/*		genericAutoProgram.push_back(IndexUp); //REMOVED ON 3/24 BECAUSE SQUEEZER NOT WORKING => GETSQUEEZED IN STACKERCONTROL IS COMMENTED OUT
		genericAutoProgram.push_back(IndexUp);
		genericAutoProgram.push_back(IndexUp);
		genericAutoProgram.push_back(WaitForLift);
		genericAutoProgram.push_back(MoveAwayFromRamp);
		genericAutoProgram.push_back(Turn90);
		genericAutoProgram.push_back(DriveForward15);
		genericAutoProgram.push_back(ForwardToTote);
		genericAutoProgram.push_back(DropForPickup);
		genericAutoProgram.push_back(IndexUp);
		genericAutoProgram.push_back(WaitForLiftCoast);
//		genericAutoProgram.push_back(DrivePastContainer);
//		genericAutoProgram.push_back(ForwardToThirdTote);
		genericAutoProgram.push_back(DriveForward15);
		genericAutoProgram.push_back(ForwardToTote);
		genericAutoProgram.push_back(DropForPickup);
		genericAutoProgram.push_back(PickThirdTote);
		genericAutoProgram.push_back(WaitForLiftFinal);
		genericAutoProgram.push_back(DriveToAutoZone1);
		genericAutoProgram.push_back(DriveToAutoZone2);
		genericAutoProgram.push_back(DropTotes);
//		genericAutoProgram.push_back(WaitForDart);
		genericAutoProgram.push_back(PickThirdTote);
		genericAutoProgram.push_back(WaitForLiftFinal);
//		genericAutoProgram.push_back(DartBack);
		genericAutoProgram.push_back(End);

		stackerControl->SetLiftPosition(0);
		stackerControl->SetAutoSpeed(true);
		stacker->liftFrontRight->SetPosition(-3500);
		stacker->dart->Set(dartVert);
*/		break;


	case ThreeToteSuckerStacker:
		genericAutoProgram.push_back(StartSqueezeAndSuck);
		genericAutoProgram.push_back(WaitForLift);
		genericAutoProgram.push_back(DriveToFirstTote1);
		genericAutoProgram.push_back(DriveToFirstTote2);
		genericAutoProgram.push_back(WaitForLift);
		genericAutoProgram.push_back(TurnToSecondTote);
		genericAutoProgram.push_back(DriveToSecondTote1);
		genericAutoProgram.push_back(DriveToSecondTote2);
		genericAutoProgram.push_back(WaitForLift);
//		genericAutoProgram.push_back(DriveAwayFromWall);
		genericAutoProgram.push_back(DriveToSecondContainer);
//		genericAutoProgram.push_back(DrivePastSecondContainer);
		genericAutoProgram.push_back(DriveToThirdTote1);
		genericAutoProgram.push_back(DriveToThirdTote2);
		genericAutoProgram.push_back(WaitForLiftFinal);
		genericAutoProgram.push_back(DriveToAutoZone1);
		genericAutoProgram.push_back(DriveToAutoZone2);
		genericAutoProgram.push_back(DropTotes);
		genericAutoProgram.push_back(IndexUp);
		genericAutoProgram.push_back(WaitForLiftFinal);
		genericAutoProgram.push_back(End);

		stackerControl->SetAutoSpeed(true);
		stacker->liftFrontRight->SetPosition(34281);
		stacker->dart->Set(dartVert);
		stackerControl->SetLiftPosition(4);
		driveBase->imu->SetYaw(autoStartAngle);
		robotAngle = -60;
		break;

	case ThreeToteCrabStacker:
		genericAutoProgram.push_back(DriveAwayFromTote);
		genericAutoProgram.push_back(WaitForUS);
		genericAutoProgram.push_back(DriveBackOntoBump);
		genericAutoProgram.push_back(DriveToContainer);
		genericAutoProgram.push_back(DrivePastContainer);
		genericAutoProgram.push_back(IndexUp);
		genericAutoProgram.push_back(DriveAcrossToTote);
		genericAutoProgram.push_back(StartTurningWheels);
		genericAutoProgram.push_back(DriveUpToTote);
		genericAutoProgram.push_back(CenterOnTote);
		genericAutoProgram.push_back(DriveAwayFromTote);
		genericAutoProgram.push_back(WaitForUS);
		genericAutoProgram.push_back(DriveBackOntoBump);
		genericAutoProgram.push_back(DriveToContainer);
		genericAutoProgram.push_back(DrivePastContainer);
		genericAutoProgram.push_back(IndexUp);
		genericAutoProgram.push_back(DriveAcrossToTote);
		genericAutoProgram.push_back(StartTurningWheels);
		genericAutoProgram.push_back(DriveUpToTote);
		genericAutoProgram.push_back(CenterOnTote);
		genericAutoProgram.push_back(IndexUp);
		genericAutoProgram.push_back(WaitForLiftFinal);
		genericAutoProgram.push_back(DriveToAutoZone1);
		genericAutoProgram.push_back(DropTotes);
		genericAutoProgram.push_back(IndexUp);
		genericAutoProgram.push_back(WaitForLiftFinal);
		genericAutoProgram.push_back(End);

		stackerControl->SetAutoSpeed(true);
		stacker->liftFrontRight->SetPosition(22500);
		stacker->dart->Set(dartVert);
		stackerControl->SetLiftPosition(0);
		robotAngle = 0;
		driveBase->imu->ZeroYaw();
		break;

	case ContainerGrabberSFR:
		genericAutoProgram.push_back(GrabberDown);
		genericAutoProgram.push_back(DriveForwardWithContainers1);
		genericAutoProgram.push_back(DriveForwardWithContainers2);
		genericAutoProgram.push_back(ReleaseContainers);
		genericAutoProgram.push_back(GrabberUp);
		genericAutoProgram.push_back(GrabberUp);
		genericAutoProgram.push_back(GrabberUp);
		genericAutoProgram.push_back(End);

		grabberSpeed = -.75;
		stacker->dart->Set(dartFullBack);
		robotAngle = 0;
		driveBase->imu->ZeroYaw();
		break;

	case ContainerGrabberSSR:
		genericAutoProgram.push_back(GrabberDown);
		genericAutoProgram.push_back(DriveForwardWithContainers1);
		genericAutoProgram.push_back(ReleaseContainers);
		genericAutoProgram.push_back(GrabberUp);
		genericAutoProgram.push_back(GrabberUp);
		genericAutoProgram.push_back(GrabberUp);
		genericAutoProgram.push_back(End);

		grabberSpeed = -.75;
		stacker->dart->Set(dartFullBack);
		robotAngle = 0;
		driveBase->imu->ZeroYaw();
		break;

	case ContainerGrabberSFN:
		genericAutoProgram.push_back(GrabberDown);
		genericAutoProgram.push_back(DriveForwardWithContainers1);
		genericAutoProgram.push_back(DriveForwardWithContainers2);
		genericAutoProgram.push_back(End);

		stacker->dart->Set(dartFullBack);
		robotAngle = 0;
		driveBase->imu->ZeroYaw();
		break;

	case ContainerGrabberSSN:
		genericAutoProgram.push_back(GrabberDown);
		genericAutoProgram.push_back(DriveForwardWithContainers1);
		genericAutoProgram.push_back(End);

		grabberSpeed = -.75;
		stacker->dart->Set(dartFullBack);
		robotAngle = 0;
		driveBase->imu->ZeroYaw();
		break;

	case ContainerGrabberFFR:
		genericAutoProgram.push_back(GrabberDown);
		genericAutoProgram.push_back(DriveForwardWithContainers1);
		genericAutoProgram.push_back(DriveForwardWithContainers2);
		genericAutoProgram.push_back(ReleaseContainers);
		genericAutoProgram.push_back(GrabberUp);
		genericAutoProgram.push_back(GrabberUp);
		genericAutoProgram.push_back(GrabberUp);
		genericAutoProgram.push_back(End);

		grabberSpeed = -1.0;
		stacker->dart->Set(dartFullBack);
		robotAngle = 0;
		driveBase->imu->ZeroYaw();
		break;

	case ContainerGrabberFSR:
		genericAutoProgram.push_back(GrabberDown);
		genericAutoProgram.push_back(DriveForwardWithContainers1);
		genericAutoProgram.push_back(ReleaseContainers);
		genericAutoProgram.push_back(GrabberUp);
		genericAutoProgram.push_back(GrabberUp);
		genericAutoProgram.push_back(GrabberUp);
		genericAutoProgram.push_back(End);

		grabberSpeed = -1.0;
		stacker->dart->Set(dartFullBack);
		robotAngle = 0;
		driveBase->imu->ZeroYaw();
		break;

	case ContainerGrabberFFN:
		genericAutoProgram.push_back(GrabberDown);
		genericAutoProgram.push_back(DriveForwardWithContainers1);
		genericAutoProgram.push_back(DriveForwardWithContainers2);
		genericAutoProgram.push_back(End);

		grabberSpeed = -1.0;
		stacker->dart->Set(dartFullBack);
		robotAngle = 0;
		driveBase->imu->ZeroYaw();
		break;

	case ContainerGrabberFSN:
		genericAutoProgram.push_back(GrabberDown);
		genericAutoProgram.push_back(DriveForwardWithContainers1);
		genericAutoProgram.push_back(End);

		grabberSpeed = -1.0;
		stacker->dart->Set(dartFullBack);
		robotAngle = 0;
		driveBase->imu->ZeroYaw();
		break;

	case DriveForward:
		genericAutoProgram.push_back(DriveToAutoZone1);
		genericAutoProgram.push_back(DriveToAutoZone2);
		genericAutoProgram.push_back(End);
		robotAngle = 0;
		driveBase->imu->ZeroYaw();
		break;

	case ToteAndContainer:
		genericAutoProgram.push_back(IndexUp);
		genericAutoProgram.push_back(IndexUp);
		genericAutoProgram.push_back(IndexUp);
		genericAutoProgram.push_back(WaitForLift);
		genericAutoProgram.push_back(DriveToAutoZone1);
		genericAutoProgram.push_back(DriveToAutoZone2);
		genericAutoProgram.push_back(End);
		stackerControl->SetLiftPosition(0);
		stackerControl->SetAutoSpeed(true);
		stacker->liftFrontRight->SetPosition(-3500);
		stacker->dart->Set(dartVert);
		robotAngle = 0;
		driveBase->imu->ZeroYaw();
		break;

	case DoNothing:
		genericAutoProgram.push_back(End);
		driveBase->imu->ZeroYaw();
		break;
	}
	autoStart = GetClock();
	autoStepi = 0;
	autoStep = genericAutoProgram[autoStepi];
	autoStepTime = GetClock();
	onTargetCounter = 0;

	stackerControl->SetDartClosedLoop(true);
}
void Robot::AutonomousPeriodic() {
	/*******************SmartDashboard*********************/
	if(oi->getDriverRight()->GetRawButton(7))
		StackerSMDB();
	if(oi->getDriverRight()->GetRawButton(8))
		DriveBaseSMDB();
	if(oi->getDriverRight()->GetRawButton(9))
		SystemSMDB();

	SendArduinoOutputs();

	float x = 0;
	float y = 0;
	bool useDriveParams;

	switch(autoStep) {

	case StartSqueezeAndSuck:
		SmartDashboard::PutString("AutoStep", "StartSqueezeAndSuck");
		centerTote->Start();
		squeezeControl->Close(false);
		autoStepComplete = true;
		driveBase->DriveControlTwist->SetPID(.01, 0, 0);
		driveBase->DriveControlTwist->SetOutputRange(-.08, .08);
		break;

	case DriveToFirstTote1:
		SmartDashboard::PutString("AutoStep", "DriveToFirstTote1");
		x = -.2;
		y = 0;
		useDriveParams = true;
		if(GetClock() - autoStepTime > .7) {
			autoStepComplete = true;
		}
		break;

	case DriveToFirstTote2:
		SmartDashboard::PutString("AutoStep", "DriveToFirstTote2");
		x = -.15;
		if (GetClock() - autoStepTime < .7)
			y = -x*1.25;
		else
			y = 0;
		robotAngle = -90;
		useDriveParams = true;
		driveBase->Crab(-.05,.2,0,false);
		if(stackerControl->Geti() > 4) {
			autoStepComplete = true;
		}
		break;

	case TurnToSecondTote:
		SmartDashboard::PutString("AutoStep", "TurnToSecondTote");
		useDriveParams = true;
		x=0;
		y=0;
		stacker->dart->Set(dartLeanBackPickup);
		if(driveBase->imu->GetYaw() < 0 && driveBase->imu->GetYaw() > -110)
			robotAngle = -170;
		else
			robotAngle = 90;
		driveBase->DriveControlTwist->SetPID(.2, 0, 1.5);
		driveBase->DriveControlTwist->SetOutputRange(-.4, .4);
		SmartDashboard::PutNumber("DriveControlTwistError", driveBase->DriveControlTwist->GetError());
		if(fabs(driveBase->DriveControlTwist->GetError()) < 2)
			onTargetCounter++;
		else
			onTargetCounter = 0;
		if(onTargetCounter > 2) {
			autoStepComplete = true;
		}
		break;

	case DriveToSecondTote1:
		SmartDashboard::PutString("AutoStep", "DriveToSecondTote1");
		driveBase->DriveControlTwist->SetPID(.01, 0, 0);
		if(driveBase->leftUS->GetFilteredDistance(30, 150)) {
			y = -.02*(driveBase->leftUS->GetFilteredError(30, 150, autoleftUSDistanceToWall));
			if (y<-.2)
				y=-.2;
			if (y>.2)
				y=.2;
		}
		else
			y = 0;
		if(driveBase->leftUS->GetFilteredDistance(75, 81)) {
			if(GetClock() - autoStepTime < .3)
				x = .5;
			else
				x = .15;
		}
		else
			x = 0;
		useDriveParams = true;
		if(pdp->GetCurrent(6) > 4 || pdp->GetCurrent(9) > 4) {
			onTargetCounter++;
		}
		if(onTargetCounter > 3)
			autoStepComplete = true;
		break;

	case DriveToSecondTote2:
		SmartDashboard::PutString("AutoStep", "DriveToSecondTote2");
		if(driveBase->leftUS->GetFilteredDistance(30, 150)) {
			y = -.02*(driveBase->leftUS->GetFilteredError(30, 150, autoleftUSDistanceToWall));
			if (y<-.2)
				y=-.2;
			if (y>.2)
				y=.2;
		}
		else
			y = 0;
		if(GetClock() - autoStepTime < .4)
			x = -.2;
		else
			x = 0;
		useDriveParams = true;
		if(stackerControl->Geti() > 5 && GetClock() - autoStepTime > .4) {
			autoStepComplete = true;
		}
		break;

	case DriveAwayFromWall:
		SmartDashboard::PutString("AutoStep", "DriveAwayFromWall");
		y=-.5;
		x=0;
		useDriveParams = true;
		if(driveBase->leftUS->GetFilteredDistance(30, 250) > 150) {
			autoStepComplete = true;
		}
		break;

	case DriveToSecondContainer:
		SmartDashboard::PutString("AutoStep", "DriveToSecondContainer");
/*		if(driveBase->leftUS->GetFilteredDistance(130, 200)) {
			y = -.005*(driveBase->leftUS->GetFilteredError(130, 200, 150));
			if (y<-.2)
				y=-.2;
			if (y>.2)
				y=.2;
		}
		else */
			y = 0;
		x = .4;
		robotAngle = 135;
		useDriveParams = true;
		if(GetClock() - autoStepTime > 1.5) {
			autoStepComplete = true;
		}

		break;

	case DrivePastSecondContainer:
		SmartDashboard::PutString("AutoStep", "DrivePastSecondContainer");
		SmartDashboard::PutString("AutoStep", "DrivePastContainer");
		if(driveBase->leftUS->GetFilteredDistance(130, 200)) {
			y = -.02*(driveBase->leftUS->GetFilteredError(130, 200, 150));
			if (y<-.2)
				y=-.2;
			if (y>.2)
				y=.2;
		}
		else
			y = 0;
		x = .25;
		useDriveParams = true;
		if(driveBase->leftUS->GetDistance(1) > 45) {
			autoStepComplete = true;
		}
		break;

	case DriveToThirdTote1:
		SmartDashboard::PutString("AutoStep", "DriveToThirdTote1");
		robotAngle = 90;
		if(driveBase->leftUS->GetFilteredDistance(30, 150)) {
			y = -.02*(driveBase->leftUS->GetFilteredError(30, 150, autoleftUSDistanceToWall));
			if (y<-.2)
				y=-.2;
			if (y>.2)
				y=.2;
		}
		else
			y = 0;
		if(driveBase->leftUS->GetFilteredDistance(75, 81))
			x = .2;
		else
			x = 0;
		useDriveParams = true;
		if(pdp->GetCurrent(6) > 4 || pdp->GetCurrent(9) > 4) {
			onTargetCounter++;
		}
		if(onTargetCounter > 5)
			autoStepComplete = true;
		break;

	case DriveToThirdTote2:
		SmartDashboard::PutString("AutoStep", "DriveToThirdtote2");
		if(driveBase->leftUS->GetFilteredDistance(30, 150)) {
			y = -.02*(driveBase->leftUS->GetFilteredError(30, 150, autoleftUSDistanceToWall));
			if (y<-.2)
				y=-.2;
			if (y>.2)
				y=.2;
		}
		else
			y = 0;
		if(GetClock() - autoStepTime < 1.0)
			x = -.2;
		else
			x = 0;
		useDriveParams = true;
		if(stackerControl->Geti() > 6) {
			autoStepComplete = true;
		}
		break;


	case WaitForUS:
		SmartDashboard::PutString("AutoStep", "WaitForUS");
		x = 0;
		y = 0;
		useDriveParams = true;
		if((stackerControl->Geti() == 0 && stacker->liftFrontRight->GetPosition() < 70000) || (stackerControl->Geti() == 2 && stacker->liftFrontRight->GetPosition() < 89000))
			stackerControl->LiftOpenLoop(1.0);
		else
			stackerControl->LiftOpenLoop(0);
		driveBase->DriveControlTwist->SetPID(.01, 0, 0);
		if(driveBase->leftUS->GetFilteredDistance(40,200))
			autoStepComplete = true;
		break;

	case DriveAwayFromTote:
		SmartDashboard::PutString("AutoStep", "DriveAwayFromTote");
		x = -.6;
		y = 0.0;
		useDriveParams = true;
		if(GetClock() - autoStepTime > .2) {
			autoStepComplete = true;
			stacker->dart->Set(dartBumpPos);
		}
		break;

	case DriveBackOntoBump:
		SmartDashboard::PutString("AutoStep", "DriveBackOntoBump");
		x = 0;
		if(driveBase->leftUS->GetFilteredDistance(130,500) < 150)
			y = -.5;
		else
			y = 0;
		useDriveParams = true;
		if((stackerControl->Geti() == 0 && stacker->liftFrontRight->GetPosition() < 70000) || (stackerControl->Geti() == 2 && stacker->liftFrontRight->GetPosition() < 89000))
			stackerControl->LiftOpenLoop(1.0);
		else
			stackerControl->LiftOpenLoop(0);
		if((driveBase->leftUS->GetFilteredDistance(130,500) > 150) && ((stackerControl->Geti() == 0 && stacker->liftFrontRight->GetPosition() > 70000) || (stackerControl->Geti() == 2 && stacker->liftFrontRight->GetPosition() > 89000))) {
			autoStepComplete = true;
			stackerControl->LiftOpenLoop(0);
		}
		break;

	case DriveToContainer:
		SmartDashboard::PutString("AutoStep", "DriveToContainer");
		if(driveBase->leftUS->GetFilteredDistance(130, 200)) {
			y = -.005*(driveBase->leftUS->GetFilteredError(130, 200, 150));
			if (y<-.2)
				y=-.2;
			if (y>.2)
				y=.2;
		}
		else
			y = 0;
		x = .35;
		useDriveParams = true;
		if(driveBase->leftUS->GetDistance(2) < 45) {
			autoStepComplete = true;
		}
		break;

	case DrivePastContainer:
		SmartDashboard::PutString("AutoStep", "DrivePastContainer");
		if(driveBase->leftUS->GetFilteredDistance(130, 200)) {
			y = -.005*(driveBase->leftUS->GetFilteredError(130, 200, 150));
			if (y<-.2)
				y=-.2;
			if (y>.2)
				y=.2;
		}
		else
			y = 0;
		x = .25;
		useDriveParams = true;
		if(driveBase->leftUS->GetDistance(2) > 45) {
			autoStepComplete = true;
		}
		break;

	case DriveUpToTote:
		SmartDashboard::PutString("AutoStep", "DriveUpToTote");
		y = .3;
		x = 0.0;
		useDriveParams = true;
		if(GetClock() - autoStepTime > .2) {
			autoStepComplete = true;
		}
		break;

	case DriveAcrossToTote:
		SmartDashboard::PutString("AutoStep", "DriveAcrossToTote");
		if(driveBase->leftUS->GetFilteredDistance(100, 500) != 0) {
			y = -.005*(driveBase->leftUS->GetFilteredError(100, 500, 149));
			if (y<-.2)
				y=-.2;
			if (y>.2)
				y=.2;
		}
		else
			y = 0;
		x = .2;
		useDriveParams = true;
		if(!driveBase->autoToteSensor->Get()) {
			autoStepComplete = true;
			stacker->dart->Set(dartVert);
		}
		break;

	case StartTurningWheels:
		SmartDashboard::PutString("AutoStep", "StartTurningWheels");
		driveBase->frontLeft->Disable();
		driveBase->frontRight->Disable();
		driveBase->rearLeft->Disable();
		driveBase->rearRight->Disable();
		driveBase->frontLeftSteer->Set(1.0);
		driveBase->frontRightSteer->Set(1.0);
		driveBase->rearLeftSteer->Set(1.0);
		driveBase->rearRightSteer->Set(1.0);
		driveBase->frontLeftDrive->Set(0);
		driveBase->frontRightDrive->Set(0);
		driveBase->rearLeftDrive->Set(0);
		driveBase->rearRightDrive->Set(0);

		onTargetCounter++;
		if(onTargetCounter > 3) {
			autoStepComplete = true;
			driveBase->frontLeft->Enable();
			driveBase->frontRight->Enable();
			driveBase->rearLeft->Enable();
			driveBase->rearRight->Enable();

		}
		break;
	case CenterOnTote:
		SmartDashboard::PutString("AutoStep", "CenterOnTote");
		centerTote->Start();
		useDriveParams = false;
		if(stackeriStart != stackerControl->Geti() && fabs(stackerControl->GetError() < 1000)) {
			autoStepComplete = true;
			centerTote->Pause();
			centerTote->Reset();
		}
		break;

	case IndexUp:
		SmartDashboard::PutString("AutoStep", "IndexUp");
		x = 0;
		y = 0;
		useDriveParams = true;
		stackerControl->IncLiftPosition();
		autoStepComplete = true;
		break;

	case WaitForLift:
		SmartDashboard::PutString("AutoStep", "WaitForLift");
		x = 0;
		y = 0;
		useDriveParams = true;
		if(stackerControl->GetError() < 15000) {
			autoStepComplete = true;
			centerTote->Reset();
		}
		break;

	case WaitForLiftFinal:
		SmartDashboard::PutString("AutoStep", "WaitForLiftFinal");
		x = 0;
		y = 0;
		useDriveParams = true;
		if(stackerControl->GetError() < 500) {
			autoStepComplete = true;
		}
		break;

	case DriveToAutoZone1:
		SmartDashboard::PutString("AutoStep", "DriveToAutoZone");
		x = 0;
		y = 0;
		useDriveParams = true;
		robotAngle = 180;
		if(GetClock() - autoStepTime > .25) {
			autoStepComplete = true;
		}
		break;

	case DriveToAutoZone2:
		SmartDashboard::PutString("AutoStep", "DriveToAutoZone");
		x = 0;
		y = -8;
		useDriveParams = true;
		if(GetClock() - autoStepTime > 1.0) {
			autoStepComplete = true;
		}
		break;


	case DropTotes:
		SmartDashboard::PutString("AutoStep", "DropTotes");
		if(robotAngle == 90)
			x = -.1;
		else if (robotAngle == -90)
			x = .1;
		else
			x = 0;
		if(robotAngle == 0)
			y = -.1;
		else if (robotAngle == 180)
			y = .1;
		else
			y = 0;
		useDriveParams = true;
		stackerControl->LiftOpenLoop(-1.0);
		stacker->suckerLeft->Set(1.0);
		stacker->suckerRight->Set(1.0);
		if(stacker->liftFrontRight->GetPosition() < 89000 /*54000*/) {
			autoStepComplete = true;
			stackerControl->LiftOpenLoop(0);
		}
		break;

	case GrabberDown:
		SmartDashboard::PutString("AutoStep", "GrabberDown");
		driveBase->Lock();
		useDriveParams = false;
		grabber->grabber->Set(grabberSpeed);
		if(GetClock() - autoStepTime > 0.3) {
			grabber->grabber->Set(0);
			autoStepComplete = true;
		}
		break;

	case DriveForwardWithContainers1:
		SmartDashboard::PutString("AutoStep", "DriveForwardWithContainers");
		x = 0;
		y = 1.0;
		useDriveParams = true;
		if(GetClock() - autoStepTime > 1.2) {
			autoStepComplete = true;
		}
		break;

	case DriveForwardWithContainers2:
		SmartDashboard::PutString("AutoStep", "DriveForwardWithContainers");
		x = 0;
		y = .2;
		useDriveParams = true;
		if(GetClock() - autoStepTime > 1.5) {
			autoStepComplete = true;
		}
		break;

	case ReleaseContainers:
		SmartDashboard::PutString("AutoStep", "ReleaseContainers");
		x = 0;
		y = 0;
		useDriveParams = true;
		stacker->dart->Set(dartPos2);
		if(GetClock() - autoStepTime > 1.0) {
			autoStepComplete = true;
		}
		break;

	case GrabberUp:
		if(GetClock() - autoStepTime < 1.0) {
			grabber->grabber->Set(.2);
		}
		else if (GetClock() - autoStepTime < 2.0) {
			grabber->grabber->Set(0);
		}
		else
			autoStepComplete = true;
		SmartDashboard::PutString("AutoStep", "GrabberUp");
		break;

	case End:
		SmartDashboard::PutString("AutoStep", "End");
		x = 0;
		y = 0;
		useDriveParams = false;
		stacker->suckerLeft->Set(0);
		stacker->suckerRight->Set(0);
		if(autoStart > 0) {
			SmartDashboard::PutNumber("AutonomousTime", GetClock() - autoStart);
			autoStart = 0;
		}
		Robot::driveBase->Lock();
		break;

	}

	if(useDriveParams) {
		driveBase->DriveControlTwist->SetSetpoint(robotAngle);
		driveBase->Crab(driveBase->CrabSpeedTwist->Get(), y, x, true);
	}

	/***********************Increment Through Program****************************/
	if (autoStepComplete) {
		autoStepTime = GetClock();
		autoCurrentStop = 0;
		onTargetCounter = 0;
		autoStepComplete = false;
		autoStepi ++;
		stackeriStart = stackerControl->Geti();
		autoStep = genericAutoProgram.at(autoStepi);
	}
}

void Robot::TeleopInit() {
	stackerControl->SetDartClosedLoop(true);
	stacker->dart->SetControlMode(CANSpeedController::kPosition);
	stacker->dart->Set(stacker->dart->GetPosition());
	stackerControl->LiftOpenLoop(0);
	stackerControl->SetAutoSpeed(false);
	centerTote->Pause();
}

void Robot::TeleopPeriodic() {

	SmartDashboard::PutBoolean("LiftSmokedB",!stackerControl->GetSmoked());
	SendArduinoOutputs();
	/*******************SmartDashboard*********************/
	if(oi->getDriverRight()->GetRawButton(7))
		StackerSMDB();
	if(oi->getDriverRight()->GetRawButton(8))
		DriveBaseSMDB();
	if(oi->getDriverRight()->GetRawButton(9))
		SystemSMDB();


	/**************DRIVEBASE************************/
	//Resets gyro to zero when crab starts
	if (!prevTrigger && oi->getDriverRight()->GetRawButton(1)) {
		driveBase->imu->ZeroYaw();
	}
	prevTrigger = oi->getDriverRight()->GetRawButton(1);

	if(oi->getDriverLeft()->GetRawButton(1))
		driveBase->Lock();

	else if(oi->getDriverLeft()->GetRawButton(1) && oi->getDriverRight()->GetMagnitude() < .1)
	{
		driveBase->Lock();
	}
	else if(oi->getDriverRight()->GetRawButton(1))
	{
		driveBase->Crab(oi->getJoystickTwist(),-oi->getJoystickY(),oi->getJoystickX(),true);
	}
	else if (oi->getDriverLeft()->GetRawButton(3)) {
		driveBase->Steer(3.14159,oi->getDriverLeft()->GetX(),2.1);
	}
	else
	{
		driveBase->Steer(oi->getLeftJoystickXRadians(),oi->getJoystickY(),.5);
	}


	/**************STACKER************************/


	/*********Dart Control***************/
	if(driveBase->imu->GetPitch() < -3 && !oi->getGamePad()->GetRawButton(8)) {
		pitchControlCounter++;
	}
	else
		pitchControlCounter = 0;

	if (fabs(oi->getGamePad()->GetRawAxis(3)) > .1 && stackerControl->GetDartClosedLoop()) {
		stackerControl->SetDartClosedLoop(false);
		stacker->dart->SetControlMode(CANSpeedController::kPercentVbus);
		dartPitchControl = false;
	}

	if(oi->getGamePad()->GetRawButton(6)) {

		if (!stackerControl->GetDartClosedLoop()) {
			stackerControl->SetDartClosedLoop(true);
			stacker->dart->SetControlMode(CANSpeedController::kPosition);
			dartPitchControl = false;
		}
		if(stackerControl->Geti() == 7)
			stacker->dart->Set(dartVert - 10);
		else
			stacker->dart->Set(dartVert);
	}

	else if(oi->getGamePad()->GetRawButton(4)) {

		if (!stackerControl->GetDartClosedLoop()) {
			stackerControl->SetDartClosedLoop(true);
			stacker->dart->SetControlMode(CANSpeedController::kPosition);
			dartPitchControl = false;
		}
		stacker->dart->Set(dartLeanBackPickup);
	}

	else if(!stackerControl->GetDartClosedLoop())
		stacker->dart->Set(oi->getGamePad()->GetRawAxis(3)*3/4);

/*	if (pitchControlCounter > 20)	{
		if(!dartPitchControl) {
			stacker->dart->SetControlMode(CANSpeedController::kPosition);
			stackerControl->SetDartClosedLoop(true);
			stacker->dart->Set(dartBumpPos);
			dartPitchControl = true;
		}
	}
	else if (dartPitchControl){
		stacker->dart->Set(dartVert);
		dartPitchControl = false;
	}
*/
	if(oi->getGamePad()->GetPOV() == 0) {
		stacker->dart->SetControlMode(CANSpeedController::kPosition);
		stackerControl->SetDartClosedLoop(true);
		stacker->dart->Set(dartFullBack);
		dartPitchControl = false;
		stackerControl->SetLiftPosition(10);
		squeezeOpenLoop = false;
	}



	/*******Lift Control*********/

//Drop-Inc Lift
	if(oi->getGamePad()->GetRawButton(5) || oi->getDriverRight()->GetRawButton(2)) {
		if (!pressed5) {
//			if(oi->getDriverRight()->GetRawButton(2) && !stackerControl->GetDropPos())
//				stackerControl->SetDropPos(true);
//			else
				stackerControl->IncLiftPosition();
			pressed5 = true;
		}
	}
	else
		pressed5 = false;


//LiftContainer
	if(oi->getDriverRight()->GetRawButton(4))
		stackerControl->SetLiftPosition(5);

//DecLift
	if (oi->getGamePad()->GetRawButton(7) || oi->getDriverRight()->GetRawButton(3)) {
		if (!pressed7) {
			stackerControl->DecLiftPoistion();
			pressed7 = true;
			}
	}
	else
		pressed7 = false;

//Automated Release
	if(oi->getGamePad()->GetRawButton(2)) {
		if(!pressed2) {
			stackerControl->PickupContainer();
			pressed2 = true;
		}
	}
	else
		pressed2 = false;

//Reset Lift
	if (oi->getGamePad()->GetPOV(0) == 180) {
		stackerControl->Reset();
		squeezeOpenLoop = false;
		if (!stackerControl->GetDartClosedLoop()) {
			stackerControl->SetDartClosedLoop(true);
			stacker->dart->SetControlMode(CANSpeedController::kPosition);
		}
		stacker->dart->Set(dartVert);
	}

if(oi->getGamePad()->GetPOV(0) == 180)
	homeCounter++;
else
	homeCounter = 0;

//Home Lift
	if (homeCounter > 50) {
		stackerControl->Home();
		squeezeOpenLoop = false;
		if (!stackerControl->GetDartClosedLoop()) {
			stackerControl->SetDartClosedLoop(true);
			stacker->dart->SetControlMode(CANSpeedController::kPosition);
		}
		stacker->dart->Set(dartVert);
	}


//Lift Open Loop
	if (fabs(oi->getGamePad()->GetRawAxis(1)) > .05) {
		stackerControl->LiftOpenLoop(-oi->getScaledJoystick(oi->getGamePad()->GetRawAxis(1),3));
		liftUsingJoystick = true;
	}
	else if(liftUsingJoystick) {
		stackerControl->LiftOpenLoop(0);
		liftUsingJoystick = false;
	}


	/******Squeezer*******/
	if(oi->getGamePad()->GetRawButton(10)) {
		squeezeControl->SetOpenLoop(-oi->getGamePad()->GetRawAxis(0));
		squeezeOpenLoop = true;
	}
	else if(squeezeOpenLoop)
		squeezeControl->SetOpenLoop(0);

	if(oi->getGamePad()->GetPOV() == 270) {
		squeezeControl->Close(false);
		squeezeOpenLoop = false;
	}
	if(oi->getGamePad()->GetPOV() == 90) {
		squeezeControl->Open();
		squeezeOpenLoop = false;
	}

	if(oi->getDriverLeft()->GetRawButton(8))
		centerTote->Start();
	else {
		centerTote->Reset();
		centerTote->Pause();
	}


	/************Sucker*********************/
	//automated intake-index up
	if(oi->getDriverLeft()->GetRawButton(2)) {
		stacker->suckerLeft->Set(-1.0);
		stacker->suckerRight->Set(-1.0);
		//centerTote->Start();
		}
	else {
		stacker->suckerLeft->Set(0);
		stacker->suckerRight->Set(0);
//		centerTote->Pause();
//		centerTote->Reset();

		//runs suckers backwards to eject totes
		if (oi->getDriverRight()->GetRawButton(5)) {
			stacker->suckerLeft->Set(.5);
			stacker->suckerRight->Set(.5);
		}

		//modifies joystick output to run the wheels in the same direction
		else if(oi->getGamePad()->GetRawButton(9)) {
			stacker->suckerLeft->Set(oi->getGamePad()->GetRawAxis(2));
			stacker->suckerRight->Set(-oi->getGamePad()->GetRawAxis(2));
		}
		else {
			stacker->suckerLeft->Set(oi->getGamePad()->GetRawAxis(2));
			stacker->suckerRight->Set(oi->getGamePad()->GetRawAxis(2));
		}
	}


	/**************GRABBER************************/
	if(oi->getGamePad()->GetRawButton(11))
		grabber->grabber->Set(-0.5);
	else if(oi->getGamePad()->GetRawButton(12))
		grabber->grabber->Set(0.5);
	else
		grabber->grabber->Set(0);

}

void Robot::TestPeriodic() {
	lw->Run();
}

void Robot::SystemSMDB() {
	SmartDashboard::PutNumber("FrontLeftDriveCurrentPDP",pdp->GetCurrent(14));
	SmartDashboard::PutNumber("RearLeftDriveCurrentPDP",pdp->GetCurrent(15));
	SmartDashboard::PutNumber("FrontRightDriveCurrentPDP",pdp->GetCurrent(1));
	SmartDashboard::PutNumber("RearRightDriveCurrentPDP",pdp->GetCurrent(0));

	SmartDashboard::PutNumber("FrontLeftSteerCurrentPDP",Robot::driveBase->frontLeftSteer->GetOutputCurrent());
	SmartDashboard::PutNumber("FrontRightSteerCurrentPDP",Robot::driveBase->frontRightSteer->GetOutputCurrent());
	SmartDashboard::PutNumber("RearLeftSteerCurrentPDP",Robot::driveBase->rearLeftSteer->GetOutputCurrent());
	SmartDashboard::PutNumber("RearRightSteerCurrentPDP",Robot::driveBase->rearRightSteer->GetOutputCurrent());

	SmartDashboard::PutNumber("IntakeCurrent(6)", pdp->GetCurrent(6));
	SmartDashboard::PutNumber("IntakeCurrent(9)", pdp->GetCurrent(9));

	SmartDashboard::PutNumber("DartCurrent", Robot::stacker->dart->GetOutputCurrent());
	SmartDashboard::PutNumber("SqueezerCurrent", Robot::stacker->squeeze->GetOutputCurrent());
	SmartDashboard::PutNumber("LiftRightCurrent", Robot::stacker->liftFrontRight->GetOutputCurrent());
	SmartDashboard::PutNumber("LiftLeftCurrent", Robot::stacker->liftFrontLeft->GetOutputCurrent());

	SmartDashboard::PutNumber("TotalEnergy", pdp->GetTotalEnergy());

}

void Robot::DriveBaseSMDB() {
	SmartDashboard::PutNumber("IMUYaw",driveBase->imu->GetYaw());
	SmartDashboard::PutNumber("IMUPitch",driveBase->imu->GetPitch());
	SmartDashboard::PutNumber("IMURoll",driveBase->imu->GetRoll());
	SmartDashboard::PutNumber("DriverRightJoystickY",oi->getDriverRight()->GetRawAxis(1));
	SmartDashboard::PutNumber("FrontLeftSteerCurrent",driveBase->frontRightSteer->GetOutputCurrent());
	SmartDashboard::PutNumber("frontleftvolt",driveBase->frontLeftPos->GetAverageVoltage());
	SmartDashboard::PutNumber("rearleftvolt",driveBase->rearLeftPos->GetAverageVoltage());
	SmartDashboard::PutNumber("frontrightvolt",driveBase->frontRightPos->GetAverageVoltage());
	SmartDashboard::PutBoolean("ToteWideLeftB", driveBase->toteWideLeft->Get());
	SmartDashboard::PutBoolean("ToteNarrowLeftB", driveBase->toteNarrowLeft->Get());
	SmartDashboard::PutBoolean("ToteNarrowRightB", driveBase->toteNarrowRight->Get());
	SmartDashboard::PutBoolean("ToteWideRightB", driveBase->toteWideRight->Get());
	SmartDashboard::PutNumber("UltrasonicDistance1(Front)", driveBase->leftUS->GetDistance(1));
	SmartDashboard::PutNumber("UltrasonicDistance2(Rear)", driveBase->leftUS->GetDistance(2));
	SmartDashboard::PutNumber("UltrasonicFilterDistance", driveBase->leftUS->GetFilteredDistance(65, 300));
	SmartDashboard::PutNumber("JoystickRadians", oi->getLeftJoystickXRadians());
}

void Robot::StackerSMDB() {
	SmartDashboard::PutNumber("DartPosition",stacker->dart->GetAnalogIn());
	SmartDashboard::PutNumber("LitPosition",stacker->liftFrontLeft->GetEncPosition());
	SmartDashboard::PutNumber("SqueezePosition", stacker->squeezePosition->GetDistance());
	SmartDashboard::PutNumber("SqueezeRawPosition", stacker->squeezePosition->GetRaw());
	SmartDashboard::PutBoolean("StackerHomeSwitch", stacker->home->Get());
	SmartDashboard::PutNumber("SqueezeOutput", stacker->squeeze->Get());
	SmartDashboard::PutNumber("LiftPosition", stacker->liftFrontRight->GetPosition());
	SmartDashboard::PutNumber("LiftError", stackerControl->GetError());
	SmartDashboard::PutNumber("i", stackerControl->Geti());
	SmartDashboard::PutBoolean("StackerHoming", stackerControl->GetHoming());
	SmartDashboard::PutBoolean("DartForwardLimitSwitch", Robot::stacker->dart->GetForwardLimitOK());
	SmartDashboard::PutBoolean("DartReverseLimitSwitch", Robot::stacker->dart->GetReverseLimitOK());
}

void Robot::SendArduinoOutputs() {
	Robot::arduino->sensorLeft->Set(!Robot::driveBase->toteWideLeft->Get());
	Robot::arduino->sensorMiddleLeft->Set(!Robot::driveBase->toteNarrowLeft->Get());
	Robot::arduino->sensorMiddleRight->Set(!Robot::driveBase->toteNarrowRight->Get());
	Robot::arduino->sensorMiddle->Set(!Robot::driveBase->toteWideRight->Get());
	Robot::arduino->robotState1->Set(DriverStation::GetInstance()->GetAlliance() == DriverStation::kBlue);
	Robot::arduino->robotState2->Set(DriverStation::GetInstance()->GetAlliance() == DriverStation::kRed);
}

bool Robot::ReadyForPickup() {
	if(!(Robot::driveBase->toteWideLeft->Get() &&
			Robot::driveBase->toteNarrowLeft->Get() &&
			Robot::driveBase->toteNarrowRight->Get() &&
			Robot::driveBase->toteWideRight->Get())) {
		pickupStartTime = GetClock();
	}

	return GetClock() - pickupStartTime > .2;
}

START_ROBOT_CLASS(Robot);

