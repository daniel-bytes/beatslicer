#include "GrainSampler.h"
#include "Phasor.h"

GrainSampler::GrainSampler(void)
	: speed(0), grainSize(0), gain(0)
{	
	this->phasor = new Phasor();
}

GrainSampler::~GrainSampler(void)
{
	this->phasor = nullptr;
}

void GrainSampler::initialize(double sampleRate)
{
	this->phasor->setSampleRate(sampleRate);
}

float GrainSampler::processSample(int channel)
{
	float value = this->phasor->process();

	return value * gain;
}


void GrainSampler::setParameterValue(GrainSamplerParameter parameter, float value)
{
	switch(parameter)
	{
	case GrainSamplerParameter::Gain:
		gain = value;
		break;
	case GrainSamplerParameter::Speed:
		speed = value;
		this->phasor->setFrequency(100.f + (value * 2000.f));
		break;
	case GrainSamplerParameter::GrainSize:
		grainSize = value;
		break;
	}
}

float GrainSampler::getParameterValue(GrainSamplerParameter parameter)
{
	switch(parameter)
	{
	case GrainSamplerParameter::Gain:
		return gain;
		break;
	case GrainSamplerParameter::Speed:
		return speed;
		break;
	case GrainSamplerParameter::GrainSize:
		return grainSize;
		break;
	}
}