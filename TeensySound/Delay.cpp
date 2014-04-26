/* 
 * *** Delay.cpp ****
 *
 * created by Lukas Vogel 31.03.14
 * 
 */
 
 #include "Delay.h"
 
void DelayBuffer::Write(uint32_t* inBuffer, uint32_t bufferSize, bool hold)
 {
	if(!hold)
	{
		if(bufferSize < mDelayLength - mWritePtr)
		{
			for(size_t i=0; i<bufferSize; i++)
			{
				mContent[mWritePtr++] = inBuffer[i];
			}
		}
		else
		{
			size_t i = 0;
			while(mWritePtr < mDelayLength)
			{
				mContent[mWritePtr++] = inBuffer[i++];
			}
			mWritePtr = 0;
			while(i < bufferSize)
			{
				mContent[mWritePtr++] = inBuffer[i++];
			}
		}
	}
	else
	{
		mWritePtr += bufferSize;
		if(mWritePtr > mDelayLength)
			mWritePtr-=mDelayLength;
	}
 }
 
 void DelayTap::Read_WR(DelayBuffer& delayBuffer, uint32_t* outBuffer, float targetDelay, uint32_t bufferSize)
 {
	float readEndPtr = (float)delayBuffer.mWritePtr - targetDelay;
	// make sure it is within buffer bounds
	if (readEndPtr < 0)
		readEndPtr += (float)delayBuffer.mDelayLength;

	// Length and stepsize of interpolated read
	// number of read samples is altered by difference between old and new delay
	// if target delay hase not changed, step == 1.0

	float length = readEndPtr - mReadPtr;
	// make sure it is within buffer bounds

	if (length < 0)
		length += delayBuffer.mDelayLength;
	float step = length / (float)bufferSize;
	
	for (size_t i = 0; i < bufferSize; i++)
	{
		size_t readInt = (size_t)mReadPtr;
		float readFrac = mReadPtr - readInt;

		float x1, x2;
		if (readInt != delayBuffer.mDelayLength - 1)
		{
		  x1 = (float)delayBuffer.mContent[readInt];
		  x2 = (float)delayBuffer.mContent[readInt + 1];
		}
		else
		{
		  x1 = (float)delayBuffer.mContent[readInt];
		  x2 = (float)delayBuffer.mContent[0];
		}

		//outBuffer[i] = LinearInterpolation(x1, x2, readFrac);

		//allpass interpolation
		mOldValue = (readFrac*x2 + x1 - readFrac*mOldValue);
		outBuffer[i] = (uint32_t) mOldValue;

		mReadPtr += step;
		if (mReadPtr >= (float)delayBuffer.mDelayLength)
		  mReadPtr -= (float)delayBuffer.mDelayLength;
		if (mReadPtr < 0)
		  mReadPtr += (float)delayBuffer.mDelayLength;
	}	
	mReadPtr = readEndPtr;

 }
 
  void Delay::update(void)
 {

	audio_block_t* block;
	block = receiveWritable();
	if(!block) return;
	
	mDelay.Write((uint32_t*)block->data, AUDIO_BLOCK_SAMPLES, mHold);

		
	myTap.Read_WR(mDelay, (uint32_t*)block->data, 512, AUDIO_BLOCK_SAMPLES);
	
	transmit(block);
	release(block);
	
 }