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


//------------------------------------------------
//----------------- Tone Volume ------------------
//------------------------------------------------

const int toneVolumeSeq1_raw[2] = { 
  -80, 2000};
ToneVolumeSeq* toneVolumeSeq1 = new ToneVolumeSeq(toneVolumeSeq1_raw, 1);

const int toneVolumeSeq2_raw[6] = { 
  -10, 1000, -20, 1000, -15, 1000};
ToneVolumeSeq* toneVolumeSeq2 = new ToneVolumeSeq(toneVolumeSeq2_raw, 3);

const int toneVolumeSeq3_raw[2] = { 
  -80, 1000};
ToneVolumeSeq* toneVolumeSeq3 = new ToneVolumeSeq(toneVolumeSeq3_raw, 1);

const int toneVolumeSeq4_raw[2] = { 
  -80, 1000};
ToneVolumeSeq* toneVolumeSeq4 = new ToneVolumeSeq(toneVolumeSeq4_raw, 1);

const int toneVolumeSeq5_raw[2] = { 
  -10, 2000};
ToneVolumeSeq* toneVolumeSeq5 = new ToneVolumeSeq(toneVolumeSeq5_raw, 1);

const int toneVolumeSeq6_raw[2] = { 
  -80, 1000};
ToneVolumeSeq* toneVolumeSeq6 = new ToneVolumeSeq(toneVolumeSeq6_raw, 1);

const int toneVolumeSeq7_raw[2] = { 
  -80, 1000};
ToneVolumeSeq* toneVolumeSeq7 = new ToneVolumeSeq(toneVolumeSeq7_raw, 1);

const int toneVolumeSeq8_raw[2] = { 
  -80, 2000};
ToneVolumeSeq* toneVolumeSeq8 = new ToneVolumeSeq(toneVolumeSeq8_raw, 1);

ToneVolumeSeq* masterToneVolSeq[8];

void setToneVolSeq()
{
  masterToneVolSeq[0] = toneVolumeSeq1;
  masterToneVolSeq[1] = toneVolumeSeq2;
  masterToneVolSeq[2] = toneVolumeSeq3;
  masterToneVolSeq[3] = toneVolumeSeq4;
  masterToneVolSeq[4] = toneVolumeSeq5;
  masterToneVolSeq[5] = toneVolumeSeq6;
  masterToneVolSeq[6] = toneVolumeSeq7;
    masterToneVolSeq[7] = toneVolumeSeq8;
}

//------------------------------------------------
//---------------- Input Volume ------------------
//------------------------------------------------

const int inVolumeSeq1_raw[8] = { 
  -10, 1000, 0, 2000, -10, 1000,  0, 2000};
InVolumeSeq* inVolumeSeq1 = new InVolumeSeq(inVolumeSeq1_raw, 4);

const int inVolumeSeq2_raw[4] = { 
  -30, 1000, -40, 1000 };
InVolumeSeq* inVolumeSeq2 = new InVolumeSeq(inVolumeSeq2_raw, 2);

const int inVolumeSeq3_raw[2] = { 
  -60, 1000};
InVolumeSeq* inVolumeSeq3 = new InVolumeSeq(inVolumeSeq3_raw, 1);

const int inVolumeSeq4_raw[2] = { 
  -1, 1000};
InVolumeSeq* inVolumeSeq4 = new InVolumeSeq(inVolumeSeq4_raw, 1);

const int inVolumeSeq5_raw[2] = { 
  -30, 1000};
InVolumeSeq* inVolumeSeq5 = new InVolumeSeq(inVolumeSeq5_raw, 1);

const int inVolumeSeq6_raw[2] = { 
  -1, 2000};
InVolumeSeq* inVolumeSeq6 = new InVolumeSeq(inVolumeSeq6_raw, 1);

const int inVolumeSeq7_raw[2] = { 
  -1, 2000};
InVolumeSeq* inVolumeSeq7 = new InVolumeSeq(inVolumeSeq7_raw, 1);

const int inVolumeSeq8_raw[4] = { 
  -50, 1000, -40, 1000 };
InVolumeSeq* inVolumeSeq8 = new InVolumeSeq(inVolumeSeq8_raw, 2);

InVolumeSeq* masterInVolSeq[8];

void setInVolSeq()
{
  masterInVolSeq[0] = inVolumeSeq1;
  masterInVolSeq[1] = inVolumeSeq2;
  masterInVolSeq[2] = inVolumeSeq3;
  masterInVolSeq[3] = inVolumeSeq4;
  masterInVolSeq[4] = inVolumeSeq5;
  masterInVolSeq[5] = inVolumeSeq6;
  masterInVolSeq[6] = inVolumeSeq7;
    masterInVolSeq[7] = inVolumeSeq8;
}

//------------------------------------------------
//----------------- Delay State ------------------
//------------------------------------------------
//States: 0 --> mute / 1 --> delayed / 2 --> loop hold / 3 --> trough ohne delay

const int delayStateSeq1_raw[4] = {
  1, 100, 2, 200};
DelayStateSeq* delayStateSeq1 = new DelayStateSeq(delayStateSeq1_raw, 2);

const int delayStateSeq2_raw[8] = {
  1, 50, 2, 300, 1, 1000, 2, 300};
DelayStateSeq* delayStateSeq2 = new DelayStateSeq(delayStateSeq2_raw, 4);

const int delayStateSeq3_raw[2] = {
  0, 1000};
DelayStateSeq* delayStateSeq3 = new DelayStateSeq(delayStateSeq3_raw, 1);

const int delayStateSeq4_raw[2] = {
  2, 3000};
DelayStateSeq* delayStateSeq4 = new DelayStateSeq(delayStateSeq4_raw, 1);

const int delayStateSeq5_raw[2] = {
  1, 4000};
DelayStateSeq* delayStateSeq5 = new DelayStateSeq(delayStateSeq5_raw, 1);

const int delayStateSeq6_raw[2] = {
  1, 500};
DelayStateSeq* delayStateSeq6 = new DelayStateSeq(delayStateSeq6_raw, 1);

const int delayStateSeq7_raw[12] = {
  1, 1000, 1, 250, 1, 500, 1, 250, 1, 500, 1, 500};
DelayStateSeq* delayStateSeq7 = new DelayStateSeq(delayStateSeq7_raw, 6);

const int delayStateSeq8_raw[8] = {
  1, 50, 2, 300, 1, 1000, 2, 300};
DelayStateSeq* delayStateSeq8 = new DelayStateSeq(delayStateSeq8_raw, 4);

DelayStateSeq* masterDelayStateSeq[8];

void setDelayStateSeq()
{
  masterDelayStateSeq[0] = delayStateSeq1;
  masterDelayStateSeq[1] = delayStateSeq2;
  masterDelayStateSeq[2] = delayStateSeq3;
  masterDelayStateSeq[3] = delayStateSeq4;
  masterDelayStateSeq[4] = delayStateSeq5;
  masterDelayStateSeq[5] = delayStateSeq6;
  masterDelayStateSeq[6] = delayStateSeq7;
    masterDelayStateSeq[7] = delayStateSeq8;
}

//------------------------------------------------
//------------- Delay Loop Length ----------------
//------------------------------------------------

const int delayLoopLengthSeq1_raw[6] = {
  50, 1000, 65, 1000, 40, 1000};
DelayLoopLengthSeq* delayLoopLengthSeq1 = new DelayLoopLengthSeq(delayLoopLengthSeq1_raw, 3);

const int delayLoopLengthSeq2_raw[8] = {
  20, 50, 65, 300, 20, 1000, 65, 300};
DelayLoopLengthSeq* delayLoopLengthSeq2 = new DelayLoopLengthSeq(delayLoopLengthSeq2_raw, 4);

const int delayLoopLengthSeq3_raw[2] = {
  40, 1000};
DelayLoopLengthSeq* delayLoopLengthSeq3 = new DelayLoopLengthSeq(delayLoopLengthSeq3_raw, 1);

const int delayLoopLengthSeq4_raw[10] = {
  40, 1000, 65, 1000, 60, 1000, 45, 1000, 30, 1000};
DelayLoopLengthSeq* delayLoopLengthSeq4 = new DelayLoopLengthSeq(delayLoopLengthSeq4_raw, 5);

const int delayLoopLengthSeq5_raw[2] = {
  65, 2000};
DelayLoopLengthSeq* delayLoopLengthSeq5 = new DelayLoopLengthSeq(delayLoopLengthSeq5_raw, 1);

const int delayLoopLengthSeq6_raw[8] = {
  40, 500, 65, 500, 50, 500, 55, 500};
DelayLoopLengthSeq* delayLoopLengthSeq6 = new DelayLoopLengthSeq(delayLoopLengthSeq6_raw, 4);

const int delayLoopLengthSeq7_raw[2] = {
  65, 1000};
DelayLoopLengthSeq* delayLoopLengthSeq7 = new DelayLoopLengthSeq(delayLoopLengthSeq7_raw, 1);

const int delayLoopLengthSeq8_raw[8] = {
  20, 50, 65, 300, 20, 1000, 65, 300};
DelayLoopLengthSeq* delayLoopLengthSeq8 = new DelayLoopLengthSeq(delayLoopLengthSeq8_raw, 4);

DelayLoopLengthSeq* masterDelayLoopLengthSeq[8];

void setDelayLoopLengthSeq()
{
  masterDelayLoopLengthSeq[0] = delayLoopLengthSeq1;
  masterDelayLoopLengthSeq[1] = delayLoopLengthSeq2;
  masterDelayLoopLengthSeq[2] = delayLoopLengthSeq3;
  masterDelayLoopLengthSeq[3] = delayLoopLengthSeq4;
  masterDelayLoopLengthSeq[4] = delayLoopLengthSeq5;
  masterDelayLoopLengthSeq[5] = delayLoopLengthSeq6;
  masterDelayLoopLengthSeq[6] = delayLoopLengthSeq7;
    masterDelayLoopLengthSeq[7] = delayLoopLengthSeq8;
}

//------------------------------------------------
//----------------- Formant Filter ---------------
//------------------------------------------------
// vokal, interpolation, delay

const int formantSeq1_raw[24] = { 
  a1, 1, 1000, e1, 0, 1000, i1, 1, 1000, o1, 0, 1000, u1, 1, 1000, o1, 0, 1000, i1, 1, 1000, e1, 0, 1000};
FormantFilterSequence* formantSeq1 = new FormantFilterSequence(formantSeq1_raw, 8);

const int formantSeq2_raw[24] = { 
  a1, 0, 1000, e1, 0, 1000, i1, 0, 1000, o1, 0, 1000, u1, 0, 1000, o1, 0, 1000, i1, 0, 1000, e1, 0, 1000 };
FormantFilterSequence* formantSeq2 = new FormantFilterSequence(formantSeq2_raw, 8);

const int formantSeq3_raw[3] = { 
  a1, 0, 1000};
FormantFilterSequence* formantSeq3 = new FormantFilterSequence(formantSeq3_raw, 1);

const int formantSeq4_raw[24] = { 
  a1, 1, 1000, e1, 1, 1000, i1, 1, 1000, o1, 1, 1000, u1, 1, 2000, o1, 1, 1000, i1, 1, 1000, e1, 1, 1000 };
FormantFilterSequence* formantSeq4 = new FormantFilterSequence(formantSeq4_raw, 8);

const int formantSeq5_raw[3] = { 
  open, 0, 2000};
FormantFilterSequence* formantSeq5 = new FormantFilterSequence(formantSeq5_raw, 1);

const int formantSeq6_raw[3] = { 
  open, 1, 3000};
FormantFilterSequence* formantSeq6 = new FormantFilterSequence(formantSeq6_raw, 1);

const int formantSeq7_raw[3] = { 
  open, 1, 3000};
FormantFilterSequence* formantSeq7 = new FormantFilterSequence(formantSeq7_raw, 1);

const int formantSeq8_raw[24] = { 
  a1, 0, 1000, e1, 0, 1000, i1, 0, 1000, o1, 0, 1000, u1, 0, 1000, o1, 0, 1000, i1, 0, 1000, e1, 0, 1000 };
FormantFilterSequence* formantSeq8 = new FormantFilterSequence(formantSeq8_raw, 8);

FormantFilterSequence* masterFormantSeq[8];

void setFormantSeq()
{
  masterFormantSeq[0] = formantSeq1;
  masterFormantSeq[1] = formantSeq2;
  masterFormantSeq[2] = formantSeq3;
  masterFormantSeq[3] = formantSeq4;
  masterFormantSeq[4] = formantSeq5;
  masterFormantSeq[5] = formantSeq6;
  masterFormantSeq[6] = formantSeq7;
    masterFormantSeq[7] = formantSeq8;
}

//------------------------------------------------
//----------------- LowPass Filter ---------------
//------------------------------------------------
// interpolation, Freq, Delay
//
//Array Length +1

const int LPSeq1_raw[17] = { 
  1, 200, 1000, 300, 1000, 600, 1000, 3000, 1000, 800, 700, 1200, 1000, 300, 1000, 7000, 1000};
LPFilterSequence* LPSeq1 = new LPFilterSequence(LPSeq1_raw, 8);

const int LPSeq2_raw[3] = { 
  1, 700, 1000};
LPFilterSequence* LPSeq2 = new LPFilterSequence(LPSeq2_raw, 1);

const int LPSeq3_raw[3] = { 
  1, 700, 1000};
LPFilterSequence* LPSeq3 = new LPFilterSequence(LPSeq3_raw, 1);

const int LPSeq4_raw[13] = {
  1, 1000 , 750 , 1000 , 0 , 500 , 1050 , 500 , 2226 , 2000 , 0 , 100 , 1335 };
LPFilterSequence* LPSeq4 = new LPFilterSequence(LPSeq4_raw, 6);

const int LPSeq5_raw[3] = { 
  1, 1000, 1000};
LPFilterSequence* LPSeq5 = new LPFilterSequence(LPSeq5_raw, 1);

const int LPSeq6_raw[13] = {
  1, 300 , 750 , 400 , 200 , 200 , 1050 , 400 , 2226 , 80 , 1000 , 100 , 1335 };
LPFilterSequence* LPSeq6 = new LPFilterSequence(LPSeq6_raw, 6);

const int LPSeq7_raw[3] = { 
  0, 100, 1000};
LPFilterSequence* LPSeq7 = new LPFilterSequence(LPSeq7_raw, 1);

const int LPSeq8_raw[3] = { 
  1, 700, 1000};
LPFilterSequence* LPSeq8 = new LPFilterSequence(LPSeq8_raw, 1);

LPFilterSequence* masterLPSeq[8];

void setLPSeq()
{
  masterLPSeq[0] = LPSeq1;
  masterLPSeq[1] = LPSeq2;
  masterLPSeq[2] = LPSeq3;
  masterLPSeq[3] = LPSeq4;
  masterLPSeq[4] = LPSeq5;
  masterLPSeq[5] = LPSeq6;
  masterLPSeq[6] = LPSeq7;
  masterLPSeq[7] = LPSeq8;
}

//------------------------------------------------
//-------------------- Tone Osc ------------------
//------------------------------------------------
//
// vol, MidiNote, Delay


const int toneSeq1_1_raw[3] = {
  0, 38, 1000 };
const int toneSeq1_2_raw[3] = {
  0, 40, 1000 };
const int toneSeq1_3_raw[3] = {
  0, 42, 1000 };
ToneSequence* toneSeq1_1 = new ToneSequence(toneSeq1_1_raw, 1, &osc1, TONE_TYPE_SQUARE);
ToneSequence* toneSeq1_2 = new ToneSequence(toneSeq1_2_raw, 1, &osc2, TONE_TYPE_SQUARE);
ToneSequence* toneSeq1_3 = new ToneSequence(toneSeq1_3_raw, 1, &osc3, TONE_TYPE_SQUARE);

const int toneSeq2_1_raw[12] = { 
  100 , 26 , 2000 , 0 , 30 , 1800 , 120 , 38 , 2500 , 0 , 30 , 3000 } 
;
const int toneSeq2_2_raw[12] = { 
  100 , 47 , 2100, 120 , 42 , 4000 , 120 , 52 , 3000 , 120 , 40 , 3000 } 
;
const int toneSeq2_3_raw[27] = { 
  0 , 59 , 1300 , 120 , 59 , 2010 , 0 , 59 , 3000 , 120 , 59 , 5000 , 0 , 59 , 1400 , 120 , 59 , 3000 , 0 , 59 , 2300 , 120 , 59 , 2200 , 0 , 59 , 2300  } 
;
ToneSequence* toneSeq2_1 = new ToneSequence(toneSeq2_1_raw, 4, &osc1, TONE_TYPE_SINE);
ToneSequence* toneSeq2_2 = new ToneSequence(toneSeq2_2_raw, 4, &osc2, TONE_TYPE_SINE);
ToneSequence* toneSeq2_3 = new ToneSequence(toneSeq2_3_raw, 9, &osc3, TONE_TYPE_SINE);


const int toneSeq3_1_raw[3] = {
  1, 38, 1000 };
const int toneSeq3_2_raw[3] = {
  1, 40, 1000 };
const int toneSeq3_3_raw[3] = {
  1, 42, 1000 };
ToneSequence* toneSeq3_1 = new ToneSequence(toneSeq3_1_raw, 1, &osc1, TONE_TYPE_SQUARE);
ToneSequence* toneSeq3_2 = new ToneSequence(toneSeq3_2_raw, 1, &osc2, TONE_TYPE_SQUARE);
ToneSequence* toneSeq3_3 = new ToneSequence(toneSeq3_3_raw, 1, &osc3, TONE_TYPE_SQUARE);

const int toneSeq4_1_raw[3] = {
  1, 38, 1000 };
const int toneSeq4_2_raw[3] = {
  1, 40, 1000 };
const int toneSeq4_3_raw[3] = {
  1, 42, 1000 };
ToneSequence* toneSeq4_1 = new ToneSequence(toneSeq4_1_raw, 1, &osc1, TONE_TYPE_SQUARE);
ToneSequence* toneSeq4_2 = new ToneSequence(toneSeq4_2_raw, 1, &osc2, TONE_TYPE_SQUARE);
ToneSequence* toneSeq4_3 = new ToneSequence(toneSeq4_3_raw, 1, &osc3, TONE_TYPE_SQUARE);

const int toneSeq5_1_raw[24] = {
  40, 30, 1000, 41, 30, 1000, 45, 30, 1000, 41, 30, 1000, 47, 30, 1000, 41, 30, 1000, 47, 30, 1000, 40, 30, 1000   };
const int toneSeq5_2_raw[24] = {
  100, 50, 1000, 101, 50, 1000, 107, 50, 1000, 101, 50, 1000, 103, 50, 1000, 107, 50, 1000, 107, 50, 1000, 101, 50, 1000   };
const int toneSeq5_3_raw[24] = {
  100, 52, 1000, 101, 52, 1000, 102, 52, 1000, 101, 52, 1000, 107, 52, 1000, 101, 52, 1000, 107, 52, 1000, 101, 52, 1000   };
ToneSequence* toneSeq5_1 = new ToneSequence(toneSeq5_1_raw, 8, &osc1, TONE_TYPE_SINE);
ToneSequence* toneSeq5_2 = new ToneSequence(toneSeq5_2_raw, 8, &osc2, TONE_TYPE_SINE);
ToneSequence* toneSeq5_3 = new ToneSequence(toneSeq5_3_raw, 8, &osc3, TONE_TYPE_SINE);

const int toneSeq6_1_raw[3] = {
  1, 38, 1000 };
const int toneSeq6_2_raw[3] = {
  1, 40, 1000 };
const int toneSeq6_3_raw[3] = {
  1, 42, 1000 };
ToneSequence* toneSeq6_1 = new ToneSequence(toneSeq6_1_raw, 1, &osc1, TONE_TYPE_SQUARE);
ToneSequence* toneSeq6_2 = new ToneSequence(toneSeq6_2_raw, 1, &osc2, TONE_TYPE_SQUARE);
ToneSequence* toneSeq6_3 = new ToneSequence(toneSeq6_3_raw, 1, &osc3, TONE_TYPE_SQUARE);

const int toneSeq7_1_raw[3] = {
  1, 38, 1000 };
const int toneSeq7_2_raw[3] = {
  1, 40, 1000 };
const int toneSeq7_3_raw[3] = {
  1, 42, 1000 };
ToneSequence* toneSeq7_1 = new ToneSequence(toneSeq7_1_raw, 1, &osc1, TONE_TYPE_SQUARE);
ToneSequence* toneSeq7_2 = new ToneSequence(toneSeq7_2_raw, 1, &osc2, TONE_TYPE_SQUARE);
ToneSequence* toneSeq7_3 = new ToneSequence(toneSeq7_3_raw, 1, &osc3, TONE_TYPE_SQUARE);

const int toneSeq8_1_raw[3] = {
  1, 38, 4000 };
const int toneSeq8_2_raw[3] = {
  1, 40, 4000 };
const int toneSeq8_3_raw[3] = {
  1, 42, 4000 };
ToneSequence* toneSeq8_1 = new ToneSequence(toneSeq8_1_raw, 1, &osc1, TONE_TYPE_SQUARE);
ToneSequence* toneSeq8_2 = new ToneSequence(toneSeq8_2_raw, 1, &osc2, TONE_TYPE_SQUARE);
ToneSequence* toneSeq8_3 = new ToneSequence(toneSeq8_3_raw, 1, &osc3, TONE_TYPE_SQUARE);

ToneSequence* masterToneSeq[24];

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

  masterToneSeq[15] = toneSeq6_1;
  masterToneSeq[16] = toneSeq6_2;
  masterToneSeq[17] = toneSeq6_3;

  masterToneSeq[18] = toneSeq7_1;
  masterToneSeq[19] = toneSeq7_2;
  masterToneSeq[20] = toneSeq7_3;

  masterToneSeq[21] = toneSeq8_1;
  masterToneSeq[22] = toneSeq8_2;
  masterToneSeq[23] = toneSeq8_3;
}

#endif













