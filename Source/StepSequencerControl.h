#ifndef __STEPSEQUENCERCONTROL_H__
#define __STEPSEQUENCERCONTROL_H__

#include "../JuceLibraryCode/JuceHeader.h"

#define SEQUENCER_STEP_OFF -1

class StepSequencerControl
	: public Component
{
public:
	StepSequencerControl(int numSteps, int numRows);
	virtual ~StepSequencerControl(void);

public:
	int getNumSteps(void) const { return numSteps; }
	int getNumRows(void) const { return numRows; }
	int getCurrentStep(void) const { return currentStep; }

	void setNumSteps(int value);
	void setNumRows(int value);
	void setCurrentStep(int value);
	void setStepValue(int step, int value);
	void setValues(const Array<int> &values);

public:
    void paint(Graphics &g);
	void mouseDown(const MouseEvent &event);

private:
	int numSteps;
	int numRows;
	int currentStep;
	Array<int> values;
};

#endif //__STEPSEQUENCERCONTROL_H__