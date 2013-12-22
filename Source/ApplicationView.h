#ifndef __APPLICATIONVIEW_H__
#define __APPLICATIONVIEW_H__

#include "ParameterID.h"

/*
 * ApplicationView defines an interface for a presentation layer.
 * This allows a controller to notify this view layer of any parameter
 * changes that have occured from the model layer.
 *
 * The class implementing ApplicationView should hold a reference to the ApplicationController
 * to notify the controller of any changes from the user interface (which the controller can
 * pass to the model).
 */
class ApplicationView
{
public:
	virtual void setParameterValue(ParameterID parameter, var value) = 0;
};

#endif //__APPLICATIONVIEW_H__