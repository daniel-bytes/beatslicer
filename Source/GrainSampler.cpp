#include "GrainSampler.h"
#include "Phasor.h"
#include "Utilities.h"

GrainSampler::GrainSampler(void)
	: rate(0), grainSize(0), gain(0), direction(0)
{	
	this->phasor = new Phasor();
}

GrainSampler::~GrainSampler(void)
{
	this->phasor = nullptr;
	this->sampleBuffer = nullptr;
	this->reader = nullptr;
}

void GrainSampler::initialize(double sampleRate)
{
	this->sampleRate = sampleRate;
	this->phasor->setSampleRate(sampleRate);
}

float GrainSampler::processSample(int channel)
{
	if (this->sampleBuffer == nullptr) {
		return 0.f;
	}

	if (channel >= this->sampleBuffer->getNumChannels()) {
		channel = (this->sampleBuffer->getNumChannels() - 1);
	}

	if (channel == 0) {
		this->phasor->process();
	}

	int maxIndex = this->sampleBuffer->getNumSamples() - 1;
	float **channels = this->sampleBuffer->getArrayOfChannels();
	float *channelData = channels[channel];

	float fIndex = clip(phasor->getValue(), 0.f, 1.f);
	int index = (int)(maxIndex * fIndex);
	int prevIndex = index == 0 ? 0 : index - 1;
	float value = (channelData[index] *.5f) + (channelData[prevIndex] * .5);

	return value * gain;
}

void GrainSampler::loadFromFile(File &file)
{
	this->sampleBuffer = nullptr;
	this->reader = nullptr;

	AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    this->reader = formatManager.createReaderFor(file);
	
    if (reader != nullptr)
    { 
		this->sampleBuffer = new AudioSampleBuffer(reader->numChannels, reader->lengthInSamples);
		reader->read(this->sampleBuffer, 0, reader->lengthInSamples, 0, true, true);
		initializePhasor();
    }
}

void GrainSampler::initializePhasor(void)
{
	float freq = 0;
	if (this->sampleBuffer != nullptr) {
		freq = this->sampleRate / this->sampleBuffer->getNumSamples();
		//float mult = this->sampleRate / this->reader->sampleRate;

		this->phasor->setFrequency(freq * this->rate * this->direction);
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
		initializePhasor();
		break;
	case GrainSamplerParameter::GrainSize:
		this->grainSize = value;
		break;
	case GrainSamplerParameter::Direction:
		this->direction = value < .5f ? -1.f : 1.f;
		initializePhasor();
		break;
	}
}