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
 
 void DelayTap::Read_WR(DelayBuffer& delayBuffer, uint32_t* outBuffer, int targetDelay, uint32_t bufferSize, int loopLength)
 {
	int readEndPtr = delayBuffer.mWritePtr - targetDelay;
	// make sure it is within buffer bounds
	if (readEndPtr < 0)
		readEndPtr += loopLength;
		
	mReadPtr = readEndPtr - bufferSize;
	
	if (mReadPtr < 0)
		mReadPtr += loopLength;
	
	for (size_t i = 0; i < bufferSize; i++)
	{
		outBuffer[i] = delayBuffer.mContent[mReadPtr++];
		
		if (mReadPtr >= loopLength)
		  mReadPtr -= loopLength;
	}
 }
 
  void AudioEffectDelay::update(void)
 {

	audio_block_t* block;
	block = receiveWritable();
	if(!block) return;
	
	mDelay.Write((uint32_t*)block->data, AUDIO_BLOCK_SAMPLES, mHold);

		
	myTap.Read_WR(mDelay, (uint32_t*)block->data, targetDelay, AUDIO_BLOCK_SAMPLES, loopLength);
	
	transmit(block);
	release(block);
 }
