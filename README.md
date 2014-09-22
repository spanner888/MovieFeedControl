MovieFeedControl Summary:
  This code controls two stepper motors for film feed and take-off.
  Sendkeys is used to send keystrokes to computer application to trigger frame capture via chosen camera - application.
  So you need an Arduino Leonardo or Teensy. Currently only tested on Teensy3, minor changes required for Leonardo.
  
  The take-off stepper - film reel is controlled by two limit switches
  so that the take off speed is adjusted automatically during loooong processing
  sessions. This is necessary as the take of efective reel diameter changes as
  more film winds on.
  
  The are buttons for user control (jog, bigJog, Frame{repeat until stopped}, Stop, ....}
  and switches for takeOff reel control, emergency stop , .......
  
  Note any serial terminal logging will slow down capture speed. 
  A ToDo is to only write to serial during the wait for film to stop or photo to transfer to PC waits.
  
  Code is split into seperate files (tabs in Arduino GUI).
  
Status - working prototype.
  Code was written to be "good & polished as is" ... and it is not too bad, 
  however I create chaos everywhere and overcomplicate things,
  so feel free to tidy up.
  
  There are heaps of  TODOs throughout for refinements and features to be added.....
  
  There is also a lot of debug code that can be removed, mostly in the #def's


Acknowledgements of other peoples code are in place throughout the code.
Refer to original source for relevant licencing

MovieFeedControl by Spanner888 at {U}sable{D}evices is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
Permissions beyond the scope of this license may be available via http://usabledevices.com/2011/11/contact-emails-and-legal-and-copyright-information/.

Blog of hardware build and other Telecine info can be found at http://usabledevices.com/category/telecine/
