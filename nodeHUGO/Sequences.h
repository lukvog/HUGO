/* 
 * *** Sequences.h ****
 *
 * created by Lukas Vogel 15.04.14
 * 
 */

#ifndef Sequences_h_
#define Sequences_h_

#include <vector>

/////////////////
//ALWAYS CHANGE:  SeqName, ArrayLength, generate Object with right Names, Steplength, NumberOfSeqs in Array, Mapping Name in Array
/////////////////////


//TODO!!
//Feedback mit 1em Delay Modus


//------------------------------------------------
//----------------- Tone Volume ------------------
//------------------------------------------------

const int toneVolumeSeq1_raw[2] = { 
  -60, 2000};
const int toneVolumeSeq2_raw[4] = { 
  0, 1000, -10, 1000};
const int toneVolumeSeq3_raw[2] = { 
  -60, 1000};
const int toneVolumeSeq4_raw[2] = { 
  -60, 1000};
const int toneVolumeSeq5_raw[2] = { 
  0, 2000};
const int toneVolumeSeq6_raw[2] = { 
  -60, 1000};

ToneVolumeSeq* toneVolumeSeq1 = new ToneVolumeSeq(toneVolumeSeq1_raw, 1);
ToneVolumeSeq* toneVolumeSeq2 = new ToneVolumeSeq(toneVolumeSeq2_raw, 2);
ToneVolumeSeq* toneVolumeSeq3 = new ToneVolumeSeq(toneVolumeSeq3_raw, 1);
ToneVolumeSeq* toneVolumeSeq4 = new ToneVolumeSeq(toneVolumeSeq4_raw, 1);
ToneVolumeSeq* toneVolumeSeq5 = new ToneVolumeSeq(toneVolumeSeq5_raw, 1);


ToneVolumeSeq* masterToneVolSeq[5];

void setToneVolSeq()
{
  masterToneVolSeq[0] = toneVolumeSeq1;
  masterToneVolSeq[1] = toneVolumeSeq2;
  masterToneVolSeq[2] = toneVolumeSeq3;
  masterToneVolSeq[3] = toneVolumeSeq4;
  masterToneVolSeq[4] = toneVolumeSeq5;
}

//------------------------------------------------
//---------------- Input Volume ------------------
//------------------------------------------------

const int inVolumeSeq1_raw[8] = { 
  -10, 1000, 0, 2000, -10, 1000,  0, 2000};
const int inVolumeSeq2_raw[4] = { 
  -20, 1000, -40, 1000 };
const int inVolumeSeq3_raw[2] = { 
  -60, 1000};
const int inVolumeSeq4_raw[2] = { 
  0, 1000};
const int inVolumeSeq5_raw[2] = { 
  -60, 1000};

InVolumeSeq* inVolumeSeq1 = new InVolumeSeq(inVolumeSeq1_raw, 4);
InVolumeSeq* inVolumeSeq2 = new InVolumeSeq(inVolumeSeq2_raw, 2);
InVolumeSeq* inVolumeSeq3 = new InVolumeSeq(inVolumeSeq3_raw, 1);
InVolumeSeq* inVolumeSeq4 = new InVolumeSeq(inVolumeSeq4_raw, 1);
InVolumeSeq* inVolumeSeq5 = new InVolumeSeq(inVolumeSeq5_raw, 1);

InVolumeSeq* masterInVolSeq[5];

void setInVolSeq()
{
  masterInVolSeq[0] = inVolumeSeq1;
  masterInVolSeq[1] = inVolumeSeq2;
  masterInVolSeq[2] = inVolumeSeq3;
  masterInVolSeq[3] = inVolumeSeq4;
  masterInVolSeq[4] = inVolumeSeq5;
}

//------------------------------------------------
//----------------- Delay State ------------------
//------------------------------------------------
//States: 0 --> mute / 1 --> delayed / 2 --> loop hold / 3 --> trough ohne delay

const int delayStateSeq1_raw[4] = {
  1, 100, 2, 200};
const int delayStateSeq2_raw[8] = {
  1, 50, 2, 300, 1, 1000, 2, 300};
const int delayStateSeq3_raw[2] = {
  0, 1000};
const int delayStateSeq4_raw[2] = {
  2, 1000};
const int delayStateSeq5_raw[2] = {
  1, 2000};



DelayStateSeq* delayStateSeq1 = new DelayStateSeq(delayStateSeq1_raw, 2);
DelayStateSeq* delayStateSeq2 = new DelayStateSeq(delayStateSeq2_raw, 4);
DelayStateSeq* delayStateSeq3 = new DelayStateSeq(delayStateSeq3_raw, 1);
DelayStateSeq* delayStateSeq4 = new DelayStateSeq(delayStateSeq4_raw, 1);
DelayStateSeq* delayStateSeq5 = new DelayStateSeq(delayStateSeq5_raw, 1);

DelayStateSeq* masterDelayStateSeq[5];

void setDelayStateSeq()
{
  masterDelayStateSeq[0] = delayStateSeq1;
  masterDelayStateSeq[1] = delayStateSeq2;
  masterDelayStateSeq[2] = delayStateSeq3;
  masterDelayStateSeq[3] = delayStateSeq4;
  masterDelayStateSeq[4] = delayStateSeq5;
}

//------------------------------------------------
//------------- Delay Loop Length ----------------
//------------------------------------------------

const int delayLoopLengthSeq1_raw[6] = {
  50, 1000, 65, 1000, 40, 1000};
const int delayLoopLengthSeq2_raw[8] = {
  20, 50, 65, 300, 20, 1000, 65, 300};
const int delayLoopLengthSeq3_raw[2] = {
  40, 1000};
const int delayLoopLengthSeq4_raw[6] = {
  40, 1000, 65, 1000, 60, 1000};
const int delayLoopLengthSeq5_raw[2] = {
  65, 1000};

DelayLoopLengthSeq* delayLoopLengthSeq1 = new DelayLoopLengthSeq(delayLoopLengthSeq1_raw, 3);
DelayLoopLengthSeq* delayLoopLengthSeq2 = new DelayLoopLengthSeq(delayLoopLengthSeq2_raw, 4);
DelayLoopLengthSeq* delayLoopLengthSeq3 = new DelayLoopLengthSeq(delayLoopLengthSeq3_raw, 1);
DelayLoopLengthSeq* delayLoopLengthSeq4 = new DelayLoopLengthSeq(delayLoopLengthSeq4_raw, 3);
DelayLoopLengthSeq* delayLoopLengthSeq5 = new DelayLoopLengthSeq(delayLoopLengthSeq5_raw, 1);

DelayLoopLengthSeq* masterDelayLoopLengthSeq[5];

void setDelayLoopLengthSeq()
{
  masterDelayLoopLengthSeq[0] = delayLoopLengthSeq1;
  masterDelayLoopLengthSeq[1] = delayLoopLengthSeq2;
  masterDelayLoopLengthSeq[2] = delayLoopLengthSeq3;
  masterDelayLoopLengthSeq[3] = delayLoopLengthSeq4;
  masterDelayLoopLengthSeq[4] = delayLoopLengthSeq5;
}

//------------------------------------------------
//----------------- Formant Filter ---------------
//------------------------------------------------
// vokal, interpolation, delay

const int formantSeq1_raw[24] = { 
  a1, 1, 1000, e1, 0, 1000, i1, 1, 1000, o1, 0, 1000, u1, 1, 1000, o1, 0, 1000, i1, 1, 1000, e1, 0, 1000};
const int formantSeq2_raw[24] = { 
  a1, 0, 1000, e1, 0, 1000, i1, 0, 1000, o1, 0, 1000, u1, 0, 1000, o1, 0, 1000, i1, 0, 1000, e1, 0, 1000 };
const int formantSeq3_raw[3] = { 
  a1, 0, 1000};
const int formantSeq4_raw[24] = { 
  a1, 0, 1000, e1, 0, 1000, i1, 0, 1000, o1, 0, 1000, u1, 0, 1000, o1, 0, 1000, i1, 0, 1000, e1, 0, 1000 };
const int formantSeq5_raw[3] = { 
  a1, 0, 1000};

FormantFilterSequence* formantSeq1 = new FormantFilterSequence(formantSeq1_raw, 8);
FormantFilterSequence* formantSeq2 = new FormantFilterSequence(formantSeq2_raw, 8);
FormantFilterSequence* formantSeq3 = new FormantFilterSequence(formantSeq3_raw, 1);
FormantFilterSequence* formantSeq4 = new FormantFilterSequence(formantSeq4_raw, 8);
FormantFilterSequence* formantSeq5 = new FormantFilterSequence(formantSeq5_raw, 1);


FormantFilterSequence* masterFormantSeq[5];

void setFormantSeq()
{
  masterFormantSeq[0] = formantSeq1;
  masterFormantSeq[1] = formantSeq2;
  masterFormantSeq[2] = formantSeq3;
  masterFormantSeq[3] = formantSeq4;
  masterFormantSeq[4] = formantSeq5;
}

//------------------------------------------------
//----------------- LowPass Filter ---------------
//------------------------------------------------
// interpolation, Freq, Delay
//
//Array Length +1

const int LPSeq1_raw[17] = { 
  0, 800, 1000, 1200, 1000, 600, 1000, 6000, 1000, 800, 700, 1200, 1000, 300, 1000, 7000, 1000};
const int LPSeq2_raw[3] = { 
  1, 700, 1000};
const int LPSeq3_raw[3] = { 
  1, 700, 1000};
const int LPSeq4_raw[13] = {
  1, 1000 , 750 , 1000 , 0 , 500 , 1050 , 500 , 2226 , 2000 , 0 , 100 , 1335 };
const int LPSeq5_raw[3] = { 
  1, 7000, 1000};
  
/*
const int LPSeq6_raw[3] = { 
  0, 100, 1000};
const int LPSeq7_raw[3] = { 
  0, 100, 1000};
const int LPSeq8_raw[3] = { 
  0, 100, 1000};
const int LPSeq9_raw[3] = { 
  0, 100, 1000};
const int LPSeq10_raw[3] = { 
  0, 100, 1000};
 */

LPFilterSequence* LPSeq1 = new LPFilterSequence(LPSeq1_raw, 8);
LPFilterSequence* LPSeq2 = new LPFilterSequence(LPSeq2_raw, 1);
LPFilterSequence* LPSeq3 = new LPFilterSequence(LPSeq3_raw, 1);
LPFilterSequence* LPSeq4 = new LPFilterSequence(LPSeq4_raw, 6);
LPFilterSequence* LPSeq5 = new LPFilterSequence(LPSeq5_raw, 1);
/*
LPFilterSequence* LPSeq6 = new LPFilterSequence(LPSeq6_raw, 1);
LPFilterSequence* LPSeq7 = new LPFilterSequence(LPSeq7_raw, 1);
LPFilterSequence* LPSeq8 = new LPFilterSequence(LPSeq8_raw, 1);
LPFilterSequence* LPSeq9 = new LPFilterSequence(LPSeq9_raw, 1);
LPFilterSequence* LPSeq10 = new LPFilterSequence(LPSeq10_raw, 1);
*/


LPFilterSequence* masterLPSeq[5];

void setLPSeq()
{
  masterLPSeq[0] = LPSeq1;
  masterLPSeq[1] = LPSeq2;
  masterLPSeq[2] = LPSeq3;
  masterLPSeq[3] = LPSeq4;
  masterLPSeq[4] = LPSeq5;
  /*
  masterLPSeq[5] = LPSeq6;
  masterLPSeq[6] = LPSeq7;
  masterLPSeq[7] = LPSeq8;
  masterLPSeq[8] = LPSeq9;
  masterLPSeq[9] = LPSeq10;
  */
}

//------------------------------------------------
//-------------------- Tone Osc ------------------
//------------------------------------------------
//
// vol, MidiNote, Delay


const int toneSeq1_1_raw[6] = {
  120, 35, 200, 0, 35, 400};
const int toneSeq1_2_raw[9] = {
  0, 42, 200, 120, 42, 200, 0, 42, 200};
const int toneSeq1_3_raw[6] = {
  0, 42, 400, 120, 49, 200};

const int toneSeq2_1_raw[12] = { 
  100 , 26 , 2000 , 0 , 30 , 1800 , 120 , 38 , 2500 , 0 , 30 , 3000 } 
;
const int toneSeq2_2_raw[12] = { 
  100 , 47 , 2100, 120 , 42 , 4000 , 120 , 52 , 3000 , 120 , 40 , 3000 } 
;
const int toneSeq2_3_raw[27] = { 
  0 , 59 , 1300 , 120 , 59 , 2010 , 0 , 59 , 3000 , 120 , 59 , 5000 , 0 , 59 , 1400 , 120 , 59 , 3000 , 0 , 59 , 2300 , 120 , 59 , 2200 , 0 , 59 , 2300  } 
;

const int toneSeq3_1_raw[3] = {
  0, 38, 1000 };
const int toneSeq3_2_raw[3] = {
  0, 40, 1000 };
const int toneSeq3_3_raw[3] = {
  0, 42, 1000 };

const int toneSeq4_1_raw[3] = {
  0, 38, 1000 };
const int toneSeq4_2_raw[3] = {
  0, 40, 1000 };
const int toneSeq4_3_raw[3] = {
  0, 42, 1000 };

const int toneSeq5_1_raw[24] = {
  127, 18, 1000, 121, 18, 1000, 115, 18, 1000, 121, 18, 1000, 107, 18, 1000, 121, 18, 1000, 127, 18, 1000, 100, 18, 1000   };
const int toneSeq5_2_raw[24] = {
  127, 50, 1000, 121, 50, 1000, 117, 18, 1000, 111, 18, 1000, 103, 18, 1000, 127, 18, 1000, 127, 18, 1000, 101, 18, 1000   };
const int toneSeq5_3_raw[24] = {
  127, 52, 1000, 121, 52, 1000, 122, 18, 1000, 101, 18, 1000, 107, 18, 1000, 121, 18, 1000, 127, 18, 1000, 121, 18, 1000   };


ToneSequence* toneSeq1_1 = new ToneSequence(toneSeq1_1_raw, 2, &osc1, TONE_TYPE_SQUARE);
ToneSequence* toneSeq1_2 = new ToneSequence(toneSeq1_2_raw, 3, &osc2, TONE_TYPE_SQUARE);
ToneSequence* toneSeq1_3 = new ToneSequence(toneSeq1_3_raw, 2, &osc3, TONE_TYPE_SQUARE);

ToneSequence* toneSeq2_1 = new ToneSequence(toneSeq2_1_raw, 4, &osc1, TONE_TYPE_SINE);
ToneSequence* toneSeq2_2 = new ToneSequence(toneSeq2_2_raw, 4, &osc2, TONE_TYPE_SINE);
ToneSequence* toneSeq2_3 = new ToneSequence(toneSeq2_3_raw, 9, &osc3, TONE_TYPE_SINE);

ToneSequence* toneSeq3_1 = new ToneSequence(toneSeq3_1_raw, 1, &osc1, TONE_TYPE_SQUARE);
ToneSequence* toneSeq3_2 = new ToneSequence(toneSeq3_2_raw, 1, &osc2, TONE_TYPE_SQUARE);
ToneSequence* toneSeq3_3 = new ToneSequence(toneSeq3_3_raw, 1, &osc3, TONE_TYPE_SQUARE);

ToneSequence* toneSeq4_1 = new ToneSequence(toneSeq4_1_raw, 1, &osc1, TONE_TYPE_SQUARE);
ToneSequence* toneSeq4_2 = new ToneSequence(toneSeq4_2_raw, 1, &osc2, TONE_TYPE_SQUARE);
ToneSequence* toneSeq4_3 = new ToneSequence(toneSeq4_3_raw, 1, &osc3, TONE_TYPE_SQUARE);

ToneSequence* toneSeq5_1 = new ToneSequence(toneSeq5_1_raw, 8, &osc1, TONE_TYPE_SINE);
ToneSequence* toneSeq5_2 = new ToneSequence(toneSeq5_2_raw, 8, &osc2, TONE_TYPE_SINE);
ToneSequence* toneSeq5_3 = new ToneSequence(toneSeq5_3_raw, 8, &osc3, TONE_TYPE_SINE);


ToneSequence* masterToneSeq[15];

void setToneSeq()
{
  masterToneSeq[0] = toneSeq1_1;
  masterToneSeq[1] = toneSeq1_2;
  masterToneSeq[2] = toneSeq1_3;

  masterToneSeq[3] = toneSeq2_1;
  masterToneSeq[4] = toneSeq2_2;
  masterToneSeq[5] = toneSeq2_3;

  masterToneSeq[6] = toneSeq3_1;
  masterToneSeq[7] = toneSeq3_2;
  masterToneSeq[8] = toneSeq3_3;

  masterToneSeq[9] = toneSeq4_1;
  masterToneSeq[10] = toneSeq4_2;
  masterToneSeq[11] = toneSeq4_3;

  masterToneSeq[12] = toneSeq5_1;
  masterToneSeq[13] = toneSeq5_2;
  masterToneSeq[14] = toneSeq5_3;
}

#endif




