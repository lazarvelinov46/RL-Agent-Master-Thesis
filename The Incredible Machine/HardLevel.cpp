#include "stdafx.h"
#include "HardLevel.h"

void HardLevel::initState()
{
}

void HardLevel::initStaticObjects()
{
	this->staticObjects.push_back(new StaticObject(0, 760, 1000, 50, StaticObjectType::FLOOR));

	this->staticObjects.push_back(new StaticObject(101, 650, 148, 20, StaticObjectType::PLATFORM_RIGHT));
	this->staticObjects.push_back(new StaticObject(451, 500, 148, 20, StaticObjectType::PLATFORM_LEFT));
	this->staticObjects.push_back(new StaticObject(201, 300, 148, 20, StaticObjectType::PLATFORM_RIGHT));
	this->staticObjects.push_back(new StaticObject(501, 200, 148, 20, StaticObjectType::PLATFORM_RIGHT));

	this->staticObjects.push_back(new StaticObject(830, 300, 20, 40, StaticObjectType::WALL));
	this->staticObjects.push_back(new StaticObject(830, 340, 160, 20, StaticObjectType::GOAL));
	this->staticObjects.push_back(new StaticObject(970, 300, 20, 40, StaticObjectType::WALL));
}

void HardLevel::initStaticWheels()
{
	this->staticWheels.push_back(new StaticWheel(200, 660.f));
	this->staticWheels.push_back(new StaticWheel(475, 510.f));
	this->staticWheels.push_back(new StaticWheel(300, 310.f));
	this->staticWheels.push_back(new StaticWheel(600, 210.f));
}

void HardLevel::initDynamicObjects()
{
	this->dynamicObjects.push_back(new DynamicObject(25, 470, false));
	this->dynamicObjects.push_back(new DynamicObject(100, 600, false));
	this->dynamicObjects.push_back(new DynamicObject(540, 450, false));
	this->dynamicObjects.push_back(new DynamicObject(200, 250, false));
	this->dynamicObjects.push_back(new DynamicObject(500, 150, true));
}

void HardLevel::initForbiddenActions()
{
}

void HardLevel::initResources()
{
	this->currentNumberOfBelts = this->startingNumberOfBelts;
	this->currentNumberOfGears = this->startingNumberOfGears;
	this->currentNumberOfResources = 0;
	for (size_t i = 0; i < this->currentNumberOfBelts; i++) {
		sf::Sprite* beltSprite = new sf::Sprite(this->beltTexture);
		beltSprite->setScale(0.5f, 0.5f);
		beltSprite->setPosition(1050, 150);

		this->resources.push_back(new Resource(beltSprite, this->currentNumberOfResources, this->currentNumberOfBelts));
	}
	this->currentNumberOfResources++;
	for (size_t i = 0; i < this->currentNumberOfGears; i++) {
		sf::Sprite* gearSprite = new sf::Sprite(this->gearTexture);
		gearSprite->setScale(0.19531f, 0.19531f);
		gearSprite->setPosition(1050, 350);
		sf::Text gearText;
		gearText.setFont(font);
		gearText.setString("3");
		gearText.setCharacterSize(18);
		gearText.setPosition(gearSprite->getPosition().x + gearSprite->getGlobalBounds().width / 2 - 18, gearSprite->getPosition().y + gearSprite->getGlobalBounds().height);
		gearText.setFillColor(sf::Color::Black);

		this->resources.push_back(new Resource(gearSprite, this->currentNumberOfResources, this->currentNumberOfGears));
	}
	this->currentNumberOfResources++;
	for (size_t i = 0; i < this->currentNumberOfResources; i++) {
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

HardLevel::HardLevel(bool mode) :Level(5, 4,4,4,250.f)
{
	this->modeAI = mode;
}

HardLevel::~HardLevel()
{
}
