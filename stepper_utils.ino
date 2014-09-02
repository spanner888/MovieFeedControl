void setStepperDirection(boolean dir){
    fwdDir = dir;
    if (fwdDir) dirCtl = 1;           // fwdDir sign is used to control sign (ie direction) of steppers by multiplying this with number of steps or the speed.
    else        dirCtl = -1;
}

// stuff that has to be done every time one stepper var or microStepRatio changes
void calcStepperVars(){
  if (microStepRatio > 1)
    digitalWrite(microStep_PIN, true);    //microStepping & signal is active low
  else
    digitalWrite(microStep_PIN, false);   //NOT microStepping & signal is active low

  stepsPerRevolution = 200 * microStepRatio;  // change this to fit the number of steps per revolution of stepper used
  maxStepperSpeed = 1000 * microStepRatio;            //Speeds of more than 1000 steps per second are unreliable;
    #ifdef USE_FEEDIN_STEPPER
        feedInStepper.setMaxSpeed(maxStepperSpeed * microStepRatio);
        feedInStepper.setAcceleration(900 * microStepRatio);
    #endif
    #ifdef USE_TAKEOFF_STEPPER
      takeOffStepper.setMaxSpeed(maxStepperSpeed * microStepRatio);
      takeOffStepper.setAcceleration(900 * microStepRatio);
    #endif

  stepsPerFrame = stepsPerRevolution/numbSprockets;
  stepsPerJog = 1;  //stepsPerFrame/100;          //TODO review if this is realistic!!!!!!
  stepsPerBigJog = stepsPerFrame/20;        //TODO review if this is realistic!!!!!!

    #ifdef USE_FEEDIN_STEPPER
        int speedInc =  maxStepperSpeed/50;    //Speeds of more than 1000 steps per second are unreliable;
        int feedMotorSpeed = dfltStepperSpeed;
    #endif
    #ifdef USE_TAKEOFF_STEPPER
        #ifdef TAKE_PHOTO
            int dfltTakeOffStepperSpeed = 7;
        #else
            int dfltTakeOffStepperSpeed = 4;    // dif speed as no delays!
        #endif
        int takeOffStepperSpeed = dfltTakeOffStepperSpeed;
        int takeOffStepperSpeedInc = 1;
    #endif
}

void stepperInit(){
  pinMode(microStep_PIN, OUTPUT);  // connected to ALL stepper ctlr /enable
  setStepperDirection(STEPPERS_FWD);
  calcStepperVars();
}

