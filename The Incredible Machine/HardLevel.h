#pragma once
#include "Level.h"
/// <summary>
/// Manages gameplay area
/// Placement of resources
/// Interactions between objects
/// AI state management
/// </summary>
class HardLevel : public Level
{
private:
	//Box resource size
	static const float IN_GAME_BOX_SIZE;


	/* Image texture used to represent gear */
	sf::Texture boxTexture;

	void initState()override;
	void initTextures()override;
	void initStaticObjects()override;
	void initStaticWheels()override;
	void initDynamicObjects()override;
	void initForbiddenActions()override;
	void initResources()override;

public:
	HardLevel(GameModes mode = GameModes::PLAYER_MODE);

	virtual ~HardLevel() override;

};

