#include "GrainSampler.h"
#include "Utilities.h"

GrainSampler::GrainSampler(void)
	: rate(0), grainSize(0), gain(0), direction(0)
{
    formatManager.registerBasicFormats();
}

GrainSampler::~GrainSampler(void)
{
	this->sampleBuffer = nullptr;
	this->reader = nullptr;
}

void GrainSampler::initialize(double sampleRate)
{
	this->sampleRate = (float)sampleRate;
}

float GrainSampler::processSample(int channel)
{
	if (this->sampleBuffer == nullptr || sampleBuffer->getNumChannels() == 0) {
		return 0.f;
	}

	int numSamples = sampleBuffer->getNumSamples();
	int channelIndex = (channel % sampleBuffer->getNumChannels());
	float *channelData = *(this->sampleBuffer->getArrayOfChannels() + channelIndex);

	// get interpolated valued
	float value = linearInterpolate(channelData, numSamples, phasor.getCurrentPhase());
	//float value = channelData[(int)phasor.phase];

	if (channel == 0) {
		phasor.calculateNextPhase();
	}

	return value * gain;
}

void GrainSampler::loadFromFile(File &file)
{
	this->sampleBuffer = nullptr;
	this->reader = nullptr;

    this->reader = formatManager.createReaderFor(file);
	
    if (reader != nullptr) { 
		int numSamples = (int)reader->lengthInSamples;
		this->sampleBuffer = new AudioSampleBuffer(reader->numChannels, numSamples);
		reader->read(this->sampleBuffer, 0, numSamples, 0, true, true);

		phasor.initialize(sampleRate, sampleBuffer->getNumSamples(), rate, direction >= .5);
    }
}

void GrainSampler::setParameterValue(GrainSamplerParameter parameter, float value)
{
	switch(parameter)
	{
	case GrainSamplerParameter::Gain:
		this->gain = value;
		break;
	case GrainSamplerParameter::Speed:
		this->rate = value * 2.f;
		if (sampleBuffer != nullptr) {
			phasor.initialize(sampleRate, sampleBuffer->getNumSamples(), rate, direction >= .5);
		}
		break;
	case GrainSamplerParameter::GrainSize:
		this->grainSize = value;
		break;
	case GrainSamplerParameter::Direction:
		this->direction = value < .5f ? -1.f : 1.f;
		if (sampleBuffer != nullptr) {
			phasor.initialize(sampleRate, sampleBuffer->getNumSamples(), rate, direction >= .5);
		}
		break;
	}
}