#pragma once
#include "Level.h"
#include "MediumLevel.h"
/// <summary>
/// Manages gameplay area
/// Placement of resources
/// Interactions between objects
/// AI state management
/// </summary>
class MediumLevel : public Level
{
private:

	void initState()override;
	void initStaticObjects()override;
	void initStaticWheels()override;
	void initDynamicObjects()override;
	void initForbiddenActions()override;
	void initResources()override;

public:
	MediumLevel(bool mode=false);
	
	virtual ~MediumLevel() override;

};

