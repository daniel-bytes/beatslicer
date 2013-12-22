#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include "../JuceLibraryCode/JuceHeader.h"

class Sampler
{
public:
	Sampler();
	~Sampler(void);

public:
	float processSample(int channel, float phase);
	void loadFile(AudioFormatManager *formatManager, String filePath);

	void setSampleRate(double value);
	void setGain(float value);
	void setGrainSize(float value);
	void setPitch(float value);

	int getSamplerBufferSize(void) const;
	float getSamplerBufferSampleRate(void) const;

private:
	float sampleRate;
	float bufferSampleRate;
	float grainSize;
	float grainRate;
	float gain;
	
	String filePath;
	ScopedPointer<AudioFormatReader> reader;
	ScopedPointer<AudioSampleBuffer> sampleBuffer;
};

#endif //__SAMPLER_H__