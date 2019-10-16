// Written by Nick Gammon
// February 2011

#include <SPI.h>
#include <string.h>
#include <stdio.h>

char buf [100];
char readable [500];
volatile byte pos;
volatile boolean process_it;

void setup (void)
{
  Serial.begin (9600);   // debugging

  // turn on SPI in slave mode
  SPCR |= bit (SPE);

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // get ready for an interrupt
  pos = 0;   // buffer empty
  memset(buf, 0, sizeof(buf));
  process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();
  Serial.println("Hey there");
}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR;  // grab byte from SPI Data Register

  // add to buffer if room
  if (pos < (sizeof (buf) - 1))
    buf [pos++] = c;
  // example: newline means time to process buffer
  //Serial.println (c);
  if (c == '\0')
    process_it = true;

}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine
void loop (void)
{
  if (process_it)
  {
    if (buf[0] == 1) { //global
      //Serial.println("Global");
      snprintf(readable, 500, "master_volume: %5d, env_attack: %5d, env_decay: %5d, env_sustain: %3d, env_release: %5d",
      *(uint16_t*)&buf[1], *(uint16_t*)&buf[3], *(uint16_t*)&buf[5], buf[7], *(uint16_t*)&buf[8]);
      Serial.println(readable);
      Serial.println("Pitch wheels:");
      for (uint8_t i = 0; i < 16; i++) {
        Serial.print("\t");
        Serial.println(buf[10+i], HEX);
      }
      Serial.print('\n');
    } else if (buf[0] == 2) { //generator
      //Serial.println("Generator");
      snprintf(readable, 500, "generator_index: %6d, reset_note: %1d, enabled: %1d, instruments: %10d, note_index: %3d, channel_index: %3d, velocity: %3d",
      *(uint16_t*)&buf[1], buf[3], buf[4], *(uint16_t*)&buf[5], buf[9], buf[10], buf[11]);
      Serial.println(readable);
    } else {
      for (uint16_t i = 0; i < 100; i++) {
          Serial.print(buf[i], HEX);
          Serial.print(" ");
      }
      Serial.print('\n');
    }
    pos = 0;
    memset(buf, 0, sizeof(buf));
    memset(readable, 0, sizeof(readable));
    process_it = false;
  }  // end of flag set

}  // end of loop
