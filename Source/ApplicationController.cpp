#include "ApplicationController.h"

#include "ApplicationModel.h"
#include "ApplicationView.h"
#include "Parameter.h"

#define XMLTYPE_STRING "String"
#define XMLTYPE_FLOAT "Float"
#define XMLTYPE_INT "Integer"
#define XMLTAG_PARAMETER "Parameter"
#define XMLATTRIBUTE_ID "id"
#define XMLATTRIBUTE_NAME "name"
#define XMLATTRIBUTE_TYPE "type"
#define XMLATTRIBUTE_VALUE "value"

ApplicationController::ApplicationController()
	: model(nullptr), view(nullptr)
{
}

ApplicationController::~ApplicationController()
{
}

void ApplicationController::setModel(ApplicationModel *model)
{
	this->model = model;
}

void ApplicationController::setView(ApplicationView *view)
{
	this->view = view;
}


void ApplicationController::serializeParameters(XmlElement *xml)
{
	auto parameters = model->getAllParameters();

	for (auto parameter : parameters) {
		auto child = xml->createNewChildElement(XMLTAG_PARAMETER);
		child->setAttribute(XMLATTRIBUTE_NAME, parameter->getName());
		child->setAttribute(XMLATTRIBUTE_ID, (int)parameter->getGlobalID());
		
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
	auto parameters = model->getAllParameters();

	for (auto parameter : parameters) {
		String id((int)parameter->getGlobalID());
		auto element = xml->getChildByAttribute(XMLATTRIBUTE_ID, StringRef(id));

		if (element != nullptr) {
			auto type = element->getStringAttribute(StringRef(XMLATTRIBUTE_TYPE), "");

			if (type == XMLTYPE_STRING) {
				auto value = element->getStringAttribute(StringRef(XMLATTRIBUTE_VALUE), "");
				this->updateParameterModelAndUI(parameter->getGlobalID(), value);
			}
			else if (type == XMLTYPE_FLOAT) {
				auto value = (float)element->getDoubleAttribute(StringRef(XMLATTRIBUTE_VALUE));
				this->updateParameterModelAndUI(parameter->getGlobalID(), value);
			}
			else if (type == XMLTYPE_INT) {
				auto value = element->getIntAttribute(StringRef(XMLATTRIBUTE_VALUE));
				this->updateParameterModelAndUI(parameter->getGlobalID(), value);
			}
		}
	}
}

void ApplicationController::initializeUIParameters(const Array<Parameter*> &parameters)
{
	if (this->view != nullptr) {
		for (auto parameter : parameters) {
			jassert(parameter != nullptr);
			updateParameterUI(parameter->getGlobalID(), parameter->getValue());
		}
	}
}

void ApplicationController::updateParameterUI(GlobalParameter parameter, var value)
{
	if (this->view != nullptr) {
		this->view->setGlobalParameterValue(parameter, value);
	}
}

void ApplicationController::updateParameterModel(GlobalParameter parameter, var value)
{
	if (this->model != nullptr) {
		this->model->setGlobalParameterValue(parameter, value);
	}
}

void ApplicationController::updateParameterModelAndUI(GlobalParameter parameter, var value)
{
	updateParameterModel(parameter, value);
	updateParameterUI(parameter, value);
}