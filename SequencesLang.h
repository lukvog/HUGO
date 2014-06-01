/* 
 * *** Sequences.h ****
 *
 * created by Lukas Vogel 15.04.14
 * 
 */

#ifndef Sequences_h_
#define Sequences_h_

#include <vector>

//------------------------------------------------
//----------------- Tone Volume ------------------
//------------------------------------------------

const int toneVolumeSeq1_raw[8] = { 
  0, 2000, -30, 1000, 0, 2000, -30, 1000 };
const int toneVolumeSeq2_raw[4] = { 
  -10, 1000, -50, 1000 };
const int toneVolumeSeq3_raw[2] = { 
  -60, 1000};

ToneVolumeSeq toneVolumeSeq1(toneVolumeSeq1_raw, 4);
ToneVolumeSeq toneVolumeSeq2(toneVolumeSeq2_raw, 2);
ToneVolumeSeq toneVolumeSeq3(toneVolumeSeq3_raw, 1);

std::vector<ToneVolumeSeq*> masterToneVolSeq;

void setToneVolSeq()
{
  masterToneVolSeq.push_back(&toneVolumeSeq1);
  masterToneVolSeq.push_back(&toneVolumeSeq2);
  masterToneVolSeq.push_back(&toneVolumeSeq3);
}

//------------------------------------------------
//---------------- Input Volume ------------------
//------------------------------------------------

const int inVolumeSeq1_raw[8] = { 
  -10, 1000, 0, 2000, -10, 1000,  0, 2000};
const int inVolumeSeq2_raw[4] = { 
  -10, 1000, -50, 1000 };
const int inVolumeSeq3_raw[2] = { 
  -60, 1000};

InVolumeSeq inVolumeSeq1(inVolumeSeq1_raw, 4);
InVolumeSeq inVolumeSeq2(inVolumeSeq2_raw, 2);
InVolumeSeq inVolumeSeq3(inVolumeSeq3_raw, 1);

std::vector<InVolumeSeq*> masterInVolSeq;

void setInVolSeq()
{
  masterInVolSeq.push_back(&inVolumeSeq1);
  masterInVolSeq.push_back(&inVolumeSeq2);
  masterInVolSeq.push_back(&inVolumeSeq3);
}

//------------------------------------------------
//----------------- Delay State ------------------
//------------------------------------------------
//
//0: mute / 1: normal Delay / 2: delay hold / 3: talk through without delay / 4 not implemented yet


const int delayStateSeq1_raw[4] = {
 1, 100, 2, 200};
const int delayStateSeq2_raw[8] = {
  1, 500, 2, 300, 1, 1000, 2, 300};
const int delayStateSeq3_raw[2] = { 
  1, 1000};


DelayStateSeq delayStateSeq1(delayStateSeq1_raw, 2);
DelayStateSeq delayStateSeq2(delayStateSeq2_raw, 4);
DelayStateSeq delayStateSeq3(delayStateSeq3_raw, 1);

std::vector<DelayStateSeq*> masterDelayStateSeq;

void setDelayStateSeq()
{
  masterDelayStateSeq.push_back(&delayStateSeq1);
  masterDelayStateSeq.push_back(&delayStateSeq2);
  masterDelayStateSeq.push_back(&delayStateSeq3);
}

//------------------------------------------------
//------------- Delay Loop Length ----------------
//------------------------------------------------
//
// 10 - 65x


int delayLoopLengthSeq1_raw[4] = {20, 100, 20, 100};
int delayLoopLengthSeq2_raw[8] = {20, 50, 20, 300, 20, 1000, 20, 300};
const int delayLoopLengthSeq3_raw[2] = { 
  20, 1000};

DelayLoopLengthSeq delayLoopLengthSeq1(delayLoopLengthSeq1_raw, 2);
DelayLoopLengthSeq delayLoopLengthSeq2(delayLoopLengthSeq2_raw, 4);
DelayLoopLengthSeq delayLoopLengthSeq3(delayLoopLengthSeq3_raw, 1);

std::vector<DelayLoopLengthSeq*> masterDelayLoopLengthSeq;

void setDelayLoopLengthSeq()
{
  masterDelayLoopLengthSeq.push_back(&delayLoopLengthSeq1);
  masterDelayLoopLengthSeq.push_back(&delayLoopLengthSeq2);
  masterDelayLoopLengthSeq.push_back(&delayLoopLengthSeq3);
}

//------------------------------------------------
//----------------- Formant Filter ---------------
//------------------------------------------------

const int formantSeq1_raw[24] = { 
  a1, 1, 1000, e1, 0, 1000, i1, 1, 1000, o1, 0, 1000, u1, 1, 1000, o1, 0, 1000, i1, 1, 1000, e1, 0, 1000};
const int formantSeq2_raw[3] = { 
  a1, 0, 1000};
const int formantSeq3_raw[3] = { 
  a1, 0, 1000};

FormantFilterSequence formantSeq1(formantSeq1_raw, 8);
FormantFilterSequence formantSeq2(formantSeq2_raw, 1);
FormantFilterSequence formantSeq3(formantSeq3_raw, 1);

std::vector<FormantFilterSequence*> masterFormantSeq;

void setFormantSeq()
{
  masterFormantSeq.push_back(&formantSeq1);
  masterFormantSeq.push_back(&formantSeq2);
  masterFormantSeq.push_back(&formantSeq3);
}

//------------------------------------------------
//----------------- LowPass Filter ---------------
//------------------------------------------------
//
// const interpolation, freq, delay


const int LPSeq1_raw[8+1] = { 
  0, 400, 1000, 1000, 1000, 200, 1000, 6000, 1000};
const int LPSeq2_raw[2+1] = { 
  1, 700, 1000};
const int LPSeq3_raw[2+1] = { 
  0, 100, 1000};

LPFilterSequence LPSeq1(LPSeq1_raw, 4);
LPFilterSequence LPSeq2(LPSeq2_raw, 1);
LPFilterSequence LPSeq3(LPSeq3_raw, 1);

std::vector<LPFilterSequence*> masterLPSeq;

void setLPSeq()
{
  masterLPSeq.push_back(&LPSeq1);
  masterLPSeq.push_back(&LPSeq2);  
  masterLPSeq.push_back(&LPSeq3);
}

//------------------------------------------------
//-------------------- Tone Osc ------------------
//------------------------------------------------
//
// vel, note, delay


const int toneSeq1_1_raw[6] = {
  120, 35, 200, 0, 35, 400};
const int toneSeq1_2_raw[9] = {
  0, 42, 200, 120, 42, 200, 0, 42, 200};
const int toneSeq1_3_raw[6] = {
  0, 42, 400, 120, 49, 200};

const int toneSeq2_1_raw[12] = { 
  120 , 38 , 145 , 0 , 38 , 152 , 120 , 43 , 152 , 0 , 43 , 152 };
const int toneSeq2_2_raw[12] = { 
  120 , 47 , 145 , 120 , 42 , 152 , 120 , 52 , 152 , 120 , 40 , 152 };
const int toneSeq2_3_raw[27] = { 
  0 , 59 , 69 , 120 , 59 , 76 , 0 , 59 , 76 , 120 , 59 , 76 , 0 , 59 , 114 , 120 , 59 , 38 , 0 , 59 , 76 , 120 , 59 , 38 , 0 , 59 , 38 };
  
//const int toneSeq3_1_raw[6] = {
//  1, 38, 1000, 0, 38, 1000 };
//const int toneSeq3_2_raw[6] = {
//  1, 40, 1000, 0, 40, 1000 };
//const int toneSeq3_3_raw[6] = {
//  1, 42, 1000, 0, 42, 1000 };


ToneSequence toneSeq1_1(toneSeq1_1_raw, 2, &osc1, TONE_TYPE_SQUARE);
ToneSequence toneSeq1_2(toneSeq1_2_raw, 3, &osc2, TONE_TYPE_SQUARE);
ToneSequence toneSeq1_3(toneSeq1_3_raw, 2, &osc3, TONE_TYPE_SQUARE);

ToneSequence toneSeq2_1(toneSeq2_1_raw, 4, &osc1, TONE_TYPE_SQUARE);
ToneSequence toneSeq2_2(toneSeq2_2_raw, 4, &osc2, TONE_TYPE_SQUARE);
ToneSequence toneSeq2_3(toneSeq2_3_raw, 9, &osc3, TONE_TYPE_SQUARE);

//ToneSequence toneSeq3_1(toneSeq3_1_raw, 2, &osc1, TONE_TYPE_SQUARE);
//ToneSequence toneSeq3_2(toneSeq3_2_raw, 2, &osc2, TONE_TYPE_SQUARE);
//ToneSequence toneSeq3_3(toneSeq3_3_raw, 2, &osc3, TONE_TYPE_SQUARE);

std::vector<ToneSequence*> masterToneSeq;

void setToneSeq()
{
  masterToneSeq.push_back(&toneSeq1_1);
  masterToneSeq.push_back(&toneSeq1_2);
  masterToneSeq.push_back(&toneSeq1_3);

  masterToneSeq.push_back(&toneSeq2_1);
  masterToneSeq.push_back(&toneSeq2_2);
  masterToneSeq.push_back(&toneSeq2_3);

//  masterToneSeq.push_back(&toneSeq3_1);
//  masterToneSeq.push_back(&toneSeq3_2);
//  masterToneSeq.push_back(&toneSeq3_3);
}

#endif







