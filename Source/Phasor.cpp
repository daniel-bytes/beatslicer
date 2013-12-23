#include "Phasor.h"
#include "Utilities.h"

Phasor::Phasor()
	: sampleRate(0), 
	  playbackRate(0), 
	  direction(true),
	  bufferSize(0),
	  bufferSampleRate(0),
	  phase(0),
	  increment(0),
	  bpmOffset(1.0f)
{
}

void Phasor::setCurrentPhase(float value) 
{ 
	phase = value; 
}

void Phasor::setSampleRate(double value) 
{ 
	this->sampleRate = (float)value;
	this->initialize();
}

void Phasor::setPlaybackRate(float value) 
{ 
	this->playbackRate = value; 
	this->initialize();
}

void Phasor::setDirection(bool value) 
{ 
	this->direction = value; 
	this->initialize();
}

void Phasor::setBufferSize(int value, double bufferSampleRate) 
{ 
	this->bufferSize = value; 
	this->bufferSampleRate = (float)bufferSampleRate;
	this->initialize();
}

void Phasor::setBpmOffset(float value)
{
	this->bpmOffset = value;
	this->initialize();
}

float Phasor::calculateNextPhase()
{
	if ((phase += increment) >= (float)bufferSize) {
		phase = 0.f;
	}
	else if (phase < 0.f) {
		phase = (float)(bufferSize - 1);
	}

	return phase;
}

void Phasor::initialize() {
	if (bufferSize == 0 || sampleRate == 0 || bufferSampleRate == 0) {
		return;
	}

	float maxSamples = (float)bufferSize;
	float secondsPerSample = (1.f / sampleRate);
	float speedMultiplier = (playbackRate * (direction ? 1.f : -1.f));
	float frequencyMultiplier = (sampleRate / maxSamples);
	float sampleRateCorrection = this->sampleRate / this->bufferSampleRate;

	this->increment = ((((secondsPerSample * speedMultiplier) * frequencyMultiplier) * sampleRateCorrection) * maxSamples) * bpmOffset;
}
