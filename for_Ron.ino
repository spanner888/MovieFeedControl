/*   Mechanical Endstop v1.2
http://www.thingiverse.com/thing:4971

                              o
                            /
        -------------------------------
Signal  | -                             |
Gnd     | -                             |
Gnd     | -                             |
Vcc     | -                             |
        --------------------------------
*/


/* TODO

  also takeOff stepper STILL whines when NOT moving!

  BUG: - sometimes when push stop - it stops BUT not moving light stays OFF!!!!
  - output still says moving ... is this just not reporting or part of teh bug
    code still running OK - can pres fwdFrame & goes OK

  BUG - stop - tricky to get button press timing correct - just increase that delay???

    .... items still in testing below - serial coms => loop speed & stepper ctl....

 review need for stepper.stop() - .run will not move if at desired position!
 so only need for emergency stop!


*** in emergency stop - can hear takeOff stepper whining AND both steppers are still energised!!!!
      ... even after addign disable outputs() in emerg stop code!!!!
??? actually in microstep but speeds are wrong - SO STILL RUNNING!!!


??? is IDLE a state or another var used in state decisions????
eg - if motor stopped stepping & WAITING stepper/film to stop moving before taking photo = IDLE, can do other stuff
but how to control taking of photo????

    - log EVENTS - at least start, stop, emergency, # frames, ??time of each frame
    EVEN ADD ONE SPECIAL BUTTON to create a log event .... eg for scene changes .. so can do post processing etc
    .... thinking for mass, semi/auto processing help track issue and which frame!!!!!
    ... but impact on speed/memory
    ... change to teensy??? Teensy 3 	16k RAM, Teensy 3.1 64k RAM!!!!
        FILMS on hand range from 8,000 to 48,000 FRAMES
            Note capture times at 1 second / frame
                8000 frame = 8.2 hours
                48,000 frame = 13.3 hours
                ... and with USB camera, may NOT GET FASTER THAN ONE PER SECOND!!!!!
        ... but of course can stop part way through on scene changes etc
    Teensy can also send keyboard keys!!!!!


 - add sensor to detect frame holes
 + code to detect frame start/end .... in SETUP code!!!!!!
 - then code to align to frame
 ...
 exposed film does NOT block light!!!!
 ** try a metal plate/slit with hole ~ film sprocket hole size.....

 ??should code be changed to use interrput frame detection / control??
 ... or just check if "about" right every frame advance?????


**********************************************************************************************************
** Takeoff belt - jumping/slips - stepper is slipping - not eh belt due high load.
** Fixed - by incresing stepper drive current - tweaked to adjustment pot on the A988 controller
**********************************************************************************************************
*/




/* direct pin/port IO reads - my code & Teensy AND OTHER TEENSY THINGS TO NOTE:

    - if still get issues - then do stepper,, button & switch code in std Ard and let it trigger
        a Teensy/Leonardo/Freaduino to send keys to take photo.
        working example code in MFCStepperOnly & MFCPhotoOnly sketches.

    - For Teensy to OUPUT serial - you MUST select one option that includes serial from the Arduino GUI tools menu - USB Type!!!!!
    - ditto for sending keyboard keys
    - and for both - both!!!!

    ====== Teensy3 ========
    - Teensy3 clock speed - was overclocked at 96MHz - reduced to 48MHz!!!
    ... but always gets reset to 96MHz!


    - Teensy3 serial behavior is different. See https://www.pjrc.com/teensy/td_serial.html
    => can MISS once only serial from setup!!!!
    => different behavior/fussiness trying to get com port to appear in Arduino GUI menu, so can select it and get serial terminal to work!
    => com port might have different name/series name. eg on several Linux O/s that I often use, com ports normally appear as /dev/ttyUSBnn
            but Teensy3 appears as /dev/ttyACMnn
    ====== Teensy3 ========

    ====== Teensy2 ========
        .. also (more?) fussy about com port....
    ====== Teensy2 ========
*/



