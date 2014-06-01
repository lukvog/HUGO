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

const int toneVolumeSeq1_raw[8] = { 0, 2000, -30, 1000, 0, 2000, -30, 1000 };
const int toneVolumeSeq2_raw[4] = { -10, 1000, -50, 1000 };
const int toneVolumeSeq3_raw[2] = { -60, 1000};
// const int toneVolumeSeq4_raw[2] = { -60, 1000};

ToneVolumeSeq toneVolumeSeq1(toneVolumeSeq1_raw, 4);
ToneVolumeSeq toneVolumeSeq2(toneVolumeSeq2_raw, 2);
ToneVolumeSeq toneVolumeSeq3(toneVolumeSeq3_raw, 1);
// ToneVolumeSeq toneVolumeSeq4(toneVolumeSeq4_raw, 1);


ToneVolumeSeq* masterToneVolSeq[4];

void setToneVolSeq()
{
	masterToneVolSeq[0] = &toneVolumeSeq1;
	masterToneVolSeq[1] = &toneVolumeSeq2;
	masterToneVolSeq[2] = &toneVolumeSeq3;
	// masterToneVolSeq[3] = &toneVolumeSeq4;
}

//------------------------------------------------
//---------------- Input Volume ------------------
//------------------------------------------------

const int inVolumeSeq1_raw[8] = { -10, 1000, 0, 2000, -10, 1000,  0, 2000};
const int inVolumeSeq2_raw[4] = { -10, 1000, -50, 1000 };
const int inVolumeSeq3_raw[2] = { -60, 1000};
// const int inVolumeSeq4_raw[2] = { -60, 1000};

InVolumeSeq inVolumeSeq1(inVolumeSeq1_raw, 4);
InVolumeSeq inVolumeSeq2(inVolumeSeq2_raw, 2);
InVolumeSeq inVolumeSeq3(inVolumeSeq3_raw, 1);
// InVolumeSeq inVolumeSeq4(inVolumeSeq4_raw, 1);

InVolumeSeq* masterInVolSeq[4];

void setInVolSeq()
{
	masterInVolSeq[0] = &inVolumeSeq1;
	masterInVolSeq[1] = &inVolumeSeq2;
	masterInVolSeq[2] = &inVolumeSeq3;
	// masterInVolSeq[3] = &inVolumeSeq4;
}

//------------------------------------------------
//----------------- Delay State ------------------
//------------------------------------------------

const int delayStateSeq1_raw[4] = {1, 100, 2, 200};
const int delayStateSeq2_raw[8] = {1, 50, 2, 300, 1, 1000, 2, 300};
const int delayStateSeq3_raw[2] = {0, 1000};
// int delayStateSeq4_raw[2] = {0, 1000};


DelayStateSeq delayStateSeq1(delayStateSeq1_raw, 2);
DelayStateSeq delayStateSeq2(delayStateSeq2_raw, 4);
DelayStateSeq delayStateSeq3(delayStateSeq3_raw, 1);
// DelayStateSeq delayStateSeq4(delayStateSeq4_raw, 1);

DelayStateSeq* masterDelayStateSeq[4];

void setDelayStateSeq()
{
	masterDelayStateSeq[0] = &delayStateSeq1;
	masterDelayStateSeq[1] = &delayStateSeq2;
	masterDelayStateSeq[2] = &delayStateSeq3;
	// masterDelayStateSeq[3] = &delayStateSeq4;
}

//------------------------------------------------
//------------- Delay Loop Length ----------------
//------------------------------------------------

const int delayLoopLengthSeq1_raw[4] = {65, 100, 40, 100};
const int delayLoopLengthSeq2_raw[8] = {65, 50, 40, 300, 65, 1000, 40, 300};
const int delayLoopLengthSeq3_raw[2] = {40, 1000};
// const int delayLoopLengthSeq4_raw[2] = {40, 1000};


DelayLoopLengthSeq delayLoopLengthSeq1(delayLoopLengthSeq1_raw, 2);
DelayLoopLengthSeq delayLoopLengthSeq2(delayLoopLengthSeq2_raw, 4);
DelayLoopLengthSeq delayLoopLengthSeq3(delayLoopLengthSeq3_raw, 1);
// DelayLoopLengthSeq delayLoopLengthSeq4(delayLoopLengthSeq4_raw, 1);

DelayLoopLengthSeq* masterDelayLoopLengthSeq[4];

void setDelayLoopLengthSeq()
{
	masterDelayLoopLengthSeq[0] = &delayLoopLengthSeq1;
	masterDelayLoopLengthSeq[1] = &delayLoopLengthSeq2;
	masterDelayLoopLengthSeq[2] = &delayLoopLengthSeq3;
	// masterDelayLoopLengthSeq[3] = &delayLoopLengthSeq4;
}

//------------------------------------------------
//----------------- Formant Filter ---------------
//------------------------------------------------

const int formantSeq1_raw[24] = { a1, 1, 1000, e1, 0, 1000, i1, 1, 1000, o1, 0, 1000, u1, 1, 1000, o1, 0, 1000, i1, 1, 1000, e1, 0, 1000};
const int formantSeq2_raw[3] = { a1, 0, 1000};
const int formantSeq3_raw[3] = { a1, 0, 1000};
// const int formantSeq4_raw[3] = { a1, 0, 1000};

FormantFilterSequence formantSeq1(formantSeq1_raw, 8);
FormantFilterSequence formantSeq2(formantSeq2_raw, 1);
FormantFilterSequence formantSeq3(formantSeq3_raw, 1);
// FormantFilterSequence formantSeq4(formantSeq4_raw, 1);

FormantFilterSequence* masterFormantSeq[4];

void setFormantSeq()
{
	masterFormantSeq[0] = &formantSeq1;
	masterFormantSeq[1] = &formantSeq2;
	masterFormantSeq[2] = &formantSeq3;
	// masterFormantSeq[3] = &formantSeq4;
}

//------------------------------------------------
//----------------- LowPass Filter ---------------
//------------------------------------------------

const int LPSeq1_raw[8+1] = { 0, 400, 1000, 1000, 1000, 200, 1000, 6000, 1000};
const int LPSeq2_raw[2+1] = { 1, 700, 1000};
const int LPSeq3_raw[12+1] = {1, 1000 , 750 , 1000 , 0 , 500 , 1050 , 500 , 2226 , 2000 , 0 , 100 , 1335 };
const int LPSeq4_raw[2+1] = { 0, 100, 1000};
// const int LPSeq5_raw[2+1] = { 0, 100, 1000};

LPFilterSequence LPSeq1(LPSeq1_raw, 4);
LPFilterSequence LPSeq2(LPSeq2_raw, 1);
LPFilterSequence LPSeq3(LPSeq3_raw, 6);
LPFilterSequence LPSeq4(LPSeq4_raw, 1);
// LPFilterSequence LPSeq5(LPSeq5_raw, 1);

LPFilterSequence* masterLPSeq[5];

void setLPSeq()
{
	masterLPSeq[0] = &LPSeq1;
	masterLPSeq[1] = &LPSeq2;
	masterLPSeq[2] = &LPSeq3;
	masterLPSeq[3] = &LPSeq4;
	// masterLPSeq[4] = &LPSeq5;
}

//------------------------------------------------
//-------------------- Tone Osc ------------------
//------------------------------------------------

const int toneSeq1_1_raw[6] = {120, 35, 200, 0, 35, 400};
const int toneSeq1_2_raw[9] = {0, 42, 200, 120, 42, 200, 0, 42, 200};
const int toneSeq1_3_raw[6] = {0, 42, 400, 120, 49, 200};

const int toneSeq2_1_raw[12] = { 120 , 38 , 145 , 0 , 38 , 152 , 120 , 43 , 152 , 0 , 43 , 152 } ;
const int toneSeq2_2_raw[12] = { 120 , 47 , 145 , 120 , 42 , 152 , 120 , 52 , 152 , 120 , 40 , 152 } ;
const int toneSeq2_3_raw[27] = { 0 , 59 , 69 , 120 , 59 , 76 , 0 , 59 , 76 , 120 , 59 , 76 , 0 , 59 , 114 , 120 , 59 , 38 , 0 , 59 , 76 , 120 , 59 , 38 , 0 , 59 , 38 } ;

const int toneSeq3_1_raw[3] = {
  0, 38, 1000 };
const int toneSeq3_2_raw[3] = {
  0, 40, 1000 };
const int toneSeq3_3_raw[3] = {
  0, 42, 1000 };

// const int toneSeq4_1_raw[3] = {
  // 0, 38, 1000 };
// const int toneSeq4_2_raw[3] = {
  // 0, 40, 1000 };
// const int toneSeq4_3_raw[3] = {
  // 0, 42, 1000 };
  
ToneSequence toneSeq1_1(toneSeq1_1_raw, 2, &osc1, TONE_TYPE_SQUARE);
ToneSequence toneSeq1_2(toneSeq1_2_raw, 3, &osc2, TONE_TYPE_SQUARE);
ToneSequence toneSeq1_3(toneSeq1_3_raw, 2, &osc3, TONE_TYPE_SQUARE);

ToneSequence toneSeq2_1(toneSeq2_1_raw, 4, &osc1, TONE_TYPE_SQUARE);
ToneSequence toneSeq2_2(toneSeq2_2_raw, 4, &osc2, TONE_TYPE_SQUARE);
ToneSequence toneSeq2_3(toneSeq2_3_raw, 9, &osc3, TONE_TYPE_SQUARE);

ToneSequence toneSeq3_1(toneSeq3_1_raw, 1, &osc1, TONE_TYPE_SQUARE);
ToneSequence toneSeq3_2(toneSeq3_2_raw, 1, &osc2, TONE_TYPE_SQUARE);
ToneSequence toneSeq3_3(toneSeq3_3_raw, 1, &osc3, TONE_TYPE_SQUARE);

// ToneSequence toneSeq4_1(toneSeq4_1_raw, 1, &osc1, TONE_TYPE_SQUARE);
// ToneSequence toneSeq4_2(toneSeq4_2_raw, 1, &osc2, TONE_TYPE_SQUARE);
// ToneSequence toneSeq4_3(toneSeq4_3_raw, 1, &osc3, TONE_TYPE_SQUARE);

ToneSequence* masterToneSeq[12];

void setToneSeq()
{
	masterToneSeq[0] = &toneSeq1_1;
	masterToneSeq[1] = &toneSeq1_2;
	masterToneSeq[2] = &toneSeq1_3;
	masterToneSeq[3] = &toneSeq2_1;
	masterToneSeq[4] = &toneSeq2_2;
	masterToneSeq[5] = &toneSeq2_3;
	              
	masterToneSeq[6] = &toneSeq3_1;
	masterToneSeq[7] = &toneSeq3_2;
	masterToneSeq[8] = &toneSeq3_3;
	
	// masterToneSeq[9] = &toneSeq4_1;
	// masterToneSeq[10] = &toneSeq4_2;
	// masterToneSeq[11] = &toneSeq4_3;
}

#endif
