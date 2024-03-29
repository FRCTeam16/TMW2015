// RobotBuilder Version: 1.5
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef OI_H
#define OI_H

#include "WPILib.h"

class OI {
private:
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	Joystick* gamePad;
	Joystick* driverRight;
	Joystick* driverLeft;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	double pi;
	double scaledRadians(double radians);

public:
	OI();
 
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=PROTOTYPES

	Joystick* getDriverLeft();
	Joystick* getDriverRight();
	Joystick* getGamePad();

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=PROTOTYPES
	double getScaledJoystickRadians();
	double getJoystickMagnitude();
	double getJoystickTwist();
	double getJoystickX();
	double getJoystickY();
	double getLeftJoystickXRadians();
	double getScaledJoystick (double value, double scaling);

};

#endif
