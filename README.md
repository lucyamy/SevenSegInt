# SevenSegInt
An Arduino and Arduinoish library to drive seven segment displays.
You can have up to 8 instances of **SevenSegInt**, for 8 separate displays, and there
can be up to 16 digits in each display - which is pushing it, really, as the
drive currents will be getting rather silly by the time you have 16 digits,
and the consequences of your Arduino crashing and leaving one digit illuminated
with the full current flowing through it are likely to be nasty. But you can do it.
I plan to design a very simple hardware watchdog, and add support for that. Someday.

**SevenSegInt** inherits the **Print** class, so you use normal Arduino print calls to update 
the display. **SevenSegInt** will display numbers and text, though there are of course a few
restrictions on text :). The letters k, m, w, x, and z cannot be displayed. The only punctuation
marks/special characters possible are '.' and '-'. All undisplayable characters will be 
displayed as a space. 7 letters - c, g, h, i, o, u, and v (u and v are displayed the same, btw)
can be displayed in upper or lowercase. For other letters, the old word processing feature of
**WYGIWYG** applies.

println() should be used if you want to get **SevenSegInt** ready to display a new line of text.
If you don't send a newline, text will keep being *displayed* off to the right of the display.

The period, full stop, decimal point, or as we know it now *dot*, is a special case. It doesn't 
take up a space in the display, being simply added to the last character displayed. Worth remembering.

As explained below, **SevenSegInt** can use various hardware and software techniques to keep
the number of pins needed low, including using demultiplexer and shift register chips.

The simplest invocation of **SevenSegInt** is:

SevenSegInt myseg(uint8_t segmentPins[], uint8_t commonPins[], bool common, uint8_t digits);

Segments are:

            A
         ,-----,
      F /  G  / B
       /-----/
    E /     / C
     `-----' . DP
        D

        
The segmentPins array passed to **SevenSegInt** is:
  { A, B, C, D, E, F, G, DP }

The commonPins array is the common anode/cathodes from
left to right.

The common parameter is one of SEVENSEG_COMMONCATHODE or
SEVENSEG_COMMONANODE.

The digits parameter is the number of digits.

    e.g.
     SevenSegInt myseg(spin, dpin, SEVENSEG_COMMONCATHODE, 4);
     SevenSegInt myseg2(spin2, dpin2, SEVENSEG_COMMONANODE, 2);

**SevenSegInt** uses the Timer1 and/or Timer3 libraries internally (and others if
available).

Timers
------
To select a timer to use (Timer1 and Timer3 should work on pretty much
anything), you need to specify another parameter:

    SevenSegInt(const uint8_t segmentPins[], const uint8_t commonPins[], bool common,
             uint8_t digits, const char *timer);

where timer is "1" or "3" (or "2"/"4" etc. if supported - the "userdefs.h" file
in the SevenSegInt library root directory contains lines that can be uncommented to
include other timers). Please note that the double quotes are needed.

Note that the timer is selected for each invocation, so you can use different
timers for different displays.

Pin count reduction 1: Using 3 to 8 demultiplexers
--------------------------------------------------
To drive many digits, without using an inordinate number of pins, **SevenSegInt**
supports the use of 74XX138 (for common cathode), or 74XX238 (for common
anode) 3 to 8 demultiplexers. To use **SevenSegInt** in that mode, there is a
different constructor:

    SevenSegInt myseg(uint8_t segmentPins[], uint8_t a0, uint8_t a1, uint8_t a2,
                   bool common, uint8_t digits);

When used like this, the pins for the three address lines to the demultiplexer
are specified, instead of an array of common pins.

If you are using 1 to 8 digits, and one demultiplexer chip, then this works fine.
If you are using two demultiplexers, however, to drive 9-16 digits, then there
is an additional parameter, which is the pin that selects the chip - LOW for the
first chip, HIGH for the second.

    SevenSegInt myseg(uint8_t segmentPins[], uint8_t a0, uint8_t a1, uint8_t a2,
                   uint8_t demuxSelect, bool common, uint8_t digits);

When used like this, connect the demuxSelect pin to one of the two active low
chip selects on the first (left-hand) chip, and to the active high chip select
on the other.

Pin count reduction 2: Doing it in software
-------------------------------------------
Another (software, no cost, less wiring!) way to cut down on segment pins is to
share them between displays. This is accomplished by creating one (hardware)
display, and splitting the digits among more than one invocation - creating a
parent display, and one or more child displays. You do this by invoking **SevenSegInt**
to create a display that includes all the digits, but telling it only to use some
of them. Then you invoke it one or more times to utilise the remaining digits.
There are two basic constructors to do this. To create the first (parent) display:

    SevenSegInt(const uint8_t segmentPins[], const uint8_t commonPins[], bool common,
             uint8_t digits, const char *timer, uint8_t digitsUsed);

where timer is "1" or "3" etc. Just specify "1" for the default if you're not
bothered about which timer to use. The constructor simply needs the timer argument
to distinguish this use from others.

The second, to create a display to use digits of a display created as above is:

    SevenSegInt(uint8_t digits);

This will use the first free digits of the display created with the first invocation.
You should place child invocations immediately after the parent one, or at least pay
attention to what you're doing. Otherwise, a child could associate itself with the
wrong parent. I believe this happens in real life too :).

For example, to create an 8-digit display, split into 4, 2, and 2 digits, use:

    SevenSegInt myseg1(spin4, dpin4, SEVENSEG_COMMONANODE, 8, "1", 4);
    SevenSegInt myseg2(2);
    SevenSegInt myseg3(2);

Note that this only counts as one of the eight displays available to use.

Pin count reduction 3: Using a shift register
---------------------------------------------
Another way to use fewer pins for the segments is to use a 74XX595 shift
register chip to drive the segments. This only requires 3 pins instead
of 8. Connect:

     DS (pin 14) to dpin (data in)
     SH_CP (pin 11) cpin (clock)
     ST_CP (pin 12) lpin (latch)
     and the Q0 to Q7 outputs to the segments, A to DP.

The constructor to do this, in the simplest case, is:

    SevenSegInt myseg(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t commonPins[],
                   bool common, uint8_t digits);

All the versions described before have a similar variant, where the segment pin array
is replaced by three parameters for the data, latch, and clock pins.

You can use both a 74XX138/74XX238 and a 74XX595 to drive an 8-digit display using 6 pins.

Constructors
------------
There is a total of 19 constructors. The full list is:

    SevenSegInt(const uint8_t segmentPins[], const uint8_t commonPins[], bool common,
             uint8_t digits);
    SevenSegInt(const uint8_t segmentPins[], uint8_t a0, uint8_t a1, uint8_t a2,
             bool common, uint8_t digits);
    SevenSegInt(const uint8_t segmentPins[], uint8_t a0, uint8_t a1, uint8_t a2,
             uint8_t demuxSelect, bool common, uint8_t digits);
    SevenSegInt(const uint8_t segmentPins[], const uint8_t commonPins[], bool common,
             uint8_t digits, const char *timer);
    SevenSegInt(const uint8_t segmentPins[], uint8_t a0, uint8_t a1, uint8_t a2,
             bool common, uint8_t digits, const char *timer);
    SevenSegInt(const uint8_t segmentPins[], uint8_t a0, uint8_t a1, uint8_t a2,
             uint8_t demuxSelect, bool common, uint8_t digits, const char *timer);
    SevenSegInt(const uint8_t segmentPins[], const uint8_t commonPins[], bool common,
             uint8_t digits, const char *timer, uint8_t digitsUsed);
    SevenSegInt(const uint8_t segmentPins[], uint8_t a0, uint8_t a1, uint8_t a2,
             bool common, uint8_t digits, const char *timer, uint8_t digitsUsed);
    SevenSegInt(const uint8_t segmentPins[], uint8_t a0, uint8_t a1, uint8_t a2,
             uint8_t demuxSelect, bool common, uint8_t digits, const char *timer,
             uint8_t digitsUsed);
    SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, const uint8_t commonPins[],
             bool common, uint8_t digits);
    SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1,
             uint8_t a2, bool common, uint8_t digits);
    SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1,
             uint8_t a2, uint8_t demuxSelect, bool common, uint8_t digits);
    SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, const uint8_t commonPins[],
             bool common, uint8_t digits, const char *timer);
    SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1,
             uint8_t a2, bool common, uint8_t digits, const char *timer);
    SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1,
             uint8_t a2, uint8_t demuxSelect, bool common, uint8_t digits,
             const char *timer);
    SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, const uint8_t commonPins[],
             bool common, uint8_t digits, const char *timer, uint8_t digitsUsed);
    SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1,
             uint8_t a2, bool common, uint8_t digits, const char *timer,
             uint8_t digitsUsed);
    SevenSegInt(uint8_t dpin, uint8_t lpin, uint8_t cpin, uint8_t a0, uint8_t a1,
             uint8_t a2, uint8_t demuxSelect, bool common, uint8_t digits,
             const char *timer, uint8_t digitsUsed);
    SevenSegInt(uint8_t digits);


Pin saving thoughts, by me.
---------------------------
Let's look at the pros and cons of using different Arduino pin reducing techniques:
    Let's assume you want one 8 digit, one four digit, and two 2 digit displays.

    1. No pin reduction
       You need 4 * 8 segment pins, and 8 + 4 + 2 + 2 common pins, = 48 pins.
       Pros: None, really.
       Cons: That's a huge number of pins, even on an Arduino Mega256. I mean, try
             even DOING that on an Uno or Due!

    2. Using SevenSegInt's display splitting, with one 16-digit parent display,
       and 3 child displays.
       You need 8 segment pins, and 16 common pins, = 24 pins.
       Pros: Halves the number of pins over the simple case.
       Cons: Requires a much higher peak current through the segments to
             achieve the same intensity.

    3. Using demultiplexers for the commons. I'll assume you use one even if
       it saves only a single pin.
       You need 4 * 8 segment pins, and 3 + 3 + 2 + 2 common pins, = 42 pins.
       Pros: In this case, not many. Demuxes only really help with the 8-digit
             display.
       Cons: More expensive*, more complicated hardware.

    4. Using shift registers for the segments.
       You need 4 * 3 segment pins, and 8 + 4 + 2 + 2 common pins, = 28 pins.
       Pros: Almost halves the number of pins over the simple case.
       Cons: Takes a bit longer in the timer interrupt to shift out the segment
             bits. More expensive*, more complicated hardware.

    5. Using display splitting and demultiplexers for the commons.
       You need 8 segment pins, and 4 common pins, = 12 pins.
       Pros: Uses 1/4 of the number of pins over the simple case.
       Cons: Requires a much higher peak current through the segments to
             achieve the same intensity. More expensive*, more complicated
             hardware.

    5. Using display splitting and shift registers for the segments.
       You need 3 segment pins, and 16 common pins, = 19 pins.
       Pros: Uses less than half of the number of pins over the simple case.
       Cons: Requires a much higher peak current through the segments to
             achieve the same intensity. Takes a bit longer in the timer
             interrupt to shift out the segment bits. More expensive*, more
             complicated hardware.

    6. Using demultiplexers for the commons and shift registers for the
       segments.
       You need 3 * 4 segment pins, and 3 + 3 + 2 + 2 common pins, = 22 pins.
       Pros: Uses less than half of the number of pins over the simple case.
       Cons: Takes a bit longer in the timer interrupt to shift out the
             segment bits. More expensive*, more complicated hardware.

    7. Using display splitting, demultiplexers for the commons, and shift
       registers for the segments.
       You need 3 segment pins, and 4 common pins, = 7 pins.
       Pros: Uses very few pins - about 1/7th of the simple case.
       Cons: Requires a much higher peak current through the segments to
             achieve the same intensity. Takes a bit longer in the timer
             interrupt to shift out the segment bits. More expensive*, more
             complicated hardware.

  I know which one I'd use :).

*I mean, when I say more expensive, both 74HC595s and 74HC138s are available
 on Amazon for 30 cents each. So not bank-emptyingly expensive, really.



