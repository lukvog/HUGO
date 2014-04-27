//#include <arm_math.h>
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Bounce.h>

#include <Metro.h>

#define MOD_RATE 10 // modulation rate in ms

#include "Delay.h"
#include "FormantFilter.h"
#include "Tone.h"
#include "Sequences.h"

#define Test1_PIN 1
#define Test0_PIN 0



Bounce b_test1 = Bounce(Test1_PIN,15);
Bounce b_test0   = Bounce(Test0_PIN,15);


const int myInput = AUDIO_INPUT_MIC;
//const int myInput = AUDIO_INPUT_LINEIN;


AudioInputI2S       audioInput;         // audio shield: mic or line-in
Delay 	staticDelay;
AudioSynthWaveform 	osc1;
AudioSynthWaveform 	osc2;
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
AudioConnection c4(mixSources, 0, formantFilter1, 0);
AudioConnection c5(mixSources, 0, formantFilter2, 0);
AudioConnection c6(mixSources, 0, formantFilter3, 0);

AudioConnection c7(mixSources, 0, mixFormants, 0);
AudioConnection c8(formantFilter1, 0, mixFormants, 1);
AudioConnection c9(formantFilter2, 0, mixFormants, 2);
AudioConnection c10(formantFilter3, 0, mixFormants, 3);

AudioConnection c11(mixFormants, 0, audioOutput, 0);

AudioControlSGTL5000 audioShield;


void setup() {

	Serial.begin(9600);
	//while (!Serial) ;
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
	
	mixSources.gain(0, 2);
	mixSources.gain(1, 0.1);
	mixSources.gain(2, 0.1);
	
	setSopranA();
	//osc1.begin(0.4,tune_frequencies2_PGM[60], TONE_TYPE_SQUARE);
	
	masterFormantSeq.push_back(firstFormantSeq);
	masterFormantSeq.push_back(secondFormantSeq);
	masterToneSeq.push_back(toneSeq1_1);
	masterToneSeq.push_back(toneSeq1_2);
	masterToneSeq.push_back(toneSeq2_1);
	masterToneSeq.push_back(toneSeq2_2);
	
	osc1.set_ramp_length(88);
	osc2.set_ramp_length(88);

	Serial.println("setup done");
}

// audio volume
int mainVolume = 0;
int oldValue = 0;
int activeSeq = 0;
int activeToneSeq = 0;

Metro MonitorMetro = Metro(1000);
Metro ReadMetro = Metro(10);
Metro TimingMetro = Metro(MOD_RATE);

void loop()
{
	//////////////////
	//Mem and CPU Usage  
	// Change this to if(1) for measurement output and to if(0) to stop it
	if(1) {
		if(MonitorMetro.check() == 1) {
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
	}
	// volume control
	// every 10 ms, check for adjustment
	if (ReadMetro.check() == 1) {
		int n = analogRead(15);
		if (n != mainVolume) {
			mainVolume = n;
			audioShield.volume((float)n / 10.23);
		}
	}
	
	if (TimingMetro.check() == 1) {
	
		if (masterFormantSeq[activeSeq].seqCounter <= masterFormantSeq[activeSeq].seqLength)
		{		
			masterFormantSeq[activeSeq].seqProceed();
		}
		else
		{
			if (masterFormantSeq[activeSeq].loop == true)
			{
				masterFormantSeq[activeSeq].reset();
				masterFormantSeq[activeSeq].seqProceed();
			}			
		}
		
		
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
		
		// osc2 sequence
		if (masterToneSeq[activeToneSeq+1].seqCounter <= masterToneSeq[activeToneSeq].seqLength)
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
		staticDelay.hold(true);
		activeSeq = 0;
	}
	// if pin 0 is open
	if(b_test0.risingEdge()) {
		staticDelay.hold(false);
		activeSeq = 1;
	}
}



