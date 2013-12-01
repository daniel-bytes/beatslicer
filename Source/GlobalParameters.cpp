#include "GlobalParameters.h"

String ParameterName(GlobalParameter parameter)
{
	static HashMap<GlobalParameter, String, GlobalParameterHash, juce::CriticalSection> parameterNames;

	if (parameterNames.size() == 0) {
		ScopedLock lock(parameterNames.getLock());
		parameterNames[GlobalParameter::GrainSampler_Speed] = "Speed";
		parameterNames[GlobalParameter::GrainSampler_Gain] = "Gain";
		parameterNames[GlobalParameter::GrainSampler_GrainSize] = "Grains";
		parameterNames[GlobalParameter::GrainSampler_Direction] = "Direction";
		parameterNames[GlobalParameter::GrainSampler_Pitch] = "Pitch";
		parameterNames[GlobalParameter::GrainSampler_FilePath] = "Sample";
	}

	return parameterNames[parameter];
}