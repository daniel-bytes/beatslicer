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
	SerialOscController(String devicePrefix, int defaultPort);
	virtual ~SerialOscController(void);

	void start(Listener *listener);
	void stop(void);

	int getDeviceCount(void) const { return devices.size(); }

public:
	void sendDeviceQueryMessage(void);
	void sendDeviceNotifyMessage(void);
	void sendDeviceInfoMessage(int port);
	void sendDevicePortMessage(int port);
	void sendDevicePrefixMessage(int port);
	void sendDeviceLedCommand(int x, int y, bool state);
	void sendDeviceLedMapCommand(int xOffset, int yOffset, Array<int> values);
	void sendDeviceLedMapCommand(int xOffset, int yOffset, int row0, int row1, int row2, int row3, int row4, int row5, int row6, int row7);

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