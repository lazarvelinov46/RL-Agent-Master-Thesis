#pragma once
#include "StaticObject.h"
#include "DynamicObject.h"
#include "StaticWheel.h"
#include "StateRL.h"

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
	std::vector<DynamicObject*> dynamicObjects;
	std::vector<StaticWheel*> staticWheels;
	std::vector<std::vector<sf::Vertex>> belts;

	std::vector<Resource*> resources;
	sf::Texture beltTexture;
	sf::Texture gearTexture;
	sf::Sprite* selectedResource;
	sf::Font font;	

	sf::Vector2f beltStart;
	sf::Vertex endPointsBelt[2];
	
	std::vector<int> resourceNumbers;
	std::vector<sf::Text> resourceNumbersText;

	bool clickedResource = false;
	bool activeBeltPlacement = false;
	bool validBeltPlacement = false;
	int selectedResoureceIndex = -1;
	int selectedResourceListPosition = -1;
	int levelWidth = 1000;
	float gravity = 30.f;
	float drag = 0.98f;
	float velocityMinX = 0.5f;
	float velocityMinY = 1.0f;

	bool isPlaying = false;
	//AI
	StateRL state;

	void initFont();
	void initTextures();
	void initStaticObjects();
	void initStaticWheels();
	void initDynamicObjects();
	void initResources();

	void startPlatform(const StaticObject* object);
	bool hasWheel(const StaticObject* object);
	bool checkOverlaping(const sf::Sprite& newObject);
	void applyDrag(DynamicObject* object);

	const Resource* getResourceById(const int id)const;

	const sf::Vector2f& alignToGrid(const sf::Vector2f& pos)const;

	void updateBalls(float deltaTime = 0);
public:
	Level();
	void initLevel();
	void handleInput(sf::RenderWindow& window);
	void handleClick(sf::Vector2f& mousePosition);
	void handleRightClick();
	void update(float deltaTime=0);
	void render(sf::RenderTarget& target);



	const sf::FloatRect& getBouds()const;
	void setIsPlaying(bool playing);
	virtual ~Level();

	//AI Game functions 
	bool tryGearPlacement(sf::Vector2f position);
	int getNumberOfGears();
	std::vector<StaticObject*> getStaticObjects();
	std::vector<StaticWheel*> getStaticWheels();

	int getNumberOfBelts();
	void placeBelt(sf::Vector2f start, sf::Vector2f end);
	StateRL getStatusChange();
};

