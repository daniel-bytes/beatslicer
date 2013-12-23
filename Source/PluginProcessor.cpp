/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AudioEngine.h"
#include "ApplicationController.h"
#include "MonomeControl.h"

//==============================================================================
BeatSlicerAudioProcessor::BeatSlicerAudioProcessor()
{
	model = new AudioEngine();
	monomeView = new MonomeControl();

	controller = new ApplicationController(
							std::bind(&BeatSlicerAudioProcessor::getModel, this),
							std::bind(&BeatSlicerAudioProcessor::getViews, this));
}

BeatSlicerAudioProcessor::~BeatSlicerAudioProcessor()
{
	model = nullptr;
	controller = nullptr;
}

//==============================================================================
const String BeatSlicerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int BeatSlicerAudioProcessor::getNumParameters()
{
	return model->getNumPluginParameters();
}

float BeatSlicerAudioProcessor::getParameter (int index)
{
	return model->getPluginParameterValue(index);
}

void BeatSlicerAudioProcessor::setParameter (int index, float newValue)
{
	model->setPluginParameterValue(index, newValue);
}

const String BeatSlicerAudioProcessor::getParameterName (int index)
{
	return model->getPluginParameterName(index);
}

const String BeatSlicerAudioProcessor::getParameterText (int index)
{
	return model->getPluginParameterDisplay(index);
}

const String BeatSlicerAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String BeatSlicerAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool BeatSlicerAudioProcessor::isInputChannelStereoPair (int index) const
{
	(void)index;
    return true;
}

bool BeatSlicerAudioProcessor::isOutputChannelStereoPair (int index) const
{
	(void)index;
    return true;
}

bool BeatSlicerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BeatSlicerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BeatSlicerAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double BeatSlicerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BeatSlicerAudioProcessor::getNumPrograms()
{
    return 0;
}

int BeatSlicerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BeatSlicerAudioProcessor::setCurrentProgram (int index)
{
	(void)index;
}

const String BeatSlicerAudioProcessor::getProgramName (int index)
{
	(void)index;
    return String::empty;
}

void BeatSlicerAudioProcessor::changeProgramName (int index, const String& newName)
{
	(void)index;
	(void)newName;
}

//==============================================================================
void BeatSlicerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	(void)samplesPerBlock;
	// Initialize model and wire up to controller
	model->initialize(this->controller, sampleRate);
}

void BeatSlicerAudioProcessor::releaseResources()
{
	model->stop();
}

void BeatSlicerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	// Handle clock & midi
    AudioPlayHead::CurrentPositionInfo posInfo;
	auto playhead = this->getPlayHead();

	if (playhead != nullptr) {
		playhead->getCurrentPosition(posInfo);
		model->processClockMessage(posInfo);
	}

	model->processMidi(midiMessages);

	// Handle current audio frame
	model->processBlock(buffer, getNumInputChannels(), getNumOutputChannels());

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool BeatSlicerAudioProcessor::hasEditor() const
{
	return true;
}

AudioProcessorEditor* BeatSlicerAudioProcessor::createEditor()
{
    auto view = new BeatSlicerAudioProcessorEditor(this);
	view->initialize(this->controller);

	return view;
}

ApplicationModel* BeatSlicerAudioProcessor::getModel(void) 
{
	return this->model;
}

Array<ApplicationView*> BeatSlicerAudioProcessor::getViews(void)
{
	Array<ApplicationView*> views;

	views.add(monomeView);

	auto editor = this->getActiveEditor();

	if (editor != nullptr) {
		auto view = dynamic_cast<ApplicationView*>(editor);
		if (view != nullptr) {
			views.add(view);
		}
	}

	return views;
}

//==============================================================================
void BeatSlicerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Create an outer XML element..
    XmlElement xml("GrainrSettings");

	controller->serializeParameters(&xml);

    copyXmlToBinary(xml, destData);
}

void BeatSlicerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	ScopedPointer<XmlElement> xml(getXmlFromBinary (data, sizeInBytes));

    if (xml != nullptr)
	{
		controller->deserializeParameters(xml);
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BeatSlicerAudioProcessor();
}
