#include "stdafx.h"
#include "Level.h"

void Level::initState()
{
	this->state.setInitialState();
	this->stateChanged = false;
}

void Level::initFont()
{
	if (!font.loadFromFile("assets/Fonts/ARIAL.TTF")) {
		std::cout << "ERROR: Could not load font" << std::endl;
	}
}

void Level::initTextures()
{
	if (!this->beltTexture.loadFromFile("assets/Textures/belt.png")) {
		std::cout << "ERROR: Could not load texture" << std::endl;
	}

	if (!this->gearTexture.loadFromFile("assets/Textures/gear.png")) {
		std::cout << "ERROR: Could not load texture" << std::endl;
	}
}

void Level::initStaticObjects()
{
	this->staticObjects.push_back(new StaticObject(0, 760, 1200, 50, sf::Color::Red,StaticObjectType::FLOOR));

	this->staticObjects.push_back(new StaticObject(100, 600, 150, 20, sf::Color::Blue, StaticObjectType::PLATFORM));
	this->staticObjects.push_back(new StaticObject(300, 500, 150, 20, sf::Color::Blue, StaticObjectType::PLATFORM));
	this->staticObjects.push_back(new StaticObject(600, 300, 150, 20, sf::Color::Blue, StaticObjectType::PLATFORM));

	this->staticObjects.push_back(new StaticObject(830, 400, 20, 40, sf::Color::Green, StaticObjectType::WALL));
	this->staticObjects.push_back(new StaticObject(830, 440, 160, 20, sf::Color::Green, StaticObjectType::GOAL));
	this->staticObjects.push_back(new StaticObject(970, 400, 20, 40, sf::Color::Green, StaticObjectType::WALL));
}

void Level::initStaticWheels()
{
	this->staticWheels.push_back(new StaticWheel(200, 610.f, 10.f, sf::Color::Yellow));
	this->staticWheels.push_back(new StaticWheel(400, 510.f, 10.f, sf::Color::Yellow));
	this->staticWheels.push_back(new StaticWheel(700, 310.f, 10.f, sf::Color::Yellow));
}

void Level::initDynamicObjects()
{
	this->dynamicObjects.push_back(new DynamicObject(25, 500, 25, sf::Color::Cyan));
	this->dynamicObjects.push_back(new DynamicObject(100, 550, 25, sf::Color::Cyan));
	this->dynamicObjects.push_back(new DynamicObject(300, 450, 25, sf::Color::Cyan));
	this->dynamicObjects.push_back(new DynamicObject(600, 250, 25, sf::Color::Magenta));
}

void Level::initResources()
{
	int beltNum = 4;
	int gearNum = 3;
	int resNum = 0;
	for (size_t i = 0;i < beltNum;i++) {
		sf::Sprite* beltSprite = new sf::Sprite(this->beltTexture);
		beltSprite->setScale(0.5f, 0.5f);
		beltSprite->setPosition(1050, 150);

		this->resources.push_back(new Resource(beltSprite, resNum,beltNum));
	}
	resNum++;
	for (size_t i = 0;i < gearNum;i++) {
		sf::Sprite* gearSprite = new sf::Sprite(this->gearTexture);
		gearSprite->setScale(0.19531f, 0.19531f);
		gearSprite->setPosition(1050, 350);

		sf::Text gearText;
		gearText.setFont(font);
		gearText.setString("3");
		gearText.setCharacterSize(18);
		gearText.setPosition(gearSprite->getPosition().x + gearSprite->getGlobalBounds().width / 2 - 18, gearSprite->getPosition().y + gearSprite->getGlobalBounds().height);
		gearText.setFillColor(sf::Color::Black);

		this->resources.push_back(new Resource(gearSprite, resNum,gearNum));
	}
	resNum++;
	for (size_t i = 0;i < resNum;i++) {
		const Resource* res = this->getResourceById(i);
		sf::Text text;
		text.setFont(font);
		text.setString(std::to_string(res->maxNum));
		text.setCharacterSize(18);
		text.setPosition(res->sprite->getPosition().x + res->sprite->getGlobalBounds().width / 2 - 18,
			res->sprite->getPosition().y + res->sprite->getGlobalBounds().height);
		text.setFillColor(sf::Color::Black);
		this->resourceNumbers.push_back(res->maxNum);
		this->resourceNumbersText.push_back(text);
	}
}

void Level::startPlatform(const StaticObject* object)
{
	for (const std::vector<sf::Vertex>& belt : this->belts) {
		if (object->getGlobalBounds().contains(belt[0].position)) {
			int i = 0;
			for (StaticObject* platform : this->staticObjects) {
				if (!platform->getMoving()&&platform->getObjectType() == StaticObjectType::PLATFORM &&
					platform->getGlobalBounds().contains(belt[1].position)) {
					platform->move(5.f);
					this->state.setWheelStarted(i, true);
					this->stateChanged = true;

					std::cout << "startovana platforma" << std::endl;
				}
				if (platform->getObjectType() == StaticObjectType::PLATFORM)i++;
			}
		}
	}
}

bool Level::hasWheel(const StaticObject* object)
{
	return false;
}

bool Level::checkOverlaping(const sf::Sprite& newObject)
{
	std::cout << "TO PLACE X: " << newObject.getGlobalBounds().left
		<< "; Y: " << newObject.getGlobalBounds().top
		<< "; W: " << newObject.getGlobalBounds().width
		<< "; H: " << newObject.getGlobalBounds().height << std::endl;
	for (size_t i = 0; i < this->staticObjects.size(); i++) {
		std::cout << "STAT X: " << this->staticObjects[i]->getGlobalBounds().left
			<< "; Y: " << this->staticObjects[i]->getGlobalBounds().top
			<< "; W: " << this->staticObjects[i]->getGlobalBounds().width
			<< "; H: " << this->staticObjects[i]->getGlobalBounds().height << std::endl;
		if (newObject.getGlobalBounds().intersects(this->staticObjects[i]->getGlobalBounds())) {
			return false;
		}
	}
	for (size_t i = 0; i < this->dynamicObjects.size(); i++) {
		std::cout << "DYN X: " << this->dynamicObjects[i]->getGlobalBounds().left
			<< "; DYN: " << this->dynamicObjects[i]->getGlobalBounds().top
			<< "; DYN: " << this->dynamicObjects[i]->getGlobalBounds().width
			<< "; DYN: " << this->dynamicObjects[i]->getGlobalBounds().height << std::endl;
		if (newObject.getGlobalBounds().intersects(this->dynamicObjects[i]->getGlobalBounds())) {
			return false;
		}
	}
	return true;
}

void Level::applyDrag(DynamicObject* object)
{
	float newVelocity = std::abs(object->getVelocity().x * this->drag) < this->velocityMinX ? 0.f : object->getVelocity().x * this->drag;
	object->setVelocity(sf::Vector2f(newVelocity, object->getVelocity().y));
}

const Resource* Level::getResourceById(int id) const
{
	for (size_t i = 0;i < this->resources.size();i++) {
		if (this->resources[i]->id == id) {
			return this->resources[i];
		}
	}
}

const sf::Vector2f& Level::alignToGrid(const sf::Vector2f& pos) const
{
	float x = (float)(pos.x - (int)pos.x % 50);
	float y = (float)(pos.y - (int)pos.y % 50);
	return sf::Vector2f(x, y);
}

void Level::updateBalls(float deltaTime)
{
	if (!isPlaying)return;
	for (size_t i = 0; i < this->dynamicObjects.size(); i++) {
		DynamicObject* ball = this->dynamicObjects[i];
		sf::Vector2f oldVelocity = ball->getVelocity();
		sf::Vector2f newPosition = ball->getPosition()+ oldVelocity;
		sf::FloatRect ballBounds = ball->getShape().getGlobalBounds();
		float ballBottom = ballBounds.top + ballBounds.height;
		float ballRight = ballBounds.left + ballBounds.width;

		bool collision = false;
		int gears = 0;
		for (const StaticObject* object : this->staticObjects) {
			sf::FloatRect objectBounds = object->getGlobalBounds();
			
			if (ballBounds.intersects(objectBounds)) {
				
				collision = true;
				float objectBottom = objectBounds.top+objectBounds.height;
				float objectRight = objectBounds.left + objectBounds.width;

				float topCollision = ballBottom - objectBounds.top;
				float bottomCollision = objectBottom - ballBounds.top;
				float leftCollision = ballRight - objectBounds.left;
				float rightCollision = objectRight - ballBounds.left;
				
				if (topCollision <leftCollision && topCollision<rightCollision) {
					//top collision
					newPosition.y = objectBounds.top - ballBounds.height;
					ball->setVelocity(sf::Vector2f(ball->getVelocity().x, (abs(ball->getVelocity().y*-.5f)<this->velocityMinY)?0.f: ball->getVelocity().y * -.7f));

					if (object->getObjectType() == StaticObjectType::GOAL) {
						if (!this->state.getTargetHit()) {
							this->state.setTargetHit(true);
							this->stateChanged = true;

							std::cout << "cilj" << std::endl;
						}
					}
				}

				else if (bottomCollision<leftCollision && bottomCollision<rightCollision) {
					//bottom collision
					newPosition.y = objectBottom;
					ball->setVelocity(sf::Vector2f(ball->getVelocity().x, -ball->getVelocity().y));
				}

				else if (leftCollision<topCollision && leftCollision<bottomCollision) {
					//left collision
					newPosition.x = objectBounds.left - ballBounds.width;
					ball->setVelocity(sf::Vector2f(ball->getVelocity().x*-0.9f, ball->getVelocity().y));
				}
				else if (rightCollision<topCollision&& rightCollision<bottomCollision) {
					//left collision
					newPosition.x = objectRight;
					ball->setVelocity(sf::Vector2f(ball->getVelocity().x * -0.9f, ball->getVelocity().y));
				}
				if (object->getObjectType() == StaticObjectType::GEAR) {
					if (!this->state.getGearStarted(gears)) {
						this->state.setGearStarted(gears, true);
						this->stateChanged = true;

						std::cout << "zupcanik" << std::endl;
						this->startPlatform(object);
					}
				}
				if (object->getMoving() && ball->getVelocity().x == 0.f) {
					ball->setVelocity(sf::Vector2f(object->getSpeed(), 0.f));
				}

				else {
					ball->setVelocity(sf::Vector2f(ball->getVelocity().x, ball->getVelocity().y));
				}

				ball->setPosition(newPosition);
				break;
			}
			if (object->getObjectType() == StaticObjectType::GEAR) {
				gears++;
			}
		}

		if (newPosition.x < 0) {
			newPosition.x = 0;
			ball->setVelocity(sf::Vector2f(-0.9f*ball->getVelocity().x, ball->getVelocity().y));
		}

		else if (newPosition.x + ball->getGlobalBounds().width > this->levelWidth) {
			newPosition.x = this->levelWidth - ball->getGlobalBounds().width;
			ball->setVelocity(sf::Vector2f(-0.9f*ball->getVelocity().x, ball->getVelocity().y));
		}
		if (!collision) {
			ball->setPosition(newPosition);
			this->applyDrag(ball);
		}
		if (oldVelocity.x == 0.0f && oldVelocity.y == 0.0f && (abs(ball->getVelocity().x > 1) || abs(ball->getVelocity().y  >1))) {
			this->state.setBallMoving(i, true);
			this->stateChanged = true;

		}
		else if ((abs(oldVelocity.x > 1) || abs(oldVelocity.y >1)) && ball->getVelocity().x == 0.0f && ball->getVelocity().y == 0.0f) {
			this->state.setBallMoving(i, false);
			this->stateChanged = true;

		}


		ball->setVelocity(sf::Vector2f(ball->getVelocity().x, ball->getVelocity().y + this->gravity * deltaTime));
	}
}

Level::Level()
{
	//this->initLevel();
}

void Level::initLevel()
{
	this->initFont();

	this->initTextures();

	this->initStaticObjects();

	this->initStaticWheels();

	this->initDynamicObjects();

	this->initResources();

	this->initState();

}

void Level::handleInput(sf::RenderWindow& window)
{
	if (this->clickedResource) {
		sf::Vector2i position = sf::Mouse::getPosition(window);
		this->selectedResource->setPosition(position.x, position.y);
	}

	
}

void Level::handleClick(sf::Vector2f& mousePosition)
{
	if (!this->clickedResource)
	{
		/*
		if there is not clicked resource checking if mouse click happened on some
		resource element from menu
		*/
		for (size_t i = 0;i < this->resources.size();i++) {
			if (this->resources[i]->sprite->getGlobalBounds().contains(mousePosition) && 
				mousePosition.x>1000 && 
				this->resourceNumbers[this->resources[i]->id]>0) {
				/*
				* if mouse click position overlaps resource boundaries while
				* RESOURCE IS IN MENU (hence the mousePosition.x>1000 part)
				* gets the pointer of the selected resource and updates appropriate variables
				*/
				this->selectedResource = this->resources[i]->sprite;
				this->clickedResource = true;
				this->selectedResoureceIndex = this->resources[i]->id;
				this->selectedResourceListPosition = i;
				break;
			}
		}
	}
	else {
		/*
		* If resource is already clicked than its placement needs to be handled
		*/
		if (this->selectedResoureceIndex == 0&&!this->activeBeltPlacement) {
			//placing belt first time
			for (const StaticWheel* wheel : this->staticWheels) {
				if (wheel->getGlobalBounds().contains(mousePosition)) {
					/*
					if belt placing starts from wheel
					updating belt start and endPointBelt variables
					makes belt follow the mouse
					*/
					this->activeBeltPlacement = true;
					this->beltStart = sf::Vector2f(wheel->getGlobalBounds().left, wheel->getGlobalBounds().top);
					this->endPointsBelt[0].position = this->beltStart;
					this->endPointsBelt[1].position = mousePosition;
					this->endPointsBelt[0].color = sf::Color::Yellow;
					this->endPointsBelt[1].color = sf::Color::Yellow;
					break;
				}
			}
			for (const StaticObject* object : this->staticObjects) {
				if (object->getGlobalBounds().contains(mousePosition)&&object->getObjectType()==StaticObjectType::GEAR) {
					/*
					if belt placing starts from gear already placed
					updating belt start and endPointBelt variables
					makes belt follow the mouse
					*/
					this->activeBeltPlacement = true;
					this->beltStart = sf::Vector2f(object->getGlobalBounds().left, object->getGlobalBounds().top);
					this->endPointsBelt[0].position = this->beltStart;
					this->endPointsBelt[1].position = mousePosition;
					this->endPointsBelt[0].color = sf::Color::Yellow;
					this->endPointsBelt[1].color = sf::Color::Yellow;
					break;
				}
			}

		}
		else if (this->selectedResoureceIndex == 0 && this->activeBeltPlacement) {
			//placing belt second time
			this->validBeltPlacement = false;
			for (const StaticWheel* wheel : this->staticWheels) {
				if (wheel->getGlobalBounds().contains(mousePosition)) {
					/*
					checking if belt placement is valid (ends on a wheel)
					*/
					this->validBeltPlacement = true;
					break;
				}
			}
			if (this->validBeltPlacement) {
				/*
				if belt placement is valid, belt positions are pushed into list
				as if it is two lines (simple graphic representation)
				*/
				this->belts.push_back({ this->beltStart,mousePosition });
				this->belts.push_back({ sf::Vector2f(mousePosition.x + this->staticWheels[0]->getGlobalBounds().width,mousePosition.y + this->staticWheels[0]->getGlobalBounds().height),
					sf::Vector2f(this->beltStart.x + this->staticWheels[0]->getGlobalBounds().width,this->beltStart.y + this->staticWheels[0]->getGlobalBounds().height) });
				/*
				Updating variables and UI elements
				*/
				this->activeBeltPlacement = false;
				this->resourceNumbersText[this->selectedResoureceIndex].setString(std::to_string(--this->resourceNumbers[this->selectedResoureceIndex]));
				this->clickedResource = false;
				this->selectedResoureceIndex = -1;
				this->resources.erase(this->resources.begin()+this->selectedResourceListPosition);
				this->selectedResourceListPosition = 0;
				
			}

		}
		else {
			/*
			placing gear, aligning it to grid first
			*/
			sf::Vector2f alignedPosition = this->alignToGrid(mousePosition);

			this->selectedResource->setPosition(alignedPosition);
			if (this->checkOverlaping(*this->selectedResource)) {
				/*
				if it does not overlap anything, gear can be placed
				updating variables and UI elements
				*/
				if (alignedPosition.x < 900 && alignedPosition.y < 700) {
					this->staticObjects.push_back(new StaticObject(*this->selectedResource, this->selectedResoureceIndex == 1 ? StaticObjectType::GEAR : StaticObjectType::PLATFORM));
					this->resourceNumbersText[this->selectedResoureceIndex].setString(std::to_string(--this->resourceNumbers[this->selectedResoureceIndex]));
				}
				this->clickedResource = false;
				this->selectedResoureceIndex = -1;
				std::cout << "STAVLJENO " << this->selectedResource->getGlobalBounds().left << this->selectedResource->getGlobalBounds().top
					<< this->selectedResource->getGlobalBounds().height << this->selectedResource->getGlobalBounds().width << std::endl;

			}
		}
		
	}
}

void Level::handleRightClick()
{
	/*
	Cancels belt placement
	*/
	this->activeBeltPlacement = false;
}

void Level::update(float deltaTime)
{
	/*
	Updates ball position
	*/
	this->updateBalls(deltaTime);
	if (this->activeBeltPlacement) {
		for (const StaticWheel* wheel : this->staticWheels) {
			if (wheel->getGlobalBounds().contains(this->endPointsBelt[1].position)) {
				this->endPointsBelt[0].color = sf::Color::Green;
				this->endPointsBelt[1].color = sf::Color::Green;
				break;
			}
		}
	}
}


void Level::render(sf::RenderTarget& target)
{
	/*
	Renders all game elements
	*/
	for (StaticObject* object : this->staticObjects) {
		object->render(target);
	}
	for (StaticWheel* object : this->staticWheels) {
		object->render(target);
	}
	for (size_t i = 0;i < this->belts.size();i++) {
		sf::Vertex line[] = { this->belts[i][0],this->belts[i][1]};
		target.draw(line, 2, sf::Lines);
	}
	for (Resource* res : this->resources) {
		target.draw(*res->sprite);
	}
	for (DynamicObject* object : this->dynamicObjects) {
		object->render(target);
	}
	for (sf::Text t : this->resourceNumbersText) {
		target.draw(t);
	}
	/*
	if (this->clickedResource) {
		target.draw(*this->selectedResource);
	}
	*/
	if (this->activeBeltPlacement) {
		sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Mouse::getPosition(*dynamic_cast<sf::RenderWindow*>(&target)));
		this->endPointsBelt[1].position = mousePosition;
		target.draw(this->endPointsBelt, 2, sf::Lines);
	}
}

bool Level::tryGearPlacement(sf::Vector2f position)
{
	/*
	Selects gear from resources
	Aligns it to the grid
	*/
	this->selectedResource = this->resources.back()->sprite;
	sf::Vector2f alignedPosition = this->alignToGrid(position);
	this->selectedResource->setPosition(alignedPosition);
	if (this->checkOverlaping(*this->selectedResource)) {
		/*
		* If it does not overlap anything it places gear in appropriate place
		* Adds it to static objects on level map
		* Lowers the resources
		*/
		if (alignedPosition.x < 900 && alignedPosition.y < 700) {
			this->staticObjects.push_back(new StaticObject(*this->selectedResource, StaticObjectType::GEAR ));
			this->resourceNumbersText[1].setString(std::to_string(--this->resourceNumbers[1]));
			this->resources.pop_back();
		}
		std::cout << "STAVLJENO " << this->selectedResource->getGlobalBounds().left << this->selectedResource->getGlobalBounds().top
			<< this->selectedResource->getGlobalBounds().height << this->selectedResource->getGlobalBounds().width << std::endl;
		return true;
	}
	else {
		this->selectedResource->setPosition(1050, 350);
		return false;
	}
}

int Level::getNumberOfGears()
{
	return this->resourceNumbers[1];
}

std::vector<StaticObject*> Level::getStaticObjects()
{
	return this->staticObjects;
}

std::vector<StaticWheel*> Level::getStaticWheels()
{
	return this->staticWheels;
}

int Level::getNumberOfBelts()
{
	return this->resourceNumbers[0];
}

void Level::placeBelt(sf::Vector2f start, sf::Vector2f end)
{
	/*
	* Adds belt into array (start and end vectors)
	* Lowers the resources
	* If gear attached to belt is started, it starts the platform
	*/
	this->belts.push_back({start,end });
	this->belts.push_back({ sf::Vector2f(end.x + this->staticWheels[0]->getGlobalBounds().width,end.y + this->staticWheels[0]->getGlobalBounds().height),
		sf::Vector2f(start.x + this->staticWheels[0]->getGlobalBounds().width,start.y + this->staticWheels[0]->getGlobalBounds().height) });
	//this->activeBeltPlacement = false;
	this->resourceNumbersText[0].setString(std::to_string(--this->resourceNumbers[0]));
	this->resources.erase(this->resources.begin());
	
	int gears = 0;
	for (const StaticObject* object : this->staticObjects) {
		if (object->getObjectType() == StaticObjectType::GEAR) {
			//maybe start and end
			if (object->getGlobalBounds().contains(start)) {
				if (this->state.getGearStarted(gears)) {
					
					this->startPlatform(object);
				}
			}
			gears++;
		}
	}
}

StateRL Level::getStatusChange()
{
	return this->state;
}

bool Level::getStateChanged()
{
	bool retVal = this->stateChanged;
	this->stateChanged = false;
	return retVal;
}

const sf::FloatRect& Level::getBouds() const
{
	return sf::FloatRect(0,0,1000,800);
}

void Level::setIsPlaying(bool playing)
{
	this->isPlaying = playing;
}

Level::~Level()
{
	for (size_t i = 0;i < this->staticObjects.size();i++) {
		delete this->staticObjects[i];
	}
	for (size_t i = 0; i < this->resources.size(); i++) {
		delete this->resources[i]->sprite;
		delete this->resources[i];
	}
	for (size_t i = 0; i < this->dynamicObjects.size(); i++) {
		delete this->dynamicObjects[i];
	}
	for (size_t i = 0;i < this->staticWheels.size();i++) {
		delete this->staticWheels[i];
	}
}
