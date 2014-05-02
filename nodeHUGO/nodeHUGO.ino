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
 
 LIGHTPIN
 
 PWM --> 5
 
 */


#include <Audio.h>
#include <Wire.h>
#include <stdio.h>
#include <SPI.h>
#include <SD.h>
#include <RF24Network.h>
#include <RF24.h>
#include <Bounce.h>
#include "nodeconfig.h"

#include <Metro.h>


#define MOD_RATE 5 // modulation rate in ms
#define MOD_RATE_DIF 0.2

#include "Delay.h"
#include "FormantFilter.h"
#include "Tone.h"
#include "Sequences.h"

#define Test1_PIN 1
#define Test0_PIN 0

Bounce b_test1 = Bounce(Test1_PIN,15);
Bounce b_test0   = Bounce(Test0_PIN,15);


//___________________________________________________________________________________
/////////////////////////
//AUDIO CONFIGURATION

//const int myInput = AUDIO_INPUT_LINEIN;
const int myInput = AUDIO_INPUT_MIC;

float delayVolume;

AudioInputI2S       audioInput;         // audio shield: mic or line-in
Delay 	staticDelay;
AudioSynthWaveform 	osc1;
AudioSynthWaveform 	osc2;
AudioSynthWaveform	osc3;
AudioMixer4			mixSources;
AudioFilterBiquad    formantFilter1(ToneFilter1);
AudioFilterBiquad    formantFilter2(ToneFilter2);
AudioFilterBiquad    formantFilter3(ToneFilter3);
AudioMixer4        mixFormants;
AudioOutputI2S      audioOutput;        // audio shield: headphones & line-out

// Create Audio connections between the components
// Both channels of the audio input go to the FIR filter
AudioConnection c1(audioInput, 0, staticDelay, 0);
AudioConnection c2(staticDelay, 0, mixSources, 0);

AudioConnection c3(osc1, 0, mixSources, 1);
AudioConnection c12(osc2, 0, mixSources, 2);
AudioConnection c13(osc3, 0, mixSources, 3);
AudioConnection c4(mixSources, 0, formantFilter1, 0);
AudioConnection c5(mixSources, 0, formantFilter2, 0);
AudioConnection c6(mixSources, 0, formantFilter3, 0);

AudioConnection c7(mixSources, 0, mixFormants, 0);
AudioConnection c8(formantFilter1, 0, mixFormants, 1);
AudioConnection c9(formantFilter2, 0, mixFormants, 2);
AudioConnection c10(formantFilter3, 0, mixFormants, 3);

AudioConnection c11(mixFormants, 0, audioOutput, 0);

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


//___________________________________________________________________________________
/////////////////////////
//Light

int led = 5;           // the pin that the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 1;    // how many points to fade the LED by



//_________________________________________________________________________________
//////////////////////
//Ultrasonic aka IR


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
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  delay(3000);

	pinMode(Test1_PIN,INPUT_PULLUP);
	pinMode(Test0_PIN,INPUT_PULLUP);

	// It doesn't work properly with any less than 8
	AudioMemory(12);

	audioShield.enable();
	audioShield.inputSelect(myInput);
	audioShield.volume(90);
	
	mixFormants.gain(0, 0.0001);
	mixFormants.gain(1, 0.4);
	mixFormants.gain(2, 0.4);
	mixFormants.gain(3, 0.4);
	
	delayVolume = 2;
	
	mixSources.gain(0, delayVolume);
	mixSources.gain(1, 0.1);
	mixSources.gain(2, 0.1);
	mixSources.gain(3, 0.1);
	
	setSopranA();
	
	osc1.set_ramp_length(88);
	osc2.set_ramp_length(88);
	osc3.set_ramp_length(88);
	
	masterFormantSeq.push_back(formantSeq1);
	masterFormantSeq.push_back(formantSeq2);
	masterToneSeq.push_back(toneSeq1_1);
	masterToneSeq.push_back(toneSeq1_2);
	masterToneSeq.push_back(toneSeq1_3);
	masterToneSeq.push_back(toneSeq2_1);
	masterToneSeq.push_back(toneSeq2_2);
	masterToneSeq.push_back(toneSeq2_3);
	masterDelayStateSeq.push_back(delayStateSeq1);
	masterDelayStateSeq.push_back(delayStateSeq2);
	masterToneVolSeq.push_back(toneVolumeSeq1);
	masterToneVolSeq.push_back(toneVolumeSeq2);
	
	// osc1.begin(0.1,tune_frequencies2_PGM[30], TONE_TYPE_SQUARE);
	// osc2.begin(0.1,tune_frequencies2_PGM[37], TONE_TYPE_SQUARE);
	// osc3.begin(0.1,tune_frequencies2_PGM[44], TONE_TYPE_SQUARE);
	
	Serial.println("audio setup done");

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

  //___________________________________________________________________________________
  //LIGHT

  pinMode(led, OUTPUT);

}

//___________________________________________________________

//Metros
Metro LightMetro = Metro(400);
Metro ComMetro = Metro(1000);
Metro SensMetro = Metro(100);
Metro WriteMetro = Metro(500);
Metro MonitorMetro = Metro(1000);
Metro VolMetro = Metro(10);

Metro TimingMetro = Metro(MOD_RATE);

// audio volume
int mainVolume = 0;
int oldValue = 0;
int actFromSeq = 0;
int activeToneSeq = 0;
int actDelStateSeq = 0;
int actOscVolSeq = 0;



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
  //LIGHT

  if (LightMetro.check() == 1) {

    // set the brightness of pin 9:
    brightness = constrain(brightness, 0, 255);
    analogWrite(led, brightness);

    // change the brightness for next time through the loop:
    int fadeAmountPWR = 1;
    if (fadeAmount > 0) { 
      fadeAmountPWR = fadeAmount + (brightness * 0.1); 
    }
    else { 
      fadeAmountPWR = fadeAmount - (brightness * 0.1); 
    }
    brightness = brightness + fadeAmountPWR;
    // reverse the direction of the fading at the ends of the fade:

    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount ;
    }
  }

  //___________________________________________________________________________________
  //AUDIO
  // volume control
  // every 10 ms, check for adjustment
  // volume control
  
	// every 10 ms, check for adjustment
	if (VolMetro.check() == 1) {
		int n = analogRead(15);
		if (n != mainVolume) {
			mainVolume = n;
			audioShield.volume((float)n / 10.23);
		}
	}
	
	if (TimingMetro.check() == 1) {
		
		// formant filter sequence
		if (masterFormantSeq[actFromSeq].seqCounter < masterFormantSeq[actFromSeq].seqLength)
		{		
			masterFormantSeq[actFromSeq].seqProceed();
		}
		else
		{
			if (masterFormantSeq[actFromSeq].loop == true)
			{
				masterFormantSeq[actFromSeq].reset();
				masterFormantSeq[actFromSeq].seqProceed();
			}			
		}
				
		// osc1 sequence
		if (masterToneSeq[activeToneSeq].seqCounter < masterToneSeq[activeToneSeq].seqLength)
		{			
			masterToneSeq[activeToneSeq].seqProceed();
		}
		else
		{
			if (masterToneSeq[activeToneSeq].loop == true)
			{
				masterToneSeq[activeToneSeq].reset();
				masterToneSeq[activeToneSeq].seqProceed();
			}			
		}
		
		// osc2 sequence
		if (masterToneSeq[activeToneSeq+1].seqCounter < masterToneSeq[activeToneSeq+1].seqLength)
		{			
			masterToneSeq[activeToneSeq+1].seqProceed();
		}
		else
		{
			if (masterToneSeq[activeToneSeq+1].loop == true)
			{
				masterToneSeq[activeToneSeq+1].reset();
				masterToneSeq[activeToneSeq+1].seqProceed();
			}			
		}
		
		// osc3 sequence
		if (masterToneSeq[activeToneSeq+2].seqCounter < masterToneSeq[activeToneSeq+2].seqLength)
		{			
			masterToneSeq[activeToneSeq+2].seqProceed();
		}
		else
		{
			if (masterToneSeq[activeToneSeq+2].loop == true)
			{
				masterToneSeq[activeToneSeq+2].reset();
				masterToneSeq[activeToneSeq+2].seqProceed();
			}			
		}
		
		// delay state sequence
		if (masterDelayStateSeq[actDelStateSeq].seqCounter < masterDelayStateSeq[actDelStateSeq].seqLength)
		{	
			masterDelayStateSeq[actDelStateSeq].seqProceed();
		}
		else
		{
			if (masterDelayStateSeq[actDelStateSeq].loop == true)
			{
				masterDelayStateSeq[actDelStateSeq].reset();
				masterDelayStateSeq[actDelStateSeq].seqProceed();
			}			
		}
		
		
		// tone volume sequence
		if (masterToneVolSeq[actOscVolSeq].seqCounter < masterToneVolSeq[actOscVolSeq].seqLength)
		{	
			masterToneVolSeq[actOscVolSeq].seqProceed();
		}
		else
		{
			if (masterToneVolSeq[actOscVolSeq].loop == true)
			{
				masterToneVolSeq[actOscVolSeq].reset();
				masterToneVolSeq[actOscVolSeq].seqProceed();
			}			
		}

		
	}
	
	// update the two buttons
	b_test0.update();
	b_test1.update();


	// if pin1 is grounded
	if(b_test1.fallingEdge()) {
		//staticDelay.setLoopLength(AUDIO_BLOCK_SAMPLES * 32);
	}

	// If pin1 is open
	if(b_test1.risingEdge()) {
		//staticDelay.setLoopLength(AUDIO_BLOCK_SAMPLES * 64);
	}

	// if pin0 is grounded
	if(b_test0.fallingEdge()) {
		//staticDelay.hold(false);	
		//actFromSeq = 1;
	}
	// if pin 0 is open
	if(b_test0.risingEdge()) {
		//staticDelay.hold(true);	
		//actFromSeq = 0;
	}

  //////////////////
  //Mem and CPU Usage

  /*
  For PlaySynthMusic this produces:
   Proc = 20 (21),  Mem = 2 (8)
   */
  if (MonitorMetro.check() == 1) {
    Serial.print("Proc = ");
    Serial.print(AudioProcessorUsage());
    Serial.print(" (");    
    Serial.print(AudioProcessorUsageMax());
    Serial.print("),  Mem = ");
    Serial.print(AudioMemoryUsage());
    Serial.print(" (");    
    Serial.print(AudioMemoryUsageMax());
    Serial.println(")");
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
    //int ligthDur = 
    LightMetro.interval(map(valueRF, 0, 1023, 5, 500));
    LightMetro.reset();
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




