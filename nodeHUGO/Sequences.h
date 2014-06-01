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

const int toneVolumeSeq1_raw[8] = { -60, 2000, -60, 1000, -60, 2000, -60, 1000 };
const int toneVolumeSeq2_raw[4] = { 0, 1000, 0, 1000 };

ToneVolumeSeq toneVolumeSeq1(toneVolumeSeq1_raw, 4);
ToneVolumeSeq toneVolumeSeq2(toneVolumeSeq2_raw, 2);

std::vector<ToneVolumeSeq*> masterToneVolSeq;

void setToneVolSeq()
{
	masterToneVolSeq.push_back(&toneVolumeSeq1);
	masterToneVolSeq.push_back(&toneVolumeSeq2);
}

//------------------------------------------------
//---------------- Input Volume ------------------
//------------------------------------------------

const int inVolumeSeq1_raw[8] = { 0, 1000, 0, 2000, 0, 1000,  0, 2000};
const int inVolumeSeq2_raw[4] = { -30, 1000, -20, 1000 };

InVolumeSeq inVolumeSeq1(inVolumeSeq1_raw, 4);
InVolumeSeq inVolumeSeq2(inVolumeSeq2_raw, 2);

std::vector<InVolumeSeq*> masterInVolSeq;

void setInVolSeq()
{
	masterInVolSeq.push_back(&inVolumeSeq1);
	masterInVolSeq.push_back(&inVolumeSeq2);
}

//------------------------------------------------
//----------------- Delay State ------------------
//------------------------------------------------

const int delayStateSeq1_raw[4] = {1, 100, 2, 200};
const int delayStateSeq2_raw[8] = {1, 50, 2, 300, 1, 1000, 2, 300};


DelayStateSeq delayStateSeq1(delayStateSeq1_raw, 2);
DelayStateSeq delayStateSeq2(delayStateSeq2_raw, 4);

std::vector<DelayStateSeq*> masterDelayStateSeq;

void setDelayStateSeq()
{
	masterDelayStateSeq.push_back(&delayStateSeq1);
	masterDelayStateSeq.push_back(&delayStateSeq2);
}

//------------------------------------------------
//------------- Delay Loop Length ----------------
//------------------------------------------------

const int delayLoopLengthSeq1_raw[6] = {50, 1000, 65, 1000, 40, 1000};
const int delayLoopLengthSeq2_raw[8] = {20, 50, 65, 300, 20, 1000, 65, 300};


DelayLoopLengthSeq delayLoopLengthSeq1(delayLoopLengthSeq1_raw, 3);
DelayLoopLengthSeq delayLoopLengthSeq2(delayLoopLengthSeq2_raw, 4);

std::vector<DelayLoopLengthSeq*> masterDelayLoopLengthSeq;

void setDelayLoopLengthSeq()
{
	masterDelayLoopLengthSeq.push_back(&delayLoopLengthSeq1);
	masterDelayLoopLengthSeq.push_back(&delayLoopLengthSeq2);
}

//------------------------------------------------
//----------------- Formant Filter ---------------
//------------------------------------------------

const int formantSeq1_raw[24] = { a1, 1, 1000, e1, 0, 1000, i1, 1, 1000, o1, 0, 1000, u1, 1, 1000, o1, 0, 1000, i1, 1, 1000, e1, 0, 1000};
const int formantSeq2_raw[24] = { a1, 1, 1000, e1, 0, 1000, i1, 1, 1000, o1, 0, 1000, u1, 1, 1000, o1, 0, 1000, i1, 1, 1000, e1, 0, 1000};

FormantFilterSequence formantSeq1(formantSeq1_raw, 8);
FormantFilterSequence formantSeq2(formantSeq2_raw, 8);

std::vector<FormantFilterSequence*> masterFormantSeq;

void setFormantSeq()
{
	masterFormantSeq.push_back(&formantSeq1);
	masterFormantSeq.push_back(&formantSeq2);
}

//------------------------------------------------
//----------------- LowPass Filter ---------------
//------------------------------------------------
//{ const interpolation, Freq, De}

const int LPSeq1_raw[16+1] = { 0, 800, 1000, 1200, 1000, 600, 1000, 6000, 1000, 800, 700, 1200, 1000, 300, 1000, 7000, 1000};
const int LPSeq2_raw[2+1] = { 1, 700, 1000};
//const int LPSeq3_raw[12+1] = {1,1000 , 750 , 1000 , 0 , 500 , 1050 , 500 , 2226 , 2000 , 0 , 100 , 1335 };

LPFilterSequence LPSeq1(LPSeq1_raw, 8);
LPFilterSequence LPSeq2(LPSeq2_raw, 1);
//LPFilterSequence LPSeq3(LPSeq3_raw, 6);

std::vector<LPFilterSequence*> masterLPSeq;

void setLPSeq()
{
	masterLPSeq.push_back(&LPSeq1);
	masterLPSeq.push_back(&LPSeq2);
//	masterLPSeq.push_back(&LPSeq3);
}

//------------------------------------------------
//-------------------- Tone Osc ------------------
//------------------------------------------------

const int toneSeq1_1_raw[6] = {120, 35, 200, 0, 35, 400};
const int toneSeq1_2_raw[9] = {0, 42, 200, 120, 42, 200, 0, 42, 200};
const int toneSeq1_3_raw[6] = {0, 42, 400, 120, 49, 200};

const int toneSeq2_1_raw[12] = { 100 , 26 , 1000 , 0 , 30 , 800 , 120 , 38 , 1500 , 0 , 30 , 300 } ;
const int toneSeq2_2_raw[12] = { 100 , 47 , 1400, 120 , 42 , 4000 , 120 , 52 , 3000 , 120 , 40 , 2000 } ;
const int toneSeq2_3_raw[27] = { 0 , 59 , 1300 , 120 , 59 , 2010 , 0 , 59 , 1000 , 120 , 59 , 5000 , 0 , 59 , 400 , 120 , 59 , 3000 , 0 , 59 , 300 , 120 , 59 , 200 , 0 , 59 , 300 } ;

ToneSequence toneSeq1_1(toneSeq1_1_raw, 2, &osc1, TONE_TYPE_SQUARE);
ToneSequence toneSeq1_2(toneSeq1_2_raw, 3, &osc2, TONE_TYPE_SQUARE);
ToneSequence toneSeq1_3(toneSeq1_3_raw, 2, &osc3, TONE_TYPE_SQUARE);

ToneSequence toneSeq2_1(toneSeq2_1_raw, 4, &osc1, TONE_TYPE_SINE);
ToneSequence toneSeq2_2(toneSeq2_2_raw, 4, &osc2, TONE_TYPE_SINE);
ToneSequence toneSeq2_3(toneSeq2_3_raw, 9, &osc3, TONE_TYPE_SINE);

std::vector<ToneSequence*> masterToneSeq;

void setToneSeq()
{
	masterToneSeq.push_back(&toneSeq1_1);
	masterToneSeq.push_back(&toneSeq1_2);
	masterToneSeq.push_back(&toneSeq1_3);

	masterToneSeq.push_back(&toneSeq2_1);
	masterToneSeq.push_back(&toneSeq2_2);
	masterToneSeq.push_back(&toneSeq2_3);
}

#endif
