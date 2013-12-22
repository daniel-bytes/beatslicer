#include "StepSequencerControl.h"
#include <limits>

StepSequencerControl::StepSequencerControl(int numSteps, int numRows)
	: numSteps(numSteps), numRows(numRows), currentStep(0)
{
	values.resize(numSteps);

	for (int i = 0; i < values.size(); i++) {
		values.set(i, SEQUENCER_STEP_OFF);
	}
}

StepSequencerControl::~StepSequencerControl(void)
{
}

void StepSequencerControl::setNumSteps(int value)
{
	numSteps = jlimit(1, std::numeric_limits<int>::max(), value);
}

void StepSequencerControl::setNumRows(int value)
{
	numRows = jlimit(1, std::numeric_limits<int>::max(), value);
}

void StepSequencerControl::setCurrentStep(int value)
{
	currentStep = jlimit(0, numSteps - 1, value);
}

void StepSequencerControl::setStepValue(int step, int value)
{
	step = jlimit(0, numSteps - 1, step);
	value = jlimit(SEQUENCER_STEP_OFF, numRows - 1, value);
	this->values.set(step, value);
}

void StepSequencerControl::setValues(const Array<int> &values)
{
	this->values.resize(values.size());

	for (int i = 0; i < values.size(); i++) {
		int newValue = jlimit(SEQUENCER_STEP_OFF, numRows - 1, values[i]);
		this->values.set(i, newValue);
	}
}

void StepSequencerControl::paint(Graphics &g)
{
	Rectangle<int> sequencerBounds(this->getWidth(), this->getHeight());

	g.fillAll(Colours::white);

	g.drawRect(sequencerBounds, 2);
}

void StepSequencerControl::mouseDown(const MouseEvent &event)
{
	(void)event;
}
