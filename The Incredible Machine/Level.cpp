#include "stdafx.h"
#include "Level.h"

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
	this->staticObjects.push_back(new StaticObject(0, 760, 1200, 40, sf::Color::Red,StaticObjectType::PLATFORM));

	this->staticObjects.push_back(new StaticObject(100, 600, 150, 20, sf::Color::Blue, StaticObjectType::PLATFORM));
	this->staticObjects.push_back(new StaticObject(300, 500, 150, 20, sf::Color::Blue, StaticObjectType::PLATFORM));
	this->staticObjects.push_back(new StaticObject(600, 300, 150, 20, sf::Color::Blue, StaticObjectType::PLATFORM));

	this->staticObjects.push_back(new StaticObject(850, 400, 20, 40, sf::Color::Green, StaticObjectType::PLATFORM));
	this->staticObjects.push_back(new StaticObject(850, 440, 140, 20, sf::Color::Green, StaticObjectType::PLATFORM));
	this->staticObjects.push_back(new StaticObject(970, 400, 20, 40, sf::Color::Green, StaticObjectType::PLATFORM));
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
		gearSprite->setScale(0.2f, 0.2f);
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
		if (newObject.getGlobalBounds().intersects(this->dynamicObjects[i]->getGlobalBounds())) {
			return false;
		}
	}
	return true;
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
	float x = (float)(pos.x - (int)pos.x % 20);
	float y = (float)(pos.y - (int)pos.y % 20);
	return sf::Vector2f(x, y);
}

void Level::updateBalls(float deltaTime)
{
	for (size_t i = 0; i < this->dynamicObjects.size(); i++) {
		this->dynamicObjects[i]->setVelocity(sf::Vector2f(this->dynamicObjects[i]->getVelocity().x,
			this->dynamicObjects[i]->getVelocity().y + this->gravity*deltaTime));

		sf::Vector2f newPosition = this->dynamicObjects[i]->getPosition()+this->dynamicObjects[i]->getVelocity();

		bool collision = false;
		for (const StaticObject* object : this->staticObjects) {
			if (this->dynamicObjects[i]->getShape().getGlobalBounds().intersects(object->getGlobalBounds())) {
				collision = true;

				float staticObjectY = object->getGlobalBounds().top;
				float ballHeight = this->dynamicObjects[i]->getShape().getGlobalBounds().height;

				newPosition.y = staticObjectY - ballHeight;

				this->dynamicObjects[i]->setVelocity(sf::Vector2f(this->dynamicObjects[i]->getVelocity().x, 0.f));
				this->dynamicObjects[i]->setPosition(newPosition);
				break;
			}
		}
		if (!collision) {
			this->dynamicObjects[i]->setPosition(newPosition);
		}
	}
}

Level::Level()
{
}

void Level::initLevel()
{
	this->initFont();

	this->initTextures();

	this->initStaticObjects();

	this->initStaticWheels();

	this->initDynamicObjects();

	this->initResources();
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
		for (size_t i = 0;i < this->resources.size();i++) {
			if (this->resources[i]->sprite->getGlobalBounds().contains(mousePosition) && 
				mousePosition.x>1000 && 
				this->resourceNumbers[this->resources[i]->id]>0) {
				this->selectedResource = this->resources[i]->sprite;
				this->clickedResource = true;
				this->selectedResoureceIndex = this->resources[i]->id;
				break;
			}
		}
	}
	else {
		if (this->selectedResoureceIndex == 0&&!this->activeBeltPlacement) {
			//placing belt first time
			
			for (const StaticWheel* wheel : this->staticWheels) {
				if (wheel->getGlobalBounds().contains(mousePosition)) {
					this->activeBeltPlacement = true;
					this->beltStart = sf::Vector2f(wheel->getGlobalBounds().left, wheel->getGlobalBounds().top);
					this->endPointsBelt[0].position = this->beltStart;
					this->endPointsBelt[1].position = mousePosition;
					this->endPointsBelt[0].color = sf::Color::Yellow;
					this->endPointsBelt[1].color = sf::Color::Yellow;
					std::cout << "jea" << std::endl;
					break;
				}
			}
			for (const StaticObject* object : this->staticObjects) {
				if (object->getGlobalBounds().contains(mousePosition)&&object->getObjectType()==StaticObjectType::GEAR) {
					this->activeBeltPlacement = true;
					this->beltStart = sf::Vector2f(object->getGlobalBounds().left, object->getGlobalBounds().top);
					this->endPointsBelt[0].position = this->beltStart;
					this->endPointsBelt[1].position = mousePosition;
					this->endPointsBelt[0].color = sf::Color::Yellow;
					this->endPointsBelt[1].color = sf::Color::Yellow;
					std::cout << "jea" << std::endl;
					break;
				}
			}

		}
		else if (this->selectedResoureceIndex == 0 && this->activeBeltPlacement) {
			//placing belt second time
			this->validBeltPlacement = false;
			for (const StaticWheel* wheel : this->staticWheels) {
				if (wheel->getGlobalBounds().contains(mousePosition)) {
					this->endPointsBelt[0].color = sf::Color::Green;
					this->endPointsBelt[1].color = sf::Color::Green;
					std::cout << "jea" << std::endl;
					break;
				}
			}
			for (const StaticObject* object : this->staticObjects) {
				if (object->getGlobalBounds().contains(mousePosition) && object->getObjectType() == StaticObjectType::GEAR) {
					this->activeBeltPlacement = true;
					this->beltStart = sf::Vector2f(object->getGlobalBounds().left, object->getGlobalBounds().top);
					this->endPointsBelt[0].position = this->beltStart;
					this->endPointsBelt[1].position = mousePosition;
					this->endPointsBelt[0].color = sf::Color::Yellow;
					this->endPointsBelt[1].color = sf::Color::Yellow;
					std::cout << "jea" << std::endl;
					break;
				}
			}

		}
		else {
			sf::Vector2f alignedPosition = this->alignToGrid(mousePosition);

			this->selectedResource->setPosition(alignedPosition);
			if (this->checkOverlaping(*this->selectedResource)) {

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

void Level::update(float deltaTime)
{
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
	for (StaticObject* object : this->staticObjects) {
		object->render(target);
	}
	for (StaticWheel* object : this->staticWheels) {
		object->render(target);
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
	if (this->clickedResource) {
		target.draw(*this->selectedResource);
	}
	if (this->activeBeltPlacement) {
		sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Mouse::getPosition(*dynamic_cast<sf::RenderWindow*>(&target)));
		this->endPointsBelt[1].position = mousePosition;
		target.draw(this->endPointsBelt, 2, sf::Lines);
	}
}

const sf::FloatRect& Level::getBouds() const
{
	return sf::FloatRect(0,0,1000,800);
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
