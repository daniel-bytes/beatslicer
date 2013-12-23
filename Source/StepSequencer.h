#ifndef __STEPSEQUENCER_H__
#define __STEPSEQUENCER_H__

#include "../JuceLibraryCode/JuceHeader.h"

#include "StepSequencerData.h"

class StepSequencer
{
public:
	StepSequencer(int numSteps, int numRows, int ticksPerStep);
	virtual ~StepSequencer(void);

public:
	// return a copy of the current sequencer data
	const StepSequencerData* getSequencerData(void) const { return data; }

	int getNumSteps(void) const { return data->numSteps; }
	int getNumRows(void) const { return data->numRows; }

	void setNumSteps(int value);
	void setNumRows(int value);
	void setNumStepsAndRows(int steps, int rows);
	void setTicksPerStep(int value);
	void setStepValue(int step, StepSequencerValue value);
	void setAllValues(Array<var> *newValues);
	
	void onClockStep(double ppq);

public:
	class Listener
	{
	public:
		virtual void onStepTriggered(const StepSequencer &source, int step, StepSequencerValue value) = 0;
	};

	void setListener(Listener *value);

private:
	void configureSteps(void);

private:
	StepSequencerData *data;
	Listener *listener;
};

#endif //__STEPSEQUENCER_H__