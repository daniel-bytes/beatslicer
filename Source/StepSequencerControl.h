#ifndef __STEPSEQUENCERCONTROL_H__
#define __STEPSEQUENCERCONTROL_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "StepSequencerValue.h"

class ApplicationController;

class StepSequencerControl
	: public Component
{
public:
	StepSequencerControl(void);
	virtual ~StepSequencerControl(void);

	void initialize(ApplicationController *controller);

public:
    void paint(Graphics &g);
	void mouseDown(const MouseEvent &event);
	void mouseDrag(const MouseEvent &event);

private:
	void onMouseEvent(const MouseEvent &event, bool isDrag);

private:
	int lastX, lastY;
	bool isDelete;
	ApplicationController *controller;
};

#endif //__STEPSEQUENCERCONTROL_H__