#include "AudioEngine.h"
#include "Parameter.h"

#include "ApplicationController.h"
#include "Sampler.h"
#include "Phasor.h"
#include "ClockPhasor.h"

#define TEST_FILEPATH "C:\\Users\\Daniel\\Documents\\Samples\\musicradar-drum-break-samples\\Clean Breaks\\Drum_Break01(94BPM).wav"

AudioEngine::AudioEngine()
	: controller(nullptr)
{
	formatManager = new AudioFormatManager();
	formatManager->registerBasicFormats();
	sampler = new Sampler();
	phasor = new Phasor();
	clockPhasor = new ClockPhasor();
	
	// run after all DSP processors are created
	configureParameters();
}

AudioEngine::~AudioEngine(void)
{
	formatManager->clearFormats();
	controller = nullptr;
	clockPhasor = nullptr;
	phasor = nullptr;
	sampler = nullptr;
	formatManager = nullptr;
}

void AudioEngine::initialize(ApplicationController *controller, double sampleRate)
{
	this->sampleRate = sampleRate;
	this->controller = controller;
	this->sampler->setSampleRate(sampleRate);
	this->phasor->setSampleRate(sampleRate);
}

void AudioEngine::stop()
{
	this->controller->endUITimer();
}

// Parameter configuration
void AudioEngine::configureParameters(void)
{
	configurePluginParameter(ParameterID::Sampler_Gain, "Gain", .9f, 0.f, 1.f);
	configurePluginParameter(ParameterID::Sampler_Speed, "Speed", 1.f, 0.f, 2.f);
	configurePluginParameter(ParameterID::Sampler_GrainSize, "Grain", .5f, 0.f, 1.f);
	configurePluginParameter(ParameterID::Sampler_Direction, "Direction", true, 0.f, 1.0f);
	configurePluginParameter(ParameterID::Sampler_Pitch, "Pitch", 0.f, -24.f, 24.f);
	configureStandardParameter(ParameterID::Sampler_FilePath, "File Path", TEST_FILEPATH);
	configureStandardParameter(ParameterID::Sampler_Phase, "Phase", 0.f);
	configureStandardParameter(ParameterID::Sampler_NumSlices, "Slices", 16);
	configureStandardParameter(ParameterID::Sampler_NumBars, "Bars", 4);

	// just to show test/debug values
	configureStandardParameter(ParameterID::TEST, "Test 1",  0.0f);
	configureStandardParameter(ParameterID::TEST1, "Test 2", 0.0f);
}

Parameter* AudioEngine::configureStandardParameter(ParameterID id, String name, var initialValue)
{
	Parameter *parameter = new Parameter(id, name, initialValue);
	allParameters.add(parameter);
	parameterMap.set(id, parameter);

	// Ensure that the processor handling this parameter gets the initial value
	setParameterValue(id, initialValue);

	return parameter;
}

Parameter* AudioEngine::configurePluginParameter(ParameterID id, String name, var initialValue, float minValue, float maxValue)
{
	PluginParameter *parameter = new PluginParameter(id, name, initialValue, minValue, maxValue);
	allParameters.add(parameter);
	parameterMap.set(id, parameter);

	pluginParameterLookups.add(id);
	pluginParameterIdMap.set(id, pluginParameterLookups.size() - 1);

	// Ensure that the processor handling this parameter gets the initial value
	setParameterValue(id, initialValue);

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
	ParameterID lookup = pluginParameterLookups[index];
	PluginParameter *parameter = static_cast<PluginParameter*>(parameterMap[lookup]);
	return parameter->getNormalizedValue();
}

void AudioEngine::setPluginParameterValue(int index, float value)
{
	ParameterID lookup = pluginParameterLookups[index];
	PluginParameter *parameter = static_cast<PluginParameter*>(parameterMap[lookup]);
	parameter->setNormalizedValue(value);
}

String AudioEngine::getPluginParameterName(int index) const
{
	ParameterID lookup = pluginParameterLookups[index];
	return parameterMap[lookup]->getName();
}

// Global parameter handling.
// This is where all applications parameter values are handled.
var AudioEngine::getParameterValue(ParameterID parameter) const
{
	return parameterMap[parameter]->getValue();
}

void AudioEngine::setParameterValue(ParameterID parameter, var value)
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
	case ParameterID::Sampler_Gain:
		this->sampler->setGain((float)value);
		break;
	case ParameterID::Sampler_Speed:
		this->phasor->setPlaybackRate(jlimit(.01f, 20.f, (float)value));
		break;
	case ParameterID::Sampler_GrainSize:
		this->sampler->setGrainSize((float)value);
		break;
	case ParameterID::Sampler_Direction:
		this->phasor->setDirection((bool)value);
		break;
	case ParameterID::Sampler_Pitch:
		this->sampler->setPitch((float)value);
		break;
	case ParameterID::Sampler_FilePath:
		this->sampler->loadFile(this->formatManager, (String)value);
		this->phasor->setBufferSize(sampler->getSamplerBufferSize(), sampler->getSamplerBufferSampleRate());
		break;
	case ParameterID::Sampler_Phase:
		phasor->setCurrentPhase((float)param->getValue() * sampler->getSamplerBufferSize());
		break;
	case ParameterID::Sampler_NumBars:
		clockPhasor->setNumBars((int)value);
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

	this->controller->setSequencerParameters(posInfo.isPlaying, posInfo.ppqPosition);

	if (posInfo.isPlaying) {
		this->clockPhasor->setPhase(posInfo.ppqPosition);

		if (this->clockPhasor->getCurrentPhase() == 0) {
			this->phasor->setCurrentPhase(0);
		}
	}
}

void AudioEngine::processMidi(MidiBuffer& midiMessages)
{
	(void)midiMessages;
}

void AudioEngine::processBlock(AudioSampleBuffer& buffer, int numInputChannels, int numOutputChannels)
{
	(void)numInputChannels;

	for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
		float phase = phasor->getCurrentPhase();

		for (int channel = 0; channel < numOutputChannels; channel++) {
			float* channelData = buffer.getSampleData(channel);
			float data = sampler->processSample(channel, phase);
			*(channelData + sample) = data;
		}

		phasor->calculateNextPhase();
	}
}
