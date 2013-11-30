#include "Phasor.h"
#include "Utilities.h"

Phasor::Phasor()
	: value(0), increment(0), bufferSize(0)
{
}

void Phasor::calculateNextPhase()
{
	if ((value += increment) >= (float)bufferSize) {
		value = 0.f;
	}
	else if (value < 0.f) {
		value = (float)(bufferSize - 1);
	}
}

void Phasor::initialize(float sampleRate, int bufferSize, float rate, bool direction) {
	this->bufferSize = bufferSize;
	float maxSamples = (float)bufferSize;

	float freqMult = sampleRate / maxSamples;
	//float sampleRateCorrection = this->sampleRate / (float)this->reader->sampleRate;
	increment = (1.f / sampleRate) * maxSamples * rate * freqMult * (direction ? 1.f : -1.f);
}