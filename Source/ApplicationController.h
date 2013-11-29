#ifndef __APPLICATIONCONTROLLER_H__
#define __APPLICATIONCONTROLLER_H__

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


private:
	ApplicationModel *model;
	ApplicationView *view;
};

#endif //__APPLICATIONCONTROLLER_H__