#include <TimerOne.h>
#ifdef SEVENSEG_TIMER2
#include <TimerTwo.h>
#endif
#include <TimerThree.h>
#ifdef SEVENSEG_TIMER4
#include <TimerFour.h>
#endif
#ifdef SEVENSEG_TIMER5
#include <TimerFive.h>
#endif
#include "SevenSegInt.h"
#include "SevenSegSys.h"
#include <Arduino.h>

#define _segmentOn (!_digitOn)
#define SEGOFF (_segmentOn ? HIGH : LOW)
#define SEGON (_segmentOn ? LOW : HIGH)
#define SEGD(d, s) (digitalWrite(_segmentPins[(s)], pgm_read_byte(&_segs[(d)]) & (1 << (6 - (s))) ? SEGON : SEGOFF))
#define SHIFTSEGD(d) ((_segmentOn ? pgm_read_byte(&_segs[(d)]) : ~pgm_read_byte(&_segs[(d)])) << 1)
#define DOTON (digitalWrite(_segmentPins[7], SEGON))
#define SEGBLANK(s) (digitalWrite(_segmentPins[(s)], SEGOFF))

//
// Array of pointers to the 8 (max) SevenSegInt instances, used by the interrupt
// routine to select the code to run in each instance.
//
SevenSegInt *_instance[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

//
// Interrupt routine. Must exist out of the class, 'cos it's an interrupt routine :).
//
void _segTimerInt1() {
  for(uint8_t i = 0; i < 8; i++) {
    if(_instance[i] != NULL && _instance[i]->Timer == 1) {
      _instance[i]->SevenSegIntdrive();
    }
  }
}

#ifdef SEVENSEG_TIMER2
void _segTimerInt2() {
  for(uint8_t i = 0; i < 8; i++) {
    if(_instance[i] != NULL && _instance[i]->Timer == 2) {
      _instance[i]->SevenSegIntdrive();
    }
  }
}
#endif

void _segTimerInt3() {
  for(uint8_t i = 0; i < 8; i++) {
    if(_instance[i] != NULL && _instance[i]->Timer == 3) {
      _instance[i]->SevenSegIntdrive();
    }
  }
}

#ifdef SEVENSEG_TIMER4
void _segTimerInt4() {
  for(uint8_t i = 0; i < 8; i++) {
    if(_instance[i] != NULL && _instance[i]->Timer == 4) {
      _instance[i]->SevenSegIntdrive();
    }
  }
}
#endif

#ifdef SEVENSEG_TIMER5
void _segTimerInt5() {
  for(uint8_t i = 0; i < 8; i++) {
    if(_instance[i] != NULL && _instance[i]->Timer == 5) {
      _instance[i]->SevenSegIntdrive();
    }
  }
}
#endif

//
// Constructors
//
// 1
SevenSegInt::SevenSegInt(const uint8_t *segmentPins, const uint8_t *commonPins, bool common, uint8_t digits) {
  _sevenseg(segmentPins, commonPins, common, digits, SEVENSEG_NODEMUXSHIFT, SEVENSEG_NOSPLIT, SEVENSEG_TIMER1);
}

// 2
SevenSegInt::SevenSegInt(const uint8_t *segmentPins, uint8_t a0, uint8_t a1, uint8_t a2, bool common, uint8_t digits) {
  uint8_t _d[3];
  _d[SEVENSEG_A0] = a0;
  _d[SEVENSEG_A1] = a1;
  _d[SEVENSEG_A2] = a2;
  _sevenseg(segmentPins, _d, common, digits, SEVENSEG_DEMUX, SEVENSEG_NOSPLIT, SEVENSEG_TIMER1);
}

// 3
SevenSegInt::SevenSegInt(const uint8_t *segmentPins, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t demuxSelect, \
                   bool common, uint8_t digits) {
  uint8_t _d[4];
  _d[SEVENSEG_A0] = a0;
  _d[SEVENSEG_A1] = a1;
  _d[SEVENSEG_A2] = a2;
  _d[SEVENSEG_CS] = demuxSelect;
  _sevenseg(segmentPins, _d, common, digits, SEVENSEG_DEMUX2, SEVENSEG_NOSPLIT, SEVENSEG_TIMER1);
}

// 4
SevenSegInt::SevenSegInt(const uint8_t *segmentPins, const uint8_t *commonPins, bool common, uint8_t digits, \
                   const char *timer) {
  _sevenseg(segmentPins, commonPins, common, digits, SEVENSEG_NODEMUXSHIFT, SEVENSEG_NOSPLIT, *timer - '0');
}

// 5
SevenSegInt::SevenSegInt(const uint8_t *segmentPins, uint8_t a0, uint8_t a1, uint8_t a2, bool common, \
                   uint8_t digits, const char *timer) {
  uint8_t _d[3];
  _d[SEVENSEG_A0] = a0;
  _d[SEVENSEG_A1] = a1;
  _d[SEVENSEG_A2] = a2;
  _sevenseg(segmentPins, _d, common, digits, SEVENSEG_DEMUX, SEVENSEG_NOSPLIT, *timer - '0');
}

// 6
SevenSegInt::SevenSegInt(const uint8_t *segmentPins, uint8_t a0, uint8_t a1, uint8_t a2, \
                   uint8_t demuxSelect, bool common, uint8_t digits, const char *timer) {
  uint8_t _d[4];
  _d[SEVENSEG_A0] = a0;
  _d[SEVENSEG_A1] = a1;
  _d[SEVENSEG_A2] = a2;
  _d[SEVENSEG_CS] = demuxSelect;
  _sevenseg(segmentPins, _d, common, digits, SEVENSEG_DEMUX2, SEVENSEG_NOSPLIT, *timer - '0');
}

// 7
SevenSegInt::SevenSegInt(const uint8_t *segmentPins, const uint8_t *commonPins, bool common, uint8_t digits, \
                   const char *timer, uint8_t digitsUsed) {
  _sevenseg(segmentPins, commonPins, common, digits, SEVENSEG_NODEMUXSHIFT, digitsUsed, *timer - '0');
}

// 8
SevenSegInt::SevenSegInt(const uint8_t *segmentPins, uint8_t a0, uint8_t a1, uint8_t a2, \
                   bool common, uint8_t digits, const char *timer, uint8_t digitsUsed) {
  uint8_t _d[3];
  _d[SEVENSEG_A0] = a0;
  _d[SEVENSEG_A1] = a1;
  _d[SEVENSEG_A2] = a2;
  _sevenseg(segmentPins, _d, common, digits, SEVENSEG_DEMUX, digitsUsed, *timer - '0');
}

// 9
SevenSegInt::SevenSegInt(const uint8_t *segmentPins, uint8_t a0, uint8_t a1, uint8_t a2, \
                   uint8_t demuxSelect, bool common, uint8_t digits, const char *timer, uint8_t digitsUsed) {
  uint8_t _d[4];
  _d[SEVENSEG_A0] = a0;
  _d[SEVENSEG_A1] = a1;
  _d[SEVENSEG_A2] = a2;
  _d[SEVENSEG_CS] = demuxSelect;
  _sevenseg(segmentPins, _d, common, digits, SEVENSEG_DEMUX2, digitsUsed, *timer - '0');
}

// 10
SevenSegInt::SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, const uint8_t *commonPins, bool common, uint8_t digits) {
  uint8_t _s[3];
  _s[SEVENSEG_DATA] = dpin;
  _s[SEVENSEG_LATCH] = lpin;
  _s[SEVENSEG_CLOCK] = cpin;
  _sevenseg(_s, commonPins, common, digits, SEVENSEG_SHIFT, SEVENSEG_NOSPLIT, SEVENSEG_TIMER1);
}

// 11
SevenSegInt::SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1, uint8_t a2, \
                   bool common, uint8_t digits) {
  uint8_t _s[3];
  uint8_t _d[3];
  _s[SEVENSEG_DATA] = dpin;
  _s[SEVENSEG_LATCH] = lpin;
  _s[SEVENSEG_CLOCK] = cpin;
  _d[SEVENSEG_A0] = a0;
  _d[SEVENSEG_A1] = a1;
  _d[SEVENSEG_A2] = a2;
  _sevenseg(_s, _d, common, digits, SEVENSEG_DEMUX | SEVENSEG_SHIFT, SEVENSEG_NOSPLIT, SEVENSEG_TIMER1);
}

// 12
SevenSegInt::SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1, uint8_t a2, \
                   uint8_t demuxSelect, bool common, uint8_t digits) {
  uint8_t _s[3];
  uint8_t _d[4];
  _s[SEVENSEG_DATA] = dpin;
  _s[SEVENSEG_LATCH] = lpin;
  _s[SEVENSEG_CLOCK] = cpin;
  _d[SEVENSEG_A0] = a0;
  _d[SEVENSEG_A1] = a1;
  _d[SEVENSEG_A2] = a2;
  _d[SEVENSEG_CS] = demuxSelect;
  _sevenseg(_s, _d, common, digits, SEVENSEG_DEMUX2 | SEVENSEG_SHIFT, SEVENSEG_NOSPLIT, SEVENSEG_TIMER1);
}

// 13
SevenSegInt::SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, const uint8_t *commonPins, bool common, uint8_t digits, \
                   const char *timer) {
  uint8_t _s[3];
  _s[SEVENSEG_DATA] = dpin;
  _s[SEVENSEG_LATCH] = lpin;
  _s[SEVENSEG_CLOCK] = cpin;
  _sevenseg(_s, commonPins, common, digits, SEVENSEG_SHIFT, SEVENSEG_NOSPLIT, *timer - '0');
}

// 14
SevenSegInt::SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1, uint8_t a2, \
                   bool common, uint8_t digits, const char *timer) {
  uint8_t _s[3];
  uint8_t _d[3];
  _s[SEVENSEG_DATA] = dpin;
  _s[SEVENSEG_LATCH] = lpin;
  _s[SEVENSEG_CLOCK] = cpin;
  _d[SEVENSEG_A0] = a0;
  _d[SEVENSEG_A1] = a1;
  _d[SEVENSEG_A2] = a2;
  _sevenseg(_s, _d, common, digits, SEVENSEG_DEMUX | SEVENSEG_SHIFT, SEVENSEG_NOSPLIT, *timer - '0');
}

// 15
SevenSegInt::SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1, uint8_t a2, \
                   uint8_t demuxSelect, bool common, uint8_t digits, const char *timer) {
  uint8_t _s[3];
  uint8_t _d[4];
  _s[SEVENSEG_DATA] = dpin;
  _s[SEVENSEG_LATCH] = lpin;
  _s[SEVENSEG_CLOCK] = cpin;
  _d[SEVENSEG_A0] = a0;
  _d[SEVENSEG_A1] = a1;
  _d[SEVENSEG_A2] = a2;
  _d[SEVENSEG_CS] = demuxSelect;
  _sevenseg(_s, _d, common, digits, SEVENSEG_DEMUX2 | SEVENSEG_SHIFT, SEVENSEG_NOSPLIT, *timer - '0');
}

// 16
SevenSegInt::SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, const uint8_t *commonPins, bool common, uint8_t digits, \
                   const char *timer, uint8_t digitsUsed) {
  uint8_t _s[3];
  _s[SEVENSEG_DATA] = dpin;
  _s[SEVENSEG_LATCH] = lpin;
  _s[SEVENSEG_CLOCK] = cpin;
  _sevenseg(_s, commonPins, common, digits, SEVENSEG_NODEMUXSHIFT, digitsUsed, *timer - '0');
}

// 17
SevenSegInt::SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1, uint8_t a2, \
                   bool common, uint8_t digits, const char *timer, uint8_t digitsUsed) {
  uint8_t _s[3];
  uint8_t _d[3];
  _s[SEVENSEG_DATA] = dpin;
  _s[SEVENSEG_LATCH] = lpin;
  _s[SEVENSEG_CLOCK] = cpin;
  _d[SEVENSEG_A0] = a0;
  _d[SEVENSEG_A1] = a1;
  _d[SEVENSEG_A2] = a2;
  _sevenseg(_s, _d, common, digits, SEVENSEG_DEMUX | SEVENSEG_SHIFT, digitsUsed, *timer - '0');
}

// 18
SevenSegInt::SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1, uint8_t a2, \
                   uint8_t demuxSelect, bool common, uint8_t digits, const char *timer, uint8_t digitsUsed) {
  uint8_t _s[3];
  uint8_t _d[4];
  _s[SEVENSEG_DATA] = dpin;
  _s[SEVENSEG_LATCH] = lpin;
  _s[SEVENSEG_CLOCK] = cpin;
  _d[SEVENSEG_A0] = a0;
  _d[SEVENSEG_A1] = a1;
  _d[SEVENSEG_A2] = a2;
  _d[SEVENSEG_CS] = demuxSelect;
  _sevenseg(_s, _d, common, digits, SEVENSEG_DEMUX2 | SEVENSEG_SHIFT, digitsUsed, *timer - '0');
}

// 19
SevenSegInt::SevenSegInt(uint8_t digits) {
  _sevenseg(NULL, NULL, false, 0, SEVENSEG_CHILD, digits, 0);
}

//
// Constructor helper
//
void SevenSegInt::_sevenseg(const uint8_t *segmentPins, const uint8_t *commonPins, bool common, uint8_t digits, \
                         uint8_t hardwareFlags, uint8_t digitsUsed, uint8_t timer) {
  uint8_t t;
  _current = 0;
  memset((void *) _string, SEVENSEG_SPACE, 16);
  Timer = timer;
  _control = hardwareFlags;
  _digitsUsed = digitsUsed;
  _useInstance = NULL;
  _digits = digits > 16 ? 16 : digits;
  // Check for parent or child instance
  if(_digitsUsed) {
    if(_control & SEVENSEG_CHILD) {
      // Child display. Just allocated space in the parent display buffer,
      // point to the right part of it, and bugger off.
      for(uint8_t i = 0; i < 8; i++) {
        if(_instance[i] != NULL && _instance[i]->_digitsRemaining >= _digitsUsed) {
          _useInstance = _instance[i];
          // For a child display, _digits is used as the pointer into the parent buffer.
          _digits = _useInstance->_digits - _useInstance->_digitsRemaining;
          _useInstance->_digitsRemaining -= _digitsUsed;
          return;
        }
      }
    } else {
      _digitsRemaining = _digits - _digitsUsed;
    }
    // Not part of a split display, so all digits are used.
  } else {
    _digitsRemaining = 0;  // REALLY important to zero this, so a child display knows
                           // not to steal space.
    _digitsUsed = digits;
  }

  // Set comman anode or cathode
  if(common == SEVENSEG_COMMONCATHODE) _digitOn = false;
  else _digitOn = true;

  // Set up the common pins.
  if(_control & SEVENSEG_DEMUX2) t = 4;
  else if(_control & SEVENSEG_DEMUX) t = 3;
  else t = _digits;
  for(uint8_t i = 0; i < t; i++) {
    _commonPins[i] = commonPins[i];
    pinMode(_commonPins[i], OUTPUT);
  }

  // Set up the segment pins.
  if(_control & SEVENSEG_SHIFT) t = 3;
  else t = 8;
  for(uint8_t i = 0; i < t; i++) {
    _segmentPins[i] = segmentPins[i];
    pinMode(_segmentPins[i], OUTPUT);
    digitalWrite(_segmentPins[i], SEGOFF);
  }
  // For a display using a segment shift register, set the latch high to stop
  // the display being updated (until needed).
  if(_control & SEVENSEG_SHIFT) {
    digitalWrite(_segmentPins[SEVENSEG_LATCH], HIGH);
  }

  // Put a pointer to this instance in a global array, for the interrupt routines.
  for(uint8_t i = 0; i < 8; i++) {
    if(_instance[i] == NULL) {
      _instance[i] = this;
      break;
    }
  }
}

//
// Functions to control the SevenSegInt instance
//
void SevenSegInt::begin(void) {
  // begin() just starts the timer interrupt, if not already running.
  // Not needed (or wanted!) for a child display.
  if(_control & SEVENSEG_CHILD) return;
  for(uint8_t i = 0; i < 8; i++) {
    // If some other display has already started the timer we need, we're done.
    if(_instance[i] != NULL && _instance[i] != this && _instance[i]->Timer == Timer
       && (_instance[i]->_control & SEVENSEG_TIMERRUNNING)) return;
  }
  switch(Timer) {
    case 1:
      Timer1.attachInterrupt(_segTimerInt1);
      Timer1.initialize(1000);
      _control |= SEVENSEG_TIMERRUNNING;
      break;

#ifdef SEVENSEG_TIMER2
    case 2:
      Timer2.attachInterrupt(_segTimerInt2);
      Timer2.initialize(1000);
      _control |= SEVENSEG_TIMERRUNNING;
      break;
#endif

    case 3:
      Timer3.attachInterrupt(_segTimerInt3);
      Timer3.initialize(1000);
      _control |= SEVENSEG_TIMERRUNNING;
      break;

#ifdef SEVENSEG_TIMER4
    case 4:
      Timer4.attachInterrupt(_segTimerInt4);
      Timer4.initialize(1000);
      _control |= SEVENSEG_TIMERRUNNING;
      break;
#endif

#ifdef SEVENSEG_TIMER5
    case 5:
      Timer5.attachInterrupt(_segTimerInt5);
      Timer5.initialize(1000);
      _control |= SEVENSEG_TIMERRUNNING;
      break;
#endif
  }
}

void SevenSegInt::end(void) {
  bool allDone = true;
  if(_control == SEVENSEG_CHILD) {
    _digitsUsed = 0;
    return;
  }
  _setOff();
  for(uint8_t i = 0; i < 8; i++) {
    if(_instance[i] == this) _instance[i] = NULL;
    else if(_instance[i] != NULL && _instance[i]->Timer == Timer) allDone = false;
  }
  if(allDone) {
    switch(Timer) {
      case 1:
        Timer1.stop();
        Timer1.detachInterrupt();
        break;

#ifdef SEVENSEG_TIMER2
      case 2:
        Timer2.stop();
        Timer2.detachInterrupt();
        break;
#endif

      case 3:
        Timer3.stop();
        Timer3.detachInterrupt();
        break;

#ifdef SEVENSEG_TIMER4
      case 4:
        Timer4.stop();
        Timer4.detachInterrupt();
        break;
#endif

#ifdef SEVENSEG_TIMER5
      case 5:
        Timer5.attachInterrupt(_segTimerInt5);
        Timer5.initialize(1000);
        break;
#endif
    }
  }
}

//
// The next 3 are primitives for the Print class
//
size_t SevenSegInt::write(uint8_t value) {
  if(!_digitsUsed) return 0;
  _putCharacter(_setCharacter(value));
  return 1;
}

size_t SevenSegInt::write(const char *str) {
  byte count = 0;
  if(!_digitsUsed) return 0;
  while(*str) {
      _putCharacter(_setCharacter(*str));
      str++;
      count++;
  }
  return count;
}

size_t SevenSegInt::write(const byte *buf, size_t len) {
  uint8_t p = 0;
  if(!_digitsUsed) return 0;
  while(len) {
    _putCharacter(_setCharacter(buf[p]));
    p++;
    len--;
  }
  return p;
}

//
// Blank the display buffer
//
void SevenSegInt::_blankLine() {
  uint8_t count = 0;
  while(count < _digitsUsed) {
    if(_useInstance == NULL) {
      _string[count] = SEVENSEG_SPACE;
    } else {
      _useInstance->_string[_digits + count] = SEVENSEG_SPACE;
    }
    count++;
  }
}

//
// Character parser for the Print primitives
//
uint8_t SevenSegInt::_setCharacter(uint8_t now) {
  now &= 0x7f;
  if(isalpha(now)) {
    switch(now) {
      case 'C':
        now = SEVENSEG_BIGC;
        break;

      case 'G':
        now = SEVENSEG_BIGG;
        break;

      case 'H':
        now = SEVENSEG_BIGH;
        break;

      case 'I':
        now = SEVENSEG_BIGI;
        break;

      case 'O':
        now = SEVENSEG_BIGO;
        break;

      case 'U':
      case 'V':
        now = SEVENSEG_BIGUV;
        break;

      default:
        now = toupper(now) - 'A' + 11;
        break;
    }
  } else if(now == '.') {
    now = SEVENSEG_CHARDOT;
  } else if(isdigit(now)) {
    now -= '0';
  } else if(now == '-') {
    now = 10;
  } else if(now == '\r' || now == '\n') {
    now = SEVENSEG_NEWLINE;
  } else if(now < ' ') {
    now = SEVENSEG_NOCHAR;
  } else {
    now = SEVENSEG_SPACE;
  }
  return now;
}

//
// Character placer for the Print primitives
//
uint8_t SevenSegInt::_putCharacter(uint8_t character) {
  static uint8_t lp = 0;
  if(character == SEVENSEG_NOCHAR) {
    return 0;
  }
  if(character == SEVENSEG_NEWLINE) {
    _linePointer = 0;
    return 0;
  }
  if(!_linePointer) {
    _blankLine();
    lp = 255;
  }
  if(_linePointer >= _digitsUsed) return 0;
  if(character == SEVENSEG_CHARDOT) {
    if(lp >= _digitsUsed) return 0;
    if(_useInstance == NULL) {
      _string[lp] |= 0x80;
    } else {
      _useInstance->_string[_digits + lp] |= 0x80;
    }
    return 0;
  }
  if(_useInstance == NULL) {
    _string[_linePointer] = character;
  } else {
    _useInstance->_string[_digits + _linePointer] = character;
  }
  lp = _linePointer;
  _linePointer++;
  return 1;
}

//
// Routines to drive the display, all done from the timer interrupt
//
void SevenSegInt::_setSegments(byte d, uint8_t dotOn) {
  if(_control & SEVENSEG_SHIFT) {
    if(d != SEVENSEG_SPACE) shiftOut(_segmentPins[SEVENSEG_DATA], _segmentPins[SEVENSEG_CLOCK], LSBFIRST, SHIFTSEGD(d) | dotOn);
    else shiftOut(_segmentPins[SEVENSEG_DATA], _segmentPins[SEVENSEG_CLOCK], LSBFIRST, dotOn);
    digitalWrite(_segmentPins[SEVENSEG_LATCH], LOW);
    digitalWrite(_segmentPins[SEVENSEG_LATCH], HIGH);
  } else {
    if(d != SEVENSEG_SPACE) for(byte i = 0; i < 7; i++) SEGD(d, i);
  }
}

void SevenSegInt::_setOff() {
  if(_control & SEVENSEG_SHIFT) {
    shiftOut(_segmentPins[SEVENSEG_DATA], _segmentPins[SEVENSEG_CLOCK], LSBFIRST, 0);
    digitalWrite(_segmentPins[SEVENSEG_LATCH], LOW);
    digitalWrite(_segmentPins[SEVENSEG_LATCH], HIGH);
  } else {
    for(byte i = 0; i < 8; i++) SEGBLANK(i);
  }
}

void SevenSegInt::_setCommon(uint8_t common) {
  uint8_t on = _digitOn ? LOW : HIGH;
  if(_control & (SEVENSEG_DEMUX | SEVENSEG_DEMUX2)) {
    if(common & 0x04) digitalWrite(_commonPins[SEVENSEG_A2], HIGH);
    else digitalWrite(_commonPins[SEVENSEG_A2], LOW);
    if(common & 0x02) digitalWrite(_commonPins[SEVENSEG_A1], HIGH);
    else digitalWrite(_commonPins[SEVENSEG_A1], LOW);
    if(common & 0x01) digitalWrite(_commonPins[SEVENSEG_A0], HIGH);
    else digitalWrite(_commonPins[SEVENSEG_A0], LOW);
    if(_control & SEVENSEG_DEMUX2) {
      if(common > 8) digitalWrite(_commonPins[SEVENSEG_CS], HIGH);
      else digitalWrite(_commonPins[SEVENSEG_CS], LOW);
    }
  } else {
    for(uint8_t i = 0; i < _digits; i++) {
      if(i == common) digitalWrite(_commonPins[i], on);
      else digitalWrite(_commonPins[i], !on);
    }
  }
}

void SevenSegInt::SevenSegIntdrive(void) {
  char c;
  uint8_t doDot = 0;
  _setOff();
  _setCommon(_current);
  c = _string[_current];
  if(c & 0x80) {
    if(_control & SEVENSEG_SHIFT) {
      doDot = 0x01;
    } else {
      DOTON;
    }
  }
  c &= 0x7F;
  _setSegments(c, doDot);
  _current = (++_current == _digits) ? 0 : _current;
}


