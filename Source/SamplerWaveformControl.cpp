#include "SamplerWaveformControl.h"
#include "ApplicationController.h"

SamplerWaveformControl::SamplerWaveformControl(int numSlices)
	: Component(),
	  numSlices(numSlices)
{
}

SamplerWaveformControl::~SamplerWaveformControl(void)
{
	waveform = nullptr;
	controller = nullptr;
	waveformCache = nullptr;
}

void SamplerWaveformControl::setNumSlices(int numSlices)
{
	this->numSlices = numSlices;
	this->repaint();
}

void SamplerWaveformControl::setCurrentSlice(int slice)
{
	this->currentSlice = slice;
	this->repaint();
}

void SamplerWaveformControl::setSource(FileInputSource *fileSource)
{
	waveform->setSource(fileSource);
	this->repaint();
}

void SamplerWaveformControl::initialize(ApplicationController *controller)
{
	this->controller = controller;

	if (controller == nullptr) {
		return;
	}

	auto formatManager = controller->getAudioFormatManager();

	if (formatManager != nullptr) {
		waveformCache = new AudioThumbnailCache(1);
		waveform = new AudioThumbnail(1, *formatManager, *waveformCache);

		auto waveformListener = controller->getWaveformChangeListener();

		if (waveformListener != nullptr) {
			waveform->addChangeListener(waveformListener);
		}
	}
}

void SamplerWaveformControl::changeListenerCallback (ChangeBroadcaster* source)
{
	if (source == waveform) {
		this->repaint();
	}
}

void SamplerWaveformControl::paint(Graphics &g)
{
	Rectangle<int> waveformBounds(this->getWidth(), this->getHeight());

    g.fillAll(Colours::white);
	g.drawRect(waveformBounds, 2);
		
	g.setColour(Colours::red);
	float stepMult = (float)this->getWidth() / (float)numSlices;
	float y = 2;
	float y2 = (float)getHeight() - 2;
	g.fillRect(currentSlice * stepMult, 0.f, stepMult, (float)waveformBounds.getHeight());
	
	g.setColour(Colours::black);
	waveform->drawChannel(g, waveformBounds, 0, waveform->getTotalLength(), 0, 1.0f);
	
	g.setColour(Colours::grey);
	for (int i = 0; i < numSlices; i++) {
		float x = (float)i * stepMult;
		g.drawLine(x, y, x, y2); 
	}
}
