#ifndef __AUDIOENGINE_H__
#define __AUDIOENGINE_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ApplicationModel.h"
#include "GlobalParameters.h"

class Parameter;
class ApplicationController;
class GrainSampler;

class AudioEngine
	: public ApplicationModel
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
	virtual var getPluginParameterValue(int index) const;
	virtual void setPluginParameterValue(int index, var value);
	virtual String getPluginParameterName(int index) const;

public:
	virtual var getGlobalParameterValue(GlobalParameter parameter) const;
	virtual void setGlobalParameterValue(GlobalParameter parameter, var value);
	virtual const Array<Parameter*> getAllParameters(void) const;

public:
	void processClockMessage(AudioPlayHead::CurrentPositionInfo &posInfo);
	void processMidi(MidiBuffer& midiMessages);
	void processBlock(AudioSampleBuffer& buffer, int numInputChannels, int numOutputChannels);

private:
	void configureParameters();
	Parameter* configureParameter(GlobalParameter globalID, int localID, var initialValue, bool isPluginParameter);

private:
	// Parameter collections
	OwnedArray<Parameter> allParameters;
	HashMap<GlobalParameter, Parameter*, GlobalParameterHash> parameterMap;
	Array<GlobalParameter> pluginParameterLookups;

	// Application controller
	ApplicationController *controller;

	// Dsp elements
	float masterGain;
	ScopedPointer<GrainSampler> grainSampler;
};

#endif //__AUDIOENGINE_H__