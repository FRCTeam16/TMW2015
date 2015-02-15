#ifndef DualMaxBoticsI2CXL_H_
#define DualMaxBoticsI2CXL_H_
#define INITREADING 0x51

#include "WPILib.h"

class DualMaxBoticsI2CXL {
 public:
  DualMaxBoticsI2CXL(I2C::Port port, uint8_t address1, uint8_t address2, float frequency);
  virtual ~DualMaxBoticsI2CXL();
  static void Run(DualMaxBoticsI2CXL* task);
  void Start();
  void Pause();
  void Terminate();
  int GetDistance1();
  int GetDistance2();
 private:
  uint8_t address1_;
  uint8_t address2_;
  bool enabled_;
  bool running_;
  bool isDead_;
  unsigned char distance1_[2] = {0,0};
  unsigned char distance2_[2] = {0,0};
  float frequency_;
  Task* task_;
  I2C* sensor1_;
  I2C* sensor2_;
};

#endif  // DualMaxBoticsI2CXL_H_
