#include "ApplicationController.h"
#include "../JuceLibraryCode/JuceHeader.h"

#include "ApplicationModel.h"
#include "ApplicationView.h"

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
