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

	Resource(sf::Sprite* res, int i,int mn) {
		sprite = res;
		id = i;
		maxNum = mn;
	}
}Resource;

/// <summary>
/// Manages gameplay area
/// Placement of resources
/// Interactions between objects
/// AI state management
/// </summary>
class Level
{
private:
	static const int NUMBER_OF_GEARS = 3;
	static const int NUMBER_OF_BELTS = 3;
	static const int NUMBER_OF_WHEELS = 3;
	static const int NUMBER_OF_BALLS = 4;
	static const float GEAR_WIDTH;

	int currentNumberOfGears;
	int currentNumberOfBelts;
	int currentNumberOfResources;
	/* Non moving parts of level, already placed when level opens unlike resources */
	std::vector<StaticObject*> staticObjects;
	/* Movable objects (balls) */
	std::vector<DynamicObject*> dynamicObjects;
	/* Wheel objects */
	std::vector<StaticWheel*> staticWheels;
	/* Belt objects (graphic representation on level screen) */
	std::vector<std::vector<sf::Vertex>> belts;

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
	StateRL state;
	double reward;
	/* Variable that is true when there is change in state of agent */
	bool stateChanged;

	void initState();
	void initFont();
	void initTextures();
	void initStaticObjects();
	void initStaticWheels();
	void initDynamicObjects();
	void initForbiddenActions();
	void initResources();

	void resetStaticObjects();
	void resetDynamicObjects();
	void resetResources();

	void startPlatform(const StaticObject* object);
	bool hasWheel(const StaticObject* object);
	bool checkOverlaping(const sf::Sprite& newObject);
	void applyDrag(DynamicObject* object);

	const Resource* getResourceById(const int id)const;

	const sf::Vector2f& alignToGrid(const sf::Vector2f& pos)const;

	void updateBalls(float deltaTime = 0);
	/// <summary>
	/// Adds element to forbidden action set based on ball location
	/// </summary>
	/// <param name="ball">Object which location is taken into consideration</param>
	void markForbiddenFromBall(DynamicObject* ball);
public:
	Level(bool mode=false);
	/// <summary>
	/// Calls all init functions
	/// </summary>
	void initLevel();
	/// <summary>
	/// Resets level when game is over (used for AI)
	/// Reverts everything to starting values
	/// </summary>
	void resetLevel();
	/// <summary>
	/// Updates position of clicked resource (if there is one)
	/// </summary>
	/// <param name="window"> Reference to main render window</param>
	void handleInput(sf::RenderWindow& window);
	/// <summary>
	/// Handles mouse click on level screen
	/// </summary>
	/// <param name="mousePosition">Coordinates where mouse click happened</param>
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
	/// <summary>
	/// Gets gear location based on gear Id
	/// gear id - lowest id for earliest placed gear etc
	/// </summary>
	/// <param name="gearId">id of gear</param>
	/// <returns>Location of gear for belt placement</returns>
	sf::Vector2f getGearLocation(int gearId);
	/// <summary>
	/// Gets wheel location based on wheel id
	/// wheel id - leftmost wheel lowest id etc
	/// </summary>
	/// <param name="wheelId">id of wheel</param>
	/// <returns>Location of static wheel for belt placement</returns>
	sf::Vector2f getWheelLocation(int wheelId);
	/// <summary>
	/// Returns reward after every state change
	/// </summary>
	/// <returns>reward value</returns>
	double getReward();

	int getNumberOfBelts();
	bool placeBelt(sf::Vector2f start, sf::Vector2f end, bool startBeltGear);
	StateRL getStatusChange();
	bool getStateChanged();
	//calculates distance between two points
	static float distance(const sf::Vector2f& a, const sf::Vector2f& b);
	static int getNumberOfGearsStatic();
	static int getNumberOfBeltsStatic();
	static int getNumberOfWheels();
	static int getNumberOfBalls();
	/// <summary>
	/// Set which contains ids of forbidden actions
	/// </summary>
	/// <returns>unordered sets of forbidden actions</returns>
	std::unordered_set<int> getBallZonesPassed();
};

