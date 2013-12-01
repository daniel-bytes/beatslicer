#include "AudioEngine.h"
#include "Parameter.h"

#include "ApplicationController.h"
#include "GrainSampler.h"

#define TEST_FILEPATH "C:\\Users\\Daniel\\Documents\\Samples\\musicradar-drum-break-samples\\Clean Breaks\\Drum_Break01(94BPM).wav"

AudioEngine::AudioEngine(AudioProcessor *parent)
	: controller(nullptr), parent(parent)
{
	grainSampler = new GrainSampler();
	
	// run after all DSP processors are created
	configureParameters();
}

AudioEngine::~AudioEngine(void)
{
	controller = nullptr;
	grainSampler = nullptr;
}

void AudioEngine::initialize(ApplicationController *controller, double sampleRate)
{
	this->controller = controller;
	this->grainSampler->initialize(sampleRate);
}

void AudioEngine::stop()
{
}

// Parameter configuration
void AudioEngine::configureParameters(void)
{
	configureParameter(GlobalParameter::GrainSampler_Gain, (int)GrainSamplerParameter::Gain, .9f, true);
	configureParameter(GlobalParameter::GrainSampler_Speed, (int)GrainSamplerParameter::Speed, .5f, true);
	configureParameter(GlobalParameter::GrainSampler_GrainSize, (int)GrainSamplerParameter::GrainSize, .5f, true);
	configureParameter(GlobalParameter::GrainSampler_Direction, (int)GrainSamplerParameter::Direction, .5f, true);
	configureParameter(GlobalParameter::GrainSampler_Pitch, (int)GrainSamplerParameter::Pitch, .5f, true);
	configureParameter(GlobalParameter::GrainSampler_FilePath, (int)GrainSamplerParameter::FilePath, TEST_FILEPATH, false);
}

Parameter* AudioEngine::configureParameter(GlobalParameter globalID, int localID, var initialValue, bool isPluginParameter)
{
	String name = ParameterName(globalID);
	Parameter *parameter = new Parameter(globalID, localID, name, initialValue, isPluginParameter);
	allParameters.add(parameter);
	parameterMap.set(globalID, parameter);

	if (isPluginParameter) {
		pluginParameterLookups.add(globalID);
		pluginParameterIdMap.set(globalID, pluginParameterLookups.size() - 1);
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

var AudioEngine::getPluginParameterValue(int index) const
{
	auto lookup = pluginParameterLookups[index];
	return getGlobalParameterValue(lookup);
}

void AudioEngine::setPluginParameterValue(int index, var value)
{
	auto lookup = pluginParameterLookups[index];
	setGlobalParameterValue(lookup, value);
}

String AudioEngine::getPluginParameterName(int index) const
{
	auto lookup = pluginParameterLookups[index];
	return parameterMap[lookup]->getName();
}

// Global parameter handling.
// This is where all applications parameter values are handled.
var AudioEngine::getGlobalParameterValue(GlobalParameter parameter) const
{
	return parameterMap[parameter]->getValue();
}

void AudioEngine::setGlobalParameterValue(GlobalParameter parameter, var value)
{
	auto param = parameterMap[parameter];
	/*
	if (param->isPluginParameter()) {
		// If the host needs to handle this, we do that now.  It will eventually
		int id = pluginParameterIdMap[parameter];

		parent->setParameterNotifyingHost(id, (float)value);
		return;
	}
	*/
	param->setValue(value);
	
	switch(parameter)
	{
	case GlobalParameter::GrainSampler_Gain:
	case GlobalParameter::GrainSampler_Speed:
	case GlobalParameter::GrainSampler_GrainSize:
	case GlobalParameter::GrainSampler_Direction:
	case GlobalParameter::GrainSampler_Pitch:
	case GlobalParameter::GrainSampler_FilePath:
		this->grainSampler->setParameterValue((GrainSamplerParameter)param->getLocalID(), param->getValue());
		break;
	}
}

const Array<Parameter*> AudioEngine::getAllParameters(void) const
{
	Array<Parameter*> parameters;

	for (auto parameter : allParameters) {
		parameters.add(parameter);
	}

	return parameters;
}

void AudioEngine::processClockMessage(AudioPlayHead::CurrentPositionInfo &posInfo)
{
	(void)posInfo;
}

void AudioEngine::processMidi(MidiBuffer& midiMessages)
{
	(void)midiMessages;
}

void AudioEngine::processBlock(AudioSampleBuffer& buffer, int numInputChannels, int numOutputChannels)
{
	(void)numInputChannels;

	for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
		for (int channel = 0; channel < numOutputChannels; channel++) {
			float* channelData = buffer.getSampleData(channel);
			*(channelData + sample) = grainSampler->processSample(channel);
		}
	}
}
