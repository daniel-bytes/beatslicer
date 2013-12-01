#include "GlobalParameters.h"

String ParameterName(GlobalParameter parameter)
{
	static HashMap<GlobalParameter, String, GlobalParameterHash, juce::CriticalSection> parameterNames;

	if (parameterNames.size() == 0) {
		ScopedLock lock(parameterNames.getLock());
		parameterNames[GlobalParameter::SampleChannel1_Speed] = "Speed";
		parameterNames[GlobalParameter::SampleChannel1_Gain] = "Gain";
		parameterNames[GlobalParameter::SampleChannel1_GrainSize] = "Grains";
		parameterNames[GlobalParameter::SampleChannel1_Direction] = "Direction";
	}

	return parameterNames[parameter];
}