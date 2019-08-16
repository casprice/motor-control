/**
 * File: MotorControl.cpp
 * Description: TODO
 */

#define FREQ 20000 // 20 kHz
#define STEP 1

MotorControl::MotorControl(vector<shared_ptr<Encoder>> encs, 
                           vector<shared_ptr<PID>> pids) {
  encoders = encs;
  pidctrls = pids;
}

MotorControl::~MotorControl() {
  if (worker_thread.joinable()) {
    shouldStop = true;
    worker_thread.join();
  }
}

void MotorControl::start() {
  if (worker_thread.joinable()) {
    shouldStop = true;
    worker_thread.join();
  }
  shouldStop = false;
  worker_thread = thread(&MotorControl::worker, this);
}

void MotorControl::stop() {
  shouldStop = true;
}

void MotorControl::callback() {
  for (int i = 0; i < NUM_MOTORS; i++) {
    if (encoders[i]->calcRotation() == -1) {
      this->stop();
    }
    pidctrls[i]->updatePWM(value, true);
  }
}

void MotorControl::worker() {
  while (!shouldStop) {
    this->callback();
    this_thread::sleep_for(chrono::milliseconds(100));
  }
}

// remove after integrating with ROS
int main() {}