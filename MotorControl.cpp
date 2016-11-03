/**
 * File: MotorControl.cpp
 * Description: TODO
 */

#define FREQ 20000 // 20 kHz
#define STEP 1

MotorControl::MotorControl(float s_dt) {

  dt = chrono::microseconds(int(s_dt*pow(10,6)));
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

  for (int i = 0; i < pidctrl_list.size(); i++) {
       encoder_list[i]->calcRotation();
       pidctrl_list[i]->setAngle(encoder_list[i]->getAngle());
  }

  shouldStop = false;
  worker_thread = thread(&MotorControl::worker, this);
}

void MotorControl::stop() {
  shouldStop = true;

  for (int i = 0; i < pidctrl_list.size(); i++) {
     encoder_list[i]->calcRotation();
     pidctrl_list[i]->setAngle(encoder_list[i]->getAngle());
     pidctrl_list[i]->setDuty(0);
  }

}

void MotorControl::callback() {
  for (int i = 0; i < pidctrl_list.size(); i++) {
      if (encoder_list[i]->calcRotation() == -1) {
        this->stop();
      }
    pidctrl_list[i]->updatePWM(true);
  }
}

void MotorControl::worker() {
  if(encoder_list.size() != pidctrl_list.size()){
    cerr << "MotorControl Encoder list and pid list not same size" << endl;
    shouldStop = true;
    return;
  }

  //all the system clock is in seconds

  chrono::time_point<chrono::system_clock> prevTime;
  chrono::time_point<chrono::system_clock> endTime;

  chrono::microseconds elapsed_ms;

  while (!shouldStop) {        
    prevTime = chrono::system_clock::now();
    this->callback();
    endTime = chrono::system_clock::now();

    //convert to ms
    elapsed_ms = std::chrono::duration_cast<chrono::microseconds>(endTime - prevTime);

    if (float(elapsed_ms.count())/float(dt.count()) >= 1.00) {
      cout << "overrun" << endl;
    }
    else{
      this_thread::sleep_for(dt - elapsed_ms);
    }
  }
}