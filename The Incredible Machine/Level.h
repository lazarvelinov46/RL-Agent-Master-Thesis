#pragma once
#include "StaticObject.h"
#include "DynamicObject.h"
#include "StaticWheel.h"
#include "StateRL.h"
#include "ActionRL.h"

/// <summary>
/// Represents resources which user/agent can place on the level screen
/// </summary>
typedef struct Resource {
	sf::Sprite* sprite;	///< sprite used for rendering the resource
	int id;				///< resource id
	int maxNum;			///< max number of resources available on level

	Resource(sf::Sprite* res, int i, int mn) {
		sprite = res;
		id = i;
		maxNum = mn;
	}
}Resource;

class Level {
private:
protected:
	int startingNumberOfGears;
	int startingNumberOfBelts;
	int startingNumberOfBoxes=0;
	int numberOfWheels;
	int numberOfBalls ;


	int currentNumberOfBoxes = 0;
	int currentNumberOfGears=0;
	int currentNumberOfBelts=0;
	int currentNumberOfResources=0;
	/* Non moving parts of level, already placed when level opens unlike resources */
	std::vector<StaticObject*> staticObjects;
	/* Movable objects (balls) */
	std::vector<DynamicObject*> dynamicObjects;
	/* Wheel objects */
	std::vector<StaticWheel*> staticWheels;
	/* Belt objects (graphic representation on level screen) */
	std::vector<std::vector<sf::Vertex>> belts;
	float maxBeltDistance;

	/* Vector of resources which could be placed */
	std::vector<Resource*> resources;
	/* Image texture used to represent belt */
	sf::Texture beltTexture;
	/* Image texture used to represent gear */
	sf::Texture gearTexture;
	/* Pointer to Sprite of selected resource to place */
	sf::Sprite* selectedResource;
	/* Font used for text */
	sf::Font font;

	/* Start position when placing the belt */
	sf::Vector2f beltStart;
	/* Visual endpoints for belt */
	sf::Vertex endPointsBelt[2];

	/* Represents how much resources user has left */
	std::vector<int> resourceNumbers;
	/* Text on screen which represents resources left */
	std::vector<sf::Text> resourceNumbersText;

	//where balls were there can't be gear in AI mode
	std::unordered_set<int> forbiddenActions;

	/* Flag if resource is clicked */
	bool clickedResource = false;
	bool activeBeltPlacement = false;
	bool startBeltGear;
	/* Flag if belt placement is currently valid */
	bool validBeltPlacement = false;
	/* Index of selected resource */
	int selectedResoureceIndex = -1;
	int selectedResourceListPosition = -1;
	int levelWidth = 1000;
	float gravity = 30.f;
	float drag = 0.98f;
	float velocityMinX = 0.5f;
	float velocityMinY = 1.0f;

	/* Represents if physics has started */
	bool isPlaying = false;

	//AI
	bool modeAI = false;
	/* Tracking in which state agent currently is */
	StateRL* state;
	double reward;
	/* Variable that is true when there is change in state of agent */
	bool stateChanged;

	virtual void initState()=0;
	void initFont();
	virtual void initTextures();
	virtual void initStaticObjects()=0;
	virtual void initStaticWheels()=0;
	virtual void initDynamicObjects()=0;
	virtual void initForbiddenActions()=0;
	virtual void initResources()=0;

	void resetStaticObjects();
	void resetDynamicObjects();

	void startPlatform(const StaticObject* object);
	bool hasWheel(const StaticObject* object);
	bool checkOverlaping(const sf::Sprite& newObject);
	void applyDrag(DynamicObject* object);

	const Resource* getResourceById(const int id)const;

	const sf::Vector2f& alignToGridGear(const sf::Vector2f& pos)const;
	const sf::Vector2f& alignToGridBox(const sf::Vector2f& pos)const;

	void updateBalls(float deltaTime = 0);
	/// <summary>
	/// Adds element to forbidden action set based on ball location
	/// </summary>
	/// <param name="ball">Object which location is taken into consideration</param>
	void markForbiddenFromBall(DynamicObject* ball);
public:
	Level(int numberOfBalls, int numberOfWheels, int numberOfGears, 
		int numberOfBelts, float maxBeltDistance,int startingNumberOfBoxes=0);
	virtual ~Level();

	virtual void initLevel();
	/// <summary>
	/// Resets level when game is over (used for AI)
	/// Reverts everything to starting values
	/// </summary>
	virtual void resetLevel();
	/// <summary>
	/// Updates position of clicked resource (if there is one)
	/// </summary>
	/// <param name="window"> Reference to main render window</param>
	virtual void handleInput(sf::RenderWindow& window);
	/// <summary>
	/// Handles mouse click on level screen
	/// </summary>
	/// <param name="mousePosition">Coordinates where mouse click happened</param>
	virtual void handleClick(sf::Vector2f& mousePosition);
	virtual void handleRightClick();
	virtual void update(float deltaTime = 0);
	virtual void render(sf::RenderTarget& target);

	const sf::FloatRect& getBouds()const;
	void setIsPlaying(bool playing);

	//AI Game functions 
	virtual bool tryGearPlacement(sf::Vector2f position);
	virtual int getNumberOfGears();
	virtual std::vector<StaticObject*> getStaticObjects();
	virtual std::vector<StaticWheel*> getStaticWheels();
	/// <summary>
	/// Gets gear location based on gear Id
	/// gear id - lowest id for earliest placed gear etc
	/// </summary>
	/// <param name="gearId">id of gear</param>
	/// <returns>Location of gear for belt placement</returns>
	virtual sf::Vector2f getGearLocation(int gearId);
	/// <summary>
	/// Gets wheel location based on wheel id
	/// wheel id - leftmost wheel lowest id etc
	/// </summary>
	/// <param name="wheelId">id of wheel</param>
	/// <returns>Location of static wheel for belt placement</returns>
	virtual sf::Vector2f getWheelLocation(int wheelId);
	/// <summary>
	/// Returns reward after every state change
	/// </summary>
	/// <returns>reward value</returns>
	virtual double getReward();

	/// <summary>
	/// Returns max belt distance
	/// </summary>
	/// <returns>Maximum possible belt distance</returns>
	float getMaxBeltDistance();

	virtual int getNumberOfBelts();
	virtual bool placeBelt(sf::Vector2f start, sf::Vector2f end, bool startBeltGear);
	virtual StateRL* getStatusChange();
	virtual bool getStateChanged();
	//calculates distance between two points
	static float distance(const sf::Vector2f& a, const sf::Vector2f& b);
	int getStartingNumberOfGears();
	int getStartingNumberOfBelts();
	int getNumberOfWheels();
	int getNumberOfBalls();
	/// <summary>
	/// Set which contains ids of forbidden actions
	/// </summary>
	/// <returns>unordered sets of forbidden actions</returns>
	std::unordered_set<int> getBallZonesPassed();
	static const float GEAR_WIDTH;
};