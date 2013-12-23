#ifndef __STEPSEQUENCERVALUE_H__
#define __STEPSEQUENCERVALUE_H__

#include "../JuceLibraryCode/JuceHeader.h"

struct StepSequencerValue
{
public:
	static StepSequencerValue deserialize(var serializedValue)
	{
		return deserialize((int)serializedValue);
	}

	static StepSequencerValue deserialize(int serializedValue)
	{
		bool isSet = (serializedValue & (1 << 17)) > 0;
		int step = (serializedValue >> 8) & 0xFF;
		int value = serializedValue & 0xFF;

		StepSequencerValue returnValue = { step, value, isSet };
		return returnValue;
	}

	int serialize(void) 
	{
		int retVal = value;
		retVal |= (step << 8);
		if (isSet) {
			retVal |= (1 << 17);
		}

		return retVal;
	}

	int step;
	int value;
	bool isSet;
};

#endif //__STEPSEQUENCERVALUE_H__