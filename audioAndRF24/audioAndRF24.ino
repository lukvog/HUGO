/*

PINS Audioboad:
 6 - MEMCS
 7 - MOSI
 9 - BCLK
10 - SDCS
11 - MCLK
12 - MISO
13 - RX
14 - SCLK
15 - VOL
18 - SDA
19 - SCL
22 - TX
23 - LRCLK

PINS RF24
GND  1 --> GND
Vin  2 --> 3.3V
CE   3 --> 2
CSN  4 --> 6
SCK  5 --> 14
MOSI 6 --> 7
MISO 7 --> 12

*/


#include <Audio.h>
#include <Wire.h>
#include <stdio.h>
#include <SPI.h>
#include <SD.h>
#include <RF24Network.h>
#include <RF24.h>
#include "nodeconfig.h"
#include <Metro.h>


//___________________________________________________________________________________
/////////////////////////
//AUDIO

const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
//AudioInputAnalog analogPinInput(16); // analog A2 (pin 16)
AudioInputI2S    audioInput;         // audio shield: mic or line-in
AudioOutputI2S   audioOutput;        // audio shield: headphones & line-out
AudioOutputPWM   pwmOutput;          // audio output with PWM on pins 3 & 4

// Create Audio connections between the components
//
AudioConnection c1(audioInput, 0, audioOutput, 0);
AudioConnection c2(audioInput, 1, audioOutput, 1);
AudioConnection c5(audioInput, 0, pwmOutput, 0);

// Create an object to control the audio shield.
// 
AudioControlSGTL5000 audioShield;

//___________________________________________________________________________________
/////////////////////////
//RF24

// This is for git version tracking.  Safe to ignore
#ifdef VERSION_H
#include "version.h"
#else
#define __TAG__ "Unknown"
#endif

// nRF24L01(+) radio using the Getting Started board
//RF24 radio(9,10);
RF24 radio(2,6);
RF24Network network(radio);

// Our node address
uint16_t this_node;

// Delay manager to send pings regularly
const unsigned long interval = 100; // ms
unsigned long last_time_sent;

// Array of nodes we are aware of
const short max_active_nodes = 10;
uint16_t active_nodes[max_active_nodes];
short num_active_nodes = 0;
short next_ping_node_index = 0;

// Prototypes for functions to send & handle messages
bool send_T(uint16_t to);
bool send_N(uint16_t to);
void handle_T(RF24NetworkHeader& header);
void handle_N(RF24NetworkHeader& header);
void add_node(uint16_t node);


//___________________________________________________________________________________
/////////////////////////
//Light

int led = 5;           // the pin that the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 1;    // how many points to fade the LED by



//___________________________________________________________________________________
void setup() 
{
  
  //___________________________________________________________________________________
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(60);
  
  //___________________________________________________________________________________
  //RF24
  
 ///SPI Setup
  SPI.setMOSI(7);
  SPI.setSCK(14);
  
  //
  // Print preamble
  //
  Serial.begin(57600);
  //printf_begin();
  delay(2000);
  Serial.printf_P(PSTR("\n\rRF24Network/examples/meshping/\n\r"));
  Serial.printf_P(PSTR("VERSION: " __TAG__ "\n\r"));
  
  //
  // Pull node address out of eeprom 
  //
  // Which node are we?
  this_node = nodeconfig_read();

  //
  // Bring up the RF network
  //
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 100, /*node address*/ this_node );
  
  //___________________________________________________________________________________
  //LIGHT
  
  pinMode(led, OUTPUT);
  
}

elapsedMillis volmsec=0;
elapsedMillis dimmsec=0;
unsigned long last_time = millis();

Metro ReadMetro = Metro(10);
Metro WriteMetro = Metro(10);


void loop() {
  
  
   //___________________________________________________________________________________
  //LIGHT
  
// if (dimmsec > 40) {
    
        // set the brightness of pin 9:
  analogWrite(led, brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;
  // reverse the direction of the fading at the ends of the fade:
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ;
  }
  
  delay(40);
  //volmsec = 0;
 // }

  return;
  
  
   //___________________________________________________________________________________
  //AUDIO
  // volume control
  // every 10 ms, check for adjustment
	if (ReadMetro.check() == 1) {
		int n = analogRead(15);
		if (n != volume) {
			volume = n;
			audioShield.volume((float)n / 10.23);
		}
	}
    
    //////////////////
    //Mem and CPU Usage
    
    // Change this to if(1) for measurement output
if(1) {
/*
  For PlaySynthMusic this produces:
  Proc = 20 (21),  Mem = 2 (8)
*/
  if(millis() - last_time >= 5000) {
    Serial.print("Proc = ");
    Serial.print(AudioProcessorUsage());
    Serial.print(" (");    
    Serial.print(AudioProcessorUsageMax());
    Serial.print("),  Mem = ");
    Serial.print(AudioMemoryUsage());
    Serial.print(" (");    
    Serial.print(AudioMemoryUsageMax());
    Serial.println(")");
    last_time = millis();
  }
  
  

}
  
    
     //___________________________________________________________________________________
  //RF24
   ////////////////////
  // Ping out role.  Repeatedly send the current time
  //
  // Pump the network regularly
  network.update();

  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, take a look at it 
    RF24NetworkHeader header;
    network.peek(header);

    // Dispatch the message to the correct handler.
    switch (header.type)
    {
    case 'T':
      handle_T(header);
      break;
    case 'N':
      handle_N(header);
      break;
    default:
      Serial.printf_P(PSTR("*** WARNING *** Unknown message type %c\n\r"),header.type);
      network.read(header,0,0);
      break;
    };
  }

  // Send a ping to the next node every 'interval' ms
  unsigned long now = millis();
  if ( now - last_time_sent >= interval )
  {
    last_time_sent = now;

    // Who should we send to?
    // By default, send to base
    uint16_t to = 00;
    
    // Or if we have active nodes,
    if ( num_active_nodes )
    {
      // Send to the next active node
      to = active_nodes[next_ping_node_index++];
      
      // Have we rolled over?
      if ( next_ping_node_index > num_active_nodes )
      {
	// Next time start at the beginning
	next_ping_node_index = 0;

	// This time, send to node 00.
	to = 00;
      }
    }

    bool ok;

    // Normal nodes send a 'T' ping
    if ( this_node > 00 || to == 00 )
      ok = send_T(to);
    
    // Base node sends the current active nodes out
    else
      ok = send_N(to);
 //     ok = send_V(to);

    // Notify us of the result
    if (ok)
    {
      Serial.printf_P(PSTR("%lu: APP Send ok\n\r"),millis());
    }
    else
    {
      Serial.printf_P(PSTR("%lu: APP Send failed\n\r"),millis());

      // Try sending at a different time next time
      last_time_sent -= 100;
    }
  }

  // Listen for a new node address
  nodeconfig_listen();
}

/**
 * Send a 'T' message, the current time
 */
bool send_T(uint16_t to)
{
  RF24NetworkHeader header(/*to node*/ to, /*type*/ 'T' /*Time*/);
  
  // The 'T' message that we send is just a ulong, containing the time
  unsigned long message = millis();
  Serial.printf_P(PSTR("---------------------------------\n\r"));
  Serial.printf_P(PSTR("%lu: APP Sending %lu to 0%o...\n\r"),millis(),message,to);
  return network.write(header,&message,sizeof(unsigned long));
}



/**
 * Send a 'V' message, the current time
 */


//bool send_V(uint16_t to)
//{
  //RF24NetworkHeader header(/*to node*/ to, /*type*/ 'T' /*Time*/);
  
 // The 'V' message is a value message
//  unsigned long message = millis();
//  Serial.printf_P(PSTR("---------------------------------\n\r"));
//  Serial.printf_P(PSTR("%lu: APP Sending %lu to 0%o...\n\r"),millis(),message,to);
//  return network.write(header,&message,sizeof(unsigned long));
//}



/**
 * Send an 'N' message, the active node list
 */
bool send_N(uint16_t to)
{
  RF24NetworkHeader header(/*to node*/ to, /*type*/ 'N' /*Time*/);
  
  Serial.printf_P(PSTR("---------------------------------\n\r"));
  Serial.printf_P(PSTR("%lu: APP Sending active nodes to 0%o...\n\r"),millis(),to);
  return network.write(header,active_nodes,sizeof(active_nodes));
}

/**
 * Handle a 'T' message
 *
 * Add the node to the list of active nodes
 */
void handle_T(RF24NetworkHeader& header)
{
  // The 'T' message is just a ulong, containing the time
  unsigned long message;
  network.read(header,&message,sizeof(unsigned long));
  Serial.printf_P(PSTR("%lu: APP Received %lu from 0%o\n\r"),millis(),message,header.from_node);

  // If this message is from ourselves or the base, don't bother adding it to the active nodes.
  if ( header.from_node != this_node || header.from_node > 00 )
    add_node(header.from_node);
}

/**
 * Handle a 'V' message
 *
 * Add the node to the list of active nodes
 */
//void handle_V(RF24NetworkHeader& header)
//{
  // The 'T' message is just a ulong, containing the time
//  unsigned long message;
//  network.read(header,&message,sizeof(unsigned long));
//  Serial.printf_P(PSTR("%lu: APP Received %lu from 0%o\n\r"),millis(),message,header.from_node);

  // If this message is from ourselves or the base, don't bother adding it to the active nodes.
 // if ( header.from_node != this_node || header.from_node > 00 )
//    add_node(header.from_node);
//}

/**
 * Handle an 'N' message, the active node list
 */
void handle_N(RF24NetworkHeader& header)
{
  static uint16_t incoming_nodes[max_active_nodes];

  network.read(header,&incoming_nodes,sizeof(incoming_nodes));
  Serial.printf_P(PSTR("%lu: APP Received nodes from 0%o\n\r"),millis(),header.from_node);

  int i = 0;
  while ( i < max_active_nodes && incoming_nodes[i] > 00 )
    add_node(incoming_nodes[i++]);
}

/**
 * Add a particular node to the current list of active nodes
 */
void add_node(uint16_t node)
{
  // Do we already know about this node?
  short i = num_active_nodes;
  while (i--)
  {
    if ( active_nodes[i] == node )
      break;
  }
  // If not, add it to the table
  if ( i == -1 && num_active_nodes < max_active_nodes )
  {
    active_nodes[num_active_nodes++] = node; 
    Serial.printf_P(PSTR("%lu: APP Added 0%o to list of active nodes.\n\r"),millis(),node);
  }
  
  
  
		
}


