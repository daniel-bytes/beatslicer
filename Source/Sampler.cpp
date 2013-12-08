#include "Sampler.h"
#include "Utilities.h"

Sampler::Sampler(void)
	: rate(0), grainSize(0), gain(0), direction(0), grainRate(0)
{
}

Sampler::~Sampler(void)
{
	this->sampleBuffer = nullptr;
	this->reader = nullptr;
}

void Sampler::initialize(double sampleRate)
{
	this->sampleRate = (float)sampleRate;
}

float Sampler::processSample(int channel)
{
	if (this->sampleBuffer == nullptr || sampleBuffer->getNumChannels() == 0) {
		return 0.f;
	}

	int numSamples = sampleBuffer->getNumSamples();
	int channelIndex = (channel % sampleBuffer->getNumChannels());
	float *channelData = *(this->sampleBuffer->getArrayOfChannels() + channelIndex);

	// get interpolated valued
	float value = linearInterpolate(channelData, numSamples, phase.getCurrentPhase());


	if (channel == 0) {
		phase.calculateNextPhase();
		//grain1Phase.calculateNextPhase();
		//grain2Phase.calculateNextPhase();
	}

	return value * gain;
}

void Sampler::loadFile(AudioFormatManager *formatManager, String filePath)
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
		
		this->initializePhasor();
    }
}


void Sampler::setParameterValue(SamplerParameter parameter, var value)
{
	switch(parameter)
	{
	case SamplerParameter::Gain:
		this->gain = value;
		break;
	case SamplerParameter::Speed:
		this->rate = (float)value * 2.f;
		this->initializePhasor();
		break;
	case SamplerParameter::GrainSize:
		this->grainSize = value;
		this->initializePhasor();
		break;
	case SamplerParameter::Direction:
		this->direction = (float)value < .5f ? -1.f : 1.f;
		this->initializePhasor();
		break;
	case SamplerParameter::Pitch:
		this->grainRate = ((float)value * 4.f) - 2.f;
		this->initializePhasor();
		break;
	case SamplerParameter::FilePath:
		this->filePath = (String)value;
		break;
	case SamplerParameter::Phase:
		this->phase.setCurrentPhase((float)value * (float)this->phase.getBufferSize());
		break;
	}
}

void Sampler::initializePhasor(void)
{
	if (sampleBuffer != nullptr) {
		phase.initialize(sampleRate, sampleBuffer->getNumSamples(), rate, direction >= .5);
	}
}