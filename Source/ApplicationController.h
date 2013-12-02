#ifndef __APPLICATIONCONTROLLER_H__
#define __APPLICATIONCONTROLLER_H__

#include "GlobalParameters.h"
#include "../JuceLibraryCode/JuceHeader.h"

class ApplicationModel;
class ApplicationView;
class Parameter;

/*
 * This is a traditional MVC controller class.
 * It's purpose is to facilitate data passing between the application (model) and
 * interface (view) layers.
 * This data is primarily Parameters, which represent both normal host-automated VST
 * parameters as well as any internal parameters (such as note sequencer values passed
 * from a sequencer to a synth).
 *
 * The core application logic will usually live in the class implementing ApplicationModel.
 *
 * In terms of the JUCE plugin wrapper system, the PluginProcessor class will hold an instance
 * of the ApplicationController and a class implementing ApplicationModel, and forward most calls
 * directly to the model class.  
 * The PluginEditor will usually implement ApplicationView.
 */
class ApplicationController
	: private Timer
{
public:
	ApplicationController();
	~ApplicationController();

	void setModel(ApplicationModel *model);
	void setView(ApplicationView *view);

	void beginUITimer(void) { this->startTimer(20); }
	void endUITimer(void) { this->stopTimer(); }

public:
	void serializeParameters(XmlElement *xml);
	void deserializeParameters(XmlElement *xml);
	void initializeUIParameters(const Array<Parameter*> &parameters);
	void updateParameterUI(GlobalParameter parameter, var value);
	void updateParameterModel(GlobalParameter parameter, var value);
	void updateParameterModelAndUI(GlobalParameter parameter, var value);

	AudioFormatManager* getAudioFormatManager(void);

private:
	virtual void timerCallback();

private:
	ApplicationModel *model;
	ApplicationView *view;
};

#endif //__APPLICATIONCONTROLLER_H__