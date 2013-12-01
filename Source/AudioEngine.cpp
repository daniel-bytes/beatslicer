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

	File file("C:\\Users\\Daniel\\Documents\\Samples\\musicradar-drum-break-samples\\Clean Breaks\\Drum_Break01(94BPM).wav");
	
	if (file.exists()) {
		this->grainSampler1->loadFromFile(file);
	}
}

void AudioEngine::stop()
{
	File f;
	this->grainSampler1->loadFromFile(f);
}

// Parameter configuration
void AudioEngine::configureParameters(void)
{
	configureParameter(GlobalParameter::SampleChannel1_Gain, (int)GrainSamplerParameter::Gain, .9f, true);
	configureParameter(GlobalParameter::SampleChannel1_Speed, (int)GrainSamplerParameter::Speed, .5f, true);
	configureParameter(GlobalParameter::SampleChannel1_GrainSize, (int)GrainSamplerParameter::GrainSize, .5f, true);
	configureParameter(GlobalParameter::SampleChannel1_Direction, (int)GrainSamplerParameter::Direction, .5f, true);
}

Parameter* AudioEngine::configureParameter(GlobalParameter globalID, int localID, float initialValue, bool isPluginParameter)
{
	String name = ParameterName(globalID);
	Parameter *parameter = new Parameter(globalID, localID, name, name, initialValue);
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
	case GlobalParameter::SampleChannel1_Gain:
	case GlobalParameter::SampleChannel1_Speed:
	case GlobalParameter::SampleChannel1_GrainSize:
	case GlobalParameter::SampleChannel1_Direction:
		this->grainSampler1->setParameterValue((GrainSamplerParameter)param->getLocalID(), param->getValue());
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
			*(channelData + sample) = grainSampler1->processSample(channel);
		}
	}
}
