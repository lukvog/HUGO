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

int LowPassFilter[]={
  0,0,0,0,0,0,0,0};

AudioInputI2S       audioInput;  // audio shield: mic or line-in
AudioPeak            peakMix;
AudioMixer4        inMix;
AudioEffectDelay 	staticDelay;
AudioSynthWaveform 	osc1;
AudioSynthWaveform 	osc2;
AudioSynthWaveform	osc3;
AudioMixer4			mixOsc;
AudioMixer4			mixSources;
AudioFilterBiquad    formantFilter1(ToneFilter1);
AudioFilterBiquad    formantFilter2(ToneFilter2);
AudioFilterBiquad    formantFilter3(ToneFilter3);
AudioMixer4        mixFormants;
AudioFilterBiquad	LowPass(LowPassFilter);
AudioPeak            peakMix2;
AudioOutputI2S      audioOutput;        // audio shield: headphones & line-out


// Create Audio connections between the components
// Both channels of the audio input go to the FIR filter

AudioConnection c1(audioInput, 0, staticDelay, 0);
AudioConnection c16(audioInput, 0, peakMix, 0);
AudioConnection c2(staticDelay, 0, inMix, 0);
AudioConnection c3(osc1, 0, mixOsc, 0);
AudioConnection c4(osc2, 0, mixOsc, 1);
AudioConnection c5(osc3, 0, mixOsc, 2);
AudioConnection c6(mixOsc, 0, mixSources, 0);
AudioConnection c7(inMix, 0, mixSources, 1);

AudioConnection c8(mixSources, 0, formantFilter1, 0);
AudioConnection c9(mixSources, 0, formantFilter2, 0);
AudioConnection c10(mixSources, 0, formantFilter3, 0);

AudioConnection c11(inMix, 0, mixFormants, 0);
AudioConnection c12(formantFilter1, 0, mixFormants, 1);
AudioConnection c13(formantFilter2, 0, mixFormants, 2);
AudioConnection c14(formantFilter3, 0, mixFormants, 3);
AudioConnection c15(mixFormants, 0, LowPass, 0);
AudioConnection c18(LowPass, 0, peakMix2, 0);
AudioConnection c17(LowPass, 0, audioOutput, 0);

AudioControlSGTL5000 audioShield;

float mainToneLevel = 4.0;
float mainInputLevel = 6.0;



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



//___________________________________________________________________________________
/////////////////////////
//Light

int bulb = 5;           // the pin that the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 1;    // how many points to fade the LED by
int lookUpIndexes = 31;


int pwmActual[] = {
  0, 1, 2, 3, 4, 5, 7, 9,
  12, 15, 18, 22, 27, 32, 37, 44,
  50, 58, 66, 75, 85, 96, 107, 120,
  133, 147, 163, 179, 196, 215, 234, 255
};

/*

 int pwmActual[] = {
 1,1,1,1,2,2,2,2,2,2,
 3,3,3,3,4,4,4,5,5,6,
 6,7,7,8,9,10,10,11,12,13,
 15,16,17,19,21,23,25,27,29,32,35,
 38,42,45,49,54,59,64,70,76,83,91,
 99,108,117,128,140,152,166,181,
 197,215,235,255
 };
 
 */


//smoothing
const int numReadingsL = 5;
int readingsL[numReadingsL];      // the readings from the analog input
int indxL = 0;                  // the index of the current reading
int totalL = 0;                  // the running total
int averageL = 0;    


//_________________________________________________________________________________
//////////////////////
//IR Sensor

int irPin = 21;
int changed;
int wallDist;
float irVal;
int changedOld;

//smoothing
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int indx = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average




//________________________________________________________________________________________
//___________________________________________________________________________________
////////////////////////////////////////

void setup() 
{

  Serial.begin(57600);

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
  }

  //Serial.println(count);

  for(int i=0; i < count; i++){
    sum += wall[i];
  }

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
  //radio.setDataRate(RF24_2MBPS);
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

  pinMode(bulb, OUTPUT);

  //___________________________________________________________________________________

  pinMode(Test1_PIN,INPUT_PULLUP);
  pinMode(Test0_PIN,INPUT_PULLUP);

  AudioMemory(15);

  audioShield.enable();

  audioShield.inputSelect(myInput);
  audioShield.volume(0.7);	

  // volumes 
  inMix.gain(0, 0.5);			// delay input :: controlled by input volume sequence (base level can be set in SeqDef.h as "InVolumeSeq.mainGainLevel")
  mixOsc.gain(0, 0.3);			// osc1
  mixOsc.gain(1, 0.3);			// osc2
  mixOsc.gain(2, 0.3);			// osc3
  mixSources.gain(0, 0.5);		// all osc's :: controlled by tone volume sequence (base level can be set in SeqDef.h as "ToneVolumeSeq.mainGainLevel")
  mixSources.gain(1, 0.5);		// delay			
  mixFormants.gain(0, 0.3);		// delay
  mixFormants.gain(1, 0.3);		// formant1
  mixFormants.gain(2, 0.3);		// formant2
  mixFormants.gain(3, 0.3);		// formant3

    // osc's init values
  osc1.set_ramp_length(88);
  osc2.set_ramp_length(88);
  osc3.set_ramp_length(88);
  osc1.amplitude(0);
  osc2.amplitude(0);
  osc3.amplitude(0);

  // load Sequence arrays
  setLPSeq();
  setToneVolSeq();
  setInVolSeq();
  setDelayStateSeq();
  setDelayLoopLengthSeq();
  setFormantSeq();
  setToneSeq();

  // set standard values for test cases when disable sequences
  // formant fiter
  setSopranO();
  // delay loopLength
  staticDelay.setLoopLength(AUDIO_BLOCK_SAMPLES * 65); // equal the delayBufferLength of 8192 + AUDIO_BLOCK_SAMPLES
  // lowPass
  calcBiquad(FILTER_LOPASS,10000,0,0.5,2147483648,44100,updateFilter);
  LowPass.updateCoefs(updateFilter);
  // osc's
  osc1.begin(0.1,tune_frequencies2_PGM[30], TONE_TYPE_SQUARE);
  osc2.begin(0.1,tune_frequencies2_PGM[37], TONE_TYPE_SQUARE);
  osc3.begin(0.1,tune_frequencies2_PGM[44], TONE_TYPE_SQUARE);

  Serial.println("audio setup done");
}



//________________________________________________________________________________________
////////////////////////////////////////


//Metros
Metro LightMetro = Metro(50);
Metro ComMetro = Metro(1000);
Metro SensMetro = Metro(100);
Metro PrintOutMetro = Metro(500);
Metro MonitorMetro = Metro(1000);
Metro VolMetro = Metro(10);
Metro ForgetMetro = Metro(200);

Metro TimingMetro = Metro(MOD_RATE);

// audio volume
float mainVolume = 0;
int oldValue = 0;
float prox = 0.0;
float mainVolumeChange;

// sequence indices
int actLPSeq = 0;
int actFromSeq = 0;
int actToneSeq = 0; //tone * 3
int actDelStateSeq = 0;
int actDelLoLeSeq = 0;
int actToneVolSeq = 0;
int actInVolSeq = 0;

bool seqReset = true;


//////////////////////
//Peak
uint8_t inputVolume = 0.0;



//_________________________________________________________________________________________
//_________________________________________________________________________________________
/////////////////////////////////////////////


void loop() {


  /////////////////////////////
  //___________________________________________________________________________________
  //PrintOut aka. Debug

  if (PrintOutMetro.check() == 1) {
    Serial.print("prox: ");
    Serial.println(prox);
    Serial.print("mainVol: ");
    Serial.println(mainVolume);
    Serial.print("wallDist: ");
    Serial.println(wallDist, DEC);
    Serial.print("changed: ");
    Serial.println(changed, DEC);
  }
  
  
  //___________________________________________________________________________________
  //LIGHT

  if (LightMetro.check() == 1) {


    int factor = 100;	// with this factor, the amount of the ligth ducking can be set (bigger values -> more ducking)
    uint8_t brightness=peakMix2.Dpp()/256;
    brightness = factor * log10(brightness);
    //brightness = pwmActual[brightness];

    // subtract the last reading:
    totalL= totalL - readingsL[indxL];        
    // read from the sensor:  
    readingsL[indxL] = brightness;
    // add the reading to the total:
    totalL= totalL + readingsL[indxL];      
    // advance to the next position in the array:  
    indxL = indxL + 1;                    
    // if we're at the end of the array...
    if (indxL >= numReadingsL)              
      // ...wrap around to the beginning:
      indxL = 0;                          
    // calculate the average:
    averageL = totalL / numReadingsL; 
    //Serial.println(averageL);

    //----> scaling factor!
    int mappedMax = 255 * (mainVolumeChange * 0.5);
    //Serial.println(mappedMax);
    int mapped = map(averageL, 0, 100, 0, mappedMax);
    mapped = constrain(mapped, 5, 255);
    //Serial.println(mapped);
    analogWrite(bulb, mapped);
    //Serial.println(mapped);
    peakMix2.begin();

    /*

     // change the brightness for next time through the loop:
     brightness = brightness + fadeAmount;
     // reverse the direction of the fading at the ends of the fade:
     if (brightness == 0 || brightness == lookUpIndexes) {
     fadeAmount = -fadeAmount ;
     }
     analogWrite(bulb, brightness);
     
     */

    //LightMetro.interval(map(valueRF, 0, 1023, 5, 500));
    //LightMetro.reset();

  }

  /////////////////////////////
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

    //Serial.print("wallDist: ");
    //Serial.println(wallDist, DEC);
    //Serial.print("changed: ");
    //Serial.println(changed, DEC);
  }



  if (ForgetMetro.check() == 1) {

    if (changed != changedOld) {
      int changedInv = map(changed, 0, 550, 0, 550);
      prox = (changed - 50.0) / 1000.0;
      //Serial.println(prox);
      changedOld = changed;
    }

    if (prox > 0.0) {
      prox = prox - 0.008;
    };


  }

  //___________________________________________________________________________________
  //AUDIO
  // volume control
  // every 10 ms, check for adjustment
  // volume control

  if (VolMetro.check() == 1) {
    int vol = analogRead(15);
    if (vol != mainVolume) {
      mainVolume = vol / 1023.0;
      mainVolumeChange = mainVolume + prox;
      audioShield.volume(mainVolumeChange);
      mainVolume = vol;
    }
  }

  if (TimingMetro.check() == 1) {

    // detect input volume
    float factor = 2.0;	// with this factor, the amount of the ducking can be set (bigger values -> more ducking)
    float tempInputVol = peakMix.Dpp()/1024.0;
    inputVolume = factor * log10(tempInputVol);
    //Serial.print(inputVolume);
    //Serial.print("\n");
    peakMix.begin();

    // In volume sequence
    if ((masterInVolSeq[actInVolSeq]->seqCounter >= masterInVolSeq[actInVolSeq]->seqLength) || seqReset)
    {
      if ((masterInVolSeq[actInVolSeq]->loop == true) || seqReset)
      {
        masterInVolSeq[actInVolSeq]->reset();
        masterInVolSeq[actInVolSeq]->seqProceed();
      }			
    }
    else
    {	
      masterInVolSeq[actInVolSeq]->seqProceed();
    }

    // tone volume sequence
    if ((masterToneVolSeq[actToneVolSeq]->seqCounter >= masterToneVolSeq[actToneVolSeq]->seqLength) || seqReset)
    {
      if ((masterToneVolSeq[actToneVolSeq]->loop == true) || seqReset)
      {
        masterToneVolSeq[actToneVolSeq]->reset();
        masterToneVolSeq[actToneVolSeq]->seqProceed();
      }			
    }
    else
    {	
      masterToneVolSeq[actToneVolSeq]->seqProceed();
    }

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
   /*
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
  
  */

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
  int message;
  network.read(header,&message,sizeof(int));
  Serial.printf_P(PSTR("%lu: APP Received Value %lu from 0%o\n\r"),millis(),message,header.from_node);
  // set sequence indices
  actLPSeq = message;
  actFromSeq = message;
  actToneSeq = message*3;
  actDelStateSeq = message;
  actDelLoLeSeq = message;
  actToneVolSeq = message;
  actInVolSeq = message;
  seqReset = true;
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
















