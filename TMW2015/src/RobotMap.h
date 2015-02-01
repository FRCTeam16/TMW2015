// RobotBuilder Version: 1.5
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef ROBOTMAP_H
#define ROBOTMAP_H
#include "WPILib.h"

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */
class RobotMap {
public:
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	static CANTalon* driveBaseFrontLeftSteer;
	static AnalogInput* driveBaseFrontLeftPos;
	static PIDController* driveBaseFrontLeft;
	static CANTalon* driveBaseFrontRightSteer;
	static AnalogInput* driveBaseFrontRightPos;
	static PIDController* driveBaseFrontRight;
	static CANTalon* driveBaseRearLeftSteer;
	static AnalogInput* driveBaseRearLeftPos;
	static PIDController* driveBaseRearLeft;
	static CANTalon* driveBaseRearRightSteer;
	static AnalogInput* driveBaseRearRightPos;
	static PIDController* driveBaseRearRight;
	static SpeedController* driveBaseFrontLeftDrive;
	static SpeedController* driveBaseFrontRightDrive;
	static SpeedController* driveBaseRearLeftDrive;
	static SpeedController* driveBaseRearRightDrive;
	static DigitalInput* driveBaseToteWideLeft;
	static DigitalInput* driveBaseToteNarrowLeft;
	static DigitalInput* driveBaseToteNarrowRight;
	static DigitalInput* driveBaseToteWideRight;
	static CANTalon* stackerLiftFrontRight;
	static CANTalon* stackerLiftFrontLeft;
	static CANTalon* stackerDart;
	static CANTalon* stackerSqueeze;
	static DigitalInput* stackerHome;
	static SpeedController* grabberExtension;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

	static void init();
};
#endif
