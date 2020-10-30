#ifndef SEVENSEGINT_H
#define SEVENSEGINT_H

#include "Arduino.h"
#include "../userdefs.h"

#define SEVENSEG_COMMONANODE    0
#define SEVENSEG_COMMONCATHODE  1

class SevenSegInt: public Print {

public:
  SevenSegInt(const uint8_t *segmentPins, const uint8_t *commonPins, bool common, uint8_t digits);
  SevenSegInt(const uint8_t *segmentPins, uint8_t a0, uint8_t a1, uint8_t a2, bool common, uint8_t digits);
  SevenSegInt(const uint8_t *segmentPins, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t demuxSelect, bool common, uint8_t digits);
  SevenSegInt(const uint8_t *segmentPins, const uint8_t *commonPins, bool common, uint8_t digits, const char *timer);
  SevenSegInt(const uint8_t *segmentPins, uint8_t a0, uint8_t a1, uint8_t a2, bool common, uint8_t digits, const char *timer);
  SevenSegInt(const uint8_t *segmentPins, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t demuxSelect, bool common, uint8_t digits, const char *timer);
  SevenSegInt(const uint8_t *segmentPins, const uint8_t *commonPins, bool common, uint8_t digits, const char *timer, uint8_t digitsUsed);
  SevenSegInt(const uint8_t *segmentPins, uint8_t a0, uint8_t a1, uint8_t a2, bool common, uint8_t digits, const char *timer, uint8_t digitsUsed);
  SevenSegInt(const uint8_t *segmentPins, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t demuxSelect, bool common, uint8_t digits, const char *timer, uint8_t digitsUsed);
  SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, const uint8_t *commonPins, bool common, uint8_t digits);
  SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1, uint8_t a2, bool common, uint8_t digits);
  SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t demuxSelect, bool common, uint8_t digits);
  SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, const uint8_t *commonPins, bool common, uint8_t digits, const char *timer);
  SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1, uint8_t a2, bool common, uint8_t digits, const char *timer);
  SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t demuxSelect, bool common, uint8_t digits, const char *timer);
  SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, const uint8_t *commonPins, bool common, uint8_t digits, const char *timer, uint8_t digitsUsed);
  SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1, uint8_t a2, bool common, uint8_t digits, const char *timer, uint8_t digitsUsed);
  SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t demuxSelect, bool common, uint8_t digits, const char *timer, uint8_t digitsUsed);
  SevenSegInt(uint8_t digits);
  virtual size_t write(uint8_t);
  virtual size_t write(const char *);
  virtual size_t write(const byte *, size_t);
  void begin(void);
  void end(void);
  // For interrupt routines
  void SevenSegIntdrive(void);
  uint8_t Timer;

private:
  void _sevenseg(const uint8_t *segmentPins, const uint8_t *commonPins, bool common, uint8_t digits, uint8_t hardwareFlags, uint8_t digitsUsed, uint8_t timer);
  void _setOff();
  void _setCommon(uint8_t);
  void _setSegments(uint8_t, uint8_t);
  void _blankLine();
  uint8_t _setCharacter(uint8_t);
  uint8_t _putCharacter(uint8_t);
  uint8_t _segmentPins[8];
  uint8_t _commonPins[16];
  int8_t _current;
  int8_t _digits;
  volatile char _string[17];
  bool _digitOn;
  uint8_t _control;
  uint8_t _digitsRemaining;
  uint8_t _digitsUsed;
  uint8_t _linePointer;
  SevenSegInt *_useInstance;
};

#endif
