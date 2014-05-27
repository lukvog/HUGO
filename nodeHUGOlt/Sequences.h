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
//---------------------- Volume ------------------
//------------------------------------------------

int volumeSeq1_raw[8] = { 0, 2000, -40, 1000, 0, 2000, -40, 1000 };
int volumeSeq2_raw[4] = { -10, 1000, -50, 1000 };

VolumeSeq volumeSeq1(volumeSeq1_raw, 4);
VolumeSeq volumeSeq2(volumeSeq2_raw, 2);

std::vector<VolumeSeq*> masterVolSeq;

void setToneVolSeq()
{
	masterVolSeq.push_back(&volumeSeq1);
	masterVolSeq.push_back(&volumeSeq2);
}

//------------------------------------------------
//----------------- Delay State ------------------
//------------------------------------------------

int delayStateSeq1_raw[4] = {1, 100, 2, 200};
int delayStateSeq2_raw[8] = {1, 50, 2, 300, 1, 1000, 2, 300};


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

int delayLoopLengthSeq1_raw[4] = {50, 100, 65, 100};
int delayLoopLengthSeq2_raw[8] = {20, 50, 65, 300, 20, 1000, 65, 300};


DelayLoopLengthSeq delayLoopLengthSeq1(delayLoopLengthSeq1_raw, 2);
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

int formantSeq1_raw[24] = { a1, 1, 1000, e1, 0, 1000, i1, 1, 1000, o1, 0, 1000, u1, 1, 1000, o1, 0, 1000, i1, 1, 1000, e1, 0, 1000};
int formantSeq2_raw[3] = { a1, 0, 1000};

FormantFilterSequence formantSeq1(formantSeq1_raw, 8);
FormantFilterSequence formantSeq2(formantSeq2_raw, 1);

std::vector<FormantFilterSequence*> masterFormantSeq;

void setFormantSeq()
{
	masterFormantSeq.push_back(&formantSeq1);
	masterFormantSeq.push_back(&formantSeq2);
}

//------------------------------------------------
//----------------- LowPass Filter ---------------
//------------------------------------------------

int LPSeq1_raw[8+1] = { 0, 400, 1000, 1000, 1000, 200, 1000, 6000, 1000};
int LPSeq2_raw[2+1] = { 1, 700, 1000};
int LPSeq3_raw[12+1] = {1,1000 , 750 , 1000 , 0 , 500 , 1050 , 500 , 2226 , 2000 , 0 , 100 , 1335 };

LPFilterSequence LPSeq1(LPSeq1_raw, 4);
LPFilterSequence LPSeq2(LPSeq2_raw, 1);
LPFilterSequence LPSeq3(LPSeq3_raw, 6);

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

int toneSeq1_1_raw[6] = {120, 35, 200, 0, 35, 400};
int toneSeq1_2_raw[9] = {0, 42, 200, 120, 42, 200, 0, 42, 200};
int toneSeq1_3_raw[6] = {0, 42, 400, 120, 49, 200};

int toneSeq2_1_raw[12] = { 120 , 38 , 145 , 0 , 38 , 152 , 120 , 43 , 152 , 0 , 43 , 152 } ;
int toneSeq2_2_raw[12] = { 120 , 47 , 145 , 120 , 42 , 152 , 120 , 52 , 152 , 120 , 40 , 152 } ;
int toneSeq2_3_raw[27] = { 0 , 59 , 69 , 120 , 59 , 76 , 0 , 59 , 76 , 120 , 59 , 76 , 0 , 59 , 114 , 120 , 59 , 38 , 0 , 59 , 76 , 120 , 59 , 38 , 0 , 59 , 38 } ;

ToneSequence toneSeq1_1(toneSeq1_1_raw, 2, &osc1, TONE_TYPE_SQUARE);
ToneSequence toneSeq1_2(toneSeq1_2_raw, 3, &osc2, TONE_TYPE_SQUARE);
ToneSequence toneSeq1_3(toneSeq1_3_raw, 2, &osc3, TONE_TYPE_SQUARE);

ToneSequence toneSeq2_1(toneSeq2_1_raw, 4, &osc1, TONE_TYPE_SQUARE);
ToneSequence toneSeq2_2(toneSeq2_2_raw, 4, &osc2, TONE_TYPE_SQUARE);
ToneSequence toneSeq2_3(toneSeq2_3_raw, 9, &osc3, TONE_TYPE_SQUARE);

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
