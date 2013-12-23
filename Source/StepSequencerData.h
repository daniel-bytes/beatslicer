#ifndef __STEPSEQUENCERDATA_H__
#define __STEPSEQUENCERDATA_H__

#include "StepSequencerValue.h"

struct StepSequencerData
{
	int numSteps;
	int numRows;
	int numBars;
	int currentStep;
	Array<StepSequencerValue> values;
};

#endif //__STEPSEQUENCERDATA_H__