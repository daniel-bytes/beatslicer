#ifndef __GRAINSAMPLER_H__
#define __GRAINSAMPLER_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Phasor.h"

enum class GrainSamplerParameter
{
	Speed,
	Gain,
	GrainSize,
	Direction,
	Pitch,
	FilePath
};

class GrainSampler
{
public:
	GrainSampler(AudioFormatManager *formatManager);
	~GrainSampler(void);

public:
	void initialize(double sampleRate);
	float processSample(int channel);
	void loadFile(String filePath);

	void setParameterValue(GrainSamplerParameter parameter, var value);

private:
	Phasor masterPhase;
	float sampleRate;
	float rate;
	float grainSize;
	float grainRate;
	float gain;
	float direction;
	
	AudioFormatManager *formatManager;
	String filePath;
	ScopedPointer<AudioFormatReader> reader;
	ScopedPointer<AudioSampleBuffer> sampleBuffer;
	//ScopedPointer<Phasor> phasor;
};

#endif //__GRAINSAMPLER_H__