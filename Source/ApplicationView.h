#ifndef __APPLICATIONVIEW_H__
#define __APPLICATIONVIEW_H__

#include "GlobalParameters.h"


class ApplicationView
{
public:
	virtual void setGlobalParameterValue(GlobalParameter parameter, float value) = 0;
};

#endif //__APPLICATIONVIEW_H__