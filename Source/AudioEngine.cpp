#include "AudioEngine.h"
#include "Parameter.h"

#include "ApplicationController.h"
#include "GrainSampler.h"

AudioEngine::AudioEngine(void)
	: controller(nullptr)
{
	grainSampler1 = new GrainSampler();

	// run after all DSP processors are created
	configureParameters();
}

AudioEngine::~AudioEngine(void)
{
	controller = nullptr;
	grainSampler1 = nullptr;
}

void AudioEngine::initialize(ApplicationController *controller, double sampleRate)
{
	this->controller = controller;
	this->grainSampler1->initialize(sampleRate);
}

void AudioEngine::stop()
{
}

// Parameter configuration
void AudioEngine::configureParameters(void)
{
	configureParameter(GlobalParameter::Master_Gain, (int)GlobalParameter::Master_Gain, "Master Volume", "Master Volume", .5f, true);
	configureParameter(GlobalParameter::SampleChannel1_Gain, (int)GrainSamplerParameter::Gain, "Sampler 1 Gain", "Gain 1", .9f, true);
	configureParameter(GlobalParameter::SampleChannel1_Speed, (int)GrainSamplerParameter::Speed, "Sampler 1 Speed", "Speed 1", .5f, true);
	configureParameter(GlobalParameter::SampleChannel1_GrainSize, (int)GrainSamplerParameter::GrainSize, "Sampler 1 Grains", "Grain Size 1", .5f, true);
}

Parameter* AudioEngine::configureParameter(GlobalParameter globalID, int localID, String name, String displayName, float initialValue, bool isPluginParameter)
{
	Parameter *parameter = new Parameter((int)globalID, localID, name, displayName, initialValue);
	allParameters.add(parameter);
	parameterMap.set(globalID, parameter);

	if (isPluginParameter) {
		pluginParameterLookups.add(globalID);
	}

	// Ensure that the processor handling this parameter gets the initial value
	setGlobalParameterValue(globalID, initialValue);

	return parameter;
}

// Plugin parameter handling.  
// These use the pluginParameterLookups collection to map a plugin parameter ID to a global ID.
int AudioEngine::getNumPluginParameters(void) const
{
	return pluginParameterLookups.size();
}

float AudioEngine::getPluginParameterValue(int index) const
{
	auto lookup = pluginParameterLookups[index];
	return getGlobalParameterValue(lookup);
}

void AudioEngine::setPluginParameterValue(int index, float value)
{
	auto lookup = pluginParameterLookups[index];
	setGlobalParameterValue(lookup, value);
}

String AudioEngine::getPluginParameterName(int index) const
{
	auto lookup = pluginParameterLookups[index];
	return parameterMap[lookup]->getName();
}

String AudioEngine::getPluginParameterText(int index) const
{
	auto lookup = pluginParameterLookups[index];
	return parameterMap[lookup]->getDisplayName();
}

// Global parameter handling.
// This is where all applications parameter values are handled.
float AudioEngine::getGlobalParameterValue(GlobalParameter parameter) const
{
	return parameterMap[parameter]->getValue();
}

void AudioEngine::setGlobalParameterValue(GlobalParameter parameter, float value)
{
	auto param = parameterMap[parameter];
	param->setValue(value);
	
	switch(parameter)
	{
	case GlobalParameter::Master_Gain:
		this->masterGain = param->getValue();
		break;
	case GlobalParameter::SampleChannel1_Gain:
	case GlobalParameter::SampleChannel1_Speed:
	case GlobalParameter::SampleChannel1_GrainSize:
		this->grainSampler1->setParameterValue((GrainSamplerParameter)param->getLocalID(), param->getValue());
		break;
	}
}

void AudioEngine::processClockMessage(AudioPlayHead::CurrentPositionInfo &posInfo)
{
}

void AudioEngine::processMidi(MidiBuffer& midiMessages)
{
}

float AudioEngine::processSample(int channel, float value)
{
	float sampledata = grainSampler1->processSample(channel);

	return sampledata * masterGain;
}
