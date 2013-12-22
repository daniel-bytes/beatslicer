#ifndef __STEPSEQUENCERCONTROL_H__
#define __STEPSEQUENCERCONTROL_H__

#include "../JuceLibraryCode/JuceHeader.h"

class ApplicationController;

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
	void setNumStepsAndRows(int steps, int rows);
	void setCurrentStep(int value);
	void setStepValue(int step, int value);
	void setAllValues(Array<var> *values);

	void initialize(ApplicationController *controller);

public:
    void paint(Graphics &g);
	void mouseDown(const MouseEvent &event);
	void mouseDrag(const MouseEvent &event);

private:
	void configureSteps(void);
	void onMouseEvent(const MouseEvent &event, bool isDrag);

private:
	int lastX, lastY;
	bool isDelete;
	int numSteps;
	int numRows;
	int currentStep;
	Array<int> values;
	ApplicationController *controller;
};

#endif //__STEPSEQUENCERCONTROL_H__