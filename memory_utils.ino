


#ifdef MEMRY1
// ------------------------------------------------------------------------------
//http://jeelabs.org/2011/05/22/atmega-MEMRY-use/
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
// ------------------------------------------------------------------------------

char buffer[50];    // make sure this is large enough for the largest string it must hold ** increasing this uses PRECIOUS RAM!!!!!!

void showFreeRam(){
  Serial.println ();
  Serial.println ();
  Serial.print(F("free SRAM is "));
  if (freeRam() > 200){
    Serial.println(freeRam());
  }
  else if (freeRam() >= 100){
    Serial.println(F("100:200 - may crash"));
  }
  else if (freeRam() < 100){
    Serial.println(F("<100 Prog HALTED!!!"));
    crashed:
    goto crashed;
  }
}
#endif

#ifdef MEMRY2

char buffer[50];    // make sure this is large enough for the largest string it must hold ** increasing this uses PRECIOUS RAM!!!!!!

// done this way - so can easily use this code OUTSIDE of this robot code.. should "just work"!!!
#ifndef __PGMSPACE_H_
#include <avr/pgmspace.h>
#endif

// limit length of strings  0123456789 123456789 123456789 123456789 123456789
  prog_char MEMRYDetailmsg0[] PROGMEM = "\n\n--------------------------------------------";
  prog_char MEMRYDetailmsg1[] PROGMEM = "\n\nMEMRY BEGIN: get_free_MEMRY() reports [";
  prog_char MEMRYDetailmsg2[] PROGMEM = "] (bytes) which must be > 0 for no heap/stack collision";
  prog_char MEMRYDetailmsg3[] PROGMEM = "\n\nSP should always be larger than HP or you'll be in big trouble!";
  prog_char MEMRYDetailmsg4[] PROGMEM = "\nheapptr=[0x";
  prog_char MEMRYDetailmsg5[] PROGMEM = "] (growing upward, ";
  prog_char MEMRYDetailmsg6[] PROGMEM = " decimal)";
  prog_char MEMRYDetailmsg7[] PROGMEM = "\nstackptr=[0x";
  prog_char MEMRYDetailmsg8[] PROGMEM = "] (growing downward, ";
  prog_char MEMRYDetailmsg9[] PROGMEM = "\ndifference should be positive: diff=stackptr-heapptr, diff=[0x";
  prog_char MEMRYDetailmsg10[] PROGMEM = "] (which is [";
  prog_char MEMRYDetailmsg11[] PROGMEM = "] (bytes decimal)";
  prog_char MEMRYDetailmsg12[] PROGMEM = "\n\nMEMRY END: get_free_MEMRY() reports [";
  prog_char MEMRYDetailmsg13[] PROGMEM = "] (bytes) which must be > 0 for no heap/stack collision";
  prog_char MEMRYDetailmsg14[] PROGMEM = "\n\n__data_start=[0x";
  prog_char MEMRYDetailmsg15[] PROGMEM = "\n__data_end=[0x";
  prog_char MEMRYDetailmsg16[] PROGMEM = "\n__bss_start=[0x";
  prog_char MEMRYDetailmsg17[] PROGMEM = "\n__bss_end=[0x";
  prog_char MEMRYDetailmsg18[] PROGMEM = "\n__heap_start=[0x";
  prog_char MEMRYDetailmsg19[] PROGMEM = "\n__malloc_heap_start=[0x";
  prog_char MEMRYDetailmsg20[] PROGMEM = "\n__malloc_margin=[0x";
  prog_char MEMRYDetailmsg21[] PROGMEM = "\n__brkval=[0x";
  prog_char MEMRYDetailmsg22[] PROGMEM = "\nSP=[0x";
  prog_char MEMRYDetailmsg23[] PROGMEM = "\nRAMEND=[0x";
  prog_char MEMRYDetailmsg24[] PROGMEM = "\n--- section size summaries ---";
  prog_char MEMRYDetailmsg25[] PROGMEM = "\nram   size=[";
  prog_char MEMRYDetailmsg26[] PROGMEM = "\n.data size=[";
  prog_char MEMRYDetailmsg27[] PROGMEM = "\n.bss  size=[";
  prog_char MEMRYDetailmsg28[] PROGMEM = "\nheap  size=[";
  prog_char MEMRYDetailmsg29[] PROGMEM = "\nstack size=[";
  prog_char MEMRYDetailmsg30[] PROGMEM = "\nfree size1=[";
  prog_char MEMRYDetailmsg31[] PROGMEM = "\nfree size2=[";
// limit length of strings   0123456789 123456789 123456789 123456789 123456789

PROGMEM const char *MEMRYDetail_ProgMem[] = 	   // pointers to the messages in PROGMEM
{
  MEMRYDetailmsg0,
  MEMRYDetailmsg1,
  MEMRYDetailmsg2,
  MEMRYDetailmsg3,
  MEMRYDetailmsg4,
  MEMRYDetailmsg5,
  MEMRYDetailmsg6,
  MEMRYDetailmsg7,
  MEMRYDetailmsg8,
  MEMRYDetailmsg9,
  MEMRYDetailmsg10,
  MEMRYDetailmsg11,
  MEMRYDetailmsg12,
  MEMRYDetailmsg13,
  MEMRYDetailmsg14,
  MEMRYDetailmsg15,
  MEMRYDetailmsg16,
  MEMRYDetailmsg17,
  MEMRYDetailmsg18,
  MEMRYDetailmsg19,
  MEMRYDetailmsg20,
  MEMRYDetailmsg21,
  MEMRYDetailmsg22,
  MEMRYDetailmsg23,
  MEMRYDetailmsg24,
  MEMRYDetailmsg25,
  MEMRYDetailmsg26,
  MEMRYDetailmsg27,
  MEMRYDetailmsg28,
  MEMRYDetailmsg29,
  MEMRYDetailmsg30,
  MEMRYDetailmsg31
};

//WIERD - this block higher up just below #define MEMRY etc - and will NOT compile!!!!!!

//http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1213583720;start=all
/*
 * Cut-and-pasted from www.arduino.cc playground section for determining heap and stack pointer locations.
 * http://www.arduino.cc/playground/Code/AvailableMEMRY
 *
 * Also taken from the Pololu thread from Paul at: http://forum.pololu.com/viewtopic.php?f=10&t=989&view=unread#p4218
 *
 * Reference figure of AVR MEMRY areas .data, .bss, heap (all growing upwards), then stack growing downward:
 * http://www.nongnu.org/avr-libc/user-manual/malloc.html
 *
 */

extern unsigned int __data_start;
extern unsigned int __data_end;
extern unsigned int __bss_start;
extern unsigned int __bss_end;
extern unsigned int __heap_start;
//extern void *__malloc_heap_start; --> apparently already declared as char*
//extern void *__malloc_margin; --> apparently already declared as a size_t
extern void *__brkval;
// RAMEND and SP seem to be available without declaration here

int16_t ramSize=0;   // total amount of ram available for partitioning
int16_t dataSize=0;  // partition size for .data section
int16_t bssSize=0;   // partition size for .bss section
int16_t heapSize=0;  // partition size for current snapshot of the heap section
int16_t stackSize=0; // partition size for current snapshot of the stack section
int16_t freeMem1=0;  // available ram calculation #1
int16_t freeMem2=0;  // available ram calculation #2

uint8_t *heapptr, *stackptr;
uint16_t diff=0;


/* This function places the current value of the heap and stack pointers in the
 * variables. You can call it from any place in your code and save the data for
 * outputting or displaying later. This allows you to check at different parts of
 * your program flow.
 * The stack pointer starts at the top of RAM and grows downwards. The heap pointer
 * starts just above the static variables etc. and grows upwards. SP should always
 * be larger than HP or you'll be in big trouble! The smaller the gap, the more
 * careful you need to be. Julian Gall 6-Feb-2009.
 */
void check_mem() {
  stackptr = (uint8_t *)malloc(4);          // use stackptr temporarily
  heapptr = stackptr;                     // save value of heap pointer
  free(stackptr);      // free up the MEMRY again (sets stackptr to 0)
  stackptr =  (uint8_t *)(SP);           // save value of stack pointer
}


/* Stack and heap MEMRY collision detector from: http://forum.pololu.com/viewtopic.php?f=10&t=989&view=unread#p4218
 * (found this link and good discussion from: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1213583720%3Bstart=all )
 * The idea is that you need to subtract your current stack pointer (conveniently given by the address of a local variable)
 * from a pointer to the top of the static variable MEMRY (__bss_end). If malloc() is being used, the top of the heap
 * (__brkval) needs to be used instead. In a simple test, this function seemed to do the job, showing MEMRY gradually
 * being used up until, with around 29 bytes free, the program started behaving erratically.
 */
//extern int __bss_end;
//extern void *__brkval;

int get_free_MEMRY()
{
  int free_MEMRY;

  if((int)__brkval == 0)
     free_MEMRY = ((int)&free_MEMRY) - ((int)&__bss_end);
  else
    free_MEMRY = ((int)&free_MEMRY) - ((int)__brkval);

  return free_MEMRY;
}


void  MEMRYDetail(){
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[0]))); Serial.print(buffer);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[1]))); Serial.print(buffer);
  Serial.print( get_free_MEMRY() );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[2]))); Serial.print(buffer);


  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[3]))); Serial.print(buffer);

  check_mem();

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[4]))); Serial.print(buffer);
  Serial.print( (int) heapptr, HEX);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[5]))); Serial.print(buffer);
  Serial.print( (int) heapptr, DEC);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[6]))); Serial.print(buffer);

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[7]))); Serial.print(buffer);
  Serial.print( (int) stackptr, HEX);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[8]))); Serial.print(buffer);
  Serial.print( (int) stackptr, DEC);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[6]))); Serial.print(buffer);

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[7]))); Serial.print(buffer);
  diff=stackptr-heapptr;
  Serial.print( (int) diff, HEX);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[8]))); Serial.print(buffer);
  Serial.print( (int) diff, DEC);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[9]))); Serial.print(buffer);


  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[10]))); Serial.print(buffer);     //guessed msg#
  Serial.print( get_free_MEMRY() );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);     //guessed msg#

  // ---------------- Print MEMRY profile -----------------
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[14]))); Serial.print(buffer);
  Serial.print( (int) &__data_start, HEX );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[10]))); Serial.print(buffer);
  Serial.print( (int) &__data_start, DEC);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[15]))); Serial.print(buffer);
  Serial.print((int) &__data_end, HEX );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[10]))); Serial.print(buffer);
  Serial.print( (int) &__data_end, DEC);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[16]))); Serial.print(buffer);
  Serial.print((int) & __bss_start, HEX );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[10]))); Serial.print(buffer);
  Serial.print( (int) &__bss_start, DEC);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[17]))); Serial.print(buffer);
  Serial.print( (int) &__bss_end, HEX );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[10]))); Serial.print(buffer);
  Serial.print( (int) &__bss_end, DEC);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[18]))); Serial.print(buffer);
  Serial.print( (int) &__heap_start, HEX );
   strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[10]))); Serial.print(buffer);
   Serial.print( (int) &__heap_start, DEC);
   strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[19]))); Serial.print(buffer);
  Serial.print( (int) __malloc_heap_start, HEX );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[10]))); Serial.print(buffer);

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[20]))); Serial.print(buffer);
  Serial.print( (int) &__malloc_margin, HEX );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[10]))); Serial.print(buffer);
  Serial.print( (int) &__malloc_margin, DEC);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[21]))); Serial.print(buffer);
  Serial.print( (int) __brkval, HEX );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[10]))); Serial.print(buffer);
  Serial.print( (int) __brkval, DEC);
   strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[22]))); Serial.print(buffer);
  Serial.print( (int) SP, HEX );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[10]))); Serial.print(buffer);
  Serial.print( (int) SP, DEC);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[22]))); Serial.print(buffer);
  Serial.print( (int) RAMEND, HEX );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[10]))); Serial.print(buffer);
  Serial.print( (int) RAMEND, DEC);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);

  // summaries:
  ramSize   = (int) RAMEND       - (int) &__data_start;
  dataSize  = (int) &__data_end  - (int) &__data_start;
  bssSize   = (int) &__bss_end   - (int) &__bss_start;
  heapSize  = (int) __brkval     - (int) &__heap_start;
  stackSize = (int) RAMEND       - (int) SP;
  freeMem1  = (int) SP           - (int) __brkval;
  freeMem2  = ramSize - stackSize - heapSize - bssSize - dataSize;

  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[24]))); Serial.print(buffer);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[25]))); Serial.print(buffer);
  Serial.print( ramSize, DEC );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[26]))); Serial.print(buffer);
  Serial.print( dataSize, DEC );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[27]))); Serial.print(buffer);
  Serial.print( bssSize, DEC );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[28]))); Serial.print(buffer);
  Serial.print( heapSize, DEC );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[29]))); Serial.print(buffer);
  Serial.print( stackSize, DEC );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[30]))); Serial.print(buffer);
  Serial.print( freeMem1, DEC );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[31]))); Serial.print(buffer);
  Serial.print( freeMem2, DEC );
  strcpy_P(buffer, (char*)pgm_read_word(&(MEMRYDetail_ProgMem[11]))); Serial.print(buffer);
  Serial.println();
  Serial.println();
}
 #endif

