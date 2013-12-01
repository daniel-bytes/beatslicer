#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalParameters.h"

class Parameter
{
public:
	Parameter(GlobalParameter globalID, int localID, const String &name, var initialValue, bool pluginParameter)
	{
		this->globalID = globalID;
		this->localID = localID;
		this->name = name;
		this->value = initialValue;
		this->pluginParameter = pluginParameter;
	}

	GlobalParameter getGlobalID(void) const {
		return this->globalID;
	}

	int getLocalID(void) const {
		return this->localID;
	}

	String getName(void) const {
		return this->name;
	}

	var getValue(void) const {
		return this->value;
	}

	void setValue(var value) {
		this->value = value;
	}

	bool isPluginParameter(void) const {
		return pluginParameter;
	}

private:
	GlobalParameter globalID;
	int localID;
	String name;
	var value;
	bool pluginParameter;
};

#endif //__PARAMETER_H__
