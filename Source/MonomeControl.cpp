#include "MonomeControl.h"

#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

MonomeControl::MonomeControl(void)
	: serialosc(nullptr)
{
	serialosc = new SerialOscController("beatslicer");
	serialosc->start(this);
}

MonomeControl::~MonomeControl(void)
{
	serialosc->stop();
	serialosc = nullptr;
}

void MonomeControl::setParameterValue(ParameterID parameter, var value)
{
	(void)parameter;
	(void)value;
}

void MonomeControl::buttonPressMessageReceived(int x, int y, bool state)
{
	serialosc->sendDeviceLedCommand(x, y, state);
}