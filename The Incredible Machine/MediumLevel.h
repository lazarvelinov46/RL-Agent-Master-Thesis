#pragma once
#include "Level.h"
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
	MediumLevel(GameModes mode=GameModes::PLAYER_MODE);
	
	virtual ~MediumLevel() override;

};

