#pragma once
#include "StaticObject.h"
#include "DynamicObject.h"
class Level
{
private:
	std::vector<StaticObject*> staticObjects;
	std::vector<DynamicObject> dynamicObjects;

	std::vector<sf::Sprite*> resources;
	sf::Texture beltTexture;
	sf::Texture gearTexture;
	sf::Sprite selectedResource;

	bool clickedResource = false;
	void initTextures();
	void initStaticObjects();
	void initResources();
public:
	Level();
	void initLevel();
	void handleInput(sf::RenderTarget& target);
	void handleClick(sf::Vector2f& mousePosition);
	void update();
	void render(sf::RenderTarget& target);

	const sf::FloatRect& getBouds()const;
	virtual ~Level();
};

