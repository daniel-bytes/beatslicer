#include "ApplicationController.h"
#include "../JuceLibraryCode/JuceHeader.h"

#include "ApplicationModel.h"
#include "ApplicationView.h"
#include "Parameter.h"

// Serialization constants.  
// Eventually this should be part of a separate serializer class.
#define XMLTYPE_STRING "String"
#define XMLTYPE_FLOAT "Float"
#define XMLTYPE_INT "Integer"
#define XMLTAG_PARAMETER "Parameter"
#define XMLATTRIBUTE_ID "id"
#define XMLATTRIBUTE_NAME "name"
#define XMLATTRIBUTE_TYPE "type"
#define XMLATTRIBUTE_VALUE "value"

ApplicationController::ApplicationController(std::function<ApplicationModel*()> modelFetch, std::function<Array<ApplicationView*>()> viewFetch)
	: getModel(modelFetch), 
	  getViews(viewFetch), 
	  isPlaying(false), 
	  bpm(120.0),
	  playbackPosition(0), 
	  sequencerPosition(0), 
	  sequencerPositionChanged(false)
{
}

ApplicationController::~ApplicationController()
{
}

const Array<Parameter*> ApplicationController::getAllParameters(void) const
{
	auto model = getModel();

	if (model == nullptr) {
		return Array<Parameter*>();
	}

	return model->getAllParameters();
}

var ApplicationController::getParameterValue(ParameterID parameter) const
{
	auto model = getModel();

	jassert(model != nullptr);
	return model->getParameterValue(parameter);
}

AudioFormatManager* ApplicationController::getAudioFormatManager(void)
{
	auto model = getModel();

	if (model == nullptr) {
		return nullptr;
	}

	return model->getAudioFormatManager();
}

const StepSequencerData* ApplicationController::getSequencerData(void) const
{
	auto model = getModel();

	if (model == nullptr) {
		return nullptr;
	}

	return model->getSequencerData();
}

void ApplicationController::resetSequencerSlices(void)
{
	auto model = getModel();

	if (model == nullptr) {
		return;
	}

	model->resetSequencerSlices();
}

void ApplicationController::serializeParameters(XmlElement *xml)
{
	auto model = getModel();

	if (model == nullptr) {
		return;
	}

	auto parameters = model->getAllParameters();

	for (auto parameter : parameters) {
		auto child = xml->createNewChildElement(XMLTAG_PARAMETER);
		child->setAttribute(XMLATTRIBUTE_NAME, parameter->getName());
		child->setAttribute(XMLATTRIBUTE_ID, (int)parameter->getParameterID());
		
		var value = parameter->getValue();

		if (value.isString()) {
			child->setAttribute(XMLATTRIBUTE_TYPE, XMLTYPE_STRING);
			child->setAttribute(XMLATTRIBUTE_VALUE, (String)value);
		}
		else if (value.isDouble()) {
			child->setAttribute(XMLATTRIBUTE_TYPE, XMLTYPE_FLOAT);
			child->setAttribute(XMLATTRIBUTE_VALUE, (float)value);
		}
		else if (value.isInt()) {
			child->setAttribute(XMLATTRIBUTE_TYPE, XMLTYPE_INT);
			child->setAttribute(XMLATTRIBUTE_VALUE, (int)value);
		}
		else if (value.isArray()) {
			// TODO
		}
	}
}

void ApplicationController::deserializeParameters(XmlElement *xml)
{
	auto model = getModel();

	if (model == nullptr) {
		return;
	}

	auto parameters = model->getAllParameters();

	for (auto parameter : parameters) {
		String id((int)parameter->getParameterID());
		auto element = xml->getChildByAttribute(XMLATTRIBUTE_ID, StringRef(id));

		if (element != nullptr) {
			auto type = element->getStringAttribute(StringRef(XMLATTRIBUTE_TYPE), "");

			if (type == XMLTYPE_STRING) {
				auto value = element->getStringAttribute(StringRef(XMLATTRIBUTE_VALUE), "");
				this->updateParameterModelAndUI(parameter->getParameterID(), value);
			}
			else if (type == XMLTYPE_FLOAT) {
				auto value = (float)element->getDoubleAttribute(StringRef(XMLATTRIBUTE_VALUE));
				this->updateParameterModelAndUI(parameter->getParameterID(), value);
			}
			else if (type == XMLTYPE_INT) {
				auto value = element->getIntAttribute(StringRef(XMLATTRIBUTE_VALUE));
				this->updateParameterModelAndUI(parameter->getParameterID(), value);
			}
		}
	}
}

void ApplicationController::updateParameterUI(ParameterID parameter, var value)
{
	auto views = getViews();

	for (auto view : views) {
		view->setParameterValue(parameter, value);
	}
}

void ApplicationController::updateParameterModel(ParameterID parameter, var value)
{
	auto model = getModel();

	if (model != nullptr) {
		model->setParameterValue(parameter, value);
	}
}

void ApplicationController::updateParameterModelAndUI(ParameterID parameter, var value)
{
	updateParameterModel(parameter, value);
	updateParameterUI(parameter, value);
}

void ApplicationController::timerCallback()
{
	auto model = getModel();

	if (model == nullptr) {
		return;
	}

	//float position = model->getParameterValue(ParameterID::Sampler_Phase);
	//updateParameterUI(ParameterID::Sampler_Phase, position);

	updateParameterUI(ParameterID::Sequencer_CurrentValue, playbackValue);
	
	if (sequencerPositionChanged) {
		updateParameterUI(ParameterID::Sequencer_CurrentStep, this->sequencerPosition);
		sequencerPositionChanged = false;
	}
}