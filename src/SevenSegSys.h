#ifndef SEVENSEGSYS_H
#define SEVENSEGSYS_H

#include "Arduino.h"
#include "../userdefs.h"

// Segment map special characters
#define SEVENSEG_BIGC         37
#define SEVENSEG_BIGH         38
#define SEVENSEG_BIGI         39
#define SEVENSEG_BIGO         40
#define SEVENSEG_BIGUV        41
#define SEVENSEG_BIGG         42
// Character special values
#define SEVENSEG_NOCHAR       0x7f
#define SEVENSEG_SPACE        0x7e
#define SEVENSEG_NEWLINE      0x7d
#define SEVENSEG_CHARDOT      0x7c
// Demultiplexer and shift register pin indices
#define SEVENSEG_DATA         0
#define SEVENSEG_LATCH        1
#define SEVENSEG_CLOCK        2
#define SEVENSEG_A0           0
#define SEVENSEG_A1           1
#define SEVENSEG_A2           2
#define SEVENSEG_CS           3
// Control word bits
#define SEVENSEG_TIMERRUNNING 0x10
#define SEVENSEG_CHILD        0x08
#define SEVENSEG_DEMUX2       0x04
#define SEVENSEG_DEMUX        0x02
#define SEVENSEG_SHIFT        0x01
// Special values for the constructors to pass to the helper
#define SEVENSEG_NODEMUXSHIFT 0
#define SEVENSEG_NOSPLIT      0
#define SEVENSEG_TIMER1       1


// Segment map. Stored in program memory
const uint8_t PROGMEM _segs[43] = {
  B1111110,
  B0110000,
  B1101101,
  B1111001,
  B0110011,
  B1011011,
  B1011111,
  B1110000,
  B1111111,
#ifndef SEVENSEG_9TAIL
  B1110011,
#else
  B1111011,
#endif
  B0000001,
  B1110111,           // A
  B0011111,           // b
  B0001101,           // c
  B0111101,           // d
  B1001111,           // E
  B1000111,           // F
  B1111011,           // g
  B0010111,           // h
  B0010000,           // i
  B0111000,           // J 
  B0000000,           // No K
  B0001110,           // L
  B0000000,           // No M
  B0010101,           // m
  B0011101,           // o
  B1100111,           // P 
  B1110011,           // q
  B0000101,           // r
  B1011011,           // S
  B0001111,           // t
  B0011100,           // u
  B0011100,           // v
  B0000000,           // No W 
  B0000000,           // No X
  B0110011,           // Y
  B0000000,           // No Z
// Free extras - Uppercase versions of letters that can be displayed both ways
  B1001110,           // C
  B0110111,           // H
  B0110000,           // I
  B1111110,           // O
  B0111110,           // U/V
  B1011110            // G
};

#endif
