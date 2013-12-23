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
	
	void setNumSlices(int numSlices);
	void setCurrentSlice(int slice);

	void initialize(ApplicationController *controller, ChangeListener *waveformChangeListener);

	void setSource(FileInputSource *fileSource);

public:
    void paint(Graphics &g);
	virtual void changeListenerCallback (ChangeBroadcaster* source);

private:
	ApplicationController *controller;
	ScopedPointer<AudioThumbnailCache> waveformCache;
	ScopedPointer<AudioThumbnail> waveform;
	int currentSlice;
	int numSlices;
};

#endif //__SAMPLERWAVEFORMCONTROL_H__