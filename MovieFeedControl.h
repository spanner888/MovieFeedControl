// See readme.md for overview & licence info.


#ifndef MFC_HEADER
#define MFC_HEADER

#define SERIAL_SPEED 115200
// IO Pin definitions.
// leave Pins 0,1 for serial coms

// Had trouble with direct port read and Teensy and switches/buttons used. (Worked OK on Duemilanove!)
// SO AVOID USING ANALOG ports for switches/buttons with Teensy & this code :/

#define takeOffStep_PIN 2
#define feedInStep_PIN 3
#define stepperDir_PIN 4    // SHARING dir pins - this means the last stepper to set dir, does so for BOTH steppers!!!!
                                /// ????? is this ie ABOVE really true???
#define microStep_PIN 5     // SHARING ALL 2x3 microStep pins

#ifdef SWITCHES                     // switches triggered by mechanical actions - NOT by user!

  #define takeOffFaster_PIN 6
  #define takeOffSlower_PIN 7

  #define EMERGENCY_STOP_PIN 8    // opto detector DOES NOT work on pin 13
  //#define feedInFaster_PIN 10
  //#define feedInSlower_PIN 11

  #define EMERGENCY_STOP_MASK B00000001

  const boolean unpressedSwitchState = 1;   // Set unpressedswitchState according to your switch UNPRESSED default value!
  unsigned long switchLastReadTime = 0;
  int switchDebounceDelay   = 50;           // milliseconds to wait BEFORE trying to read switches again!

  byte takeOffFasterSwitch = 0;
  byte takeOffSlowerSwitch = 0;

  byte takeOffFasterSwitchPrev = 1;
  byte takeOffSlowerSwitchPrev = 1;
#endif

// NOTE: these need to be contiguous in ONE port -so the "fast" read & debounce code works !!
// changes to port need to be updated in the code! Analogue port = Port C.
// **ALSO - ALL button pin#s are HARD CODED in the code for now!
#ifdef BUTTONS                      // buttons = user presses these!
  // PortB = pins 8-13, PortD = 0-7, C = A0-a5 or more on Teensys & other Arduinos...
  #define buttonIO_MASK = B00000001               // 1 is output(or is &=, then no change!), 0 is input
  #define buttonPullup_MASK = B11111110           // enable = 1, disable=0 input pullups
    // *** >>> NOTE - could not get the above to work in the code - so you need manually edit values in readButtons()

  #define BUTTON_DDR DDRB
  #define BUTTON_PORT PORTB
  #define BUTTON_PINS PINB

  #define fwdFrameButton_MASK B00000010     // pin 9
  #define jogButton_MASK      B00000100     // pin 10
  #define bigJogButton_MASK   B00001000     // pin 11

//  #define revFrameButton_MASK B00000010
//  #define fasterButton_MASK   B00000100
//  #define slowerButton_MASK   B00001000
//if need MORE buttons, have to use COMBINATION presses - ie press 2+ buttons!!!
// OR SEND CMDS FROM PC VIA SERIAL!!!!!!
  #define stopButton_MASK B00000110                   // Press both buttons stops taking moving AFTER current movement has finished!
  #define takePhoto-StopButton_MASK B00001100         // Alternate presses of both buttons starts/stops taking photos

  const boolean unpressedButtonState = 1;           // Set unpressedButtonState according to your button UNPRESSED default value!

  byte buttonsCurrent =  B00000000;
  byte buttonsPrevious = B00000000;
  unsigned long buttonLastReadTime = 0;
  int buttonDebounceDelay   = 50;        // milliseconds to wait BEFORE trying to read buttons again!
#endif

#ifdef BUTTONS_INPUT                      // buttons = user presses these!
  #define fwdFrameButton_PIN 9
  //#define revFrameButton_PIN 10
  #define jogButton_PIN 10
  #define bigJogButton_PIN 11

  #define fwdFrameButton_MASK B00000010     // pin 9
  #define jogButton_MASK      B00000100     // pin 10
  #define bigJogButton_MASK   B00001000     // pin 11

//  #define revFrameButton_MASK B00000010
//  #define fasterButton_MASK   B00000100
//  #define slowerButton_MASK   B00001000
//if need MORE buttons, have to use COMBINATION presses - ie press 2+ buttons!!!
// OR SEND CMDS FROM PC VIA SERIAL!!!!!!
  #define stopButton_MASK B00000110                   // Press both buttons stops taking moving AFTER current movement has finished!
  #define takePhoto-StopButton_MASK B00001100         // Alternate presses of both buttons starts/stops taking photos

//if need MORE buttons, have to use COMBINATION presses - ie press 2+ buttons!!!
// OR SEND CMDS FROM PC VIA SERIAL!!!!!!

  const boolean unpressedButtonState = 1;           // Set unpressedButtonState according to your button UNPRESSED default value!

  byte buttonsCurrent =  B00000000;
  byte buttonsPrevious = B00000000;
  unsigned long buttonLastReadTime = 0;
  int buttonDebounceDelay   = 50;        // milliseconds to wait BEFORE trying to read buttons again!
#endif

// END: IO Pin definitions.

// 'Easy' debug **COMMANDS** & serial messages
// from http://comments.gmane.org/gmane.comp.hardware.arduino.devel/1340
// Extended to handle strings and numbers with/out new line & 2nd param to specify output number base (BIN, OCT etc)
#ifdef DEBUG
    #define DBG(x) x
    #define DBG_PRINT_F(x) Serial.print(F(x))
    #define DBG_PRINTLN_F(x) Serial.println(F(x))
    #define DBG_PRINT(x) Serial.print(x)
    #define DBG_PRINTLN(x) Serial.println(x)

    #define DBG_PRINT2(x, y) Serial.print(x, y)
    #define DBG_PRINTLN2(x, y) Serial.println(x, y)
    #else
    #define DBG(x)
    #define DBG_PRINT_F(x)
    #define DBG_PRINTLN_F(x)
    #define DBG_PRINT(x)
    #define DBG_PRINTLN(x)

    #define DBG_PRINT2(x, y)
    #define DBG_PRINTLN2(x, y)
#endif
/* then use like this:
    DBG(if (var > 10000) Serial.print(F(Strange value for var"));
    or
    DBG_PRINTLN(var);
*/

// easy debug LED code, the !x is because LED board used is active LOW to turn LED on. You can adjust this :)
#ifdef DEBUG_LED
    #define LED1 A0
    #define LED2 A1
    #define LED3 A2
    #define LED4 A3
    #define LED5 A4
    #define LED6 A5
    #define LED7 A6
    #define LED8 A7
    #define DBG_LED1(x) digitalWrite(LED1, !x)
    #define DBG_LED2(x) digitalWrite(LED2, !x)
    #define DBG_LED3(x) digitalWrite(LED3, !x)
    #define DBG_LED4(x) digitalWrite(LED4, !x)
    #define DBG_LED5(x) digitalWrite(LED5, !x)
    #define DBG_LED6(x) digitalWrite(LED6, !x)
    #define DBG_LED7(x) digitalWrite(LED7, !x)
    #define DBG_LED8(x) digitalWrite(LED8, !x)
#else
    #define DBG_LED1(x)
    #define DBG_LED2(x)
    #define DBG_LED3(x)
    #define DBG_LED4(x)
    #define DBG_LED5(x)
    #define DBG_LED6(x)
    #define DBG_LED7(x)
    #define DBG_LED8(x)
#endif // DEBUG_LED

//STATES to help control moving film & stepper motors
#define STATE_EMERGENCY_STOP 0
#define STATE_STOPPED 1
#define STATE_MOVING 2

int currentState = STATE_STOPPED;
int prevState = STATE_STOPPED;


// ************************************************************************************
// *** SETUP stuff (const, vars, calculations) for both steppers **********************
// ************************************************************************************
const int dfltStepperSpeed = 250;
const int numbSprockets = 8;         // number of sprockets on the capstan

// Need to track direction. eg jog states assume move in SAME dir as prev cmd!.
boolean fwdDir;                // true is normal forward direction
int dirCtl;                    // used to control +/- of all step/speed values according to fwdDir;
int microStepRatio = 2;        // 16 if microstepping, 1 if in full step. Can also do 2, 4, 8 if jumpers changed

int stepsPerRevolution = 200 * microStepRatio;  // change this to fit the number of steps per revolution of stepper used

int stepsPerFrame;  // = stepsPerRevolution/numbSprockets;
int stepsPerJog;  //stepsPerFrame/100;          //TODO review if this is realistic!!!!!!
int stepsPerBigJog;  //stepsPerFrame/10;        //TODO review if this is realistic!!!!!!
                             // is +/- stepsPerFrame etc according to direction.

int maxStepperSpeed = 1000 * microStepRatio;            //Speeds of more than 1000 steps per second are unreliable;

#define STEPPERS_FWD    false
#define STEPPERS_REV    true

#ifdef USE_FEEDIN_STEPPER
    //using #def - so only have formula in ONE place!
    #define FEEDIN_MOVE_CALCULATION (stepsPerFrame * microStepRatio * dirCtl)
    AccelStepper feedInStepper(AccelStepper::DRIVER, feedInStep_PIN, stepperDir_PIN);
    int speedInc =  maxStepperSpeed/50;    //Speeds of more than 1000 steps per second are unreliable;
    int feedMotorSpeed = dfltStepperSpeed;
#endif
#ifdef USE_TAKEOFF_STEPPER
    //using #def - so only have formula in ONE place!
    #define TAKEOFF_SPEED_CALCULATION (takeOffStepperSpeed * microStepRatio * dirCtl)
    AccelStepper takeOffStepper(AccelStepper::DRIVER, takeOffStep_PIN, stepperDir_PIN);
    const int maxTakeOffStepperSpeed = 500;
    #ifdef TAKE_PHOTO
        int dfltTakeOffStepperSpeed = 60;   //7;
    #else
        int dfltTakeOffStepperSpeed = 90;   //12;    // dif speed as no delays!
    #endif
    int takeOffStepperSpeed = dfltTakeOffStepperSpeed;
    int takeOffStepperSpeedInc = 1;

    // these are temporary speeds used ONLY while either faster/slower switch activated to:
    //   - get tension arm back into working range quickly
    //   - turn switches OFF quickly and reduce unwatnted faster/slower speed changes
    //   - forget what else
//    const int tofSwitchActivatedSpeed = 15 * dfltTakeOffStepperSpeed;
//    const int tosSwitchActivatedSpeed = 0;
#endif
// ************************************************************************************
// ************************************************************************************
// ************************************************************************************




long numberFrames = 0;
const unsigned long interFrameMovementTime = 1000;
unsigned long nextFeedTime = 0;
boolean nextFrameTriggered = false;

boolean pausingFilm = false;    // controls if film held still while photo of frame taken
int pauseDelay = 10;            // required delay in milliseconds

#endif // MFC_HEADER
