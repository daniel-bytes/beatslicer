#include "StepSequencer.h"
#include "Utilities.h"

StepSequencer::StepSequencer(int numSteps, int numRows, int ticksPerStep)
	: data(nullptr), listener(nullptr)
{
	data = new StepSequencerData();
	data->numSteps = numSteps;
	data->numRows = numRows;
	data->ticksPerStep = ticksPerStep;
	configureSteps();
}

StepSequencer::~StepSequencer(void)
{
	delete data;
	data = nullptr;
}

void StepSequencer::setNumSteps(int value)
{
	data->numSteps = value;
	configureSteps();
}

void StepSequencer::setNumRows(int value)
{
	data->numRows = value;
	configureSteps();
}

void StepSequencer::setNumStepsAndRows(int steps, int rows)
{
	data->numSteps = steps;
	data->numRows = rows;
	configureSteps();
}

void StepSequencer::setTicksPerStep(int value)
{
	data->ticksPerStep = value;
	configureSteps();
}

void StepSequencer::setAllValues(Array<var> *newValues)
{
	data->numSteps = newValues->size();
	data->values.resize(newValues->size());

	for (int i = 0; i < newValues->size(); i++) {
		StepSequencerValue value = StepSequencerValue::deserialize((*newValues)[i]);
		data->values.set(i, value);
	}
}

void StepSequencer::setListener(Listener *value)
{
	this->listener = value;
	configureSteps();
}

void StepSequencer::setStepValue(int step, StepSequencerValue value)
{
	data->values.set(step, value);
}

void StepSequencer::onClockStep(double ppq)
{
	int clockPos = (int)floor(ppq) % data->numSteps;
	
	if (data->currentStep != clockPos) {
		data->currentStep = jlimit(0, data->numSteps - 1, clockPos);

		if (listener != nullptr) {
			auto value = data->values[data->currentStep];
			listener->onStepTriggered(*this, data->currentStep, value);
		}
	}
}

void StepSequencer::configureSteps(void)
{
	data->values.resize(data->numSteps);

	for (int i = 0; i < data->numRows; i++) {
		StepSequencerValue value = { i, jlimit(0, data->numRows - 1, data->values[i].value), data->values[i].isSet };
		data->values.set(i, value);
	}
}
