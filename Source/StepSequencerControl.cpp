#include "StepSequencerControl.h"
#include "ApplicationController.h"
#include "StepSequencerValue.h"
#include <limits>

StepSequencerControl::StepSequencerControl(int numSteps, int numRows)
	: numSteps(numSteps), numRows(numRows), currentStep(0), lastX(-1), lastY(-1), isDelete(false)
{
	configureSteps();
}

StepSequencerControl::~StepSequencerControl(void)
{
}

void StepSequencerControl::initialize(ApplicationController *controller)
{
	this->controller = controller;
}

void StepSequencerControl::setNumSteps(int value)
{
	numSteps = jlimit(1, std::numeric_limits<int>::max(), value);
	configureSteps();
	repaint();
}

void StepSequencerControl::setNumRows(int value)
{
	numRows = jlimit(1, std::numeric_limits<int>::max(), value);
	configureSteps();
	repaint();
}

void StepSequencerControl::setNumStepsAndRows(int steps, int rows)
{
	numSteps = jlimit(1, std::numeric_limits<int>::max(), steps);
	numRows = jlimit(1, std::numeric_limits<int>::max(), rows);
	configureSteps();
	repaint();
}

void StepSequencerControl::setCurrentStep(int value)
{
	currentStep = jlimit(0, numSteps - 1, value);
	repaint();
}

void StepSequencerControl::setStepValue(int step, int value)
{
	step = jlimit(0, numSteps - 1, step);
	value = jlimit(SEQUENCER_STEP_OFF, numRows - 1, value);
	this->values.set(step, value);
	repaint();
}

void StepSequencerControl::setAllValues(Array<var> *values)
{
	this->values.resize(values->size());

	for (int i = 0; i < values->size(); i++) {
		int newValue = jlimit(SEQUENCER_STEP_OFF, numRows - 1, (int)(*values)[i]);
		this->values.set(i, newValue);
	}

	repaint();
}

void StepSequencerControl::paint(Graphics &g)
{
	Rectangle<int> sequencerBounds(this->getWidth(), this->getHeight());

	g.fillAll(Colours::white);

	g.drawRect(sequencerBounds, 2);

	float fWidth = (float)getWidth();
	float fHeight = (float)getHeight();
	float stepMult = fWidth / (float)numSteps;
	float rowMult = fHeight / (float)numRows;

	// Set current playback position
	g.setColour(Colours::red);
	float x = (float)currentStep * stepMult;
	Rectangle<float> currentStepBounds(x, 0.f, stepMult, fHeight);
	g.fillRect(currentStepBounds);

	// draw rows and fill selected steps
	for (int i = 0; i < numSteps; i++) {
		float x = (float)i * stepMult;

		if (values[i] > SEQUENCER_STEP_OFF) {
			g.setColour(Colours::green);
			
			float y = (float)(numRows - values[i] - 1) * rowMult;
			g.fillRect(x + 1, y + 1, stepMult - 2, rowMult - 2);
		}

		g.setColour(Colours::grey);

		g.drawLine(x, 0, x, (float)getHeight());
	}

	// draw columns
	for (int i = 0; i < numRows; i++) {
		g.setColour(Colours::grey);

		float y = (float)i * rowMult;
		g.drawLine(0, y, fWidth, y); 
	}
}

void StepSequencerControl::mouseDown(const MouseEvent &event)
{
	if (event.mods.isLeftButtonDown()) {
		lastX = -1;
		lastY = -1;

		onMouseEvent(event, false);
	}
}

void StepSequencerControl::mouseDrag(const MouseEvent &event)
{
	if (event.mods.isLeftButtonDown()) {
		onMouseEvent(event, true);
	}
}

void StepSequencerControl::onMouseEvent(const MouseEvent &event, bool isDrag)
{
	auto position = event.getPosition();

	// Get the current step based on mouse coordinates
	float xdistance = jlimit(0.f, .9999f, (float)position.getX() / (float)this->getWidth());
	int stepNumber = (int)floor(xdistance * (float)numSteps);

	float ydistance = jlimit(0.f, .9999f, (float)position.getY() / (float)this->getHeight());
	int stepValue = numRows - (int)floor(ydistance * (float)numRows) - 1;

	// if we are dragging and we haven't hit a new part of the grid, return;
	if (lastX == stepNumber && lastY == stepValue) {
		return;
	}

	// If we are dragging and the last action was a delete, we only want to delete from now.
	// Same if the last action was a draw (in that case just draw, no delete).
	if (isDrag) {
		if (isDelete) {
			if (stepValue == values[stepNumber]) {
				stepValue = SEQUENCER_STEP_OFF;
			}
			else {
				return;
			}
		}
		else {
			stepValue = values[stepNumber] == stepValue ? SEQUENCER_STEP_OFF : stepValue;
		}
	}
	else {
		lastX = stepNumber;
		lastY = stepValue;

		stepValue = values[stepNumber] == stepValue ? SEQUENCER_STEP_OFF : stepValue;
		isDelete = stepValue == SEQUENCER_STEP_OFF;
	}

	// Don't allow the first step to be deleted
	if (stepNumber == 0 && stepValue == SEQUENCER_STEP_OFF) {
		return;
	}

	// Update values and notify the rest of the system
	this->setStepValue(stepNumber, stepValue);
	
	controller->updateParameterModel(ParameterID::Sequencer_StepChange, (int)SequencerStep(stepNumber, stepValue));

	repaint();
}

void StepSequencerControl::configureSteps(void)
{
	values.resize(numSteps);

	for (int i = 0; i < values.size(); i++) {
		values.set(i, SEQUENCER_STEP_OFF);
	}
}