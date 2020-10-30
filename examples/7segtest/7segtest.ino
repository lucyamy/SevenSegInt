#include <SevenSegInt.h>

byte spin4[] = { 9, 10, 14, 15, 16, 12, 11, 13 };
byte dpin4[] = { 6, 7, 8, 17 };

// Four digit display, common cathode, no demultiplexer used, using Timer1.
SevenSegInt myseg(spin4, dpin4, SEVENSEG_COMMONCATHODE, 4);
// Eight digit display, common cathode, using a 74HC138 and a 74HC595, split into
// one 5 digit display, and one 3 digit display, using Timer3. 
SevenSegInt myseg2(42, 41, 40, 45, 44, 43, SEVENSEG_COMMONCATHODE, 8, "3", 5);
SevenSegInt myseg3(3);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Start. Initialise display"));
  myseg.begin();
  myseg2.begin();
  // The next line is unnecessary, SevenSegInt doesn't need it for a child display. But
  // it just feels wrong not to have it :).
  myseg3.begin();
  Serial.println(F("Initialised"));
}

void loop() {
  char *p, *q;
  double d;
  // Some 4 max char strings
  myseg.println(F("Err."));
  delay(500);
  myseg2.println(F("123.45"));
  delay(5000);
  myseg2.println(567.89);
  delay(5000);
  myseg2.println(12345678);
  myseg3.println("Hi");
  myseg.println(F("cafe"));
  delay(500);
  myseg.println(F("love"));
  myseg2.println(F("Soul"));
  myseg3.println("Ho");
  delay(500);
  myseg2.println(F("Faun"));
  delay(500);
  myseg.println(F("tea"));
  delay(500);
  myseg.println(F("beer"));
  delay(500);
  myseg.println(F("test"));
  delay(500);
  // Scroll a string. Any string you send to the library can be longer than
  // the display, extra characters will be ignored. We use that fact here.
  // The trick here is to come up with a long string that doesn't use k, m,
  // w, x, or z. In English at least, it's k, m, and w that you really miss.
  p = (char *) "Goodbye Jodie Danielle. The good days brought happiness. I shall love you forever. Let us go forth into the future in joy.";
  // I must point out that I'm still very much in love with Jodie Danielle.
  // It just fitted the string requirements, sorry angel.
  q = (char *) "This is a string to print to the red display to see if that is functional. Ho ho bloody ho.";
  while(*p) {
    myseg.println(p);
    p++;
    if(*q) {
      myseg2.println(q);
      q++;
    }
    delay(250);
  }
  // Count up and down
  for(uint32_t  i = 0; i < 200; i++) {
    myseg.println(i);
    myseg2.println(199 - i);
    delay(100);
  }
  // And count up and down in hex
  for(uint32_t  i = 0; i < 0x0200; i++) {
    myseg.println(i, HEX);
    myseg2.println(0x01ff - i, HEX);
    delay(100);
  } 
  // And then count a long, long way, in 1/100s of a second.
  for(uint32_t  i = 0; i < 100000; i++) {
    d = i;
    d /= 100.0;
    myseg.println(i / 10);
    myseg2.println(d);
    myseg3.println(i / 100);
    delay(10);
  } 
}

