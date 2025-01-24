#pragma once
#include "StaticObject.h"
#include "DynamicObject.h"

typedef struct Resource {
	sf::Sprite* sprite;
	int id;
	int maxNum;

	Resource(sf::Sprite* res, int i,int mn) {
		sprite = res;
		id = i;
		maxNum = mn;
	}
}Resource;

class Level
{
private:
	std::vector<StaticObject*> staticObjects;
	std::vector<DynamicObject> dynamicObjects;

	std::vector<Resource*> resources;
	sf::Texture beltTexture;
	sf::Texture gearTexture;
	sf::Sprite* selectedResource;
	sf::Font font;	
	
	std::vector<int> resourceNumbers;
	std::vector<sf::Text> resourceNumbersText;

	bool clickedResource = false;
	int selectedResoureceIndex = -1;

	void initFont();
	void initTextures();
	void initStaticObjects();
	void initResources();

	const Resource* getResourceById(const int id)const;

	const sf::Vector2f& alignToGrid(const sf::Vector2f& pos)const;
public:
	Level();
	void initLevel();
	void handleInput(sf::RenderWindow& window);
	void handleClick(sf::Vector2f& mousePosition);
	void update();
	void render(sf::RenderTarget& target);

	const sf::FloatRect& getBouds()const;
	virtual ~Level();
};

