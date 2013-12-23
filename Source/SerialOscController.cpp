#include "SerialOscController.h"
#include "osc/OscOutboundPacketStream.h"
#include <exception>

#define SERIALOSC_ADDRESS "127.0.0.1"
#define SERIALOSC_PORT 12002
#define OSC_BUFFER_SIZE 1024

SerialOscController::SerialOscController(String devicePrefix, int defaultPort)
	: socket(nullptr), 
	  listener(nullptr), 
	  Thread("SerialOscControllerThread"),
	  devicePrefix(devicePrefix),
	  listenPort(defaultPort)
{
}

SerialOscController::~SerialOscController(void)
{
	if (socket != nullptr) {
		stop();
	}

	socket = nullptr;
	listener = nullptr;

	HashMap<String, MonomeDevice*>::Iterator iter(devices);
	while (iter.next()) {
		delete iter.getValue();
	}
	devices.clear();
}

void SerialOscController::start(Listener *listener)
{
	this->listener = listener;

	if (listener == nullptr || socket != nullptr) {
		return;
	}
	
	for (int i = 0; i < 1000; i++) {
		int tempPort = listenPort + i;

		try {
			socket = new UdpListeningReceiveSocket(
					IpEndpointName( IpEndpointName::ANY_ADDRESS, tempPort ),
					this);

			listenPort = tempPort;
			break;
		}
		catch(std::runtime_error) {
			// try next port
			socket = nullptr;
		}
	}

	if (socket != nullptr) {
		startThread();
	
		sendDeviceQueryMessage();
		sendDeviceNotifyMessage();
	}
}

void SerialOscController::stop(void)
{
	if (socket != nullptr) {
		socket->AsynchronousBreak();
		waitForThreadToExit(1000);
		socket = nullptr;
	}
}
	
void SerialOscController::run(void)
{
	if (socket != nullptr) {
		socket->Run();
	}
}

void SerialOscController::ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint)
{
	(void)remoteEndpoint;

	String address = m.AddressPattern();
	auto iter = m.ArgumentsBegin();

	try {
		if (address == "/serialosc/device" || address == "/serialosc/add") {
			String id = (iter++)->AsString();
			String type = (iter++)->AsString();
			int port = (iter++)->AsInt32();
			MonomeDevice *device = nullptr;

			if (devices.contains(id)) {
				device = devices[id];
			}
			else {
				device = new MonomeDevice;
				devices.set(id, device);

				if (address == "/serialosc/add") {
					sendDeviceNotifyMessage();
				}
			}

			device->id = id;
			device->type = type;
			device->port = port;
			device->prefix = devicePrefix;

			if (!device->prefix.startsWith("/")) {
				device->prefix = "/" + device->prefix;
			}

			sendDeviceInfoMessage(device->port);

			sendDevicePrefixMessage(port);
			sendDevicePortMessage(port);
		}
		else if (address == "/serialosc/remove") {
			String id = (iter++)->AsString();

			if (devices.contains(id)) {
				delete devices[id];
				devices.remove(id);
				sendDeviceNotifyMessage();
			}
		}
		else if (address == "/sys/id") {
			currentDeviceID = (iter++)->AsString();
		}
		else if (address == "/sys/size") {
			if (devices.contains(currentDeviceID)) {
				auto device = devices[currentDeviceID];
				device->width = (iter++)->AsInt32();
				device->height = (iter++)->AsInt32();
			}
		}
		else if (address == "/sys/rotation") {
			if (devices.contains(currentDeviceID)) {
				auto device = devices[currentDeviceID];
				device->rotation = (iter++)->AsInt32();
			}
		}
		else if (address == ("/" + devicePrefix + "/grid/key")) {
			int x = (iter++)->AsInt32();
			int y = (iter++)->AsInt32();
			bool state = (iter++)->AsInt32() > 0;

			listener->buttonPressMessageReceived(x, y, state);
		}
	}
	catch(osc::WrongArgumentTypeException &exc) {
		printf("WrongArgumentTypeException: %s", exc.what());
	}
}

void SerialOscController::sendDeviceQueryMessage(void)
{
	UdpTransmitSocket transmitSocket( IpEndpointName( SERIALOSC_ADDRESS, SERIALOSC_PORT ) );
    char buffer[OSC_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OSC_BUFFER_SIZE );
    
    p << osc::BeginBundleImmediate
        << osc::BeginMessage( "/serialosc/list" )
			<< SERIALOSC_ADDRESS
			<< listenPort
            << osc::EndMessage
        << osc::EndBundle;
    
    transmitSocket.Send( p.Data(), p.Size() );
}

void SerialOscController::sendDeviceNotifyMessage(void)
{
	UdpTransmitSocket transmitSocket( IpEndpointName( SERIALOSC_ADDRESS, SERIALOSC_PORT ) );
    char buffer[OSC_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OSC_BUFFER_SIZE );
    
    p << osc::BeginBundleImmediate
        << osc::BeginMessage( "/serialosc/notify" )
			<< SERIALOSC_ADDRESS
			<< listenPort
            << osc::EndMessage
        << osc::EndBundle;
    
    transmitSocket.Send( p.Data(), p.Size() );
}

void SerialOscController::sendDeviceInfoMessage(int port)
{
	UdpTransmitSocket transmitSocket( IpEndpointName( SERIALOSC_ADDRESS, port ) );
    char buffer[OSC_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OSC_BUFFER_SIZE );
    
    p << osc::BeginBundleImmediate
        << osc::BeginMessage( "/sys/info" )
			<< listenPort
            << osc::EndMessage
        << osc::EndBundle;
    
    transmitSocket.Send( p.Data(), p.Size() );
}


void SerialOscController::sendDevicePortMessage(int port)
{
	UdpTransmitSocket transmitSocket( IpEndpointName( SERIALOSC_ADDRESS, port ) );
    char buffer[OSC_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OSC_BUFFER_SIZE );
    
    p << osc::BeginBundleImmediate
		<< osc::BeginMessage( "/sys/port" )
			<< listenPort
            << osc::EndMessage
        << osc::EndBundle;
    
    transmitSocket.Send( p.Data(), p.Size() );
}

void SerialOscController::sendDevicePrefixMessage(int port)
{
	String prefix = devicePrefix;

	if (prefix.startsWith("/")) {
		prefix = prefix.substring(1);
	}

	UdpTransmitSocket transmitSocket( IpEndpointName( SERIALOSC_ADDRESS, port ) );
    char buffer[OSC_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OSC_BUFFER_SIZE );
    
    p << osc::BeginBundleImmediate
        << osc::BeginMessage( "/sys/prefix" )
			<< prefix.toUTF8()
            << osc::EndMessage
        << osc::EndBundle;
    
    transmitSocket.Send( p.Data(), p.Size() );
}

void SerialOscController::sendDeviceLedCommand(int x, int y, bool state)
{
	HashMap<String, MonomeDevice*>::Iterator iter(devices);

	while (iter.next()) {
		auto device = iter.getValue();

		UdpTransmitSocket transmitSocket( IpEndpointName( SERIALOSC_ADDRESS, device->port ) );
		char buffer[OSC_BUFFER_SIZE];
		osc::OutboundPacketStream p( buffer, OSC_BUFFER_SIZE );
		String address = (device->prefix + "/grid/led/set");

		p << osc::BeginBundleImmediate
			<< osc::BeginMessage( address.toUTF8() )
				<< x
				<< y
				<< (state ? 1 : 0)
				<< osc::EndMessage
			<< osc::EndBundle;
    
		transmitSocket.Send( p.Data(), p.Size() );
	}
}

void SerialOscController::sendDeviceLedMapCommand(int xOffset, int yOffset, Array<int> values)
{
	if (values.size() < 8) {
		return;
	}

	sendDeviceLedMapCommand(xOffset, yOffset, values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7]);
}

void SerialOscController::sendDeviceLedMapCommand(int xOffset, int yOffset, int row0, int row1, int row2, int row3, int row4, int row5, int row6, int row7)
{
	HashMap<String, MonomeDevice*>::Iterator iter(devices);

	while (iter.next()) {
		auto device = iter.getValue();

		UdpTransmitSocket transmitSocket( IpEndpointName( SERIALOSC_ADDRESS, device->port ) );
		char buffer[OSC_BUFFER_SIZE];
		osc::OutboundPacketStream p( buffer, OSC_BUFFER_SIZE );
		String address = (device->prefix + "/grid/led/map");

		p << osc::BeginBundleImmediate
			<< osc::BeginMessage( address.toUTF8() )
				<< xOffset
				<< yOffset
				<< row0
				<< row1
				<< row2
				<< row3
				<< row4
				<< row5
				<< row6
				<< row7
				<< osc::EndMessage
			<< osc::EndBundle;
    
		transmitSocket.Send( p.Data(), p.Size() );
	}
}