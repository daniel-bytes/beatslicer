#include "Sampler.h"
#include "Utilities.h"

Sampler::Sampler(void)
	: sampleRate(0), bufferSampleRate(0), grainSize(0), grainRate(0), gain(0)
{
	this->sampleBuffer = nullptr;
	this->reader = nullptr;
}

Sampler::~Sampler(void)
{
	this->sampleBuffer = nullptr;
	this->reader = nullptr;
}

float Sampler::processSample(int channel, float phase)
{
	if (this->sampleBuffer == nullptr || sampleBuffer->getNumChannels() == 0) {
		return 0.f;
	}

	int numSamples = sampleBuffer->getNumSamples();
	int channelIndex = (channel % sampleBuffer->getNumChannels());
	float *channelData = *(this->sampleBuffer->getArrayOfChannels() + channelIndex);

	// get interpolated valued
	float value = linearInterpolate(channelData, numSamples, phase);

	return value * gain;
}

void Sampler::loadFile(AudioFormatManager *formatManager, String filePath)
{
	this->bufferSampleRate = 0;
	this->filePath = filePath;

	File file = filePath.contains(":") ? File(filePath) : File::getCurrentWorkingDirectory().getChildFile(filePath);
	this->sampleBuffer = nullptr;
	this->reader = nullptr;

    this->reader = formatManager->createReaderFor(file);
	
    if (reader != nullptr) { 
		int numSamples = (int)reader->lengthInSamples;
		this->sampleBuffer = new AudioSampleBuffer(reader->numChannels, numSamples);
		reader->read(this->sampleBuffer, 0, numSamples, 0, true, true);

		this->bufferSampleRate = (float)reader->sampleRate;
    }
}

void Sampler::setSampleRate(double value)
{
	this->sampleRate = (float)value;
}

void Sampler::setGain(float value)
{
	this->gain = value;
}

void Sampler::setGrainSize(float value)
{
	this->grainSize = value;
}

void Sampler::setPitch(float value)
{
	this->grainRate = value;
}

float Sampler::getSamplerBufferSampleRate(void) const
{
	return bufferSampleRate;
}

int Sampler::getSamplerBufferSize(void) const
{
	if (sampleBuffer == nullptr) {
		return 0;
	}

	return sampleBuffer->getNumSamples();
}