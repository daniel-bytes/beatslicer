#ifndef __MONOMEDEVICE_H__
#define __MONOMEDEVICE_H__

#include "../JuceLibraryCode/JuceHeader.h"

struct MonomeDevice
{
	String id;
	String type;
	int port;
	int width;
	int height;
	String prefix;
	int rotation;
};

#endif //__MONOMEDEVICE_H__