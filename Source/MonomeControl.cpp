#include "MonomeControl.h"
#include "ApplicationController.h"
#include "StepSequencerData.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

#define SERIALOSC_CONTROLLER_PORT 12003

MonomeControl::MonomeControl(void)
	: serialosc(nullptr)
{
	serialosc = new SerialOscController("beatslicer", SERIALOSC_CONTROLLER_PORT);
	serialosc->start(this);
}

MonomeControl::~MonomeControl(void)
{
	serialosc->stop();
	serialosc = nullptr;
}

void MonomeControl::initialize(ApplicationController *controller)
{
	this->controller = controller;
	repaint();
}

void MonomeControl::repaint()
{	
	if (serialosc->getDeviceCount() == 0) {
		return;
	}

	// Only handling 8x8 grid for now
	Array<int> values;
	values.resize(8);

	auto data = this->controller->getSequencerData();
	
	for (int i = 0; i < data->numSteps && i < 8; i++) {
		StepSequencerValue value = data->values[i];
		
		if (!value.isSet || value.value > 7) {
			continue;
		}

		int index = (7 - value.value);
		values.set(index, values[index] | (1 << i));
	}

	serialosc->sendDeviceLedMapCommand(0, 0, values);
}

void MonomeControl::setParameterValue(ParameterID parameter, var value)
{
	switch(parameter)
	{
	case ParameterID::Sequencer_StepChange:
	case ParameterID::Sequencer_CurrentStep:
		repaint();
		break;
	}
}

void MonomeControl::buttonPressMessageReceived(int x, int y, bool state)
{
	if (!state) {
		return;
	}

	StepSequencerValue value;
	auto data = controller->getSequencerData();

	y = (7 - y);

	if (x > data->numRows || y > data->numRows) {
		return;
	}

	value.step = x;
	value.value = y;
	value.isSet = data->values[x].value == y ? !data->values[x].isSet : true;

	controller->updateParameterModel(ParameterID::Sequencer_StepChange, value.serialize());
}