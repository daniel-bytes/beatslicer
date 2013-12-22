#ifndef __CLOCKPHASOR_H__
#define __CLOCKPHASOR_H__

#include "..\JuceLibraryCode\JuceHeader.h"

class ClockPhasor
{
public:
	int getNumBars(void) const { return numBars; }
	void setNumBars(int numBars) { this->numBars = numBars; }
	float getCurrentPhase(void) const { return phase; }

	void setPhase(double clockPosition);

private:
	int numBars;
	float phase;
};

#endif //__CLOCKPHASOR_H__