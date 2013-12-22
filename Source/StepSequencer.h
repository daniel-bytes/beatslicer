#ifndef __STEPSEQUENCER_H__
#define __STEPSEQUENCER_H__

#include "../JuceLibraryCode/JuceHeader.h"

#define SEQUENCER_STEP_OFF -1

struct StepSequencerValue
{
public:
	StepSequencerValue()
	{
		value = SEQUENCER_STEP_OFF;
	}

	StepSequencerValue(int value)
	{
		this->value = value;
	}

	bool hasValue(void) const { return value > SEQUENCER_STEP_OFF; }

	int value;
};

class StepSequencer
{
public:
	StepSequencer(int numSteps, int numRows, int ticksPerStep);
	virtual ~StepSequencer(void);

public:
	int getNumSteps(void) const { return numSteps; }
	int getNumRows(void) const { return numRows; }
	int getTicksPerStep(void) const { return ticksPerStep; }
	int getCurrentStep(void) const { return currentStep; }

	void setNumSteps(int value);
	void setNumRows(int value);
	void setTicksPerStep(int value);
	void setStepValue(int step, int value);
	void setValues(const Array<StepSequencerValue> &values);
	
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
	int numSteps;
	int numRows;
	int ticksPerStep;
	int currentStep;
	Listener *listener;
	Array<StepSequencerValue> values;
};

#endif //__STEPSEQUENCER_H__