#include "DualMaxBoticsI2CXL.h"

#include "WPILib.h"
#include <string>

DualMaxBoticsI2CXL::DualMaxBoticsI2CXL(I2C::Port port, uint8_t address1, uint8_t address2, float frequency) {
  enabled_ = true;
  running_ = true;
  isDead_ = false;
  address1_ = address1;
  address2_ = address2;
  frequency_ = frequency;
  sensor1_ = new I2C(port, address1);
  sensor2_ = new I2C(port, address2);
  task_ = new Task("DualMaxBoticsI2CXL", (FUNCPTR)DualMaxBoticsI2CXL::Run, Task::kDefaultPriority);
  task_->Start((uint32_t)this);
}

DualMaxBoticsI2CXL::~DualMaxBoticsI2CXL(){
  task_->Stop();
  delete task_;
}

void DualMaxBoticsI2CXL::Run(DualMaxBoticsI2CXL* task) {
  while (task->running_) {
    if (task->enabled_) {
    	task->sensor1_->Write((unsigned char)INITREADING,0);
    	Wait(task->frequency_);
    	task->sensor1_->ReadOnly(2,task->distance1_);
    	task->sensor2_->Write((unsigned char)INITREADING,0);
		Wait(task->frequency_);
		task->sensor2_->ReadOnly(2,task->distance2_);

    }
    else
      Wait(1.0);   // 50 ms wait period while task is 'paused'
  }

  task->isDead_ = true; // Falling off the edge of the earth...
}

void DualMaxBoticsI2CXL::Start() {
  enabled_ = true;
}

void DualMaxBoticsI2CXL::Pause() {
  enabled_ = false;
}

void DualMaxBoticsI2CXL::Terminate() {
  running_ = false;

  // Above told the task to exit on the next loop around.
  // That could take 2ms or 50ms based on whether it's in pause or run and how long
  // the actual Run() routine takes too. So we have to wait until we're really terminated here.
  while (!isDead_) {
    Wait(0.02); // Wait until we're really dead on that task.
  }
}

int DualMaxBoticsI2CXL::GetDistance1() {
	return distance1_[1] + distance1_[0]*256;
}

int DualMaxBoticsI2CXL::GetDistance2() {
	return distance2_[1] + distance2_[0]*256;
}

