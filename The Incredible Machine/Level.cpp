#include "stdafx.h"
#include "Level.h"
#include "QTable.h"

const float Level::GEAR_WIDTH = 100.0;

void Level::initState()
{
	this->state->setInitialState();
	this->stateChanged = false;
	this->reward = 0.0f;
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

void Level::resetStaticObjects()
{
	while (this->staticObjects.back()->getObjectType() == StaticObjectType::GEAR) {
		this->staticObjects.pop_back();
	}
	for (int i = 0; i < this->staticObjects.size(); i++) {
		if (this->staticObjects[i]->getObjectType() == StaticObjectType::PLATFORM_RIGHT||
			this->staticObjects[i]->getObjectType() == StaticObjectType::PLATFORM_LEFT) {
			this->staticObjects[i]->stop();
		}
	}
}

void Level::resetDynamicObjects()
{
	for (int i = 0; i < this->dynamicObjects.size(); i++) {

	}
}

void Level::startPlatform(const StaticObject* object)
{
	for (const std::vector<sf::Vertex>& belt : this->belts) {
		if (object->getGlobalBounds().contains(belt[0].position)) {
			int i = 0;
			for (StaticObject* platform : this->staticObjects) {
				if (!platform->getMoving() && 
					(platform->getObjectType() == StaticObjectType::PLATFORM_RIGHT||
						platform->getObjectType()==StaticObjectType::PLATFORM_LEFT) &&
					platform->getGlobalBounds().contains(belt[1].position)) {
					float movingSpeed = platform->getObjectType() == StaticObjectType::PLATFORM_RIGHT ? 5.f : -5.f;
					platform->move(movingSpeed);
					this->state->setWheelStarted(i, true);
					this->stateChanged = true;
					this->reward = WHEEL_ACTIVATED;

				}
				if (platform->getObjectType() == StaticObjectType::PLATFORM_RIGHT||
					platform->getObjectType() == StaticObjectType::PLATFORM_LEFT)i++;
			}
		}
	}
}

void Level::initForbiddenActions()
{
	/*
	for (size_t i = 0; i < this->dynamicObjects.size(); i++) {
		DynamicObject* ball = this->dynamicObjects[i];
		int boundsTop = static_cast<int>(ball->getGlobalBounds().top);
		int boundsBottom = static_cast<int>(ball->getGlobalBounds().top+ball->getGlobalBounds().height);
		int boundsLeft = static_cast<int>(ball->getGlobalBounds().left);
		int boundsRight = static_cast<int>(ball->getGlobalBounds().left+ball->getGlobalBounds().width);
		float value;
		int ids[4];
		ids[0] = boundsTop / 50 * 19 + boundsLeft / 50;
		ids[1] = boundsTop / 50 * 19 + boundsRight / 50;
		ids[2] = boundsBottom / 50 * 19 + boundsLeft / 50;
		ids[3] = boundsBottom / 50 * 19 + boundsRight / 50;
		for (int j = 0; j < 4; j++) {
			if (ids[j] < ActionRL::getGridWidth() * ActionRL::getGridHeight()) {
				auto result = this->forbiddenActions.insert(ids[j]);

			}
		}

	}
	*/

}

bool Level::hasWheel(const StaticObject* object)
{
	return false;
}

bool Level::checkOverlaping(const sf::Sprite& newObject)
{
	for (size_t i = 0; i < this->staticObjects.size(); i++) {
		/*
		std::cout << "STAT X: " << newObject.getGlobalBounds().left
			<< "; Y: " << newObject.getGlobalBounds().top
			<< "; W: " << newObject.getGlobalBounds().width
			<< "; H: " << newObject.getGlobalBounds().height << std::endl;
		*/
		if (newObject.getGlobalBounds().intersects(this->staticObjects[i]->getGlobalBounds())) {
			return false;
		}
	}
	for (size_t i = 0; i < this->dynamicObjects.size(); i++) {
		/*
		std::cout << "DYN X: " << this->dynamicObjects[i]->getGlobalBounds().left
			<< "; DYN: " << this->dynamicObjects[i]->getGlobalBounds().top
			<< "; DYN: " << this->dynamicObjects[i]->getGlobalBounds().width
			<< "; DYN: " << this->dynamicObjects[i]->getGlobalBounds().height << std::endl;
			*/
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
	for (size_t i = 0; i < this->resources.size(); i++) {
		if (this->resources[i]->id == id) {
			return this->resources[i];
		}
	}
}

const sf::Vector2f& Level::alignToGridGear(const sf::Vector2f& pos) const
{
	float x = (float)(pos.x - (int)pos.x % 50);
	float y = (float)(pos.y - (int)pos.y % 50);
	return sf::Vector2f(x, y);
}

const sf::Vector2f& Level::alignToGridBox(const sf::Vector2f& pos) const
{
	float x = (float)(pos.x - (int)pos.x % 100);
	float y = (float)(pos.y - (int)pos.y % 100);
	return sf::Vector2f(x, y);
}

void Level::updateBalls(float deltaTime)
{
	if (!isPlaying)return;
	bool ballsMovingPreUpdate = this->state->getBallMoving();
	bool bMPU[5];
	for (size_t i = 0; i < this->dynamicObjects.size(); i++) {
		DynamicObject* ball = this->dynamicObjects[i];
		bMPU[i] = this->state->getBallMoving(i);
		sf::Vector2f oldVelocity = ball->getVelocity();
		sf::Vector2f newPosition = ball->getPosition() + oldVelocity;
		sf::FloatRect ballBounds = ball->getGlobalBounds();
		float ballBottom = ballBounds.top + ballBounds.height;
		float ballRight = ballBounds.left + ballBounds.width;

		bool collision = false;
		bool noGravity = false;
		bool onMovingPlatform = false;
		bool stop = false;
		int gears = 0;
		for (const StaticObject* object : this->staticObjects) {
			sf::FloatRect objectBounds = object->getGlobalBounds();
			if (objectBounds.top == ballBottom && objectBounds.left <= (ballBounds.left + ballBounds.width) &&
				objectBounds.left + objectBounds.width >= ballBounds.left + ballBounds.width) {
				noGravity = true;

				for (const StaticObject* platformObject : this->staticObjects) {
					if ((object->getObjectType() == StaticObjectType::PLATFORM_RIGHT||
						object->getObjectType()==StaticObjectType::PLATFORM_LEFT) && object->getMoving()) {
						onMovingPlatform = true;
					}
				}
			}
			if (ballBounds.intersects(objectBounds)) {

				collision = true;
				float objectBottom = objectBounds.top + objectBounds.height;
				float objectRight = objectBounds.left + objectBounds.width;

				float topCollision = ballBottom - objectBounds.top;
				float bottomCollision = objectBottom - ballBounds.top;
				float leftCollision = ballRight - objectBounds.left;
				float rightCollision = objectRight - ballBounds.left;

				if (topCollision < leftCollision && topCollision < rightCollision) {
					//top collision
					newPosition.y = objectBounds.top - ballBounds.height;
					ball->setVelocity(sf::Vector2f(ball->getVelocity().x, (abs(ball->getVelocity().y * -.5f) < this->velocityMinY) ? 0.f : ball->getVelocity().y * -.7f));



					if (object->getObjectType() == StaticObjectType::GOAL) {
						if (!this->state->getTargetHit()) {
							this->state->setTargetHit(true);
							this->stateChanged = true;
							this->reward = WON_GAME;
							std::cout << "cilj" << std::endl;
						}
					}
				}

				else if (bottomCollision < leftCollision && bottomCollision < rightCollision) {
					//bottom collision
					newPosition.y = objectBottom;
					ball->setVelocity(sf::Vector2f(ball->getVelocity().x, -ball->getVelocity().y));
				}

				else if (leftCollision < topCollision && leftCollision < bottomCollision) {
					//left collision
					for (const StaticObject* platformObject : this->staticObjects) {
						if (platformObject->getObjectType() == StaticObjectType::PLATFORM_RIGHT && platformObject->getMoving() &&
							ballBounds.top + ballBounds.height == platformObject->getGlobalBounds().top &&
							ballBounds.left + ballBounds.width / 2 >= platformObject->getGlobalBounds().left &&
							ballBounds.left + ballBounds.width / 2 <= platformObject->getGlobalBounds().left + platformObject->getGlobalBounds().width) {
							onMovingPlatform = true;
						}
					}
					if (!onMovingPlatform) {
						newPosition.x = objectBounds.left - ballBounds.width;
						ball->setVelocity(sf::Vector2f(ball->getVelocity().x * -0.9f, ball->getVelocity().y));
					}
					else {
						newPosition.x = objectBounds.left - ballBounds.width - 1.0f;
						ball->setStoppedOnPlatform(true);
						ball->setVelocity(sf::Vector2f(0.f, 0.f));
					}

				}
				else if (rightCollision < topCollision && rightCollision < bottomCollision) {
					//left collision
					for (const StaticObject* platformObject : this->staticObjects) {
						if (platformObject->getObjectType() == StaticObjectType::PLATFORM_LEFT && platformObject->getMoving() &&
							ballBounds.top + ballBounds.height == platformObject->getGlobalBounds().top &&
							ballBounds.left + ballBounds.width / 2 >= platformObject->getGlobalBounds().left &&
							ballBounds.left + ballBounds.width / 2 <= platformObject->getGlobalBounds().left + platformObject->getGlobalBounds().width) {
							onMovingPlatform = true;
						}
					}
					if (!onMovingPlatform) {
						newPosition.x = objectRight;
						ball->setVelocity(sf::Vector2f(ball->getVelocity().x * -0.9f, ball->getVelocity().y));
					}
					else {
						newPosition.x = objectBounds.left+objectBounds.width  + 1.0f;
						ball->setStoppedOnPlatform(true);
						ball->setVelocity(sf::Vector2f(0.f, 0.f));
					}
				}
				if (object->getObjectType() == StaticObjectType::GEAR) {
					if (!this->state->getGearStarted(gears)) {
						this->state->setGearStarted(gears, true);
						this->stateChanged = true;
						std::cout << this->state->getStateId() << std::endl;
						this->reward = GEAR_ACTIVATED;
						std::cout << "zupcanik" << std::endl;
						this->startPlatform(object);
					}
				}
				if (object->getMoving() && ball->getVelocity().x == 0.f && !ball->getStoppedOnPlatform()) {
					ball->setVelocity(sf::Vector2f(object->getSpeed(), 0.f));
				}

				else {
					ball->setVelocity(sf::Vector2f(ball->getVelocity().x, ball->getVelocity().y));
				}

				ball->setPosition(newPosition);
				break;
			}
			else if (noGravity == true) {
				if (object->getMoving() && ball->getVelocity().x == 0.f && onMovingPlatform && !ball->getStoppedOnPlatform()) {
					ball->setVelocity(sf::Vector2f(object->getSpeed(), 0.f));
				}

				else {
					ball->setVelocity(sf::Vector2f(ball->getVelocity().x, ball->getVelocity().y));
				}
			}
			if (object->getObjectType() == StaticObjectType::GEAR) {
				gears++;
			}
		}

		if (newPosition.x < 0) {
			newPosition.x = 0;
			ball->setVelocity(sf::Vector2f(-0.9f * ball->getVelocity().x, ball->getVelocity().y));
		}

		else if (newPosition.x + ball->getGlobalBounds().width > this->levelWidth) {
			newPosition.x = this->levelWidth - ball->getGlobalBounds().width;
			ball->setVelocity(sf::Vector2f(-0.9f * ball->getVelocity().x, ball->getVelocity().y));
		}
		if (!collision) {
			ball->setPosition(newPosition);
			if (!onMovingPlatform)this->applyDrag(ball);
		}
		if (oldVelocity.x == 0.0f && oldVelocity.y == 0.0f && ((abs(ball->getVelocity().x) > 1) || abs(ball->getVelocity().y) > 1)) {
			this->state->setBallMoving(i, true);

		}
		else if ((abs(oldVelocity.x > 1) || abs(oldVelocity.y > 1)) && ball->getVelocity().x == 0.0f && ball->getVelocity().y == 0.0f) {
			this->state->setBallMoving(i, false);

		}


		if (!(ball->getVelocity().y == 0 && noGravity)) {
			ball->setVelocity(sf::Vector2f(ball->getVelocity().x, ball->getVelocity().y + this->gravity * deltaTime));
		}
		/*
		if (i == 1) {
			std::cout << "aa	" << ball->getVelocity().x << "	bb	" << ball->getVelocity().y <<
				"	cc	" << oldVelocity.x << "	dd	" << oldVelocity.y << std::endl;
		}
		*/

		//FORBIDDEN ACTIONS

		this->markForbiddenFromBall(ball);

		/*
		float value;
		value = ball->getGlobalBounds().top;
		int verticalCheckUp = static_cast<int>(value) ;
		value = ball->getGlobalBounds().top+ball->getGlobalBounds().height;
		int verticalCheckDown = static_cast<int>(value) ;
		value = ball->getGlobalBounds().left + ball->getGlobalBounds().width;
		int horizontalCheckRight = static_cast<int>(value);
		value = ball->getGlobalBounds().left ;
		int horizontalCheckLeft = static_cast<int>(value);
		int id = verticalCheckUp / 50 * 19 + horizontalCheckLeft / 50;
		auto result = this->forbiddenActions.insert(id);
		id = verticalCheckUp / 50 * 19 + horizontalCheckRight / 50;
		result = this->forbiddenActions.insert(id);
		id = verticalCheckDown / 50 * 19 + horizontalCheckLeft / 50;
		result = this->forbiddenActions.insert(id);
		id = verticalCheckDown / 50 * 19 + horizontalCheckRight / 50;
		*/
		/*
		if (verticalCheckUp % 10 == 0) {
			int id = (verticalCheckUp / 50) * 19 + horizontalCheckRight / 50;
			auto result = this->forbiddenActions.insert(id);
			id = (verticalCheckUp / 50) * 19 + horizontalCheckLeft / 50;
			result = this->forbiddenActions.insert(id);
		}
		if (verticalCheckDown % 10 == 0) {
			int id = (verticalCheckDown / 50) * 19 + horizontalCheckRight / 50;
			auto result = this->forbiddenActions.insert(id);
			id = (verticalCheckDown / 50) * 19 + horizontalCheckLeft / 50;
			result = this->forbiddenActions.insert(id);
		}
		if (horizontalCheckLeft % 10 == 0) {
			int id = (verticalCheckUp / 50) * 19 + horizontalCheckLeft / 50;
			auto result = this->forbiddenActions.insert(id);
			id = (verticalCheckDown / 50) * 19 + horizontalCheckLeft / 50;
			result = this->forbiddenActions.insert(id);
		}
		if (horizontalCheckRight % 10 == 0) {
			int id = (verticalCheckUp / 50) * 19 + horizontalCheckRight / 50;
			auto result = this->forbiddenActions.insert(id);
			id = (verticalCheckDown / 50) * 19 + horizontalCheckRight / 50;
			result = this->forbiddenActions.insert(id);
			if (result.second) {
				std::cout << " id " << id << " x " << horizontalCheckRight
					<< " y " << verticalCheckDown << std::endl;

				std::cout << std::endl;
			}

		}
		*/
		/*
			int id = (verticalCheck / 50) * 19 +
				horizontalCheck / 50;
			if (id < ActionRL::getGridWidth() * ActionRL::getGridHeight()) {
				auto result = this->forbiddenActions.insert(id);
				/*
				if (result.second) {
					std::cout << " id " << id << " x " << horizontalCheck
						<< " y " << verticalCheck << std::endl;
					for (int i : this->forbiddenActions) {
						std::cout << i << "		";
					}
					std::cout << std::endl;
				}
			}
			*/

	}
	if (this->state->getBallMoving() != ballsMovingPreUpdate) {
		this->stateChanged = true;
		if (!this->state->getBallMoving() && !this->state->getTargetHit()) {
			this->reward += LOST_GAME_BASE - ((this->startingNumberOfGears - this->currentNumberOfGears) + (this->startingNumberOfBelts - this->currentNumberOfBelts)) * 0.6;
		}
	}
}

void Level::markForbiddenFromBall(DynamicObject* ball)
{
	const sf::FloatRect ballBounds = ball->getGlobalBounds();
	const float ballLeft = ballBounds.left;
	const float ballTop = ballBounds.top;
	const float ballRight = ballBounds.left + ballBounds.width;
	const float ballBottom = ballBounds.top + ballBounds.height;

	int colMin = static_cast<int>(std::floor((ballLeft - (Level::GEAR_WIDTH - ballBounds.width)) / 50));
	int rowMin = static_cast<int>(std::floor((ballTop - (Level::GEAR_WIDTH - ballBounds.height)) / 50));

	int colMax = static_cast<int>(std::floor((ballRight - 1.0f) / 50));
	int rowMax = static_cast<int>(std::floor((ballBottom - 1.0f) / 50));

	colMin = std::max(0, colMin);
	rowMin = std::max(0, rowMin);
	colMax = std::min(ActionRL::getGridWidth() - 1, colMax);
	rowMax = std::min(ActionRL::getGridHeight() - 1, rowMax);

	for (int i = rowMin; i <= rowMax; i++) {
		for (int j = colMin; j <= colMax; j++) {
			int id = i * ActionRL::getGridWidth() + j;
			auto result = this->forbiddenActions.insert(id);
			/*
			if (result.second) {
				std::cout << "New forbidden " << id << std::endl;
			}
			*/
		}
	}
}

Level::Level(int numberOfBalls,int numberOfWheels,int numberOfGears,int numberOfBelts,float maxBeltDistance,int startingNumberOfBoxes)
{
	this->startingNumberOfBoxes = startingNumberOfBoxes;
	this->startingNumberOfGears = numberOfGears;
	this->startingNumberOfBelts = numberOfBelts;
	this->numberOfWheels = numberOfWheels;
	this->numberOfBalls = numberOfBalls;
	this->state = new StateRL(numberOfBalls, numberOfGears, numberOfWheels);
	this->maxBeltDistance = maxBeltDistance;
}


void Level::initLevel()
{
	this->initFont();

	this->initTextures();

	this->initStaticObjects();

	this->initStaticWheels();

	this->initDynamicObjects();
	this->initForbiddenActions();

	this->initResources();

	this->initState();

}

void Level::resetLevel()
{

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
		for (size_t i = 0; i < this->resources.size(); i++) {
			if (this->resources[i]->sprite->getGlobalBounds().contains(mousePosition) &&
				mousePosition.x > 1000 &&
				this->resourceNumbers[this->resources[i]->id] > 0) {
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
		if (this->selectedResoureceIndex == 0 && !this->activeBeltPlacement) {
			//placing belt first time
			for (const StaticWheel* wheel : this->staticWheels) {
				if (wheel->getGlobalBounds().contains(mousePosition)) {
					/*
					if belt placing starts from wheel
					updating belt start and endPointBelt variables
					makes belt follow the mouse
					*/
					if (wheel->getAttached()) {
						//impossible action, select new
						this->stateChanged = true;
					}
					this->activeBeltPlacement = true;
					this->startBeltGear = false;
					this->beltStart = sf::Vector2f(wheel->getGlobalBounds().left + wheel->getGlobalBounds().width / 2, wheel->getGlobalBounds().top + 2);
					this->endPointsBelt[0].position = this->beltStart;
					this->endPointsBelt[1].position = mousePosition;
					this->endPointsBelt[0].color = sf::Color::Yellow;
					this->endPointsBelt[1].color = sf::Color::Yellow;
					break;
				}
			}
			for (const StaticObject* object : this->staticObjects) {
				if (object->getGlobalBounds().contains(mousePosition) && object->getObjectType() == StaticObjectType::GEAR) {
					/*
					if belt placing starts from gear already placed
					updating belt start and endPointBelt variables
					makes belt follow the mouse
					*/
					this->activeBeltPlacement = true;
					this->startBeltGear = true;
					this->beltStart = sf::Vector2f(object->getGlobalBounds().left + object->getGlobalBounds().width / 8,
						object->getGlobalBounds().top + object->getGlobalBounds().height / 4);
					this->endPointsBelt[0].position = this->beltStart;
					this->endPointsBelt[1].position = mousePosition;
					this->endPointsBelt[0].color = sf::Color::Yellow;
					this->endPointsBelt[1].color = sf::Color::Yellow;
					break;
				}
			}

		}
		else if (this->selectedResoureceIndex == 0 &&
			this->activeBeltPlacement &&
			Level::distance(this->endPointsBelt[0].position, this->endPointsBelt[1].position) <= this->maxBeltDistance) {
			//placing belt second time
			std::cout << "Placed belt " << Level::distance(this->endPointsBelt[0].position, this->endPointsBelt[1].position) << std::endl;
			this->validBeltPlacement = false;
			this->startBeltGear = false;
			StaticWheel* wheelToPlace = nullptr;
			for (StaticWheel* wheel : this->staticWheels) {
				if (wheel->getGlobalBounds().contains(mousePosition)) {
					/*
					checking if belt placement is valid (ends on a wheel)
					*/
					this->validBeltPlacement = true;
					wheelToPlace = wheel;
					break;
				}
			}
			if (this->validBeltPlacement) {
				/*
				if belt placement is valid, belt positions are pushed into list
				as if it is two lines (simple graphic representation)
				*/
				this->belts.push_back({ this->beltStart,
					sf::Vector2f(
						wheelToPlace->getGlobalBounds().left + 2,
						wheelToPlace->getGlobalBounds().top + wheelToPlace->getGlobalBounds().height / 2) });
				if (this->startBeltGear) {
					this->belts.push_back({
						sf::Vector2f(
							wheelToPlace->getGlobalBounds().left + wheelToPlace->getGlobalBounds().width,
							wheelToPlace->getGlobalBounds().top + wheelToPlace->getGlobalBounds().height / 2),
						sf::Vector2f(
							this->beltStart.x + this->staticWheels[0]->getGlobalBounds().width,
							this->beltStart.y + this->staticWheels[0]->getGlobalBounds().height) });
				}
				else {
					this->belts.push_back({
						sf::Vector2f(
							wheelToPlace->getGlobalBounds().left + wheelToPlace->getGlobalBounds().width,
							wheelToPlace->getGlobalBounds().top + wheelToPlace->getGlobalBounds().height / 2),
						sf::Vector2f(
							this->beltStart.x + this->staticWheels[0]->getGlobalBounds().width / 2,
							this->beltStart.y + this->staticWheels[0]->getGlobalBounds().height / 2) });
				}
				/*
				Updating variables and UI elements
				*/
				this->activeBeltPlacement = false;
				this->resourceNumbersText[this->selectedResoureceIndex].setString(std::to_string(--this->resourceNumbers[this->selectedResoureceIndex]));
				//FORBIDDEN ACTIONS
				if (this->resourceNumbers[this->selectedResoureceIndex] == 0) {
					int numActions = this->startingNumberOfGears * Level::getNumberOfWheels() + ActionRL::combination(Level::getNumberOfWheels(), 2);
					for (int i = ActionRL::getGridHeight() * ActionRL::getGridWidth(); i < numActions; i++) {
						this->forbiddenActions.insert(i);
					}
				}
				this->clickedResource = false;
				this->selectedResoureceIndex = -1;
				this->resources.erase(this->resources.begin() + this->selectedResourceListPosition);
				this->selectedResourceListPosition = 0;

			}

		}
		else if(this->selectedResoureceIndex==1){
			/*
			placing gear, aligning it to grid first
			*/
			sf::Vector2f alignedPosition = this->alignToGridGear(mousePosition);

			this->selectedResource->setPosition(alignedPosition);
			if (this->checkOverlaping(*this->selectedResource)) {
				/*
				if it does not overlap anything, gear can be placed
				updating variables and UI elements
				*/
				if (alignedPosition.x < 900 && alignedPosition.y < 700) {
					this->staticObjects.push_back(new StaticObject(*this->selectedResource, this->selectedResoureceIndex == 1 ? StaticObjectType::GEAR : StaticObjectType::BOX));
					this->resourceNumbersText[this->selectedResoureceIndex].setString(std::to_string(--this->resourceNumbers[this->selectedResoureceIndex]));
				}
				//FORBIDDEN ACTIONS
				if (this->resourceNumbers[this->selectedResoureceIndex] == 0) {
					for (int i = 0; i < ActionRL::getGridHeight() * ActionRL::getGridWidth(); i++) {
						this->forbiddenActions.insert(i);
					}
				}
				this->clickedResource = false;
				this->selectedResoureceIndex = -1;
			}
		}
		else if (this->selectedResoureceIndex == 2) {
			sf::Vector2f alignedPosition = this->alignToGridBox(mousePosition);
			this->selectedResource->setPosition(alignedPosition);
			if (this->checkOverlaping(*this->selectedResource)) {
				/*
				if it does not overlap anything, gear can be placed
				updating variables and UI elements
				*/
				if (alignedPosition.x < 900 && alignedPosition.y < 700) {
					this->staticObjects.push_back(new StaticObject(*this->selectedResource, this->selectedResoureceIndex == 2 ? StaticObjectType::BOX : StaticObjectType::GEAR));
					this->resourceNumbersText[this->selectedResoureceIndex].setString(std::to_string(--this->resourceNumbers[this->selectedResoureceIndex]));
				}
				if (this->resourceNumbers[this->selectedResoureceIndex] == 0) {
					int gearActions = ActionRL::getGridHeight() * ActionRL::getGridWidth();
					int beltActions = this->startingNumberOfGears * this->numberOfWheels;
					beltActions += ActionRL::combination(this->numberOfWheels, 2);//belt placements between two wheels
					int boxActions = ActionRL::getGridWidthBox() * ActionRL::getGridWidthBox();
					for (int i = gearActions + beltActions; i < gearActions + beltActions + boxActions; i++) {
						this->forbiddenActions.insert(i);
					}
				}
				this->clickedResource = false;
				this->selectedResoureceIndex = -1;
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
	float beltLength = Level::distance(this->endPointsBelt[0].position, this->endPointsBelt[1].position);
	if (this->activeBeltPlacement) {
		bool overWheel = false;
		for (const StaticWheel* wheel : this->staticWheels) {
			if (wheel->getGlobalBounds().contains(this->endPointsBelt[1].position)) {
				if (beltLength <= this->maxBeltDistance) {
					this->endPointsBelt[0].color = sf::Color::Green;
					this->endPointsBelt[1].color = sf::Color::Green;
					overWheel = true;
					break;
				}
				else {
					this->endPointsBelt[0].color = sf::Color::Red;
					this->endPointsBelt[1].color = sf::Color::Red;
					overWheel = true;
					break;
				}
			}
		}
		if (!overWheel) {
			if (beltLength <= 400.f) {
				this->endPointsBelt[0].color = sf::Color::Yellow;
				this->endPointsBelt[1].color = sf::Color::Yellow;
			}
			else {
				this->endPointsBelt[0].color = sf::Color::Red;
				this->endPointsBelt[1].color = sf::Color::Red;
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
	for (size_t i = 0; i < this->belts.size(); i++) {
		sf::Vertex line[] = { this->belts[i][0],this->belts[i][1] };
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
	if (this->currentNumberOfGears == 0) {
		return false;
	}
	this->selectedResource = this->resources.at(this->currentNumberOfBelts)->sprite;

	sf::Vector2f alignedPosition = this->alignToGridGear(position);
	this->selectedResource->setPosition(alignedPosition);
	//std::cout << "x " << this->selectedResource->getPosition().x << " y " << this->selectedResource->getPosition().y << std::endl;
	if (this->checkOverlaping(*this->selectedResource)) {
		/*
		* If it does not overlap anything it places gear in appropriate place
		* Adds it to static objects on level map
		* Lowers the resources
		*/
		if (alignedPosition.x < 900 && alignedPosition.y < 700) {
			this->staticObjects.push_back(new StaticObject(*this->selectedResource, StaticObjectType::GEAR));
			this->resourceNumbersText[1].setString(std::to_string(--this->resourceNumbers[1]));
			this->resources.erase(this->resources.begin()+this->currentNumberOfBelts);
			if (--this->currentNumberOfGears == 0) {
				int numActions = this->startingNumberOfGears * Level::getNumberOfWheels() + ActionRL::combination(Level::getNumberOfWheels(), 2);
				for (int i = 0; i < ActionRL::getGridWidth() * ActionRL::getGridHeight(); i++) {
					this->forbiddenActions.insert(i);
				}
			}
		}
		std::cout << "Stavljen zupcanik na poziciju X: " << this->selectedResource->getGlobalBounds().left <<
			" Y: " << this->selectedResource->getGlobalBounds().top << std::endl;
						
		return true;
	}
	else {
		this->selectedResource->setPosition(1050, 350);
		return false;
	}
}

bool Level::tryBoxPlacement(sf::Vector2f position)
{
	/*
	Selects box from resources
	Aligns it to the grid
	*/
	if (this->currentNumberOfBoxes == 0) {
		return false;
	}
	this->selectedResource = this->resources.back()->sprite;

	sf::Vector2f alignedPosition = this->alignToGridBox(position);
	this->selectedResource->setPosition(alignedPosition);
	//std::cout << "x " << this->selectedResource->getPosition().x << " y " << this->selectedResource->getPosition().y << std::endl;
	if (this->checkOverlaping(*this->selectedResource)) {
		/*
		* If it does not overlap anything it places gear in appropriate place
		* Adds it to static objects on level map
		* Lowers the resources
		*/
		if (alignedPosition.x <= 900 && alignedPosition.y <= 700) {
			this->staticObjects.push_back(new StaticObject(*this->selectedResource, StaticObjectType::BOX));
			this->resourceNumbersText[2].setString(std::to_string(--this->resourceNumbers[2]));
			this->resources.erase(this->resources.begin() + this->currentNumberOfBelts+this->currentNumberOfGears);
			if (--this->currentNumberOfBoxes == 0) {
				int numActions = this->startingNumberOfGears * Level::getNumberOfWheels() + ActionRL::combination(Level::getNumberOfWheels(), 2);
				numActions += ActionRL::getGridWidth() * ActionRL::getGridHeight();
				for (int i = numActions; i < numActions+ActionRL::getGridWidthBox() * ActionRL::getGridHeightBox(); i++) {
					this->forbiddenActions.insert(i);
				}
			}
		}
		std::cout << "Stavljena kutija na poziciju X: " << this->selectedResource->getGlobalBounds().left <<
			" Y: " << this->selectedResource->getGlobalBounds().top << std::endl;
			
		return true;
	}
	else {
		this->selectedResource->setPosition(1050, 550);
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

sf::Vector2f Level::getGearLocation(int gearId)
{
	int g = 0;
	for (int i = 0; i < this->staticObjects.size(); i++) {
		if (this->staticObjects[i]->getObjectType() == StaticObjectType::GEAR) {
			if (g == gearId) {
				if (this->staticObjects[i]->getAttached()) {
					//already placed
					std::cout << "ATTACHED" << std::endl;
					return sf::Vector2f(-2.0, -2.0);
				}
				return this->staticObjects[i]->getGlobalBounds().getPosition();
			}
			g++;
		}
	}
	return sf::Vector2f(-1.0, -1.0);
}

sf::Vector2f Level::getWheelLocation(int wheelId)
{
	if (this->staticWheels[wheelId]->getAttached()) {
		std::cout << "ATTACHED WHEEL" << std::endl;
		return sf::Vector2f(-2.0, -2.0);
	}
	return this->staticWheels[wheelId]->getGlobalBounds().getPosition();
}

double Level::getReward()
{
	float reward = this->reward;
	this->reward = 0;
	return reward;
}

float Level::getMaxBeltDistance()
{
	return this->maxBeltDistance;
}

int Level::getNumberOfBelts()
{
	return this->resourceNumbers[0];
}

int Level::getNumberOfWheels()
{
	return this->numberOfWheels;
}

int Level::getNumberOfBalls()
{
	return this->numberOfBalls;
}

int Level::getNumberOfBoxes()
{
	return this->currentNumberOfBoxes;
}

std::unordered_set<int> Level::getBallZonesPassed()
{
	return this->forbiddenActions;
}

bool Level::placeBelt(sf::Vector2f start, sf::Vector2f end, bool startBeltGear)
{
	/*
	* Adds belt into array (start and end vectors)
	* Lowers the resources
	* If gear attached to belt is started, it starts the platform
	* */
	/*
	* sf::Vector2f(object->getGlobalBounds().left+ object->getGlobalBounds().width/8,
						object->getGlobalBounds().top+object->getGlobalBounds().height/4);
	* this->belts.push_back({ this->beltStart,
					sf::Vector2f(
						wheelToPlace->getGlobalBounds().left+2,
						wheelToPlace->getGlobalBounds().top+wheelToPlace->getGlobalBounds().height/2)});
				if (this->startBeltGear) {
					this->belts.push_back({
						sf::Vector2f(
							wheelToPlace->getGlobalBounds().left + wheelToPlace->getGlobalBounds().width,
							wheelToPlace->getGlobalBounds().top + wheelToPlace->getGlobalBounds().height/2),
						sf::Vector2f(
							this->beltStart.x + this->staticWheels[0]->getGlobalBounds().width,
							this->beltStart.y + this->staticWheels[0]->getGlobalBounds().height) });
				}
				else {
					this->belts.push_back({
						sf::Vector2f(
							wheelToPlace->getGlobalBounds().left + wheelToPlace->getGlobalBounds().width,
							wheelToPlace->getGlobalBounds().top + wheelToPlace->getGlobalBounds().height / 2),
						sf::Vector2f(
							this->beltStart.x + this->staticWheels[0]->getGlobalBounds().width/2,
							this->beltStart.y + this->staticWheels[0]->getGlobalBounds().height/2) });
				}
	*/

	if (this->currentNumberOfBelts == 0) {
		return false;
	}
	if (startBeltGear) {
		/*
		this->belts.push_back({ sf::Vector2f(start.x + 100 / 8,start.y + 100 / 4),
			sf::Vector2f(end.x + 2,end.y + this->staticWheels[0]->getGlobalBounds().height / 2) });
			this->beltStart = sf::Vector2f(object->getGlobalBounds().left+ object->getGlobalBounds().width/8,
						object->getGlobalBounds().top+object->getGlobalBounds().height/4);
		*/
		sf::Vector2f beltStart1 = sf::Vector2f(start.x + 100 / 8, start.y + 100 / 4);
		sf::Vector2f beltEnd1 = sf::Vector2f(end.x + 2, end.y + this->staticWheels[0]->getGlobalBounds().height / 2);
		sf::Vector2f beltStart2 = sf::Vector2f(end.x + this->staticWheels[0]->getGlobalBounds().width, end.y + this->staticWheels[0]->getGlobalBounds().height / 2);
		sf::Vector2f beltEnd2 = sf::Vector2f(beltStart1.x + this->staticWheels[0]->getGlobalBounds().width, beltStart1.y + this->staticWheels[0]->getGlobalBounds().height / 2);
		this->belts.push_back({ beltStart1, beltEnd1 });
		this->belts.push_back({ beltStart2,beltEnd2 });
		int gears = 0;
		for (StaticObject* object : this->staticObjects) {
			if (object->getObjectType() == StaticObjectType::GEAR) {
				//maybe start and end
				if (object->getGlobalBounds().contains(start)) {
					if (this->state->getGearStarted(gears)) {
						object->setAttached(true);
						this->startPlatform(object);
					}
				}
				gears++;
			}
		}
	}
	else {
		//this->beltStart = sf::Vector2f(wheel->getGlobalBounds().left+wheel->getGlobalBounds().width/2, wheel->getGlobalBounds().top+2);
		sf::Vector2f beltStart1 = sf::Vector2f(start.x + this->staticWheels[0]->getGlobalBounds().width / 2, start.y + 2);
		sf::Vector2f beltEnd1 = sf::Vector2f(end.x + 2, end.y + this->staticWheels[0]->getGlobalBounds().height / 2);
		sf::Vector2f beltStart2 = sf::Vector2f(end.x + this->staticWheels[0]->getGlobalBounds().width, end.y + this->staticWheels[0]->getGlobalBounds().height / 2);
		sf::Vector2f beltEnd2 = sf::Vector2f(beltStart1.x + this->staticWheels[0]->getGlobalBounds().width / 2, beltStart1.y + this->staticWheels[0]->getGlobalBounds().height / 2);
		this->belts.push_back({ beltStart1,beltEnd1 });
		this->belts.push_back({ beltStart2,beltEnd2 });
	}
	this->activeBeltPlacement = false;
	this->resourceNumbersText[0].setString(std::to_string(--this->resourceNumbers[0]));
	this->resources.erase(this->resources.begin());
	this->currentNumberOfBelts--;
	for (StaticWheel* object : this->staticWheels) {
		if (object->getGlobalBounds().contains(start) || object->getGlobalBounds().contains(end)) {
			object->setAttached(true);
		}
	}
	return true;
}

StateRL* Level::getStatusChange()
{
	return this->state;
}

bool Level::getStateChanged()
{
	bool retVal = this->stateChanged;
	this->stateChanged = false;
	return retVal;
}

float Level::distance(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return std::sqrt((b.x - a.x) * (b.x - a.x) +
		(b.y - a.y) * (b.y - a.y));
}

int Level::getStartingNumberOfGears()
{
	return this->startingNumberOfGears;
}

int Level::getStartingNumberOfBelts()
{
	return this->startingNumberOfBelts;
}

int Level::getStartingNumberOfBoxes()
{
	return this->startingNumberOfBoxes;
}


const sf::FloatRect& Level::getBouds() const
{
	return sf::FloatRect(0, 0, 1000, 800);
}

void Level::setIsPlaying(bool playing)
{
	this->isPlaying = playing;
}


Level::~Level()
{
	for (size_t i = 0; i < this->staticObjects.size(); i++) {
		delete this->staticObjects[i];
	}
	for (size_t i = 0; i < this->resources.size(); i++) {
		delete this->resources[i]->sprite;
		delete this->resources[i];
	}
	for (size_t i = 0; i < this->dynamicObjects.size(); i++) {
		delete this->dynamicObjects[i];
	}
	for (size_t i = 0; i < this->staticWheels.size(); i++) {
		delete this->staticWheels[i];
	}
	delete this->state;
}
