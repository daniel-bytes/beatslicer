#include "AudioEngine.h"
#include "Parameter.h"

#include "ApplicationController.h"
#include "Sampler.h"

#define TEST_FILEPATH "C:\\Users\\Daniel\\Documents\\Samples\\musicradar-drum-break-samples\\Clean Breaks\\Drum_Break01(94BPM).wav"

AudioEngine::AudioEngine(AudioProcessor *parent)
	: controller(nullptr), parent(parent)
{
	formatManager = new AudioFormatManager();
	formatManager->registerBasicFormats();
	sampler = new Sampler();
	
	// run after all DSP processors are created
	configureParameters();
}

AudioEngine::~AudioEngine(void)
{
	formatManager->clearFormats();
	controller->setModel(nullptr);
	controller = nullptr;
	sampler = nullptr;
	formatManager = nullptr;
}

void AudioEngine::initialize(ApplicationController *controller, double sampleRate)
{
	this->controller = controller;
	this->sampler->initialize(sampleRate);
	this->controller->beginUITimer();
}

void AudioEngine::stop()
{
	this->controller->endUITimer();
}

// Parameter configuration
void AudioEngine::configureParameters(void)
{
	configureParameter(GlobalParameter::Sampler_Gain, (int)SamplerParameter::Gain, .9f, true);
	configureParameter(GlobalParameter::Sampler_Speed, (int)SamplerParameter::Speed, .5f, true);
	configureParameter(GlobalParameter::Sampler_GrainSize, (int)SamplerParameter::GrainSize, .5f, true);
	configureParameter(GlobalParameter::Sampler_Direction, (int)SamplerParameter::Direction, .5f, true);
	configureParameter(GlobalParameter::Sampler_Pitch, (int)SamplerParameter::Pitch, .5f, true);
	configureParameter(GlobalParameter::Sampler_FilePath, (int)SamplerParameter::FilePath, TEST_FILEPATH, false);
	configureParameter(GlobalParameter::Sampler_Phase, (int)SamplerParameter::Phase, 0.f, false);
	configureParameter(GlobalParameter::Sampler_NumSlices, (int)GlobalParameter::Sampler_NumSlices, 16, false);
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
	case GlobalParameter::Sampler_Gain:
	case GlobalParameter::Sampler_Speed:
	case GlobalParameter::Sampler_GrainSize:
	case GlobalParameter::Sampler_Direction:
	case GlobalParameter::Sampler_Pitch:
	case GlobalParameter::Sampler_Phase:
		this->sampler->setParameterValue((SamplerParameter)param->getLocalID(), param->getValue());
		break;
	case GlobalParameter::Sampler_FilePath:
		this->sampler->loadFile(this->formatManager, (String)value);
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
			*(channelData + sample) = sampler->processSample(channel);
		}
	}
}

float AudioEngine::getFractionalSamplerPhase(void) const
{
	if (sampler->getSamplerBufferSize() == 0) {
		return 0;
	}

	return sampler->getSamplerPhase() / (float)sampler->getSamplerBufferSize();
}