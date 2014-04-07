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

#include "firstAudioObject.h"

// If this pin is grounded the FIR filter is turned which
// makes just pass through the audio
// Don't use any of the pins listed above
#define PASSTHRU_PIN 1
// If this pin goes low the next FIR filter in the list
// is switched in.
#define FILTER_PIN 0

Bounce b_passthru = Bounce(PASSTHRU_PIN,15);
Bounce b_filter   = Bounce(FILTER_PIN,15);

const int myInput = AUDIO_INPUT_MIC;
//const int myInput = AUDIO_INPUT_LINEIN;


AudioInputI2S       audioInput;         // audio shield: mic or line-in
firstAudioOnject 	staticDelay;
AudioOutputI2S      audioOutput;        // audio shield: headphones & line-out

// Create Audio connections between the components
// Both channels of the audio input go to the FIR filter
// AudioConnection c1(audioInput, 0, audioOutput, 0);
AudioConnection c2(audioInput, 0, staticDelay, 0);
AudioConnection c3(staticDelay, 0, audioOutput, 0);

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
  audioShield.volume(50);
  
  // Warn that the passthru pin is grounded
  if(!digitalRead(PASSTHRU_PIN)) {
    Serial.print("PASSTHRU_PIN (");
    Serial.print(PASSTHRU_PIN);
    Serial.println(") is grounded");
  }

  // Warn that the filter pin is grounded
  if(!digitalRead(FILTER_PIN)) {
    Serial.print("FILTER_PIN (");
    Serial.print(FILTER_PIN);
    Serial.println(") is grounded");
  }
  
  Serial.println("setup done");
}

// audio volume
int volume = 0;

void loop()
{
	int cpuUsage = AudioProcessorUsageMax();
	
	Serial.print("cpuUsage: ");
	Serial.print(cpuUsage);
	Serial.print("%\n");
	
	delay(100);
	
  // Volume control
  int n = analogRead(15);
  if (n != volume) {
    volume = n;
    audioShield.volume((float)n / 10.23);
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



