#ifndef __SERIALOSCCONTROLLER_H__
#define __SERIALOSCCONTROLLER_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"
#include "MonomeDevice.h"

class UdpListeningReceiveSocket;


// see http://monome.org/docs/tech:osc for info on serialosc protocol
// see also http://monome.org/community/discussion/comment/186906
class SerialOscController
	: protected osc::OscPacketListener,
	  protected Thread
{
public:
	struct Listener
	{
		virtual void buttonPressMessageReceived(int x, int y, bool state) = 0;
	};

public:
	SerialOscController(String devicePrefix);
	virtual ~SerialOscController(void);

	void start(Listener *listener);
	void stop(void);

public:
	void sendDeviceQueryMessage(void);
	void sendDeviceNotifyMessage(void);
	void sendDeviceInfoMessage(int port);
	void sendDevicePortMessage(int port);
	void sendDevicePrefixMessage(int port);
	void sendDeviceLedCommand(int x, int y, bool state);

protected:
	virtual void run(void);
	virtual void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint);

private:
	ScopedPointer<UdpListeningReceiveSocket> socket;
	Listener *listener;
	HashMap<String, MonomeDevice*> devices;
	String currentDeviceID;
	String devicePrefix;
	int listenPort;
};

#endif //__SERIALOSCCONTROLLER_H__