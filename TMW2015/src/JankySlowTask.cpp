#include "WPILib.h"
#include "JankySlowTask.h"
#include <string>

JankySlowTask::JankySlowTask(const char* taskName, uint32_t priority) {
	  //cout << "JankySlowTask Constructor Start" << endl;
 /* std::string name = taskName;
  char tmp[30];

  if (!taskName)
  {
    sprintf(tmp, "%d", GetFPGATime());
    name = "JankySlowTask-";
    name += tmp;
  }
*/
  enabled_ = false;
  running_ = true;
  isDead_ = false;

  task_ = new Task("JankySlowTask", (FUNCPTR)JankySlowTask::JankyPrivateStarterTask, priority);
  //cout << "JankySlowTask Constructor Complete" << endl;
}

JankySlowTask::~JankySlowTask(){
  delete task_;   // Now kill the WPI class for the task.
}

void JankySlowTask::JankyPrivateStarterTask(JankySlowTask* task) {
  while (task->running_) {
    if (task->enabled_) {
      task->Run();
      Wait(0.020);  // Only wait 20ms when task is active.
    }
    else
      Wait(0.05);   // 50 ms wait period while task is 'paused'
  }

  task->isDead_ = true; // Falling off the edge of the earth...
}

void JankySlowTask::Start() {
  enabled_ = true;
}

void JankySlowTask::Pause() {
  enabled_ = false;
}

void JankySlowTask::Terminate() {
  running_ = false;

  // Above told the task to exit on the next loop around.
  // That could take 2ms or 50ms based on whether it's in pause or run and how long
  // the actual Run() routine takes too. So we have to wait until we're really terminated here.
  while (!isDead_) {
    Wait(0.02); // Wait until we're really dead on that task.
  }
}
