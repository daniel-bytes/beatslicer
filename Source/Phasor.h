#ifndef __PHASOR_H__
#define __PHASOR_H__

class Phasor
{
public:
	Phasor();

public:
	float process();

public:
	void setSampleRate(float value) {
		sampleRate = value;
	}

	void setFrequency(float value) {
		frequency = value;
	}

private:
	float sampleRate;
	float frequency;
	float value;
};

#endif // __PHASOR_H__