// Written by Nick Gammon
// February 2011

#include <SPI.h>
#include <string.h>
#include <stdio.h>

#define DEBUG 0

char buf[6];
volatile byte pos;
volatile boolean process_it;

void setup (void)
{
    Serial.begin(16 * 441 * 100L);   // debugging

    // turn on SPI in slave mode
    SPCR |= _BV(SPE);

    // have to send on master in, *slave out*
    pinMode(SS, INPUT_PULLUP);
    pinMode(MOSI, INPUT);
    // pinMode(MISO, OUTPUT);
    pinMode(SCK, INPUT);

    // get ready for an interrupt
    pos = 0;   // buffer empty
    memset(buf, 0, sizeof(buf));
    process_it = false;

    // now turn on interrupts
    SPI.attachInterrupt();
    Serial.println("Hey there!");
}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect) {
    byte c = SPDR;
    if (pos < (sizeof(buf) - 1))
        buf[pos++] = c;
    if (pos >= 2)
        process_it = true;
}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine
void loop(void) {
    if (process_it) {
        for (uint16_t i = 0; i < 2; i++) {
            Serial.print(buf[i]);
        }
        if (DEBUG)
            Serial.print('\n');
        pos = 0;
        memset(buf, 0, sizeof(buf));
        process_it = false;
    }

}  // end of loop
