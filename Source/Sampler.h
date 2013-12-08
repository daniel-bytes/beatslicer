#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Phasor.h"

enum class SamplerParameter
{
	Speed,
	Gain,
	GrainSize,
	Direction,
	Pitch,
	FilePath,
	Phase
};

class Sampler
{
public:
	Sampler();
	~Sampler(void);

public:
	void initialize(double sampleRate);
	float processSample(int channel);
	void loadFile(AudioFormatManager *formatManager, String filePath);

	void setParameterValue(SamplerParameter parameter, var value);
	float getSamplerPhase(void) const { return phase.getCurrentPhase(); }
	float getSamplerBufferSize(void) const { return (float)phase.getBufferSize(); }

private:
	void initializePhasor(void);

private:
	Phasor phase;
	float sampleRate;
	float rate;
	float grainSize;
	float grainRate;
	float gain;
	float direction;
	
	String filePath;
	ScopedPointer<AudioFormatReader> reader;
	ScopedPointer<AudioSampleBuffer> sampleBuffer;
	//ScopedPointer<Phasor> phasor;
};

#endif //__SAMPLER_H__