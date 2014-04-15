/* 
 * *** Delay.h ****
 *
 * created by Lukas Vogel 31.03.14
 * 
 */
 
#ifndef Delay_h_
#define Delay_h_

#include "AudioStream.h"

class DelayBuffer;

class DelayTap
{
public:
	DelayTap()
	: mReadPtr(0),
	mOldValue(0)
	{}
	~DelayTap() {}
	void Read_WR(DelayBuffer& delayBuffer, uint32_t* outBuffer, float targetDelay, uint32_t bufferSize);
	
private:
	float mReadPtr;
	float mOldValue;
};

class DelayBuffer
{

friend class DelayTap;

public:
	DelayBuffer(size_t maxDelay)
	: mDelayLength(maxDelay),
	mWritePtr(0)
	{
		mContent = new uint32_t[mDelayLength];
		for(size_t i=0; i<mDelayLength; i++)
		{
			mContent[i] = 0;
		}

	}
	~DelayBuffer()
	{
		delete[] mContent;
	}
	
	void Write(uint32_t* inBuffer, uint32_t bufferSize, bool hold);
	
protected:
	uint32_t* mContent;
	size_t mDelayLength;
	size_t mWritePtr;
};

class Delay : public AudioStream
{
public:
	Delay() :
	AudioStream(1,inputQueueArray),
	mDelay(8192+AUDIO_BLOCK_SAMPLES),
	mHold(false)
	{}
	~Delay() {}

	virtual void update(void);
	void hold(bool hold) {
		mHold = hold;
	}
	
private:
	audio_block_t *inputQueueArray[1];
	DelayBuffer mDelay;
	DelayTap myTap;
	bool mHold;
};


#endif