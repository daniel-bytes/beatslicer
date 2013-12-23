#ifndef __MONOMECONTROL_H__
#define __MONOMECONTROL_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ApplicationView.h"
#include "SerialOscController.h"

class UdpListeningReceiveSocket;
class ApplicationController;

class MonomeControl
	: public ApplicationView,
	  protected SerialOscController::Listener
{
public:
	MonomeControl(void);
	~MonomeControl(void);

public:
	void initialize(ApplicationController *controller);
	void repaint();
	virtual void setParameterValue(ParameterID parameter, var value);

protected:
	virtual void buttonPressMessageReceived(int x, int y, bool state);

private:
	ScopedPointer<SerialOscController> serialosc;
	ApplicationController *controller;
};

#endif //__MONOMECONTROL_H__