#ifndef __STANDALONEAPPLICATIONWINDOW_H__
#define __STANDALONEAPPLICATIONWINDOW_H__

#ifdef STANDALONE_VERSION
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

class StandaloneApplicationWindow
	: public DocumentWindow,
	  public AudioIODeviceCallback
{
public:
	StandaloneApplicationWindow()
		: DocumentWindow("Grainr", Colours::white, DocumentWindow::TitleBarButtons::allButtons, true), editor(nullptr)
	{
		processor = new BeatSlicerAudioProcessor();

		//selector = new AudioDeviceSelectorComponent(deviceManager, 0, 2, 0, 2, false, false, true, false);
		//setContentNonOwned(selector, false);
		//selector->setVisible(true);
		
		auto result = deviceManager.initialise(0, 2, nullptr, true);

		if (result.length() == 0) {
			deviceManager.addAudioCallback(this);
		}
	}

	~StandaloneApplicationWindow()
	{
		if (editor != nullptr) {
			processor->editorBeingDeleted(editor);
			delete editor;
		}

		editor = nullptr;
		selector = nullptr;
		processor = nullptr;
		deviceManager.closeAudioDevice();
	}

	void closeButtonPressed()
	{
		deviceManager.closeAudioDevice();
		JUCEApplicationBase::quit();

	}

	void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples)
	{
		MidiBuffer midi;
		AudioSampleBuffer buffer(jmax(numInputChannels, numOutputChannels), numSamples);
		buffer.clear();

		for (int i = 0; i < numOutputChannels; i++) {
			if (i < numInputChannels) {
				buffer.copyFrom(i, 0, *(inputChannelData + i), numSamples, 1.f);
			}
		}

		processor->processBlock(buffer, midi);
		
		for (int i = 0; i < numOutputChannels; i++) {
			auto results = buffer.getSampleData(i, 0);
			auto outputChannel = *(outputChannelData + i);

			for (int j = 0; j < numSamples; j++) {
				*(outputChannel + j) = *(results + j);
			}
		}
	}

	void audioDeviceAboutToStart (AudioIODevice* device)
	{
		processor->setPlayConfigDetails(2, 2, device->getSampleRate(0), device->getDefaultBufferSize());
		processor->prepareToPlay(device->getSampleRate(0), device->getDefaultBufferSize());
		editor = (BeatSlicerAudioProcessorEditor*)processor->createEditorIfNeeded();

		setContentNonOwned(editor, false);
		editor->setVisible(true);
	}

	void audioDeviceStopped()
	{
		processor->suspendProcessing(true);
		processor->releaseResources();
	}
private:
	AudioDeviceManager deviceManager;
	ScopedPointer<BeatSlicerAudioProcessor> processor;
	BeatSlicerAudioProcessorEditor *editor;
	ScopedPointer<AudioDeviceSelectorComponent> selector;
};
#endif //STANDALONE_VERSION
#endif //__STANDALONEAPPLICATIONWINDOW_H__