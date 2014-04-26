//#include <arm_math.h>
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Bounce.h>

//#include <Metro.h>

#define MOD_RATE 10 // modulation rate in ms
#define AMPLITUDE (0.4)

// Table of midi note frequencies * 2
//   They are times 2 for greater accuracy, yet still fits in a word.
//   Generated from Excel by =ROUND(2*440/32*(2^((x-9)/12)),0) for 0<x<128
// The lowest notes might not work, depending on the Arduino clock frequency

#include "Delay.h"
#include "FormantFilter.h"
#include "Tone.h"
#include "Sequences.h"

#define Test1_PIN 1
#define Test0_PIN 0



Bounce b_test1 = Bounce(Test1_PIN,15);
Bounce b_test0   = Bounce(Test0_PIN,15);


//const int myInput = AUDIO_INPUT_MIC;
const int myInput = AUDIO_INPUT_LINEIN;

AudioInputI2S       audioInput;         // audio shield: mic or line-in
AudioSynthWaveform 	osc1;
AudioSynthWaveform 	osc2;
//AudioSynthWaveform 	osc3;
// AudioSynthWaveform 	osc4;
//AudioMixer4			mixTones;
AudioMixer4			mixSources;
//Delay 				staticDelay;
AudioFilterBiquad   formantFilter1(ToneFilter1);
AudioFilterBiquad   formantFilter2(ToneFilter2);
AudioFilterBiquad   formantFilter3(ToneFilter3);
AudioMixer4        	mixFormants;
AudioOutputI2S     	audioOutput;        // audio shield: headphones & line-out

// Create Audio connections between the components
// Both channels of the audio input go to the FIR filter

AudioConnection c5(audioInput, 0, mixSources, 0);

AudioConnection c1(osc1, 0, mixSources, 1);
//AudioConnection c2(osc2, 0, mixTones, 1);
//AudioConnection c3(osc3, 0, mixTones, 2);
// AudioConnection c4(osc4, 0, mixTones, 3);

//AudioConnection c6(mixTones, 0, mixSources, 1);



//AudioConnection c7(mixSources, 0, staticDelay, 0);


AudioConnection c8(mixSources, 0, formantFilter1, 0);
AudioConnection c9(mixSources, 0, formantFilter2, 0);
AudioConnection c10(mixSources, 0, formantFilter3, 0);

AudioConnection c11(mixSources, 0, mixFormants, 0);
AudioConnection c12(formantFilter1, 0, mixFormants, 1);
AudioConnection c13(formantFilter2, 0, mixFormants, 2);
AudioConnection c14(formantFilter3, 0, mixFormants, 3);

AudioConnection c15(mixFormants, 0, audioOutput, 0);
AudioConnection c16(mixFormants, 0, audioOutput, 1);

AudioControlSGTL5000 audioShield;

int volume = 0;
int oldValue = 0;

int wheelValue = 0;

int activeFormantSeq = 0;
int activeToneSeq = 1;

// Metro MonitorMetro = Metro(1000);
// Metro ReadMetro = Metro(10);
// Metro TimingMetro = Metro(MOD_RATE);



void setup() {

	Serial.begin(9600);
	//while (!Serial) ;
	delay(3000);

	pinMode(Test1_PIN,INPUT_PULLUP);
	pinMode(Test0_PIN,INPUT_PULLUP);

	// It doesn't work properly with any less than 8
	AudioMemory(15);

	audioShield.enable();
	audioShield.inputSelect(myInput);
	audioShield.volume(90);
	
	mixFormants.gain(0, 0.0001);
	mixFormants.gain(1, 1);
	mixFormants.gain(2, 1);
	mixFormants.gain(3, 1);
	
	mixSources.gain(0, 1);
	mixSources.gain(1, 1);
	
	//mixTones.gain(0, 1);
	//mixTones.gain(1, 1);
	//mixTones.gain(2, 0.5);
	// mixTones.gain(3, 0.5);
	
	initToneSeqs();
	initFormantSeq();
	
	//osc1.begin(AMPLITUDE,tune_frequencies2_PGM[50], TONE_TYPE_SQUARE);
	
	masterFormantSeq[activeFormantSeq].seqProceed();
	
	Serial.println("setup done");
	
	Serial.print(masterToneSeq[activeToneSeq].seqCounter);
	Serial.print("\n");
	Serial.print(masterToneSeq.size());
	Serial.print("\n");
}

elapsedMillis chgMsec=0;
unsigned long last_time = millis();

void loop()
{
	//////////////////
	//Mem and CPU Usage  
	// Change this to if(1) for measurement output and to if(0) to stop it
	// if(1) {
		// if(MonitorMetro.check() == 1) {
			// Serial.print("Proc = ");
			// Serial.print(AudioProcessorUsage());
			// Serial.print(" (");    
			// Serial.print(AudioProcessorUsageMax());
			// Serial.print("),  Mem = ");
			// Serial.print(AudioMemoryUsage());
			// Serial.print(" (");    
			// Serial.print(AudioMemoryUsageMax());
			// Serial.println(")");
		// }
	// }
	// volume control
	// every 10 ms, check for adjustment
	// if (ReadMetro.check() == 1) {
		// wheelValue = analogRead(15);
		// if (wheelValue != volume) {
			// volume = wheelValue;
			// audioShield.volume((float)wheelValue / 10.23);
		// }
	// }
	
	// ** modulation routine **
	//if (TimingMetro.check() == 1) {
	if (millis() - last_time >= 10)
	{
		// Serial.print("check");
		// Serial.print("\n");
		//wheelValue = analogRead(15);
		// if (wheelValue != volume) {
		volume = wheelValue;
		//audioShield.volume((float)wheelValue / 10.23);
		// }
		
		AudioNoInterrupts();
		Serial.print("\counter: ");
		Serial.print(masterToneSeq[activeToneSeq].stepCounter);
		Serial.print("\n");
		Serial.print("\size: ");
		Serial.print(masterToneSeq.size());
		Serial.print("\n");
		Serial.print("\length: ");
		Serial.print(masterToneSeq[activeToneSeq].seqLength);
		Serial.print("\n");
		// osc1 sequence
		if (masterToneSeq[activeToneSeq].seqCounter <= masterToneSeq[activeToneSeq].seqLength)
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
		
		// // formant sequence
		// if (masterFormantSeq[activeFormantSeq].seqCounter <= masterFormantSeq[activeFormantSeq].seqLength)
		// {		
			// masterFormantSeq[activeFormantSeq].seqProceed();
		// }
		// else
		// {
			// if (masterFormantSeq[activeFormantSeq].loop == true)
			// {
				// masterFormantSeq[activeFormantSeq].reset();
				// masterFormantSeq[activeFormantSeq].seqProceed();
			// }			
		// }	
		
	last_time = millis();
	AudioInterrupts();
	}
	

	// update the two buttons
	b_test0.update();
	b_test1.update();


	// if pin1 is grounded
	if(b_test1.fallingEdge()) {

	}

	// If pin1 is open
	if(b_test1.risingEdge()) {

	}

	// if pin0 is grounded
	if(b_test0.fallingEdge()) {
		//staticDelay.hold(true);
		activeFormantSeq = 0;
	}
	// if pin 0 is open
	if(b_test0.risingEdge()) {
		//staticDelay.hold(false);
		activeFormantSeq = 1;
	}
}



