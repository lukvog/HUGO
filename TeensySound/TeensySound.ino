/*

The audio board uses the following pins.
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

*/

//#include <arm_math.h>
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Bounce.h>

#include "Delay.h"

// If this pin is grounded the FIR filter is turned which
// makes just pass through the audio
// Don't use any of the pins listed above
#define PASSTHRU_PIN 1
// If this pin goes low the next FIR filter in the list
// is switched in.
#define FILTER_PIN 0

Bounce b_passthru = Bounce(PASSTHRU_PIN,15);
Bounce b_filter   = Bounce(FILTER_PIN,15);

int ToneFilter[]={0,0,0,0,0,0,0,0x80000000,0,0,0,0,0,0,0,0x80000000,0,0,0,0,0,0,0,0}; 	// defines 2 sets of coefficients, not sure max possible in
																// time frame but probably quite a few.

int updateFilter[5];

const int myInput = AUDIO_INPUT_MIC;
//const int myInput = AUDIO_INPUT_LINEIN;


AudioInputI2S       audioInput;         // audio shield: mic or line-in
Delay 	staticDelay;
AudioFilterBiquad    formantFilter(ToneFilter);
AudioOutputI2S      audioOutput;        // audio shield: headphones & line-out

// Create Audio connections between the components
// Both channels of the audio input go to the FIR filter
AudioConnection c2(audioInput, 0, staticDelay, 0);
AudioConnection c3(staticDelay, 0, formantFilter, 0);
AudioConnection c4(formantFilter, 0, audioOutput, 0);
AudioConnection c5(formantFilter, 0, audioOutput, 1);

AudioControlSGTL5000 audioShield;


void setup() {
  
  Serial.begin(9600);
  while (!Serial) ;
  delay(3000);

  pinMode(PASSTHRU_PIN,INPUT_PULLUP);
  pinMode(FILTER_PIN,INPUT_PULLUP);

  // It doesn't work properly with any less than 8
  AudioMemory(12);

  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(90);
  
  calcBiquad(FILTER_BANDPASS,450,0,60,2147483648,44100,updateFilter);
  formantFilter.updateCoefs(updateFilter); // default set updateCoefs(0,updateFilter);
  calcBiquad(FILTER_BANDPASS,800,-11,90,2147483648,44100,updateFilter);
  formantFilter.updateCoefs(1,updateFilter);
  calcBiquad(FILTER_BANDPASS,2830,-22,100,2147483648,44100,updateFilter);
  formantFilter.updateCoefs(2,updateFilter);
  
  Serial.println("setup done");
}

// audio volume
int volume = 0;

unsigned long last_time = millis();
elapsedMillis chgMsec=0;
int value1 = 0;
  
void loop()
{
  //////////////////
  //Mem and CPU Usage  
  // Change this to if(1) for measurement output and to if(0) to stop it
  if(1) {
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
	
  // // Volume control
  // int n = analogRead(15);
  // if (n != volume) {
    // volume = n;
    // audioShield.volume((float)n / 10.23);
  // }
  
    // every 10 ms, check for adjustment
  if (chgMsec > 10) {
    
    int value=analogRead(15);
	
    if(value!=value1)
    {
	if (value < 500)
		{
		// sopran a
			// calcBiquad(FilterType,FrequencyC,dBgain,BW,QuantizationUnit,SampleRate,int*);
			calcBiquad(FILTER_BANDPASS,450,0,1,2147483648,44100,updateFilter);
			formantFilter.updateCoefs(updateFilter); // default set updateCoefs(0,updateFilter);
			calcBiquad(FILTER_BANDPASS,800,-11,1,2147483648,44100,updateFilter);
			formantFilter.updateCoefs(1,updateFilter);
			calcBiquad(FILTER_BANDPASS,2830,-22,1,2147483648,44100,updateFilter);
			formantFilter.updateCoefs(2,updateFilter);	
		}
		else
		{
		// alt i
			// calcBiquad(FilterType,FrequencyC,dBgain,BW,QuantizationUnit,SampleRate,int*);
			calcBiquad(FILTER_BANDPASS,350,0,1,2147483648,44100,updateFilter);
			formantFilter.updateCoefs(updateFilter); // default set updateCoefs(0,updateFilter);
			calcBiquad(FILTER_BANDPASS,1700,-20,1,2147483648,44100,updateFilter);
			formantFilter.updateCoefs(1,updateFilter);
			calcBiquad(FILTER_BANDPASS,2700,-30,1,2147483648,44100,updateFilter);
			formantFilter.updateCoefs(2,updateFilter);	
		}
      
      value1=value;
    }
    chgMsec = 0;
  }
  
  // update the two buttons
  b_passthru.update();
  b_filter.update();
  

  
  // If the passthru button is pushed, save the current
  // filter index and then switch the filter to passthru
  if(b_passthru.fallingEdge()) {

  }
  
  // If passthru button is released, restore previous filter
  if(b_passthru.risingEdge()) {

  }
  
  // switch to next filter in the list
  if(b_filter.fallingEdge()) {
		staticDelay.hold(true);
  }
  
   if(b_filter.risingEdge()) {
		staticDelay.hold(false);
  }
}



