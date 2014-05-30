/* 
 * *** SeqDef.h ****
 *
 * created by Lukas Vogel 09.05.14
 * 
 */
 
 #ifndef DeqDef_h_
#define SeqDef_h_

#include <vector>

//------------------------------------------------
//----------------- Sequence Base ----------------
//------------------------------------------------
extern bool seqReset;

class Sequence
{
public:
	Sequence(int* _seq, int _seqLength)
		:seq(_seq),
		pSeq(_seq),
		seqLength(_seqLength)
	{}
	~Sequence() {}
	
	void seqProceed()
	{}

	void reset()
	{
		pSeq = seq;
		seqCounter = 0;
		stepCounter = 0;	
	}
	
	float speed = 1.0;
	int stepCounter = 0;
	int seqCounter = 0;
	int* seq;
	int* pSeq;
	int* pSeqNext;
	int seqLength = 0;
	bool loop = true;
};


//------------------------------------------------
//------------------ Tone Volume -----------------
//------------------------------------------------

extern AudioMixer4 mixSources;

class ToneVolumeSeq : public Sequence
{
public:
	ToneVolumeSeq(int* _seq, int _seqLength)
		: Sequence(_seq, _seqLength)
	{}
	~ToneVolumeSeq() {}
	
	void setInterpolation(float _nextGain_dB)
	{
		gain_dB_step = (_nextGain_dB - gain_dB) / (float)stepCounter;
	}
	
	void seqProceed()
	{
		
		if (stepCounter == 0)
		{
			while(*(pSeq+1) == 0) // to handle delay length 0
			{			
				seqCounter++;			
				if(seqCounter+1 < seqLength)
					pSeq = pSeq + 2;
				else
					pSeq = seq;
			}
			
			gain_dB = *pSeq++ + mainGainLevel;			
			float gain = pow(10.0, (float)gain_dB/20.0);
			mixSources.gain(0, gain);
			
			stepCounter = ((*pSeq++) / (MOD_RATE)) * (1.0/speed);
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
				
			if(seqCounter+1 < seqLength)
				pSeqNext = pSeq;
			else
				pSeqNext = seq;
				
			float nextGain_dB= *pSeqNext + mainGainLevel;
			setInterpolation(nextGain_dB);

		}
		else
		{
			gain_dB = gain_dB + gain_dB_step;
			float gain = pow(10, (float)gain_dB/20.0);
			mixSources.gain(0, gain);
			
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
		}
	}
	
	float mainGainLevel = 6.0;
	float gain_dB;
	float gain_dB_step;
};

//------------------------------------------------
//----------------- Input Volume -----------------
//------------------------------------------------

extern AudioMixer4 inMix;
extern uint8_t inputVolume;

class InVolumeSeq : public Sequence
{
public:
	InVolumeSeq(int* _seq, int _seqLength)
		: Sequence(_seq, _seqLength)
	{}
	~InVolumeSeq() {}
	
	void setInterpolation(float _nextGain_dB)
	{
		gain_dB_step = (_nextGain_dB - gain_dB) / (float)stepCounter;
	}
	
	void seqProceed()
	{
		
		if (stepCounter == 0)
		{
			while(*(pSeq+1) == 0) // to handle delay length 0
			{			
				seqCounter++;			
				if(seqCounter+1 < seqLength)
					pSeq = pSeq + 2;
				else
					pSeq = seq;
			}
			
			gain_dB = *pSeq++ + mainGainLevel - (inputVolume);			
			float gain = pow(10.0, (float)gain_dB/20.0);
			inMix.gain(0, gain);
			
			stepCounter = ((*pSeq++) / (MOD_RATE)) * (1.0/speed);
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
				
			if(seqCounter+1 < seqLength)
				pSeqNext = pSeq;
			else
				pSeqNext = seq;
				
			float nextGain_dB= *pSeqNext + mainGainLevel - (inputVolume);
			setInterpolation(nextGain_dB);

		}
		else
		{
			gain_dB = gain_dB + gain_dB_step - (inputVolume);
			float gain = pow(10, (float)gain_dB/20.0);
			inMix.gain(0, gain);
			
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
		}
	}
	
	float mainGainLevel = 0;
	float gain_dB;
	float gain_dB_step;
};

//------------------------------------------------
//----------------- Delay State ------------------
//------------------------------------------------

extern AudioEffectDelay staticDelay;
extern AudioMixer4 mixSources;
extern float delayVolume;

class DelayStateSeq : public Sequence
{
public:	
	DelayStateSeq(int* _seq, int _seqLength)
		: Sequence(_seq, _seqLength)
	{}
	~DelayStateSeq() {}

	void seqProceed()
	{	
		if (stepCounter == 0)
		{
			switch(*pSeq++)
			{
				// delay mute
				case 0: mixSources.gain(0, 0.00004);
						staticDelay.hold(false);
						staticDelay.setTargetDelay(AUDIO_BLOCK_SAMPLES * 64);
						break;
				// normal delay mode
				case 1: mixSources.gain(0, delayVolume);
						staticDelay.hold(false);						
						staticDelay.setTargetDelay(AUDIO_BLOCK_SAMPLES * 64);
						break;
				// delay hold
				case 2: mixSources.gain(0, delayVolume);
						staticDelay.hold(true);
						staticDelay.setTargetDelay(AUDIO_BLOCK_SAMPLES * 64);
						break;
				// talkThrough without delay
				case 3: mixSources.gain(0, delayVolume);
						staticDelay.hold(false);
						staticDelay.setTargetDelay(0);
						break;
				// delay oneShot mode
				case 4:	break;
				default: break;			
			}			
			stepCounter = *pSeq++ * (1.0/speed);
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
		}
		else
		{
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
		}
	}
};

//------------------------------------------------
//------------- Delay Loop Length ----------------
//------------------------------------------------

// loop length value can vary between 1 and 65 (this represents value of 1*128 and 65*128 samples)

class DelayLoopLengthSeq : public Sequence
{
public:	
	DelayLoopLengthSeq(int* _seq, int _seqLength)
		: Sequence(_seq, _seqLength)
	{}
	~DelayLoopLengthSeq() {}

	void seqProceed()
	{	
		if (stepCounter == 0)
		{
			staticDelay.setLoopLength(AUDIO_BLOCK_SAMPLES * (*pSeq++)); // equal the delayBufferLength of 8192 + AUDIO_BLOCK_SAMPLES			
			stepCounter = *pSeq++ * (1.0/speed);
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
		}
		else
		{
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
		}
	}
};

//------------------------------------------------
//----------------- Formant Filter ---------------
//------------------------------------------------

#define a1 0
#define e1 1
#define i1 2
#define o1 3
#define u1 4

extern AudioMixer4 mixFormants;

class FormantFilterSequence : public Sequence
{
public:
	FormantFilterSequence(int* _seq, int _seqLength)
		: Sequence(_seq, _seqLength)
	{}
	~FormantFilterSequence() {}
	
	void setFormants(float* _fc, float* _gain, float* _BW) {
		float A_overall1 = pow(10, (_gain[0]-4)/20);
		float A_overall2 = pow(10, (_gain[1]-4)/20);
		float A_overall3 = pow(10, (_gain[2]-4)/20);
		mixFormants.gain(1, A_overall1);
		mixFormants.gain(2, A_overall2);
		mixFormants.gain(3, A_overall3);
		
		// calcBiquad(FilterType,FrequencyC,dBgain,BW,QuantizationUnit,SampleRate,int*);
		calcBiquad(FILTER_BANDPASS,_fc[0],0,_BW[0],2147483648,44100,updateFilter);
		formantFilter1.updateCoefs(updateFilter);
		calcBiquad(FILTER_BANDPASS,_fc[1],0,_BW[1],2147483648,44100,updateFilter);
		formantFilter2.updateCoefs(updateFilter);
		calcBiquad(FILTER_BANDPASS,_fc[2],0,_BW[2],2147483648,44100,updateFilter);
		formantFilter3.updateCoefs(updateFilter);
		fc[0] = _fc[0];
		fc[1] = _fc[1];
		fc[2] = _fc[2];
		gain[0] = _gain[0];
		gain[1] = _gain[1];
		gain[2] = _gain[2];
		BW[0] = _BW[0];
		BW[1] = _BW[1];
		BW[2] = _BW[2];
	}
	
	void setInterpolation(float* _fc, float* _gain, float* _BW)
	{
		fc_delta[0] = (_fc[0] - fc[0]) / (float)stepCounter;
		fc_delta[1] = (_fc[1] - fc[1]) / (float)stepCounter;
		fc_delta[2] = (_fc[2] - fc[2]) / (float)stepCounter;
		gain_delta[0] = (_gain[0] - gain[0]) / (float)stepCounter;
		gain_delta[1] = (_gain[1] - gain[1]) / (float)stepCounter;
		gain_delta[2] = (_gain[2] - gain[2]) / (float)stepCounter;
		BW_delta[0] = (_BW[0] - BW[0]) / (float)stepCounter;
		BW_delta[1] = (_BW[1] - BW[1]) / (float)stepCounter;
		BW_delta[2] = (_BW[2] - BW[2]) / (float)stepCounter;
	}
	
	void seqProceed()
	{
		if (stepCounter == 0)
		{
			switch (*pSeq++)
			{
				case a1: 	setFormants(sopranA_fc, sopranA_gain, sopranA_BW);
							break;
				case e1: 	setFormants(sopranE_fc, sopranE_gain, sopranE_BW);
							break;
				case i1: 	setFormants(sopranI_fc, sopranI_gain, sopranI_BW);
							break;
				case o1: 	setFormants(sopranO_fc, sopranO_gain, sopranO_BW);
							break;
				case u1:	setFormants(sopranU_fc, sopranU_gain, sopranU_BW);
							break;
				default:	break;
			}
			formantInterpolation = *pSeq++;
			stepCounter = ((*pSeq++) / (MOD_RATE)) * (1.0/speed);
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
			
			if(formantInterpolation)
			{
				if(seqCounter+1 < seqLength)
					pSeqNext = pSeq;
				else
					pSeqNext = seq;
						
				switch (*pSeqNext)
				{
					case a1: 	setInterpolation(sopranA_fc, sopranA_gain, sopranA_BW);
								break;          
					case e1: 	setInterpolation(sopranE_fc, sopranE_gain, sopranE_BW);
								break;          
					case i1: 	setInterpolation(sopranI_fc, sopranI_gain, sopranI_BW);
								break;          
					case o1: 	setInterpolation(sopranO_fc, sopranO_gain, sopranO_BW);
								break;          
					case u1:	setInterpolation(sopranU_fc, sopranU_gain, sopranU_BW);
								break;
					default:	break;
				}
				formantIsInterpolating = true;
			}
			else
				formantIsInterpolating = false;
		}
		else
		{
			if(formantIsInterpolating)
			{
				fc[0] += fc_delta[0];
				fc[1] += fc_delta[1];
				fc[2] += fc_delta[2];
				gain[0] += gain_delta[0];
				gain[1] += gain_delta[1];
				gain[2] += gain_delta[2];
				BW[0] += BW_delta[0];
				BW[1] += BW_delta[1];
				BW[2] += BW_delta[2];
				setFormants(fc, gain, BW);
			}
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
		}
	}

	bool formantInterpolation = true;
	bool formantIsInterpolating = false;
	
	float fc[3] = {440, 440, 440};
	float gain[3] = {0, 0, 0};
	float BW[3] = {0.5, 0.5, 0.5};
	
	float fc_delta[3] = {0, 0, 0};
	float gain_delta[3] = {0, 0, 0};
	float BW_delta[3] = {0, 0, 0};
	
	float factor = 0.3;
	float sopranA_fc[3] = {800, 1150, 2900};
	float sopranA_gain[3] = {0, -6, -32};
	float sopranA_BW[3] = {0.5*factor, 0.5*factor, 0.3*factor};
	float sopranE_fc[3] = {350, 2000, 2800};
	float sopranE_gain[3] = {0, -20, -15};
	float sopranE_BW[3] = {0.5*factor, 0.4*factor, 0.3*factor};
	float sopranI_fc[3] = {270, 2140, 2950};
	float sopranI_gain[3] = {0, -12, -26};
	float sopranI_BW[3] = {0.5*factor, 0.4*factor, 0.3*factor};
	float sopranO_fc[3] = {450, 800, 2830};
	float sopranO_gain[3] = {0, -11, -22};
	float sopranO_BW[3] = {0.5*factor, 0.4*factor, 0.3*factor};
	float sopranU_fc[3] = {325, 700, 2700};
	float sopranU_gain[3] = {0, -16, -35};
	float sopranU_BW[3] = {0.5*factor, 0.4*factor, 0.3*factor};
	
};

//------------------------------------------------
//----------------- LowPass Filter ---------------
//------------------------------------------------

extern AudioFilterBiquad LowPass;

class LPFilterSequence : public Sequence
{
public:
	LPFilterSequence(int* _seq, int _seqLength)
		: Sequence(_seq, _seqLength)
	{}
	~LPFilterSequence() {}
	
	void seqProceed()
	{
		if (stepCounter == 0)
		{
			while(*(pSeq+1) == 0) // to handle delay length 0
			{
				seqCounter++;			
				if(seqCounter+1 < seqLength)
					pSeq = pSeq + 2;
				else
					pSeq = seq+1;
			}	
			freq = *pSeq++;
			//freq = pow(2.0f,((freq+53.0f)/12.0f));
			calcBiquad(FILTER_LOPASS,freq,0,0.5,2147483648,44100,updateFilter);
			LowPass.updateCoefs(updateFilter);
			stepCounter = ((*pSeq++) / (MOD_RATE)) * (1.0/speed);
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
			
			if(freqInterpolation)
			{
				if(seqCounter+1 < seqLength)
					pSeqNext = pSeq;
				else
					pSeqNext = seq + 1;
				
				freq_delta = ((float)*pSeqNext - freq) / (float)stepCounter;
				freqIsInterpolating = true;
			}
			else
				freqIsInterpolating = false;
			
		}
		else
		{
			if(freqIsInterpolating)
			{
				freq += freq_delta;
				//freq = pow(2.0f,((freq+53.0f)/12.0f));
				// calcBiquad(FilterType,FrequencyC,dBgain,BW,QuantizationUnit,SampleRate,int*);
				calcBiquad(FILTER_LOPASS,freq,0,0.5,2147483648,44100,updateFilter);
				LowPass.updateCoefs(updateFilter);
			}
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
		}
	}
	
	void reset()
	{
		freqInterpolation = *seq;
		pSeq = seq + 1;
		seqCounter = 0;
		stepCounter = 0;	
	}

	bool freqInterpolation = true;
	bool freqIsInterpolating = false;
	
	float freq = 440;	
	float freq_delta = 0;
};

//------------------------------------------------
//-------------------- Tone Osc ------------------
//------------------------------------------------

class ToneSequence : public Sequence
{
public:	
	ToneSequence(int* _seq, int _seqLength, AudioSynthWaveform* _osc, short _waveform)
		: Sequence(_seq, _seqLength),
		osc(_osc),
		waveform(_waveform)
	{}
	~ToneSequence() {}

	void seqProceed()
	{
		if (stepCounter <= 0)
		{
			if(*pSeq != 0) // start note
			{
				float velocity = 0.3 * ((float)*pSeq++/128.0);
				osc->begin(velocity, tune_frequencies2_PGM[*pSeq++], waveform);
			}
			else	// stop note
			{
				osc->amplitude(0);
				pSeq++;
				pSeq++;
			}
			stepCounter = *pSeq++ * (1.0/speed);
			stepCounter--;
			if(stepCounter == 0)
			 seqCounter++;
		}
		else
		{
			stepCounter--;
			if(stepCounter == 0)
			 seqCounter++;
		}
	}

	short waveform;
	AudioSynthWaveform* osc;
};

#endif