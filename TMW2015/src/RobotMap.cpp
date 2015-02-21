// RobotBuilder Version: 1.5
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "RobotMap.h"
#include "LiveWindow/LiveWindow.h"


// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=ALLOCATION
CANTalon* RobotMap::driveBaseFrontLeftSteer = NULL;
AnalogInput* RobotMap::driveBaseFrontLeftPos = NULL;
PIDController* RobotMap::driveBaseFrontLeft = NULL;
CANTalon* RobotMap::driveBaseFrontRightSteer = NULL;
AnalogInput* RobotMap::driveBaseFrontRightPos = NULL;
PIDController* RobotMap::driveBaseFrontRight = NULL;
CANTalon* RobotMap::driveBaseRearLeftSteer = NULL;
AnalogInput* RobotMap::driveBaseRearLeftPos = NULL;
PIDController* RobotMap::driveBaseRearLeft = NULL;
CANTalon* RobotMap::driveBaseRearRightSteer = NULL;
AnalogInput* RobotMap::driveBaseRearRightPos = NULL;
PIDController* RobotMap::driveBaseRearRight = NULL;
SpeedController* RobotMap::driveBaseFrontLeftDrive = NULL;
SpeedController* RobotMap::driveBaseFrontRightDrive = NULL;
SpeedController* RobotMap::driveBaseRearLeftDrive = NULL;
SpeedController* RobotMap::driveBaseRearRightDrive = NULL;
DigitalInput* RobotMap::driveBaseToteWideLeft = NULL;
DigitalInput* RobotMap::driveBaseToteNarrowLeft = NULL;
DigitalInput* RobotMap::driveBaseToteNarrowRight = NULL;
DigitalInput* RobotMap::driveBaseToteWideRight = NULL;
DigitalInput* RobotMap::driveBaseToteCenter = NULL;
CANTalon* RobotMap::stackerLiftFrontRight = NULL;
CANTalon* RobotMap::stackerLiftFrontLeft = NULL;
CANTalon* RobotMap::stackerDart = NULL;
DigitalInput* RobotMap::stackerHome = NULL;
CANTalon* RobotMap::stackerSqueeze = NULL;
Encoder* RobotMap::stackerSqueezePosition = NULL;
SpeedController* RobotMap::grabberExtension = NULL;
DigitalOutput* RobotMap::arduinoSensorLeft = NULL;
DigitalOutput* RobotMap::arduinoSensorRight = NULL;
DigitalOutput* RobotMap::arduinoRobotState1 = NULL;
DigitalOutput* RobotMap::arduinoRobotState2 = NULL;
DigitalOutput* RobotMap::arduinoSensorMiddleLeft = NULL;
DigitalOutput* RobotMap::arduinoSensorMiddle = NULL;
DigitalOutput* RobotMap::arduinoSensorMiddleRight = NULL;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=ALLOCATION

void RobotMap::init() {
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
	LiveWindow* lw = LiveWindow::GetInstance();

	driveBaseFrontLeftSteer = new CANTalon(6);
	
	
	driveBaseFrontLeftPos = new AnalogInput(0);
	lw->AddSensor("DriveBase", "FrontLeftPos", driveBaseFrontLeftPos);
	
	driveBaseFrontLeft = new PIDController(2.0, 0.0, 0.0,/* F: 0.0, */ driveBaseFrontLeftPos, driveBaseFrontLeftSteer, 0.02);
	lw->AddActuator("DriveBase", "FrontLeft", driveBaseFrontLeft);
	driveBaseFrontLeft->SetContinuous(true); driveBaseFrontLeft->SetAbsoluteTolerance(0.2); 
        driveBaseFrontLeft->SetInputRange(0.0, 5.0);
        driveBaseFrontLeft->SetOutputRange(-0.75, 0.75);
	driveBaseFrontRightSteer = new CANTalon(7);
	
	
	driveBaseFrontRightPos = new AnalogInput(1);
	lw->AddSensor("DriveBase", "FrontRightPos", driveBaseFrontRightPos);
	
	driveBaseFrontRight = new PIDController(2.0, 0.0, 0.0,/* F: 0.0, */ driveBaseFrontRightPos, driveBaseFrontRightSteer, 0.02);
	lw->AddActuator("DriveBase", "FrontRight", driveBaseFrontRight);
	driveBaseFrontRight->SetContinuous(true); driveBaseFrontRight->SetAbsoluteTolerance(0.2); 
        driveBaseFrontRight->SetInputRange(0.0, 5.0);
        driveBaseFrontRight->SetOutputRange(-0.75, 0.75);
	driveBaseRearLeftSteer = new CANTalon(8);
	
	
	driveBaseRearLeftPos = new AnalogInput(2);
	lw->AddSensor("DriveBase", "RearLeftPos", driveBaseRearLeftPos);
	
	driveBaseRearLeft = new PIDController(2.0, 0.0, 0.0,/* F: 0.0, */ driveBaseRearLeftPos, driveBaseRearLeftSteer, 0.02);
	lw->AddActuator("DriveBase", "RearLeft", driveBaseRearLeft);
	driveBaseRearLeft->SetContinuous(true); driveBaseRearLeft->SetAbsoluteTolerance(0.2); 
        driveBaseRearLeft->SetInputRange(0.0, 5.0);
        driveBaseRearLeft->SetOutputRange(-0.75, 0.75);
	driveBaseRearRightSteer = new CANTalon(9);
	
	
	driveBaseRearRightPos = new AnalogInput(3);
	lw->AddSensor("DriveBase", "RearRightPos", driveBaseRearRightPos);
	
	driveBaseRearRight = new PIDController(2.0, 0.0, 0.0,/* F: 0.0, */ driveBaseRearRightPos, driveBaseRearRightSteer, 0.02);
	lw->AddActuator("DriveBase", "RearRight", driveBaseRearRight);
	driveBaseRearRight->SetContinuous(true); driveBaseRearRight->SetAbsoluteTolerance(0.2); 
        driveBaseRearRight->SetInputRange(0.0, 5.0);
        driveBaseRearRight->SetOutputRange(-0.75, 0.75);
	driveBaseFrontLeftDrive = new Talon(0);
	lw->AddActuator("DriveBase", "FrontLeftDrive", (Talon*) driveBaseFrontLeftDrive);
	
	driveBaseFrontRightDrive = new Talon(1);
	lw->AddActuator("DriveBase", "FrontRightDrive", (Talon*) driveBaseFrontRightDrive);
	
	driveBaseRearLeftDrive = new Talon(2);
	lw->AddActuator("DriveBase", "RearLeftDrive", (Talon*) driveBaseRearLeftDrive);
	
	driveBaseRearRightDrive = new Talon(3);
	lw->AddActuator("DriveBase", "RearRightDrive", (Talon*) driveBaseRearRightDrive);
	
	driveBaseToteWideLeft = new DigitalInput(1);
	lw->AddSensor("DriveBase", "ToteWideLeft", driveBaseToteWideLeft);
	
	driveBaseToteNarrowLeft = new DigitalInput(2);
	lw->AddSensor("DriveBase", "ToteNarrowLeft", driveBaseToteNarrowLeft);
	
	driveBaseToteNarrowRight = new DigitalInput(4);
	lw->AddSensor("DriveBase", "ToteNarrowRight", driveBaseToteNarrowRight);
	
	driveBaseToteWideRight = new DigitalInput(5);
	lw->AddSensor("DriveBase", "ToteWideRight", driveBaseToteWideRight);
	
	driveBaseToteCenter = new DigitalInput(3);
	lw->AddSensor("DriveBase", "ToteCenter", driveBaseToteCenter);
	
	stackerLiftFrontRight = new CANTalon(12);
	
	
	stackerLiftFrontLeft = new CANTalon(11);
	
	
	stackerDart = new CANTalon(10);
	
	
	stackerHome = new DigitalInput(0);
	lw->AddSensor("Stacker", "Home", stackerHome);
	
	stackerSqueeze = new CANTalon(16);
	
	
	stackerSqueezePosition = new Encoder(6, 7, false, Encoder::k4X);
	lw->AddSensor("Stacker", "SqueezePosition", stackerSqueezePosition);
	stackerSqueezePosition->SetDistancePerPulse(1.46484375E-4);
        stackerSqueezePosition->SetPIDSourceParameter(Encoder::kDistance);
	grabberExtension = new Talon(4);
	lw->AddActuator("Grabber", "Extension", (Talon*) grabberExtension);
	
	arduinoSensorLeft = new DigitalOutput(10);
	lw->AddActuator("Arduino", "SensorLeft", arduinoSensorLeft);
	
	arduinoSensorRight = new DigitalOutput(14);
	lw->AddActuator("Arduino", "SensorRight", arduinoSensorRight);
	
	arduinoRobotState1 = new DigitalOutput(15);
	lw->AddActuator("Arduino", "RobotState1", arduinoRobotState1);
	
	arduinoRobotState2 = new DigitalOutput(16);
	lw->AddActuator("Arduino", "RobotState2", arduinoRobotState2);
	
	arduinoSensorMiddleLeft = new DigitalOutput(11);
	lw->AddActuator("Arduino", "SensorMiddleLeft", arduinoSensorMiddleLeft);
	
	arduinoSensorMiddle = new DigitalOutput(12);
	lw->AddActuator("Arduino", "SensorMiddle", arduinoSensorMiddle);
	
	arduinoSensorMiddleRight = new DigitalOutput(13);
	lw->AddActuator("Arduino", "SensorMiddleRight", arduinoSensorMiddleRight);
	


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
	driveBaseFrontLeftSteer->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	driveBaseFrontRightSteer->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	driveBaseRearLeftSteer->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	driveBaseRearRightSteer->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	stackerLiftFrontRight->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	stackerLiftFrontLeft->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	stackerDart->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	stackerSqueeze->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);

//	driveBaseFrontRightSteer->SetVoltageRampRate(1000);

	stackerLiftFrontRight->SetFeedbackDevice(CANTalon::QuadEncoder);
	stackerLiftFrontRight->SetControlMode(CANTalon::kPercentVbus);
	stackerLiftFrontRight->SetPID(.1, 0.0, 16);
//	stackerLiftFrontRight->SetCloseLoopRampRate(2);
	stackerLiftFrontLeft->SetControlMode(CANTalon::kFollower);
	stackerLiftFrontLeft->Set(12);

	stackerDart->SetFeedbackDevice(CANTalon::AnalogPot);
	stackerDart->ConfigFwdLimitSwitchNormallyOpen(true);
	stackerDart->ConfigRevLimitSwitchNormallyOpen(true);
	stackerDart->SetControlMode(CANSpeedController::kPosition);
	stackerDart->SetPID(100,0,0);
	stackerDart->ConfigSoftPositionLimits(900, 30);

//	stackerLiftFrontLeft->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
//	stackerDart->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);


}
