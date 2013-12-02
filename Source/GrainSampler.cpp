#include "GrainSampler.h"
#include "Utilities.h"

GrainSampler::GrainSampler(AudioFormatManager *formatManager)
	: rate(0), grainSize(0), gain(0), direction(0), grainRate(0)
{
	this->formatManager = formatManager;
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
	float value = linearInterpolate(channelData, numSamples, masterPhase.getCurrentPhase());
	//float value = channelData[(int)phasor.phase];

	if (channel == 0) {
		masterPhase.calculateNextPhase();
	}

	return value * gain;
}

void GrainSampler::loadFile(String filePath)
{
	this->filePath = filePath;

	File file(filePath);
	this->sampleBuffer = nullptr;
	this->reader = nullptr;

    this->reader = formatManager->createReaderFor(file);
	
    if (reader != nullptr) { 
		int numSamples = (int)reader->lengthInSamples;
		this->sampleBuffer = new AudioSampleBuffer(reader->numChannels, numSamples);
		reader->read(this->sampleBuffer, 0, numSamples, 0, true, true);

		masterPhase.initialize(sampleRate, sampleBuffer->getNumSamples(), rate, direction >= .5);
    }
}

void GrainSampler::setParameterValue(GrainSamplerParameter parameter, var value)
{
	switch(parameter)
	{
	case GrainSamplerParameter::Gain:
		this->gain = value;
		break;
	case GrainSamplerParameter::Speed:
		this->rate = (float)value * 2.f;
		if (sampleBuffer != nullptr) {
			masterPhase.initialize(sampleRate, sampleBuffer->getNumSamples(), rate, direction >= .5);
		}
		break;
	case GrainSamplerParameter::GrainSize:
		this->grainSize = value;
		break;
	case GrainSamplerParameter::Direction:
		this->direction = (float)value < .5f ? -1.f : 1.f;
		if (sampleBuffer != nullptr) {
			masterPhase.initialize(sampleRate, sampleBuffer->getNumSamples(), rate, direction >= .5);
		}
	case GrainSamplerParameter::Pitch:
		this->grainRate = ((float)value * 4.f) - 2.f;
		break;
	case GrainSamplerParameter::FilePath:
		loadFile((String)value);
		break;
	}
}