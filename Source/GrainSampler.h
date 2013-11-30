#ifndef __GRAINSAMPLER_H__
#define __GRAINSAMPLER_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Phasor.h"

enum class GrainSamplerParameter
{
	Speed,
	Gain,
	GrainSize,
	Direction
};

class GrainSampler
{
public:
	GrainSampler(void);
	~GrainSampler(void);

public:
	void initialize(double sampleRate);
	void loadFromFile(File &file);
	float processSample(int channel);

	void setParameterValue(GrainSamplerParameter parameter, float value);

private:
	Phasor phasor;
	float sampleRate;
	float rate;
	float grainSize;
	float gain;
	float direction;
	
	AudioFormatManager formatManager;
	ScopedPointer<AudioFormatReader> reader;
	ScopedPointer<AudioSampleBuffer> sampleBuffer;
	//ScopedPointer<Phasor> phasor;
};

#endif //__GRAINSAMPLER_H__