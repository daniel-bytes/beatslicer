#ifndef __APPLICATIONCONTROLLER_H__
#define __APPLICATIONCONTROLLER_H__

#include "GlobalParameters.h"
#include "../JuceLibraryCode/JuceHeader.h"

class ApplicationModel;
class ApplicationView;
class Parameter;

class ApplicationController
{
public:
	ApplicationController();
	~ApplicationController();

	void setModel(ApplicationModel *model);
	void setView(ApplicationView *view);

public:
	void initializeUIParameters(const Array<Parameter*> &parameters);
	void updateParameterUI(GlobalParameter parameter, float value);
	void updateParameterModel(GlobalParameter parameter, float value);

private:
	ApplicationModel *model;
	ApplicationView *view;
};

#endif //__APPLICATIONCONTROLLER_H__