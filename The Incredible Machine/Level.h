#pragma once
#include "StaticObject.h"
#include "DynamicObject.h"
class Level
{
private:
	std::vector<StaticObject*> staticObjects;
	std::vector<DynamicObject> dynamicObjects;
public:
	Level();
	void initLevel();
	void render(sf::RenderTarget& target);
	virtual ~Level();
};

