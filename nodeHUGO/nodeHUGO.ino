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
#include "SeqDef.h"
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

int LowPassFilter[]={0,0,0,0,0,0,0,0};

AudioInputI2S       audioInput;  // audio shield: mic or line-in 
AudioEffectDelay 	staticDelay;
AudioSynthWaveform 	osc1;
AudioSynthWaveform 	osc2;
AudioSynthWaveform	osc3;
AudioMixer4			mixSources;
AudioFilterBiquad    formantFilter1(ToneFilter1);
AudioFilterBiquad    formantFilter2(ToneFilter2);
AudioFilterBiquad    formantFilter3(ToneFilter3);
AudioMixer4        mixFormants;
AudioFilterBiquad	LowPass(LowPassFilter);
AudioOutputI2S      audioOutput;        // audio shield: headphones & line-out

// Create Audio connections between the components
// Both channels of the audio input go to the FIR filter

AudioConnection c15(audioInput, 0, staticDelay, 0);

AudioConnection c5(staticDelay, 0, mixSources, 3);

AudioConnection c2(osc1, 0, mixSources, 0);
AudioConnection c3(osc2, 0, mixSources, 1);
AudioConnection c4(osc3, 0, mixSources, 2);

AudioConnection c6(mixSources, 0, formantFilter1, 0);
AudioConnection c7(mixSources, 0, formantFilter2, 0);
AudioConnection c16(mixSources, 0, formantFilter3, 0);

AudioConnection c8(staticDelay, 0, mixFormants, 0);
AudioConnection c9(formantFilter1, 0, mixFormants, 1);
AudioConnection c10(formantFilter2, 0, mixFormants, 2);
AudioConnection c17(formantFilter3, 0, mixFormants, 3);
AudioConnection c18(mixFormants, 0, LowPass, 0);
AudioConnection c11(LowPass, 0, audioOutput, 0);

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
//IR Sensor

int irPin = 21;
int changed;
int wallDist;
float irVal;

//smoothing
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int indx = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average



//___________________________________________________________________________________
void setup() 
{

  Serial.begin(57600);
  
    //___________________________________________________________________________________
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  //delay(3000);

	pinMode(Test1_PIN,INPUT_PULLUP);
	pinMode(Test0_PIN,INPUT_PULLUP);

	// It doesn't work properly with any less than 8
	
	AudioMemory(15);
	
	audioShield.enable();
	
	audioShield.inputSelect(myInput);
	audioShield.volume(0.7);	
	
	mixFormants.gain(0, 0.5);
	mixFormants.gain(1, 0.5);
	mixFormants.gain(2, 0.5);
	mixFormants.gain(3, 0.5);	
	
	delayVolume = 2;
	
	mixSources.gain(0, 0.1);
	mixSources.gain(1, 0.1);
	mixSources.gain(2, 0.1);
	mixSources.gain(3, delayVolume);
	
	setSopranO();
	
	osc1.set_ramp_length(88);
	osc2.set_ramp_length(88);
	osc3.set_ramp_length(88);
	
	osc1.amplitude(0);
	osc2.amplitude(0);
	osc3.amplitude(0);
	
	staticDelay.setLoopLength(AUDIO_BLOCK_SAMPLES * 65); // equal the delayBufferLength of 8192 + AUDIO_BLOCK_SAMPLES
	
	calcBiquad(FILTER_LOPASS,1000,0,0.5,2147483648,44100,updateFilter);
	LowPass.updateCoefs(updateFilter);
	
	setLPSeq();
	setToneVolSeq();
	setDelayStateSeq();
	setDelayLoopLengthSeq();
	setFormantSeq();
	setToneSeq();
	
	// osc1.begin(0.1,tune_frequencies2_PGM[30], TONE_TYPE_SQUARE);
	// osc2.begin(0.1,tune_frequencies2_PGM[37], TONE_TYPE_SQUARE);
	// osc3.begin(0.1,tune_frequencies2_PGM[44], TONE_TYPE_SQUARE);
	
	Serial.println("audio setup done");

  //___________________________________________________________________________________
  //IR Sensor

  //calibration for Walls
  int wall[100];
  int count = 0;
  int sum = 0;

  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0; 

  //calibration for Walls

  for(int i=0; i < 100; i++){
    irVal = analogRead(irPin);
    // Linearize Sharp GP2YOA1YK
    float irDist = 11.0e8 * pow(irVal, -2.438);
    //float irDist = 1000/irVal;

    if (irDist >= 100 && irDist <= 550)  
    {

      wall[count] = irDist;
      Serial.print(count);
      Serial.print("range: ");
      Serial.println(irDist, DEC);
      count++;
    }
    delay(50);
  };

  //Serial.println(count);

  for(int i=0; i < count; i++){
    sum += wall[i];
  };

  //Serial.println(sum);

  wallDist = (sum/count) - 30;
  if (wallDist < 50)
  {
    wallDist = 550;
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
float mainVolume = 0;
int oldValue = 0;

int actLPSeq = 0;
int actFromSeq = 0;
int actToneSeq = 3;
int actDelStateSeq = 0;
int actDelLoLeSeq = 0;
int actOscVolSeq = 0;

bool seqReset = false;



void loop() {

  //___________________________________________________________________________________
  //IR Sensor

  if (SensMetro.check() == 1) {
  irVal = analogRead(irPin);
  // Linearize Sharp GP2YOA1YK
  float irDist = 11.0e8 * pow(irVal, -2.438);
  //Serial.println(irVal, DEC);

  //float irDist = 1000/irVal;

  // subtract the last reading:
  total= total - readings[indx];        
  // read from the sensor:  
  readings[indx] = irDist;
  // add the reading to the total:
  total= total + readings[indx];      
  // advance to the next position in the array:  
  indx = indx + 1;                    

  // if we're at the end of the array...
  if (indx >= numReadings)              
    // ...wrap around to the beginning:
    indx = 0;                          

  // calculate the average:
  average = total / numReadings; 

  //Serial.println(irDist, DEC);

    if (average >= 50 && average <= wallDist)
    {
      changed = average;
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
		int vol = analogRead(15);
		if (vol != mainVolume) {
			mainVolume = vol / 1023.0;
			audioShield.volume(mainVolume);
		}
	}
	
	if (TimingMetro.check() == 1) {
	
		// LP filter sequence
		if ((masterLPSeq[actLPSeq]->seqCounter >= masterLPSeq[actLPSeq]->seqLength) || seqReset)
		{
			if ((masterLPSeq[actLPSeq]->loop == true) || seqReset)
			{
				masterLPSeq[actLPSeq]->reset();
				masterLPSeq[actLPSeq]->seqProceed();
			}	
		}
		else
		{		
			masterLPSeq[actLPSeq]->seqProceed();
		}
		
		// formant filter sequence
		if ((masterFormantSeq[actFromSeq]->seqCounter >= masterFormantSeq[actFromSeq]->seqLength) || seqReset)
		{
			if ((masterFormantSeq[actFromSeq]->loop == true) || seqReset)
			{
				masterFormantSeq[actFromSeq]->reset();
				masterFormantSeq[actFromSeq]->seqProceed();
			}			
		}
		else		
		{		
			masterFormantSeq[actFromSeq]->seqProceed();
		}
		
		// osc1 sequence
		if ((masterToneSeq[actToneSeq]->seqCounter >= masterToneSeq[actToneSeq]->seqLength) || seqReset)
		{
			if ((masterToneSeq[actToneSeq]->loop == true) || seqReset)
			{
				masterToneSeq[actToneSeq]->reset();
				masterToneSeq[actToneSeq]->seqProceed();
			}			
		}
		else
		{			
			masterToneSeq[actToneSeq]->seqProceed();
		}
		
		// osc2 sequence
		if ((masterToneSeq[actToneSeq+1]->seqCounter >= masterToneSeq[actToneSeq+1]->seqLength) || seqReset)
		{
			if ((masterToneSeq[actToneSeq+1]->loop == true) || seqReset)
			{
				masterToneSeq[actToneSeq+1]->reset();
				masterToneSeq[actToneSeq+1]->seqProceed();
			}			
		}
		else
		{			
			masterToneSeq[actToneSeq+1]->seqProceed();
		}
		
		// osc3 sequence
		if ((masterToneSeq[actToneSeq+2]->seqCounter >= masterToneSeq[actToneSeq+2]->seqLength) || seqReset)
		{
			if ((masterToneSeq[actToneSeq+2]->loop == true) || seqReset)
			{
				masterToneSeq[actToneSeq+2]->reset();
				masterToneSeq[actToneSeq+2]->seqProceed();
			}			
		}
		else
		{			
			masterToneSeq[actToneSeq+2]->seqProceed();
		}
		
		// delay state sequence
		if ((masterDelayStateSeq[actDelStateSeq]->seqCounter >= masterDelayStateSeq[actDelStateSeq]->seqLength) || seqReset)
		{
			if ((masterDelayStateSeq[actDelStateSeq]->loop == true) || seqReset)
			{
				masterDelayStateSeq[actDelStateSeq]->reset();
				masterDelayStateSeq[actDelStateSeq]->seqProceed();
			}			
		}
		else
		{	
			masterDelayStateSeq[actDelStateSeq]->seqProceed();
		}
		
		// delay loop length sequence
		if ((masterDelayLoopLengthSeq[actDelLoLeSeq]->seqCounter >= masterDelayLoopLengthSeq[actDelLoLeSeq]->seqLength) || seqReset)
		{
			if ((masterDelayLoopLengthSeq[actDelLoLeSeq]->loop == true) || seqReset)
			{
				masterDelayLoopLengthSeq[actDelLoLeSeq]->reset();
				masterDelayLoopLengthSeq[actDelLoLeSeq]->seqProceed();
			}			
		}		
		else
		{	
			masterDelayLoopLengthSeq[actDelLoLeSeq]->seqProceed();
		}
		
		// tone volume sequence
		if ((masterToneVolSeq[actOscVolSeq]->seqCounter >= masterToneVolSeq[actOscVolSeq]->seqLength) || seqReset)
		{
			if ((masterToneVolSeq[actOscVolSeq]->loop == true) || seqReset)
			{
				masterToneVolSeq[actOscVolSeq]->reset();
				masterToneVolSeq[actOscVolSeq]->seqProceed();
			}			
		}
		else
		{	
			masterToneVolSeq[actOscVolSeq]->seqProceed();
		}
	seqReset = false;
	}
	
	// // update the two buttons
	// b_test0.update();
	// b_test1.update();


	// // if pin1 is grounded
	// if(b_test1.fallingEdge()) {
		// //staticDelay.setLoopLength(AUDIO_BLOCK_SAMPLES * 32);
	// }

	// // If pin1 is open
	// if(b_test1.risingEdge()) {
		// //staticDelay.setLoopLength(AUDIO_BLOCK_SAMPLES * 64);
	// }

	// // if pin0 is grounded
	// if(b_test0.fallingEdge()) {
		// staticDelay.hold(true);	
		// //actFromSeq = 1;
	// }
	// // if pin 0 is open
	// if(b_test0.risingEdge()) {
		// staticDelay.hold(false);	
		// //actFromSeq = 0;
	// }

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
	case 'S':
      handle_S(header);
      break;
    // case 'T':
      // handle_T(header);
      // break;
    // case 'V':
      // handle_V(header); 

      // /////
      // //Switch um Liste aufzubrechen
      // //Switch für Sensor und Light Mapping

      // break;
    // case 'N':
      // handle_N(header);
      // break;
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


  // // Send a ping to the next node every 'interval' ms
  // //unsigned long now = millis();
  // if (ComMetro.check() == 1) {
    // //last_time_sent = now;

    // // Who should we send to?
    // // By default, send to base
    // uint16_t to = 00;

    // // Or if we have active nodes,
    // if ( num_active_nodes )
    // {
      // // Send to the next active node
      // to = active_nodes[next_ping_node_index++];
      // if (to == this_node)
      // {
        // to = active_nodes[next_ping_node_index++];
      // }
      // // Have we rolled over?
      // if ( next_ping_node_index > num_active_nodes )
      // {
        // // Next time start at the beginning
        // next_ping_node_index = 0;

        // // This time, send to node 00.
        // to = 00;
      // }
    // }

    // bool ok;

    // // Normal nodes send a 'T' ping
    // if ( this_node > 00 ) {
      // //if ( this_node > 00 || to == 00 )
      // ok = send_T(to);

      // // Notify us of the result
      // if (ok)
      // {
        // Serial.printf_P(PSTR("%lu: APP Send ok\n\r"),millis());
      // }
      // else
      // {
        // Serial.printf_P(PSTR("%lu: APP Send failed\n\r"),millis());
      // }
    // }
    // // Base node sends the current active nodes out
    // else {
      // ok = send_N(to);

      // // Notify us of the result
      // if (ok)
      // {
        // Serial.printf_P(PSTR("%lu: APP Send ok\n\r"),millis());
      // }
      // else
      // {
        // Serial.printf_P(PSTR("%lu: APP Send failed\n\r"),millis());

        // // Try sending at a different time next time
        // //last_time_sent -= 100;
      // }

      // ok = send_V(to);
      // // Notify us of the result
      // if (ok)
      // {
        // Serial.printf_P(PSTR("%lu: APP Send ok\n\r"),millis());
      // }
      // else
      // {
        // Serial.printf_P(PSTR("%lu: APP Send failed\n\r"),millis());

        // // Try sending at a different time next time
        // //last_time_sent -= 100;
      // }
    // }
  // }


  // Listen for a new node address
  nodeconfig_listen();
}

/**
 * Handle a 'S' message
 *
 */
void handle_S(RF24NetworkHeader& header)
{
  // The 'V' contains values
  int message;
  network.read(header,&message,sizeof(int));
  Serial.printf_P(PSTR("%lu: APP Received Value %lu from 0%o\n\r"),millis(),message,header.from_node);
}


// //____________________________________________________

// /**
 // * Send a 'T' message, the current time
 // */
// bool send_T(uint16_t to)
// {
  // RF24NetworkHeader header(/*to node*/ to, /*type*/ 'T' /*Time*/);

  // // The 'T' message that we send is just a ulong, containing the time
  // unsigned long message = millis();
  // Serial.printf_P(PSTR("---------------------------------\n\r"));
  // Serial.printf_P(PSTR("%lu: APP Sending %lu to 0%o...\n\r"),millis(),message,to);
  // return network.write(header,&message,sizeof(unsigned long));
// }



// /**
 // * Send a 'V' message, a value
 // */


// bool send_V(uint16_t to)
// {
  // RF24NetworkHeader header(/*to node*/ to, /*type*/ 'V' /*Time*/);

  // // The 'V' message is a value message
  // unsigned int message = analogRead(15);
  // Serial.printf_P(PSTR("---------------------------------\n\r"));
  // Serial.printf_P(PSTR("%lu: APP Sending Value %lu to 0%o...\n\r"),millis(),message,to);
  // return network.write(header,&message,sizeof(unsigned int));
// }



// /**
 // * Send an 'N' message, the active node list
 // */
// bool send_N(uint16_t to)
// {
  // RF24NetworkHeader header(/*to node*/ to, /*type*/ 'N' /*Time*/);

  // Serial.printf_P(PSTR("---------------------------------\n\r"));
  // Serial.printf_P(PSTR("%lu: APP Sending active nodes to 0%o...\n\r"),millis(),to);
  // return network.write(header,active_nodes,sizeof(active_nodes));
// }

// /**
 // * Handle a 'T' message
 // *
 // * Add the node to the list of active nodes
 // */
// void handle_T(RF24NetworkHeader& header)
// {
  // // The 'T' message is just a ulong, containing the time
  // unsigned long message;
  // network.read(header,&message,sizeof(unsigned long));
  // Serial.printf_P(PSTR("%lu: APP Received Time %lu from 0%o\n\r"),millis(),message,header.from_node);

  // // If this message is from ourselves or the base, don't bother adding it to the active nodes.
  // if ( header.from_node != this_node || header.from_node > 00 )
    // add_node(header.from_node);
// }

// /**
 // * Handle a 'V' message
 // *
 // * Add the node to the list of active nodes
 // */
// void handle_V(RF24NetworkHeader& header)
// {
  // // The 'V' contains values
  // unsigned int message;
  // network.read(header,&message,sizeof(unsigned int));
  // Serial.printf_P(PSTR("%lu: APP Received Value %lu from 0%o\n\r"),millis(),message,header.from_node);
  // valueRF = message;

  // // If this message is from ourselves or the base, don't bother adding it to the active nodes.
  // if ( header.from_node != this_node || header.from_node > 00 )
    // add_node(header.from_node);
// }


// /**
 // * Handle an 'N' message, the active node list
 // */
// void handle_N(RF24NetworkHeader& header)
// {
  // static uint16_t incoming_nodes[max_active_nodes];

  // network.read(header,&incoming_nodes,sizeof(incoming_nodes));
  // Serial.printf_P(PSTR("%lu: APP Received nodes from 0%o\n\r"),millis(),header.from_node);

  // int i = 0;
  // while ( i < max_active_nodes && incoming_nodes[i] > 00 )
    // add_node(incoming_nodes[i++]);
// }

// /**
 // * Add a particular node to the current list of active nodes
 // */
// void add_node(uint16_t node)
// {
  // // Do we already know about this node?
  // short i = num_active_nodes;
  // while (i--)
  // {
    // if ( active_nodes[i] == node )
      // break;
  // }
  // // If not, add it to the table
  // if ( i == -1 && num_active_nodes < max_active_nodes )
  // {
    // active_nodes[num_active_nodes++] = node; 
    // Serial.printf_P(PSTR("%lu: APP Added 0%o to list of active nodes.\n\r"),millis(),node);
  // }
// }




