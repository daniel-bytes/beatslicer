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

void SamplerWaveformControl::setWaveformPosition(float position)
{
	this->waveformPosition = position;
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

	waveform->drawChannel(g, waveformBounds, 0, waveform->getTotalLength(), 0, 1.0f);
		
	g.setColour(Colours::red);
	auto lineOffset = waveformPosition * (float)waveformBounds.getWidth();
	g.drawLine((float)lineOffset, (float)waveformBounds.getY(), (float)lineOffset, (float)(waveformBounds.getY() + waveformBounds.getHeight()));
		
	//int sliceNumber = (int)(waveformPosition * (float)numSlices);
	for (int i = 1; i < numSlices; i++) {
		g.setColour(Colours::grey);
		int offset = (waveformBounds.getWidth() / numSlices) * i;
		g.drawLine((float)offset, (float)waveformBounds.getY(), (float)offset, (float)(waveformBounds.getY() + waveformBounds.getHeight()));

		//if (
	}

}

void SamplerWaveformControl::mouseDown(const MouseEvent &event)
{
	auto position = event.getPosition();

	float distance = jlimit(0.f, .9999f, (float)position.getX() / (float)this->getWidth());
	float sliceNumber = distance * (float)numSlices;
	distance = floor(sliceNumber) / (float)numSlices;
	controller->updateParameterModel(ParameterID::Sampler_Phase, distance);

	this->repaint();
}
