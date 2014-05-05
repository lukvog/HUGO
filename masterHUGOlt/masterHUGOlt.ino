/*
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
//#include "sequences.h"

#include <Metro.h>


//___________________________________________________________________________________
/////////////////////////
//AUDIO CONFIGURATION


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

// Array of nodes we are aware of
const short max_active_nodes = 13;
uint16_t active_nodes[max_active_nodes];
short num_active_nodes = 0;
short next_ping_node_index = 0;

// Prototypes for functions to send & handle messages
bool send_T(uint16_t to);
bool send_N(uint16_t to);
bool send_V(uint16_t to);
void handle_T(RF24NetworkHeader& header);
void handle_N(RF24NetworkHeader& header);
void handle_V(RF24NetworkHeader& header);
void add_node(uint16_t node);

//data
int valueRF = 1;



//_________________________________________________________________________________
//////////////////////
//Ultrasonic aka IR
//ULTRASONIC!!!!


//Ultrasonic ultrasonic(21);
int irPin = 21;

int changed;
int wallDist;
int range;



//___________________________________________________________________________________
void setup() 
{

  Serial.begin(57600);

  //___________________________________________________________________________________
  //Ultrasonic aka  RF

  //calibration for Walls

  int wall[100];
  int count = 0;
  int sum = 0;

  for(int i=0; i < 100; i++){
    range = analogRead(21);
    if (range != 0 && range <= 400)
    {
      wall[count] = range;
      //Serial.print(count);
      //Serial.print("range: ");
      //Serial.println(range, DEC);
      count++;
    }
    delay(60);
  };

  //Serial.println(count);

  for(int i=0; i < count; i++){
    sum += wall[i];
  };

  //Serial.println(sum);

  wallDist = (sum/count) - 15;
  if (wallDist < 5)
  {
    wallDist = 400;
  }


  //___________________________________________________________________________________
  //RF24

  ///SPI Setup
  SPI.setMOSI(7);
  SPI.setSCK(14);
  //radio.setDataRate(RF24_1MBPS);
  //radio.setDataRate(RF24_250KBPS);

  //
  // Print preamble

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

}

//___________________________________________________________

//Metros
Metro ComMetro = Metro(1000);
Metro SensMetro = Metro(400);
Metro WriteMetro = Metro(500);
Metro ReadMetro = Metro(10);


void loop() {

  //___________________________________________________________________________________
  //Ultrasonic

  if (SensMetro.check() == 1) {
    range = analogRead(21);

    if (range != 0 && range < wallDist)
    {
      changed = range;
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
    case 'V':
      handle_V(header); 

      /////
      //Switch um Liste aufzubrechen
      //Switch für Sensor und Light Mapping

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


  //////////////////////////////
  ////////////////////////////////
  //////////////////////////
  if (WriteMetro.check() == 1) {
    Serial.print("valueRF = "); 
    Serial.println(valueRF);
    Serial.print("wallDist: ");
    Serial.println(wallDist, DEC);
    Serial.print("changed: ");
    Serial.println(changed, DEC);
  }

  //////////////////////////////
  ////////////////////////////////
  //////////////////////////


  // Send a ping to the next node every 'interval' ms
  //unsigned long now = millis();
  if (ComMetro.check() == 1) {
    //last_time_sent = now;

    // Who should we send to?
    // By default, send to base
    uint16_t to = 00;

    // Or if we have active nodes,
    if ( num_active_nodes )
    {
      // Send to the next active node
      to = active_nodes[next_ping_node_index++];
      if (to == this_node)
      {
        to = active_nodes[next_ping_node_index++];
      }
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
    if ( this_node > 00 ) {
      //if ( this_node > 00 || to == 00 )
      ok = send_T(to);

      // Notify us of the result
      if (ok)
      {
        Serial.printf_P(PSTR("%lu: APP Send ok\n\r"),millis());
      }
      else
      {
        Serial.printf_P(PSTR("%lu: APP Send failed\n\r"),millis());
      }
    }
    // Base node sends the current active nodes out
    else {
      ok = send_N(to);

      // Notify us of the result
      if (ok)
      {
        Serial.printf_P(PSTR("%lu: APP Send ok\n\r"),millis());
      }
      else
      {
        Serial.printf_P(PSTR("%lu: APP Send failed\n\r"),millis());

        // Try sending at a different time next time
        //last_time_sent -= 100;
      }

      ok = send_V(to);
      // Notify us of the result
      if (ok)
      {
        Serial.printf_P(PSTR("%lu: APP Send ok\n\r"),millis());
      }
      else
      {
        Serial.printf_P(PSTR("%lu: APP Send failed\n\r"),millis());

        // Try sending at a different time next time
        //last_time_sent -= 100;
      }
    }
  }

  // Listen for a new node address
  nodeconfig_listen();
}


//____________________________________________________

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
 * Send a 'V' message, a value
 */


bool send_V(uint16_t to)
{
  RF24NetworkHeader header(/*to node*/ to, /*type*/ 'V' /*Time*/);

  // The 'V' message is a value message
  unsigned int message = analogRead(15);
  Serial.printf_P(PSTR("---------------------------------\n\r"));
  Serial.printf_P(PSTR("%lu: APP Sending Value %lu to 0%o...\n\r"),millis(),message,to);
  return network.write(header,&message,sizeof(unsigned int));
}



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
  Serial.printf_P(PSTR("%lu: APP Received Time %lu from 0%o\n\r"),millis(),message,header.from_node);

  // If this message is from ourselves or the base, don't bother adding it to the active nodes.
  if ( header.from_node != this_node || header.from_node > 00 )
    add_node(header.from_node);
}

/**
 * Handle a 'V' message
 *
 * Add the node to the list of active nodes
 */
void handle_V(RF24NetworkHeader& header)
{
  // The 'V' contains values
  unsigned int message;
  network.read(header,&message,sizeof(unsigned int));
  Serial.printf_P(PSTR("%lu: APP Received Value %lu from 0%o\n\r"),millis(),message,header.from_node);
  valueRF = message;

  // If this message is from ourselves or the base, don't bother adding it to the active nodes.
  if ( header.from_node != this_node || header.from_node > 00 )
    add_node(header.from_node);
}


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




