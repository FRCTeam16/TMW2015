// RobotBuilder Version: 1.5
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.




#include "DriveBase.h"
#include "../RobotMap.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

DriveBase::DriveBase() : Subsystem("DriveBase") {
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	frontLeftSteer = RobotMap::driveBaseFrontLeftSteer;
	frontLeftPos = RobotMap::driveBaseFrontLeftPos;
	frontLeft = RobotMap::driveBaseFrontLeft;
	frontRightSteer = RobotMap::driveBaseFrontRightSteer;
	frontRightPos = RobotMap::driveBaseFrontRightPos;
	frontRight = RobotMap::driveBaseFrontRight;
	rearLeftSteer = RobotMap::driveBaseRearLeftSteer;
	rearLeftPos = RobotMap::driveBaseRearLeftPos;
	rearLeft = RobotMap::driveBaseRearLeft;
	rearRightSteer = RobotMap::driveBaseRearRightSteer;
	rearRightPos = RobotMap::driveBaseRearRightPos;
	rearRight = RobotMap::driveBaseRearRight;
	frontLeftDrive = RobotMap::driveBaseFrontLeftDrive;
	frontRightDrive = RobotMap::driveBaseFrontRightDrive;
	rearLeftDrive = RobotMap::driveBaseRearLeftDrive;
	rearRightDrive = RobotMap::driveBaseRearRightDrive;
	toteWideLeft = RobotMap::driveBaseToteWideLeft;
	toteNarrowLeft = RobotMap::driveBaseToteNarrowLeft;
	toteNarrowRight = RobotMap::driveBaseToteNarrowRight;
	toteWideRight = RobotMap::driveBaseToteWideRight;
	toteCenter = RobotMap::driveBaseToteCenter;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	GyroZeroFlag = false;
	GyroZeroTime = GetClock();
	FLInv = 1;
	FRInv = 1;
	RRInv = 1;
	RLInv = 1;
	FLTurns = 0;
	FRTurns = 0;
	RLTurns = 0;
	RRTurns = 0;
	MaxTurns = 1000;
	pFLVolt = 0;
	pFRVolt = 0;
	pRLVolt = 0;
	pRRVolt = 0;
	driveFront = true;
	serialport = new SerialPort(57600,SerialPort::kMXP);
    uint8_t update_rate_hz = 50;
    imu = new IMU(serialport,update_rate_hz);
//    UltrasonicFrontLeft=new I2C(I2C::kOnboard,1);
 //   UltrasonicFrontRight=new I2C(I2C::kOnboard, 2);

    leftUS = new MaxBoticsI2CXL(I2C::kOnboard, 1, .100);
    rightUS = new MaxBoticsI2CXL(I2C::kOnboard, 2, .100);

	CrabSpeedTwist = new CrabSpeed();
    DriveControlTwist = new PIDController(.035, 0, .1, imu, CrabSpeedTwist, 0.02);
	DriveControlTwist->SetContinuous(false);
	DriveControlTwist->SetAbsoluteTolerance(2.0);


}
    
void DriveBase::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
void DriveBase::SetWheelbase(float w, float x, float y) {
	W=w;
	X=x;
	Y=y;
}
void DriveBase::SetOffsets(double FLOff, double FROff, double RLOff, double RROff) {
	FLOffset = FLOff;
	FROffset = FROff;
	RLOffset = RLOff;
	RROffset = RROff;
}
void DriveBase::ToggleFrontBack(){
	driveFront = !driveFront;
}
void DriveBase::Crab(float twist, float y, float x, bool UseGyro) {

	robotangle = imu->GetYaw()*pi/180;

	float FWD = y;
	float STR = x;
	if(UseGyro)
	{
		FWD = y*cos(robotangle) + x*sin(robotangle);
		STR = -y*sin(robotangle) + x*cos(robotangle);
	}

	radius = sqrt(pow(2*Y,2)+pow(X,2));

	AP = STR - twist*X/radius;
	BP = STR + twist*X/radius;
	CP = FWD - twist*2*Y/radius;
	DP = FWD + twist*2*Y/radius;

	float FLSetPoint = 2.5;
	float FRSetPoint = 2.5;
	float RLSetPoint = 2.5;
	float RRSetPoint = 2.5;

	if(DP != 0 || BP != 0)
		FLSetPoint = (2.5 + 2.5/pi*atan2(BP,DP));
	if(BP != 0 || CP != 0)
		FRSetPoint = (2.5 + 2.5/pi*atan2(BP,CP));
	if(AP != 0 || DP != 0)
		RLSetPoint = (2.5 + 2.5/pi*atan2(AP,DP));
	if(AP != 0 || CP != 0)
		RRSetPoint = (2.5 + 2.5/pi*atan2(AP,CP));


	//SetSteerSetpoint(FLSetPoint, FRSetPoint, RLSetPoint, RRSetPoint, true);
	SetSteering(FLSetPoint, FRSetPoint, RLSetPoint, RRSetPoint, true);
	FL = sqrt(pow(BP,2)+pow(DP,2));
	FR = sqrt(pow(BP,2)+pow(CP,2));
	RL = sqrt(pow(AP,2)+pow(DP,2));
	RR = sqrt(pow(AP,2)+pow(CP,2));


	//Solve for fastest wheel speed
	double speedarray[] = {fabs(FL), fabs(FR), fabs(RL), fabs(RR)};

	 int length = 4;
     double maxspeed = speedarray[0];
     for(int i = 1; i < length; i++)
     {
          if(speedarray[i] > maxspeed)
                maxspeed = speedarray[i];
     }

	//Set ratios based on maximum wheel speed
    if(maxspeed > 1 || maxspeed < -1)
    {
		FLRatio = FL/maxspeed;
		FRRatio = FR/maxspeed;
		RLRatio = RL/maxspeed;
		RRRatio = RR/maxspeed;
    }
    else
    {
		FLRatio = FL;
		FRRatio = FR;
		RLRatio = RL;
		RRRatio = RR;
    }

	//Set drive speeds
    SetDriveSpeed(FLRatio, -FRRatio, RLRatio, -RRRatio);

}
void DriveBase::Steer(float radian, float speed, float a) {

	A=a;

	thetaRC = pi - radian;  //convert steering angle to rear center wheel angle

	if(thetaRC != pi / 2)	//If we are not driving straight forward...
	{
		if(thetaRC < pi / 2)	//Right Turn
		{
			RightTurn4Wheels();
		}
		else if(thetaRC > pi / 2)	//Left Turn
		{
			LeftTurn4Wheels();
		}
	}
	else	//thetaRC = pi / 2
	{
		thetaFL = pi / 2;
		thetaFR = pi / 2;
		thetaRL = pi / 2;
		thetaRR = pi / 2;
		FLRatio = 1;
		FRRatio = 1;
		RLRatio = 1;
		RRRatio = 1;
	}
	//Solve for fastest wheel speed
	double speedarray[] = {fabs(FL), fabs(FR), fabs(RL), fabs(RR)};

	 int length = 4;
     double maxspeed = speedarray[0];
     for(int i = 1; i < length; i++)
     {
          if(speedarray[i] > maxspeed)
                maxspeed = speedarray[i];
     }

	//Set ratios based on maximum wheel speed
	FLRatio = FL/maxspeed;
	FRRatio = FR/maxspeed;
	RLRatio = RL/maxspeed;
	RRRatio = RR/maxspeed;

	//Set drive speeds
	SetDriveSpeed(-FLRatio*speed, FRRatio*speed, -RLRatio*speed, RRRatio*speed);

	//Set Steering PID Setpoints
	float FLSetPoint = (1.25 + 2.5/pi*thetaFL);
	float FRSetPoint = (1.25 + 2.5/pi*thetaFR);
	float RLSetPoint = (1.25 + 2.5/pi*thetaRL);
	float RRSetPoint = (1.25 + 2.5/pi*thetaRR);

	SetSteering(FLSetPoint, FRSetPoint, RLSetPoint, RRSetPoint, true);
	}
void DriveBase::LeftTurn4Wheels()
{
	Z = ((A * X) * tan(pi - thetaRC));				//find turning radius

	//calculate angles based on turning radius
	thetaRL = pi - atan((Z - W) / (A * X));
	thetaRR = pi - atan((Z + W) / (A * X));
	thetaFR = pi / 2;
	thetaFL = pi / 2;

	if(A != 1) //not turning about front wheels
	{
		thetaFL = atan((Z - Y) / ((1 - A) * X));	//These are identical for right and left turns
		thetaFR = atan((Z + Y) / ((1 - A) * X));	//These are identical for right and left turns
	}
	//Solve each wheel turning radii (wheel speed)
	FL = (Z - Y) / sin(thetaFL);
	FR = (Z + Y) / sin(thetaFR);
	RL = (Z - W) / sin(pi - thetaRL);
	RR = (Z + W) / sin(pi - thetaRR);
}
void DriveBase::RightTurn4Wheels()
{
	Z = ((A * X) * tan(thetaRC));				//find turning radius

	//calculate angles based on turning radius
	thetaRL = atan((Z + W) / (A * X));
	thetaRR = atan((Z - W) / (A * X));
	thetaFR = pi / 2;
	thetaFL = pi / 2;


	if(A != 1)  //not turning about front wheels
	{
		thetaFR = pi - atan((Z - Y) / ((1 - A) * X));	//These are identical for right and left turns
		thetaFL = pi - atan((Z + Y) / ((1 - A) * X));	//These are identical for right and left turns
	}

	//Solve each wheel turning radii (wheel speed)
	FL = (Z + Y) / sin(pi - thetaFL);
	FR = (Z - Y) / sin(pi - thetaFR);
	RL = (Z + W) / sin(thetaRL);
	RR = (Z - W) / sin(thetaRR);
}
double DriveBase::CorrectSteerSetpoint(double setpoint) {
	//Used to correct steering setpoints to within the 0 to 5 V scale

	if (setpoint < 0)
	{
		return setpoint + 5;
	}
	else if (setpoint > 5)
	{
		return setpoint - 5;
	}
	else if (setpoint == 5)
	{
		return 0;
	}
	else
	{
		return setpoint;
	}
}
void DriveBase::SetSteerSetpoint(float setpoint, AnalogInput* actual, float offset, PIDController* PIDCon, CANTalon* steer, int turns, int &inv, bool UseShortcut) {

	float volt = actual->GetVoltage();

	if(turns >= MaxTurns){
		PIDCon->Disable();
		steer->Set(-1);
	}
	else if (turns <= -MaxTurns){
		PIDCon->Disable();
		steer->Set(1);
	}

	else if(fabs(CorrectSteerSetpoint(setpoint + offset) - volt) < 1.25 || fabs(CorrectSteerSetpoint(setpoint + offset) - volt) > 3.75)
	{
		PIDCon->Enable();
		if((turns + 1 == MaxTurns && CorrectSteerSetpoint(volt - offset) - CorrectSteerSetpoint(setpoint) > 2.5) || (turns - 1 == -MaxTurns && CorrectSteerSetpoint(volt - offset) - CorrectSteerSetpoint(setpoint) < -2.5))
		{
			PIDCon->SetSetpoint(CorrectSteerSetpoint(setpoint + offset-2.5));
			inv = -1;
		}
		else
		{
			PIDCon->SetSetpoint(CorrectSteerSetpoint(setpoint + offset));
			inv = 1;
		}
	}
	else
	{
		PIDCon->Enable();
		if((turns + 1 == MaxTurns && CorrectSteerSetpoint(volt - offset) - CorrectSteerSetpoint(setpoint - 2.5) > 2.5) || (turns - 1 == -MaxTurns && CorrectSteerSetpoint(volt - offset) - CorrectSteerSetpoint(setpoint - 2.5) < -2.5))
		{
			PIDCon->SetSetpoint(CorrectSteerSetpoint(setpoint + offset));
			inv = 1;
		}
		else
		{
			PIDCon->SetSetpoint(CorrectSteerSetpoint(setpoint + offset - 2.5));
			inv = -1;
		}
	}

}
void DriveBase::SetSteering(float FLSetPoint, float FRSetPoint, float RLSetPoint, float RRSetPoint, bool UseShortcut) {
	if(driveFront) {
		SetSteerSetpoint(FLSetPoint, frontLeftPos, FLOffset, frontLeft, frontLeftSteer, FLTurns, FLInv, UseShortcut);
		SetSteerSetpoint(FRSetPoint, frontRightPos, FROffset, frontRight, frontRightSteer, FRTurns, FRInv, UseShortcut);
		SetSteerSetpoint(RLSetPoint, rearLeftPos, RLOffset, rearLeft, rearLeftSteer, RLTurns, RLInv, UseShortcut);
		SetSteerSetpoint(RRSetPoint, rearRightPos, RROffset, rearRight, rearRightSteer, RRTurns, RRInv, UseShortcut);
	}
	else
	{
		SetSteerSetpoint(RRSetPoint, frontLeftPos, FLOffset, frontLeft, frontLeftSteer, FLTurns, FLInv, UseShortcut);
		SetSteerSetpoint(RLSetPoint, frontRightPos, FROffset, frontRight, frontRightSteer, FRTurns, FRInv, UseShortcut);
		SetSteerSetpoint(FRSetPoint, rearLeftPos, RLOffset, rearLeft, rearLeftSteer, RLTurns, RLInv, UseShortcut);
		SetSteerSetpoint(FLSetPoint, rearRightPos, RROffset, rearRight, rearRightSteer, RRTurns, RRInv, UseShortcut);
	}
}
void DriveBase::CheckForTurns() {
	cFLVolt = frontLeftPos->GetVoltage();
	cFRVolt = frontRightPos->GetVoltage();
	cRLVolt = rearLeftPos->GetVoltage();
	cRRVolt = rearRightPos->GetVoltage();
	TurnsCheck(CorrectSteerSetpoint(pFLVolt - FLOffset), CorrectSteerSetpoint(cFLVolt- FLOffset), FLTurns);
	TurnsCheck(CorrectSteerSetpoint(pFRVolt - FROffset), CorrectSteerSetpoint(cFRVolt - FROffset), FRTurns);
	TurnsCheck(CorrectSteerSetpoint(pRLVolt - RLOffset), CorrectSteerSetpoint(cRLVolt - RLOffset), RLTurns);
	TurnsCheck(CorrectSteerSetpoint(pRRVolt - RROffset), CorrectSteerSetpoint(cRRVolt - RROffset), RRTurns);
	/*SmartDashboard::PutNumber("pFL", CorrectSteerSetpoint(pFLVolt - FLOffset));
	SmartDashboard::PutNumber("cFL", CorrectSteerSetpoint(cFLVolt - FLOffset));
	SmartDashboard::PutNumber("Prev-Curr", CorrectSteerSetpoint(pFLVolt - FLOffset) - CorrectSteerSetpoint(cFLVolt - FLOffset));*/
	pFLVolt = cFLVolt;
	pFRVolt = cFRVolt;
	pRLVolt = cRLVolt;
	pRRVolt = cRRVolt;
	/*SmartDashboard::PutNumber("FLTurns", FLTurns);
	SmartDashboard::PutNumber("FRTurns", FRTurns);
	SmartDashboard::PutNumber("RLTurns", RLTurns);
	SmartDashboard::PutNumber("RRTurns", RRTurns);*/
}
void DriveBase::TurnsCheck(float previous, float current, int& turns) {
	if(previous - current < -3.5)
		turns--;

	else if(previous - current > 3.5)
		turns++;
}
void DriveBase::UndoTurns() {
	UndoTurns(frontLeft, frontLeftSteer, FLTurns, FLOffset);
	UndoTurns(frontRight, frontRightSteer, FRTurns, FROffset);
	UndoTurns(rearLeft, rearLeftSteer, RLTurns, RLOffset);
	UndoTurns(rearRight, rearRightSteer, RRTurns, RROffset);
}
void DriveBase::UndoTurns(PIDController* PID, CANTalon* can, int turns, double offset){
	if(turns < 0) {
		PID->Disable();
		can->Set(1);
	}
	else if(turns > 0) {
		PID->Disable();
		can->Set(-1);
	}
	else{
		PID->Enable();
		PID->SetSetpoint(2.5 + offset);
	}
}

int DriveBase::GetFLTurns() {
	return FLTurns;
}
int DriveBase::GetFRTurns() {
	return FRTurns;
}
int DriveBase::GetRLTurns() {
	return RLTurns;
}
int DriveBase::GetRRTurns() {
	return RRTurns;
}

void DriveBase::SetFLTurns(int val) {
	FLTurns = val;
}
void DriveBase::SetFRTurns(int val) {
	FRTurns = val;
}
void DriveBase::SetRLTurns(int val) {
	RLTurns = val;
}
void DriveBase::SetRRTurns(int val) {
	RRTurns = val;
}

void DriveBase::SetDriveSpeed(float FLSpeed, float FRSpeed, float RLSpeed, float RRSpeed) {
//applies inversion variables defined in SetSteerSetPoint function
	if(driveFront) {
		frontLeftDrive->Set(FLSpeed*FLInv);
		frontRightDrive->Set(FRSpeed*FRInv);
		rearLeftDrive->Set(RLSpeed*RLInv);
		rearRightDrive->Set(RRSpeed*RRInv);
	}
	else {
		frontLeftDrive->Set(RRSpeed*FLInv);
		frontRightDrive->Set(RLSpeed*FRInv);
		rearLeftDrive->Set(FRSpeed*RLInv);
		rearRightDrive->Set(FLSpeed*RRInv);
	}
}
void DriveBase::Lock() //locks wheels to prevent robot movement
{
	SetSteering(2.0, 0.75, 3.25, 4.5, true);
	SetDriveSpeed(0,0,0,0);
}
bool DriveBase::ZeroGyro(float InitTime)  //performs gyro calibration
{
	bool Done = false;

	if(GyroZeroFlag == false)
	{
		GyroZeroFlag = true;
		GyroZeroTime = GetClock();
	}
	else
	{
		if(GetClock() > GyroZeroTime + InitTime)
		{
//			gyro->InitGyro();
			GyroZeroFlag = false;
			Done = true;
		}
	}

	return GetClock() > GyroZeroTime + InitTime + 6;
}

void DriveBase::SMDB() {
	SmartDashboard::PutNumber("IMUYaw",imu->GetYaw());
	SmartDashboard::PutNumber("IMUPitch",imu->GetPitch());
	SmartDashboard::PutNumber("IMURoll",imu->GetRoll());
	SmartDashboard::PutNumber("FrontLeftSteerCurrent",frontRightSteer->GetOutputCurrent());
	SmartDashboard::PutNumber("frontleftvolt",frontLeftPos->GetAverageVoltage());
	SmartDashboard::PutNumber("rearleftvolt",rearLeftPos->GetAverageVoltage());
	SmartDashboard::PutNumber("frontrightvolt",frontRightPos->GetAverageVoltage());
	SmartDashboard::PutBoolean("ToteWideLeftB", toteWideLeft->Get());
	SmartDashboard::PutBoolean("ToteNarrowLeftB", toteNarrowLeft->Get());
	SmartDashboard::PutBoolean("ToteCenterB", toteCenter->Get());
	SmartDashboard::PutBoolean("ToteNarrowRightB", toteNarrowRight->Get());
	SmartDashboard::PutBoolean("ToteWideRightB", toteWideRight->Get());
	SmartDashboard::PutNumber("UltrasonicDistanceLeft", leftUS->GetDistance());
	SmartDashboard::PutNumber("UltrasonicDistanceRight", rightUS->GetDistance());
}
