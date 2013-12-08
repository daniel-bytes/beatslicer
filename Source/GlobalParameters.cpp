#include "GlobalParameters.h"

String ParameterName(GlobalParameter parameter)
{
	static HashMap<GlobalParameter, String, GlobalParameterHash, juce::CriticalSection> parameterNames;

	if (parameterNames.size() == 0) {
		ScopedLock lock(parameterNames.getLock());
		parameterNames[GlobalParameter::Sampler_Speed] = "Speed";
		parameterNames[GlobalParameter::Sampler_Gain] = "Gain";
		parameterNames[GlobalParameter::Sampler_GrainSize] = "Grains";
		parameterNames[GlobalParameter::Sampler_Direction] = "Direction";
		parameterNames[GlobalParameter::Sampler_Pitch] = "Pitch";
		parameterNames[GlobalParameter::Sampler_FilePath] = "Sample";
		parameterNames[GlobalParameter::Sampler_NumSlices] = "Slices";
	}

	return parameterNames[parameter];
}