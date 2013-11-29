#ifndef __GRAINSAMPLER_H__
#define __GRAINSAMPLER_H__

#include "../JuceLibraryCode/JuceHeader.h"

enum class GrainSamplerParameter
{
	Speed,
	Gain,
	GrainSize
};

class Phasor;

class GrainSampler
{
public:
	GrainSampler(void);
	~GrainSampler(void);

public:
	void initialize(double sampleRate);
	float processSample(int channel);

	void setParameterValue(GrainSamplerParameter parameter, float value);
	float getParameterValue(GrainSamplerParameter parameter);

private:
	float speed;
	float grainSize;
	float gain;

	ScopedPointer<Phasor> phasor;
};

#endif //__GRAINSAMPLER_H__