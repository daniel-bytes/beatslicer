#include "Phasor.h"
#include "Utilities.h"

Phasor::Phasor()
	: sampleRate(0), frequency(0), value(0)
{
}

float Phasor::process()
{
	float inc = (1.0f / sampleRate) * frequency;

	if ((value += inc) >= 1.f) {
		value -= 1.f;
	}
	else if (value < 0.f) {
		value += 1.f;
	}

	return value;
}