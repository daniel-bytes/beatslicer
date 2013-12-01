#ifndef __GLOBALPARAMETERS_H__
#define __GLOBALPARAMETERS_H__

#include "../JuceLibraryCode/JuceHeader.h"

// All possible parameters are defined here.  
// These include plugin parameters AND any sort of internal automation parameter (eg: pitch controlled by a sequencer).
enum class GlobalParameter
{
	Unknown = 0,

	// Master
	//Master_Gain = 1,

	// Sample channel 1
	SampleChannel1_Speed = 100,
	SampleChannel1_Gain = 101,
	SampleChannel1_GrainSize = 102,
	SampleChannel1_Direction = 103

	// 
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