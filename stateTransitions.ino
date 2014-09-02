//Choose **ONE** of these three #define's
// NOTE: if using a Teensy board - you MUST ALSO go to tools Menau - USB:type & select either
//    "Keyboard + Mouse + Joystick", or if you also want serial output "Serial + Keyboard + Mouse + Joystick"
#define TAKE_PHOTO_GUVCVIEW     // Requires an Arduino that can SEND keystrokes via USB, eg Leonardo, Teensy
//#define TAKE_PHOTO_VLC          // Requires an Arduino that can SEND keystrokes via USB, eg Leonardo, Teensy
//#define TAKE_PHOTO_CHEESE       // Requires an Arduino that can SEND keystrokes via USB, eg Leonardo, Teensy
//TODO - make last two work with Leonardo - ATM only work with Teensy's


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions that manage what the state transitions are - ie what is the next state,
// and initiate the relevant actions.
//////////////////////////////////////////////////////////////////////////////////////////////////////

void stateStoppedTransitions(){
    #if defined BUTTONS  || defined BUTTONS_INPUT
        // readButtons ONLY returns true if one or more has buttons changed!
        if (readButtons()){
//sPrintStatus("TMP: stateStoppedTransitions: ");
//while(1){;}

/*TO DO implement button precedence order
... eg stop before other code!!!!

also need watch MULTIPLE press vs single press!!!!

*/

            // If fwdFrameButton pressed, setup both steppers for frame x frame movement
            if ((buttonsCurrent & fwdFrameButton_MASK) == fwdFrameButton_MASK){
                setStepperDirection(STEPPERS_FWD);

                #ifdef USE_TAKEOFF_STEPPER
//                    takeOffStepper.setSpeed(TAKEOFF_SPEED_CALCULATION);
//sPrintStatus("stateStoppedTransitions takeOffStepper.setSpeed: ");
                #endif // USE_TAKEOFF_STEPPER
                #ifdef USE_FEEDIN_STEPPER
                    feedInStepper.move(FEEDIN_MOVE_CALCULATION);
                #endif // USE_FEEDIN_STEPPER
                nextFeedTime = millis();                                  // ie next feed time is RIGHT NOW!
                nextFrameTriggered = true;                                // enable starting ext frame now!
                currentState = STATE_MOVING;
            }

            // now if jog button pressed, setup both steppers for jog movement
            if ((buttonsCurrent & jogButton_MASK) == jogButton_MASK){
                setStepperDirection(STEPPERS_FWD);

//sPrintStatus("jogButton: ");
                #ifdef USE_TAKEOFF_STEPPER
//                    takeOffStepper.setSpeed(TAKEOFF_SPEED_CALCULATION);
                #endif // USE_TAKEOFF_STEPPER
                #ifdef USE_FEEDIN_STEPPER
                    feedInStepper.move(stepsPerJog);
                #endif // USE_FEEDIN_STEPPER
                currentState = STATE_MOVING;
            }
            if ((buttonsCurrent & bigJogButton_MASK) == bigJogButton_MASK){
                setStepperDirection(STEPPERS_FWD);

//sPrintStatus("bigJogButton: ");
                #ifdef USE_TAKEOFF_STEPPER
//                    takeOffStepper.setSpeed(TAKEOFF_SPEED_CALCULATION);
                #endif // USE_TAKEOFF_STEPPER
                #ifdef USE_FEEDIN_STEPPER
                    feedInStepper.move(stepsPerBigJog);
                #endif // USE_FEEDIN_STEPPER
                currentState = STATE_MOVING;
            }
        }
    /*
    revFrameButton_MASK
    fasterButton_MASK
    slowerButton_MASK
    takePhoto-StopButton_MASK
    */
    #endif
}


void stateMovingTransitions(){
//TODO ** FIRST CHECK FOR EMERGENCY SWITCH & act on it, then other switches
//      IGNORE buttons here - can wait until stopped to act on those!!!!

      checkEmergencyStopSwitch();     // check emergency stop switch & if ON, DO emergency stop!

      actionOtherSwitches();          // read remaining switches & act as appropriate.

// If no emergency stop, then ALL movements keep going until finished!!
// So jog/bigJog run ONCE until finished, then stop until some other cmd received.
// frame x frame movements also run until finished, BUT KEEP REPEATING until emergency or stop button combo pressed
    #ifdef USE_FEEDIN_STEPPER
      feedInStepper.run();
    #endif // USE_FEEDIN_STEPPER
    #ifdef USE_TAKEOFF_STEPPER
      takeOffStepper.runSpeed();
    #endif // USE_TAKEOFF_STEPPER

// use last button values
  // check if finished moving THIS FRAME, if so, then set time to start moving again, but don't enable moving yet!
    #ifdef USE_FEEDIN_STEPPER
      unsigned long currentTime = millis();
      if (feedInStepper.distanceToGo() == 0) {
        if (nextFrameTriggered){
            nextFeedTime = currentTime + interFrameMovementTime;
            nextFrameTriggered = false;

//temp - make LED flash off!
#ifdef DEBUG_LED
  DBG_LED3(false);
//  sPrintStatus("stateMovingTransitions **END** of frame Move: ");
#endif

            // if we were doing big/jogging, then stop
            if ((buttonsCurrent & bigJogButton_MASK) || (buttonsCurrent & jogButton_MASK)){
                currentState = STATE_STOPPED;
            }
            // frame x frame movement keeps occurring until stopped button combo pressed.
            if (readButtons() && ((buttonsCurrent & stopButton_MASK) == stopButton_MASK)){
                currentState = STATE_STOPPED;
                delay(3000);  //wait for switches to (hopefully) be released - avoid false frameFwd trigger
                              // BLOCKING delay - but does NOT matter here!
            }
    //??? if (currentState <> STATE_STOPPED) save state, and move to idle state???????

//            #if defined TAKE_PHOTO_GUVCVIEW || defined TAKE_PHOTO_VLC || defined TAKE_PHOTO_CHEESE
            /* TO DO     ... this would be good time to send any serial data... instead of wasting time!!!!!
                ALSO make this a NON-BLOCKING delay!!!!!!
            */
                // **ONLY take photo if PREVIOUS button push was fwdFrame!!!!
                // ie do not take photo on big/jog.....
//if ((buttonsPrevious && fwdFrameButton_MASK) == fwdFrameButton_MASK)
                {
                    //testing delay to allow film to stop moving
                    delay(500);

                    // send keys to camera app to take photo ... APP MUST HAVE FOCUS!!!!!
                    //https://www.pjrc.com/teensy/td_keyboard.html
                   //gucview KEY_SPACE or "i"??
Keyboard.print(" ");
                    #ifdef TAKE_PHOTO_GUVCVIEW
                        Keyboard.print("i");
                    #endif // TAKE_PHOTO_GUVCVIEW

                    // vlc - shift-s
                    #ifdef TAKE_PHOTO_VLC
                         Keyboard.set_modifier(MODIFIERKEY_SHIFT);      //Leonardo KEY_LEFT_SHIFT
                         Keyboard.send_now();
                         Keyboard.set_key1(KEY_S);
                         Keyboard.send_now();
                         //  Keyboard.print("t");
                         Keyboard.set_modifier(0);
                         Keyboard.send_now();
                         Keyboard.set_key1(0);
                         Keyboard.send_now();
                    #endif // TAKE_PHOTO_VLC

                    // cheese alt-t
                    #ifdef TAKE_PHOTO_CHEESE
                        Keyboard.set_modifier(MODIFIERKEY_ALT);         //Leonardo KEY_LEFT_ALT
                        Keyboard.send_now();
                        Keyboard.set_key1(KEY_T);
                        Keyboard.send_now();
                        //  Keyboard.print("t");
                        Keyboard.set_modifier(0);
                        Keyboard.send_now();
                        Keyboard.set_key1(0);
                        Keyboard.send_now();
                    #endif // TAKE_PHOTO_CHEESE

                    numberFrames +=1;
                    Serial.print(numberFrames);
                    Serial.print("\t");
                    printElapsedTimeDigits();

                //TO DO    make this a NON-BLOCKING delay!!!!!! -
                //??? ANY IMPACT ON OTHER CODE - ie OTHER actions ALSO must wait for this
                // OR ... could start NEXT frame movement, BUT NEXT photo has to wait!!!!!
                    delay(250);
                }
            #endif


          }
        else{
            // Now check if time to actually move again - but ONLY if already stopped, if so, then reset position & give cmd for how far to move
            // use a non-blocking delay!!!!
            if(currentTime >= nextFeedTime){
                feedInStepper.setCurrentPosition(0);    // have to reset this, so AccelStepper will move the stepper!
                feedInStepper.move(FEEDIN_MOVE_CALCULATION);
                nextFrameTriggered = true;
                // both stepper steps/speed ALREADY set in the stopped state when buttons pushed (including any speed faster/slower increments)
                //- so avoid expensive calls to these here

#ifdef DEBUG_LED
  DBG_LED3(true);
//  sPrintStatus("stateMovingTransitions **START** of frame Move: ");
#endif

/* - prob no dif as coded for fast loop/statre processing. Also removing at least temp, whole chasing takeOff speed issues
                // start moving IMMEDIATELY!
                #ifdef USE_FEEDIN_STEPPER
                    feedInStepper.run();
                #endif // USE_FEEDIN_STEPPER
                #ifdef USE_TAKEOFF_STEPPER
                    takeOffStepper.runSpeed();
                #endif // USE_TAKEOFF_STEPPER
*/
              }
            }
      }
//    #endif // USE_FEEDIN_STEPPER

}


