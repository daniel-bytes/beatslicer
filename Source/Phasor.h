#ifndef __PHASOR_H__
#define __PHASOR_H__

class Phasor
{
public:
	Phasor();

public:
	float calculateNextPhase();

	void setSampleRate(double value);
	void setPlaybackRate(float value);
	void setDirection(bool value);
	void setBufferSize(int value, double bufferSampleRate);
	void setCurrentPhase(float value);
	void setBpmOffset(float value);
	
	float getCurrentPhase(void) const { return phase; }
	int getBufferSize(void) const { return bufferSize; }

private:
	void initialize();

protected:
	float sampleRate;
	float playbackRate;
	float bpmOffset;
	bool direction;
	int bufferSize;
	float bufferSampleRate;
	float phase;
	float increment;
};

#endif // __PHASOR_H__