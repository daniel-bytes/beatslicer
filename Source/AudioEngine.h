#ifndef __AUDIOENGINE_H__
#define __AUDIOENGINE_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ApplicationModel.h"
#include "ParameterID.h"
#include "StepSequencer.h"

class Parameter;
class ApplicationController;
class Sampler;
class Phasor;

class AudioEngine
	: public ApplicationModel,
	  protected StepSequencer::Listener
{
public:
	AudioEngine(void);
	~AudioEngine(void);

public:
	void initialize(ApplicationController *controller, double sampleRate);
	void stop();

public:
	// Plugin parameter handling
	virtual int getNumPluginParameters(void) const;
	virtual float getPluginParameterValue(int index) const;
	virtual void setPluginParameterValue(int index, float value);
	virtual String getPluginParameterName(int index) const;
	virtual String getPluginParameterDisplay(int index) const;

public:
	virtual var getParameterValue(ParameterID parameter) const;
	virtual void setParameterValue(ParameterID parameter, var value);
	virtual const Array<Parameter*> getAllParameters(void) const;

	virtual AudioFormatManager* getAudioFormatManager(void) { return formatManager; }
	virtual const StepSequencerData* getSequencerData(void) const;
	virtual void resetSequencerSlices(void);

public:
	void processClockMessage(AudioPlayHead::CurrentPositionInfo &posInfo);
	void processMidi(MidiBuffer& midiMessages);
	void processBlock(AudioSampleBuffer& buffer, int numInputChannels, int numOutputChannels);

protected:
	virtual void onStepTriggered(const StepSequencer &source, int step, StepSequencerValue row);

private:
	// setup method for all application parameters
	void configureParameters(void);

	// configures a new parameter, which is not exposed to the host engine as a plugin parameter
	Parameter* configureStandardParameter(ParameterID id, String name, var initialValue);

	// configures a new parameter, which is exposed to the host engine
	Parameter* configurePluginParameter(ParameterID id, String name, var initialValue, float minValue, float maxValue);

    Array<var> getInitialStepValues();

	void updateBpmOffset(void);

private:
	ScopedPointer<AudioFormatManager> formatManager;

	// Parameter collections
	OwnedArray<Parameter> allParameters;
	HashMap<ParameterID, Parameter*, ParameterIDHash> parameterMap;
	
	Array<ParameterID> pluginParameterLookups;
	HashMap<ParameterID, int, ParameterIDHash> pluginParameterIdMap;

	// Application controller
	ApplicationController *controller;

	// Dsp elements
	double sampleRate;
	float masterGain;
	bool mute;
	ScopedPointer<Sampler> sampler;
	ScopedPointer<Phasor> phasor;
	ScopedPointer<StepSequencer> sequencer;
};

#endif //__AUDIOENGINE_H__