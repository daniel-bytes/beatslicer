#ifndef __GLOBALPARAMETERS_H__
#define __GLOBALPARAMETERS_H__

#include "../JuceLibraryCode/JuceHeader.h"

// All possible parameters are defined here.  
// These include plugin parameters AND any sort of internal automation parameter (eg: pitch controlled by a sequencer).
enum class GlobalParameter
{
	Unknown = 0,

	// Sample channel 1
	Sampler_Speed = 100,
	Sampler_Gain = 101,
	Sampler_GrainSize = 102,
	Sampler_Direction = 103,
	Sampler_Pitch = 104,
	Sampler_FilePath = 105,
	Sampler_Phase = 106,
	Sampler_NumSlices = 107
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