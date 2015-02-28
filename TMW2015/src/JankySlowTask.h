#ifndef JankySlowTask_H_
#define JankySlowTask_H_

#include "WPILib.h"

/**
 * @author Robert Wolff - based largely on Tom Bottglieri's work from Team 254
 * @author Tom Bottglieri
 *
 * @brief Abstract superclass of tasks (run on a separate thread).
 * Call Start() to begin the task and Pause() to temporarily pause it.
 * The inheriting class must implement Run() which will be called again and
 * again when the task is in 'Start/Running' mode.
 */
class JankySlowTask {
 public:
  /**
   * @brief Constructor which takes an optional taskname. In the absence of a task name,
   *        a task name will be created based upon the current system-time.
   */
  JankySlowTask(const char* taskName = NULL, uint32_t priority = Task::kDefaultPriority);
  virtual ~JankySlowTask();

  /**
   * @brief PRIVATE- Do not call this function externally as it's only passed to the constructor to initialize the object.
   *
   * This function sets up an operating-system task which will perpetually loop the Run() function.
   * @note When running, loops wait 2ms between calls to Run(). During pause, loop waits 50ms.
   * @param JankySlowTask the given JankySlowTask object to run.
   */
  static void JankyPrivateStarterTask(JankySlowTask* task);

  /**
   * @brief Starts the task.
   */
  void Start();

  /**
   * @brief Pauses the task.
   */
  void Pause();

  /**
   * @brief Terminate the task.
   */
  void Terminate();

  /**
   * @brief The inheriting class must implement this function. This is the function
   *        which gets run in the new task.
   *
   * Subclasses should override this function to specify their own behavior.
   */
  virtual void Run() = 0;

  bool TaskRunning() {return enabled_;}
 private:
  bool enabled_;
  bool running_;
  bool isDead_;
  Task* task_;
};

#endif  // JankySlowTask_H_
