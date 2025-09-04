#pragma once
#include "Level.h"
/// <summary>
/// Manages gameplay area
/// Placement of resources
/// Interactions between objects
/// AI state management
/// </summary>
class EasyLevel : public Level
{
private:

	void initState()override;
	void initStaticObjects()override;
	void initStaticWheels()override;
	void initDynamicObjects()override;
	void initForbiddenActions()override;
	void initResources()override;

public:
	EasyLevel(bool mode = false);

	virtual ~EasyLevel() override;

};

