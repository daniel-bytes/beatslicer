#ifndef __MONOMECONTROL_H__
#define __MONOMECONTROL_H__

#include "ApplicationView.h"

class MonomeControl
	: public ApplicationView
{
public:
	virtual void setParameterValue(ParameterID parameter, var value);
};


#endif //__MONOMECONTROL_H__