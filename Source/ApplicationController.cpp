#include "ApplicationController.h"

#include "ApplicationModel.h"
#include "ApplicationView.h"
#include "Parameter.h"

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
