#include "StepSequencerControl.h"
#include "ApplicationController.h"
#include "StepSequencerData.h"
#include <limits>

StepSequencerControl::StepSequencerControl(void)
	: lastX(-1), lastY(-1), isDelete(false)
{
}

StepSequencerControl::~StepSequencerControl(void)
{
}

void StepSequencerControl::initialize(ApplicationController *controller)
{
	this->controller = controller;
}

void StepSequencerControl::paint(Graphics &g)
{
	auto data = controller->getSequencerData();

	if (data == nullptr) {
		return;
	}
	Rectangle<int> sequencerBounds(this->getWidth(), this->getHeight());

	g.fillAll(Colours::white);

	g.drawRect(sequencerBounds, 2);

	float fWidth = (float)getWidth();
	float fHeight = (float)getHeight();
	float stepMult = fWidth / (float)data->numSteps;
	float rowMult = fHeight / (float)data->numRows;

	// Set current playback position
	g.setColour(Colours::red);
	float x = (float)data->currentStep * stepMult;
	Rectangle<float> currentStepBounds(x, 0.f, stepMult, fHeight);
	g.fillRect(currentStepBounds);

	// draw rows and fill selected steps
	for (int i = 0; i < data->numSteps; i++) {
		float x = (float)i * stepMult;

		StepSequencerValue currentValue = data->values[i];
		if (currentValue.isSet) {
			g.setColour(Colours::green);
			
			float y = (float)(data->numRows - currentValue.value - 1) * rowMult;
			g.fillRect(x + 1, y + 1, stepMult - 2, rowMult - 2);
		}

		g.setColour(Colours::grey);

		g.drawLine(x, 0, x, (float)getHeight());
	}

	// draw columns
	for (int i = 0; i < data->numRows; i++) {
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
	auto data = controller->getSequencerData();
	if (data == nullptr) {
		return;
	}
	auto position = event.getPosition();

	// Get the current step based on mouse coordinates
	float xdistance = jlimit(0.f, .9999f, (float)position.getX() / (float)this->getWidth());
	int stepNumber = (int)floor(xdistance * (float)data->numSteps);

	float ydistance = jlimit(0.f, .9999f, (float)position.getY() / (float)this->getHeight());
	int stepValue = data->numRows - (int)floor(ydistance * (float)data->numRows) - 1;

	// if we are dragging and we haven't hit a new part of the grid, return;
	if (isDrag && lastX == stepNumber && lastY == stepValue) {
		return;
	}
	
	// create a new value to return
	StepSequencerValue seqValue = { stepNumber, stepValue, true };

	if (data->values[stepNumber].value == stepValue) {
		seqValue.isSet = !data->values[stepNumber].isSet;
	}

	// If we are dragging and the last action was a delete, we only want to delete from now.
	// Same if the last action was a draw (in that case just draw, no delete).
	if (isDrag) {
		if (isDelete) {
			if (stepValue == seqValue.value) {
				seqValue.isSet = false;
			}
			else {
				return;
			}
		}
	}
	else {
		lastX = stepNumber;
		lastY = stepValue;
	}
	
	controller->updateParameterModel(ParameterID::Sequencer_StepChange, seqValue.serialize());

	repaint();
}