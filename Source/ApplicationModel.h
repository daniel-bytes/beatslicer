#ifndef __APPLICATIONMODEL_H__
#define __APPLICATIONMODEL_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalParameters.h"

class Parameter;

class ApplicationModel
{
public:
	// Plugin parameter handlers
	virtual int getNumPluginParameters(void) const = 0;
	virtual float getPluginParameterValue(int index) const = 0;
	virtual void setPluginParameterValue(int index, float value) = 0;
	virtual String getPluginParameterName(int index) const = 0;
	virtual String getPluginParameterText(int index) const = 0;

	
	virtual float getGlobalParameterValue(GlobalParameter parameter) const = 0;
	virtual void setGlobalParameterValue(GlobalParameter parameter, float value) = 0;
	virtual const Array<Parameter*> getAllParameters(void) const = 0;
};

#endif //__APPLICATIONMODEL_H__