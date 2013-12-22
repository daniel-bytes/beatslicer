#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterID.h"
#include "Utilities.h"

class Parameter
{
public:
	Parameter(ParameterID id, const String &name, var initialValue)
	{
		this->id = id;
		this->name = name;
		this->value = initialValue;
	}

	ParameterID getParameterID(void) const {
		return this->id;
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

private:
	ParameterID id;
	String name;
	var value;
};

class PluginParameter
	: public Parameter
{
public:
	PluginParameter(ParameterID id, const String &name, var initialValue, float minValue, float maxValue)
		: Parameter(id, name, initialValue), minValue(minValue), maxValue(maxValue)
	{
	}

	float getNormalizedValue(void) {
		var value = getValue();

		if (value.isBool()) {
			return (bool)value ? maxValue : minValue;
		}
		else {
			return normalize((float)getValue(), minValue, maxValue);
		}
	}

	void setNormalizedValue(float value) {
		if (getValue().isBool()) {
			setValue(value == maxValue);
		}
		else {
			value = scale(value, minValue, maxValue);
			setValue(value);
		}
	}

private:
	float minValue, maxValue;
};

#endif //__PARAMETER_H__
