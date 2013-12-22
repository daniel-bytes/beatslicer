#ifndef __SAMPLERWAVEFORMCONTROL_H__
#define __SAMPLERWAVEFORMCONTROL_H__

#include "../JuceLibraryCode/JuceHeader.h"

class ApplicationController;

class SamplerWaveformControl
	: public Component,
	  public juce::ChangeListener
{
public:
	SamplerWaveformControl(int numSlices);
	~SamplerWaveformControl(void);

public:
	int getNumSlices(void) const { return numSlices; }
	float getWaveformPosition(void) const { return waveformPosition; }
	
	void setNumSlices(int numSlices);
	void setWaveformPosition(float position);

	void initialize(ApplicationController *controller);

	void setSource(FileInputSource *fileSource);

public:
    void paint(Graphics &g);
	void mouseDown(const MouseEvent &event);
	virtual void changeListenerCallback (ChangeBroadcaster* source);

private:
	ApplicationController *controller;
	ScopedPointer<AudioThumbnailCache> waveformCache;
	ScopedPointer<AudioThumbnail> waveform;
	float waveformPosition;
	int numSlices;
};

#endif //__SAMPLERWAVEFORMCONTROL_H__