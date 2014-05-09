#include <Audio.h>
#include <Wire.h>
#include <stdio.h>
#include <SPI.h>
#include <SD.h>

#include <Metro.h>
#include "Tone.h"

//___________________________________________________________________________________
/////////////////////////
//AUDIO CONFIGURATION

int ToneFilter1[]={0,0,0,0,0,0,0};
int ToneFilter2[]={0,0,0,0,0,0,0};
int ToneFilter3[]={0,0,0,0,0,0,0};

int updateFilter[5];

//const int myInput = AUDIO_INPUT_LINEIN;
const int myInput = AUDIO_INPUT_MIC;

AudioInputI2S       audioInput;  // audio shield: mic or line-in 

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

AudioConnection c1(audioInput, 0, mixSources, 0);

AudioConnection c2(osc1, 0, mixSources, 1);
AudioConnection c3(osc2, 0, mixSources, 2);
AudioConnection c4(osc3, 0, mixSources, 3);

AudioConnection c5(mixSources, 0, formantFilter1, 0);
AudioConnection c6(mixSources, 0, formantFilter2, 0);
AudioConnection c7(mixSources, 0, formantFilter3, 0);

AudioConnection c8(mixSources, 0, mixFormants, 0);
AudioConnection c9(formantFilter1, 0, mixFormants, 1);
AudioConnection c10(formantFilter2, 0, mixFormants, 2);
AudioConnection c11(formantFilter3, 0, mixFormants, 3);

AudioConnection c12(mixFormants, 0, audioOutput, 0);

AudioControlSGTL5000 audioShield;

// sopran a
void setSopranA() {
	// calcBiquad(FilterType,FrequencyC,dBgain,BW,QuantizationUnit,SampleRate,int*);
	calcBiquad(FILTER_BANDPASS,800,0,0.5,2147483648,44100,updateFilter);
	formantFilter1.updateCoefs(updateFilter);
	calcBiquad(FILTER_BANDPASS,1150,0,0.4,2147483648,44100,updateFilter);
	formantFilter2.updateCoefs(updateFilter);
	calcBiquad(FILTER_BANDPASS,2900,0,0.3,2147483648,44100,updateFilter);
	formantFilter3.updateCoefs(updateFilter);	
}


//___________________________________________________________________________________
void setup() 
{

  Serial.begin(57600);
  
    //___________________________________________________________________________________
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
	
	AudioMemory(15);
	
	audioShield.enable();
	
	audioShield.inputSelect(myInput);
	audioShield.volume(90);
	
	mixFormants.gain(0, 0.0001);
	mixFormants.gain(1, 0.5);
	mixFormants.gain(2, 0.5);
	mixFormants.gain(3, 0.5);
	
	mixSources.gain(0, 0.3);
	mixSources.gain(1, 0.3);
	mixSources.gain(2, 0.3);
	mixSources.gain(3, 0.3);
	
	
	osc1.set_ramp_length(88);
	osc2.set_ramp_length(88);
	osc3.set_ramp_length(88);
	
	osc1.amplitude(0);
	osc2.amplitude(0);
	osc3.amplitude(0);
	
	osc1.begin(0.1,tune_frequencies2_PGM[30], TONE_TYPE_SQUARE);
	osc2.begin(0.1,tune_frequencies2_PGM[37], TONE_TYPE_SQUARE);
	osc3.begin(0.1,tune_frequencies2_PGM[44], TONE_TYPE_SQUARE);
	
	setSopranA();
	
	Serial.println("audio setup done");
}

//___________________________________________________________

//Metros

Metro MonitorMetro = Metro(1000);
Metro VolMetro = Metro(10);

// audio volume
int mainVolume = 0;
int oldValue = 0;



void loop() {

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
}


