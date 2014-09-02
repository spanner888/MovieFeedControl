/// BOF preprocessor bug prevent - insert me on top of your arduino-code
// From: http://www.a-control.de/arduino-fehler/?lang=en
#if 1
__asm volatile ("nop");
#endif


//test-debug & CONFIGURATION .... these can be removed once it is all working....
#define USE_FEEDIN_STEPPER
#define USE_TAKEOFF_STEPPER

#define SWITCHES

// only use ONE of next two - will get compile error if none or both included!
//#define BUTTONS               // button read using PINx
#define BUTTONS_INPUT           // button read using digitalRead

//#define DEBUG true              // en/disables debug serial coms msgs
//#define DBG2_CHANGED            // ON change of STATE: print status at end of main loop
//#define DBG2_ALL                // EVERY LOOP: print status at end of main loop
#define DEBUG_LED               // output status info via LED
//#define MEMRY1                  // simple memory info dump to serial
//#define MEMRY2                  // detailed memory info dump to serial
// END: test-debug & CONFIGURATION


#include <AccelStepper.h>
#include <Time.h>               // just for displaying time in serial terminal
#include "MovieFeedControl.h"


// **************************** CODE IS BELOW HERE :) ***************************************
void setup() {
  //memory space/info
  #ifdef MEMRY1
    Serial.begin (SERIAL_SPEED);
    showFreeRam();
  #endif

  #ifdef MEMRY2
    Serial.begin (SERIAL_SPEED);
    MEMRYDetail();
  #endif

  #ifdef DEBUG
      Serial.begin(SERIAL_SPEED);
  #endif

  #ifdef DEBUG_LED
      pinMode(LED1, OUTPUT);
      pinMode(LED2, OUTPUT);
      pinMode(LED3, OUTPUT);
      pinMode(LED4, OUTPUT);
      pinMode(LED5, OUTPUT);
      pinMode(LED6, OUTPUT);
      pinMode(LED7, OUTPUT);
      pinMode(LED8, OUTPUT);
      /* for testing
      DBG_LED1(true);
      DBG_LED2(true);
      DBG_LED3(true);
      delay(3000);
      */
  #endif // DEBUG_LED

  #ifdef SWITCHES
    pinMode(takeOffFaster_PIN, INPUT_PULLUP);
    pinMode(takeOffSlower_PIN, INPUT_PULLUP);
    pinMode(EMERGENCY_STOP_PIN, INPUT_PULLUP);
  //  pinMode(frameDetect_PIN, INPUT);
  //  pinMode(feedInFaster_PIN, INPUT_PULLUP);
  //  pinMode(feedInSlower_PIN, INPUT_PULLUP);
  #endif

  // Note: button IO pins set within readButtons() IF using the PINx method instead in digitalRead method
#ifdef BUTTONS_INPUT
    pinMode(fwdFrameButton_PIN, INPUT_PULLUP);
    //pinMode(revFrameButton_PIN, INPUT_PULLUP);
    pinMode(jogButton_PIN, INPUT_PULLUP);
    pinMode(bigJogButton_PIN, INPUT_PULLUP);
#endif // BUTTONS_INPUT


  stepperInit();
  sPrintStatus("end setup: ");        // might NOT get SEEN in serial term if sent by Teensy3!!!!
}

void loop() {
    // starting with ALL states run to completion, UNLESS EMERGENCY DETECTED.
    // ie if moving one frame, pressing a switch for different movement is ignored UNTIL current movement finishes!
    // Doing this preserves "calibration" of frame edge in photos!!! ... except for emergency!!
    switch(currentState)
      {
      case STATE_EMERGENCY_STOP:

        // FOR NOW STAY IN THIS STATE UNTIL REBOOT OF ARDUINO!!!!!

        Serial.begin(SERIAL_SPEED);           // because it's an emergency - force serial on, so can send msg.
        Serial.print(F("EMERGENCY_STOP"));
        currentState = STATE_EMERGENCY_STOP;
        break;
      case STATE_MOVING:
          stateMovingTransitions();
       break;
      case STATE_STOPPED:
          stateStoppedTransitions();
        break;
      default:
        DBG_PRINT_F("unknown state!!!! :- ");
        break;
      }

//??CAN YOU DO THIS WHILE MICRO-STEPPING??????????????????????????
// Disable motor pin outputs by setting them all LOW Depending ....may (DOES!!!) turn off the power to the motor coils, saving power.
// This is useful to support Arduino low power modes:
//  disable the outputs during sleep and then reenable with enableOutputs() before stepping again.

//?? ONLY do this ONCE at final position (ie at a FULL step pos)?????
#ifdef USE_FEEDIN_STEPPER
  feedInStepper.disableOutputs() ;
#endif // USE_FEEDIN_STEPPER
#ifdef USE_TAKEOFF_STEPPER
  takeOffStepper.disableOutputs() ;
#endif // USE_TAKEOFF_STEPPER


#ifdef DBG2_CHANGED

  if ( currentState != prevState)
  {
      sPrintStatus("End of Main loop: ");
      prevState = currentState;
      delay(300);
  }
#endif
#ifdef DBG2_ALL
      sPrintStatus("End of Main loop: ");
      prevState = currentState;
      delay(300);
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Support functions
//////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined BUTTONS
// non-blocking way to QUICKLY read a set (Ard Port) of buttons.
// returns true if buttons read, else false if still waiting for debounce time to expire.
boolean readButtons(){
    if ((millis()-buttonLastReadTime) > buttonDebounceDelay) {
        BUTTON_DDR &= B00000001;              // initialise according to mask, don't change bits not using.
        BUTTON_PORT |= B11111110;             // setup any input pullups
        byte buttonsTemp = BUTTON_PINS;       // read the port
//Serial.println(buttonsTemp, BIN);
        if (unpressedButtonState == 1) {
            buttonsTemp = ~buttonsTemp & B00111110;     // unpressed button state is normally high in this case, so invert it!
//Serial.println(buttonsTemp, BIN);
        }
//delay(500);
        // if no change to buttons, return false, else update & return true.
        if (buttonsCurrent == buttonsTemp){
            return false;
        }
        else{
            buttonsPrevious = buttonsCurrent;
            buttonsCurrent = buttonsTemp;
            buttonLastReadTime = millis();  // for debouncing - remember the time buttons read!
            return true;
        }
    }
    return false;
}
#endif // BUTTONS

#ifdef BUTTONS_INPUT
// non-blocking way to QUICKLY read a set (Ard Port) of buttons.
// returns true if buttons read, else false if still waiting for debounce time to expire.
boolean readButtons(){
    if ((millis()-buttonLastReadTime) > buttonDebounceDelay) {

        // using digitalRead, but sort of keeping code v similar to PINx code
        byte buttonsTemp = B00000000;

        if (digitalRead(fwdFrameButton_PIN)) buttonsTemp |= fwdFrameButton_MASK;
        else buttonsTemp &= ~fwdFrameButton_MASK;
        if (digitalRead(jogButton_PIN)) buttonsTemp |= jogButton_MASK;
        else buttonsTemp &= ~jogButton_MASK;
        if (digitalRead(bigJogButton_PIN)) buttonsTemp |= bigJogButton_MASK;
        else buttonsTemp &= ~bigJogButton_MASK;

        if (unpressedButtonState == 1) {
            buttonsTemp = ~buttonsTemp & B00111110;     // unpressed button state is normally high in this case, so invert it!
        }
//Serial.println(buttonsTemp, BIN);
//delay(300);

        // if no change to buttons, return false, else update & return true.
        if (buttonsCurrent == buttonsTemp){
            return false;
        }
        else{
            buttonsPrevious = buttonsCurrent;
            buttonsCurrent = buttonsTemp;
            buttonLastReadTime = millis();  // for debouncing - remember the time buttons read!
            return true;
        }
    }
    return false;
}
#endif // BUTTONS_INPUT

void checkEmergencyStopSwitch(){
      // read status of emergency stop switch {according to logic level defined by unpressedSwitchState}
      byte eSwitch = 0;
      eSwitch = digitalRead(EMERGENCY_STOP_PIN);
      if (unpressedSwitchState) {
        eSwitch = ~eSwitch & EMERGENCY_STOP_MASK;
      }

      // NOW ACT check emergency stop switch status and if required do an emergency stop!
      // do NOT care about debouncing - switch ON = panic stop!!!!!
      if(eSwitch){
            doEmergencyStop();
      }
}

void doEmergencyStop(){
    // don't care about debounce or previous state of this switch - just STOP!
    // and STAY STOPPED, until you clear issue AND RESET the Arduino!!!
    currentState = STATE_EMERGENCY_STOP;
    Serial.begin(SERIAL_SPEED);
    #ifdef USE_FEEDIN_STEPPER
      feedInStepper.disableOutputs() ;
    #endif // USE_FEEDIN_STEPPER
    #ifdef USE_TAKEOFF_STEPPER
      takeOffStepper.disableOutputs() ;
    #endif
    Serial.println();
    Serial.println();
    sPrintStatus("stateMovingTransitions Emergency Stop: ");
    Serial.print(F("STATE_EMERGENCY_STOP - program halted!"));

    //flash LED fast - FOREVER - ie wait until issue fixed AND Arduino must be rebooted!
    pinMode(LED3, OUTPUT);
    while(1){;
        DBG_LED3(false);
        delay(150);
        DBG_LED3(true);
        delay(150);
    }
}

// read remaining switches (ie all except the emergency switch) & act as appropriate.
// TO DO review if approp to do ALL other switches here - or split faster/slower & others????
void actionOtherSwitches(){
// STARTING USING ONE VAR SET TO CONTROL DEBOUNCE OF **ALL** SWTICHES!!!!
// also coding with binary values - in case decide to change to using read entire IO PORT style - like used in readButtons()

/* things to look at
    - try switchDebounceDelay = 0
    - move digitalRead to top, move if switchDebounceDelay INTO ALL FOUR if's & possibly make it longer - ie THE HARDWARE can't press switch faster than 2 per second

    KEEP TRYING CODE IN CUT DOWN SKETCH!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
*/

/*  The logic here is:
    If faster/slower JUST pressed, change to temp speed (x2/0) until switch is JUST released.
    THEN when switch is JUST released, inc/decrement speed.

    If switch STILL pressed (ie prev & now pressed) ... continue at temp speed (no added code required - just continue temp speed!)
    If switch NOT pressed NOW & PREV ... continue at temp speed (no added code required - just continue temp speed!)

    if BOTH pressed - doEmergencyStop();
*/

    if ((millis()-switchLastReadTime) > switchDebounceDelay) {
//sPrintFS_Switches("0:");
LED_fs();
        switchLastReadTime = millis();
        // now check the takeOffFaster switch & increase TAKE OFF stepper speed if required
        takeOffFasterSwitch = digitalRead (takeOffFaster_PIN);
//DBG_PRINT_F("\ttakeOffFasterSwitch: "); DBG_PRINTLN2(takeOffFasterSwitch, BIN);
        if (unpressedSwitchState) takeOffFasterSwitch = ~takeOffFasterSwitch & B00000001;
//DBG_PRINT_F("\ttakeOffFasterSwitch: "); DBG_PRINTLN2(takeOffFasterSwitch, BIN);

        // Just pressed?
        if(takeOffFasterSwitch && !takeOffFasterSwitchPrev){
            // so now set speed to temp faster speed to get idle arm back into working pos & release switch quicker
//sPrintFS_Switches("1:");
LED_fs();
//delay(2000);
            #ifdef USE_TAKEOFF_STEPPER
                takeOffStepper.setSpeed( 4 * TAKEOFF_SPEED_CALCULATION);   //tofSwitchActivatedSpeed);
            #endif // USE_TAKEOFF_STEPPER
        }

        // Just released?
        if(!takeOffFasterSwitch && takeOffFasterSwitchPrev){
            // so NOW set the new takeOffStepper speed
            takeOffStepperSpeed += takeOffStepperSpeedInc;
            if (takeOffStepperSpeed > maxTakeOffStepperSpeed ) takeOffStepperSpeed = maxTakeOffStepperSpeed;
//sPrintFS_Switches("2:");
LED_fs();
//delay(2000);
            #ifdef USE_TAKEOFF_STEPPER
              takeOffStepper.setSpeed(TAKEOFF_SPEED_CALCULATION);
            #endif // USE_TAKEOFF_STEPPER
        }
        takeOffFasterSwitchPrev = takeOffFasterSwitch;

        // now check the takeOffSlower switch & decrease TAKE OFF stepper speed if required
        takeOffSlowerSwitch = digitalRead (takeOffSlower_PIN);
        if (unpressedSwitchState) takeOffSlowerSwitch = ~takeOffSlowerSwitch & B00000001;

        // Just pressed?
        if(takeOffSlowerSwitch && !takeOffSlowerSwitchPrev){
            // so now set speed to temp slower speed to get idle arm back into working pos & release switch quicker
//sPrintFS_Switches("3:");
LED_fs();
//delay(2000);
            #ifdef USE_TAKEOFF_STEPPER
            takeOffStepper.setSpeed(0); //tosSwitchActivatedSpeed);
            #endif // USE_TAKEOFF_STEPPER
        }

        // Just released?
        if(!takeOffSlowerSwitch && takeOffSlowerSwitchPrev){
            // so NOW set the new takeOffStepper speed
            takeOffStepperSpeed -= takeOffStepperSpeedInc;
            if (takeOffStepperSpeed < 0 ) takeOffStepperSpeed = 0;
//sPrintFS_Switches("4:");
LED_fs();
//delay(2000);
            #ifdef USE_TAKEOFF_STEPPER
              takeOffStepper.setSpeed(TAKEOFF_SPEED_CALCULATION);
            #endif // USE_TAKEOFF_STEPPER
        }
        takeOffSlowerSwitchPrev = takeOffSlowerSwitch;


//sPrintStatus("actionOtherSwitches() END emerg chck: ");        // might NOT get SEEN in serial term if sent by Teensy3!!!!
//delay(2000);
        //BOTH faster AND slower pressed at same time - do emergency stop
        if(takeOffFasterSwitch && takeOffSlowerSwitch){
            doEmergencyStop();
        }
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
// Debug - Info functions
//////////////////////////////////////////////////////////////////////////////////////////////////////

void LED_fs(){
   DBG_LED5(takeOffFasterSwitch);
   DBG_LED6(takeOffFasterSwitchPrev);
   DBG_LED7(takeOffSlowerSwitch);
   DBG_LED8(takeOffSlowerSwitchPrev);
}

void sPrintStatus(const char *someString){
    DBG_PRINT(someString);
    DBG_LED1(false);
    DBG_LED2(false);
    //DBG_LED3(false);
    switch(currentState)
      {
      case STATE_EMERGENCY_STOP:
        DBG_PRINT_F("STATE_EMERGENCY_STOP:- ");
        DBG_LED1(true);
        break;
      case STATE_STOPPED:
        DBG_PRINT_F("STATE_STOPPED:- ");
        DBG_LED2(true);
        break;
      case STATE_MOVING:
        DBG_PRINT_F("STATE_MOVING:- ");
        //DBG_LED3(true);  doing IN moving code - to indicate actual frame movement
       break;
/*
      case STATE_TAKING_PHOTOS:
        DBG_PRINT_F("STATE_TAKING_PHOTOS:- ");
       break;
      case STATE_IDLE:
        DBG_PRINT_F("STATE_IDLE:- ");
       break;
*/
       default:
        DBG_PRINT_F("unknown state!!!! :- ");
        break;
      }
        DBG_PRINT_F("buttons LAST & PREVIOUS PRESSES {fwd, rev, fast, slow, jog, JOG} ");
        DBG_PRINT2(buttonsCurrent, BIN);
        DBG_PRINT_F(", ");
        DBG_PRINTLN2(buttonsPrevious, BIN);

        DBG_PRINT_F("takeOffFaster/SlowerSwitch(es) & prev ");
        DBG_PRINT2(takeOffFasterSwitch, BIN);
        DBG_PRINT_F(", ");
        DBG_PRINT2(takeOffSlowerSwitch, BIN);
        DBG_PRINT_F(", ");
        DBG_PRINT2(takeOffFasterSwitchPrev, BIN);
        DBG_PRINT_F(", ");
        DBG_PRINTLN2(takeOffSlowerSwitchPrev, BIN);

        #ifdef USE_FEEDIN_STEPPER
            DBG_PRINT_F("{CALC, speed}, {pos, tgt, togo, speed}: {");
            DBG_PRINT(FEEDIN_MOVE_CALCULATION);
            DBG_PRINT_F(", ");
            DBG_PRINT(feedMotorSpeed);
            DBG_PRINT_F("}, ");
            DBG_PRINT(feedInStepper.currentPosition());
            DBG_PRINT_F(", ");
            DBG_PRINT(feedInStepper.targetPosition());
            DBG_PRINT_F(", ");
            DBG_PRINT(feedInStepper.distanceToGo());
            DBG_PRINT_F(", ");
            DBG_PRINTLN(feedInStepper.speed());

            DBG_PRINT_F("fwdDir, dirCtl, stepsPerFrame, microStepRatio:");
            DBG_PRINT(fwdDir);
            DBG_PRINT_F(", ");
            DBG_PRINT(dirCtl);
            DBG_PRINT_F(", ");
            DBG_PRINT(stepsPerFrame);
            DBG_PRINT_F(", ");
            DBG_PRINTLN(microStepRatio);

        #endif // USE_FEEDIN_STEPPER

        #ifdef USE_TAKEOFF_STEPPER
            DBG_PRINT_F("{speed, TAKEOFF_SPEED_CALCULATION}} ");
            DBG_PRINT(takeOffStepperSpeed);
            DBG_PRINT_F(", ");
            DBG_PRINT(TAKEOFF_SPEED_CALCULATION);
        #endif // USE_TAKEOFF_STEPPER

//delay(200);
//      DBG_PRINT(digitalRead(takeOff_EMERGENCY_STOP_PIN));

      DBG_PRINTLN();
}

void sPrintFS_Switches(const char *someString){
    DBG_PRINT(someString);
        DBG_PRINT_F("takeOffFaster/SlowerSwitch(es) & prev ");
        DBG_PRINT2(takeOffFasterSwitch, BIN);
        DBG_PRINT_F(", ");
        DBG_PRINT2(takeOffFasterSwitchPrev, BIN);
        DBG_PRINT_F(", ");
        DBG_PRINT2(takeOffSlowerSwitch, BIN);
        DBG_PRINT_F(", ");
        DBG_PRINTLN2(takeOffSlowerSwitchPrev, BIN);
//delay(200);
}

/* simple time display functions */
void printElapsedTimeDigits(){
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


