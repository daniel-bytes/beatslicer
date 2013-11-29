#ifndef __GRAINSAMPLER_H__
#define __GRAINSAMPLER_H__

#include "../JuceLibraryCode/JuceHeader.h"

enum class GrainSamplerParameter
{
	Speed,
	Gain,
	GrainSize,
	Direction
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
	void loadFromFile(File &file);

	void setParameterValue(GrainSamplerParameter parameter, float value);

private:
	void initializePhasor();

private:
	float sampleRate;
	float rate;
	float grainSize;
	float gain;
	float direction;

	ScopedPointer<AudioFormatReader> reader;
	ScopedPointer<AudioSampleBuffer> sampleBuffer;
	ScopedPointer<Phasor> phasor;
};

#endif //__GRAINSAMPLER_H__