// RobotBuilder Version: 1.5
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.

#include "Stacker.h"
#include "../RobotMap.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

Stacker::Stacker() : Subsystem("Stacker") {
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	liftFrontRight = RobotMap::stackerLiftFrontRight;
	liftFrontLeft = RobotMap::stackerLiftFrontLeft;
	dart = RobotMap::stackerDart;
	squeeze = RobotMap::stackerSqueeze;
	home = RobotMap::stackerHome;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

}
    
void Stacker::InitDefaultCommand() {
}

