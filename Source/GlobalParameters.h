#ifndef __GLOBALPARAMETERS_H__
#define __GLOBALPARAMETERS_H__

#include "../JuceLibraryCode/JuceHeader.h"

// All possible parameters are defined here.  
// These include plugin parameters AND any sort of internal automation parameter (eg: pitch controlled by a sequencer).
enum class GlobalParameter
{
	Unknown = 0,

	// Sample channel 1
	GrainSampler_Speed = 100,
	GrainSampler_Gain = 101,
	GrainSampler_GrainSize = 102,
	GrainSampler_Direction = 103,
	GrainSampler_Pitch = 104,
	GrainSampler_FilePath = 105
};

// Set parameter name mappings in GlobalParameters.cpp
String ParameterName(GlobalParameter parameter);

struct GlobalParameterHash
{
	 int generateHash (const GlobalParameter key, const int upperLimit) const noexcept
	 {
		 int iKey = (int)key;
		 return std::abs (iKey) % upperLimit; 
	 }
};


#endif //__GLOBALPARAMETERS_H__