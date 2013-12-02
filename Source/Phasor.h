#ifndef __PHASOR_H__
#define __PHASOR_H__

class Phasor
{
public:
	Phasor();

public:
	void calculateNextPhase();
	void initialize(float sampleRate, int bufferSize, float rate, bool direction);
	
	float getCurrentPhase(void) const { return value; }
	void setCurrentPhase(float phase) { value = phase; }
	int getBufferSize(void) const { return bufferSize; }

protected:
	int bufferSize;
	float value;
	float increment;
};

#endif // __PHASOR_H__