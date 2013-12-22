#include "StepSequencer.h"
#include "Utilities.h"

StepSequencer::StepSequencer(int numSteps, int numRows, int ticksPerStep)
	: numSteps(numSteps), numRows(numRows), ticksPerStep(ticksPerStep), currentStep(-1), listener(nullptr)
{
	configureSteps();
}

StepSequencer::~StepSequencer(void)
{
}

void StepSequencer::setNumSteps(int value)
{
	this->numSteps = value;
	configureSteps();
}

void StepSequencer::setNumRows(int value)
{
	this->numRows = value;
	configureSteps();
}

void StepSequencer::setTicksPerStep(int value)
{
	this->ticksPerStep = value;
	configureSteps();
}

void StepSequencer::setValues(const Array<StepSequencerValue> &values)
{
	this->values.clear();

	for (StepSequencerValue value : values) {
		StepSequencerValue v = jlimit(SEQUENCER_STEP_OFF, numRows - 1, value.value);
		this->values.add(v);
	}
}

void StepSequencer::setListener(Listener *value)
{
	this->listener = value;
	configureSteps();
}

void StepSequencer::setStepValue(int step, int value)
{
	step = jlimit(0, numSteps - 1, step);
	value = jlimit(SEQUENCER_STEP_OFF, numRows - 1, value);
	values.set(step, value);
}

void StepSequencer::onClockStep(double ppq)
{
	int clockPos = (int)ppq % numSteps;
	
	if (currentStep != clockPos) {
		currentStep = jlimit(0, numSteps - 1, clockPos);

		if (listener != nullptr) {
			listener->onStepTriggered(*this, currentStep, values[currentStep]);
		}
	}
}

void StepSequencer::configureSteps(void)
{
	values.resize(numSteps);

	for (int i = 0; i < numRows; i++) {
		StepSequencerValue value = jlimit(SEQUENCER_STEP_OFF, numRows - 1, values[i].value);
		values.set(i, value);
	}
}
