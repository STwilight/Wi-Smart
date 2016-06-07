
/* подключение заголовочных файлов Sming */
#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Libraries/RF24/nRF24L01.h>
#include <Libraries/RF24/RF24.h>

/* подключение заголовочных айлов проекта */
#include <uart.h>
#include <config.h>

/* NRF24L01 Connection Map:
 *
 * VCC		3.3v
 * SCK		GPIO14
 * MOSI		GPIO13
 * MISO		GPIO12
 * CE		GPIO4 (changeable)
 * CSN		GPIO5 (changeable)
 * GND      GND
 *
 */

RF24 radio(4, 5);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

Timer procTimer;



void loopListen()
{
	// if there is data ready
    if (radio.available())
    {
      // Dump the payloads until we've gotten everything
      unsigned long got_time;
      bool done = false;
      while (!done)
      {
    	WDT.alive();

    	// Fetch the payload, and see if this was the last one.
        done = radio.read( &got_time, sizeof(unsigned long) );

        // Spew it
        Serial.printf("Got payload %lu...",got_time);

        // Forward this data packet to UDP/HTTP/MQTT here.
        // Example:
        // 		thingSpeak.downloadString("http://api.thingspeak.com/update?key=7XXUJWCWYTMXKN3L&field1=" + String(got_time), onSent);
        // (more details in HttpClient_ThingSpeak example)


		// Delay just a little bit to let the other unit
		// make the transition to receiver
        delay(5);
      }

      // First, stop listening so we can talk
      radio.stopListening();

      // Send the final one back.
      radio.write( &got_time, sizeof(unsigned long) );
      Serial.println("Sent response.");

      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }
}

void init()
{
	uartInit();

	radio.begin();

	// optionally, increase the delay between retries & # of retries
	radio.setRetries(15,15);

	// optionally, reduce the payload size.  seems to
	// improve reliability
	radio.setPayloadSize(8);

	// This simple sketch opens two pipes for these two nodes to communicate
	// back and forth.
	// Open 'our' pipe for writing
	// Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)
	// In this case we will pong back (because we are the server)
	radio.openWritingPipe(pipes[1]);
	radio.openReadingPipe(1,pipes[0]);

	radio.startListening();
	Serial.println("Initialization completed.");

	procTimer.initializeMs(100, loopListen).start();
	Serial.println("Listening...");
}
