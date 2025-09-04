#include "stdafx.h"
#include "EasyLevel.h"

void EasyLevel::initState()
{
}

void EasyLevel::initStaticObjects()
{
	this->staticObjects.push_back(new StaticObject(0, 760, 1000, 50, StaticObjectType::FLOOR));

	this->staticObjects.push_back(new StaticObject(100, 600, 150, 20, StaticObjectType::PLATFORM));
	this->staticObjects.push_back(new StaticObject(600, 300, 150, 20, StaticObjectType::PLATFORM));

	this->staticObjects.push_back(new StaticObject(830, 400, 20, 40, StaticObjectType::WALL));
	this->staticObjects.push_back(new StaticObject(830, 440, 160, 20, StaticObjectType::GOAL));
	this->staticObjects.push_back(new StaticObject(970, 400, 20, 40, StaticObjectType::WALL));
}

void EasyLevel::initStaticWheels()
{
	this->staticWheels.push_back(new StaticWheel(200, 610.f));
	this->staticWheels.push_back(new StaticWheel(700, 310.f));
}

void EasyLevel::initDynamicObjects()
{
	this->dynamicObjects.push_back(new DynamicObject(25, 470, false));
	this->dynamicObjects.push_back(new DynamicObject(100, 550, false));
	this->dynamicObjects.push_back(new DynamicObject(600, 250, true));
}

void EasyLevel::initForbiddenActions()
{
}

void EasyLevel::initResources()
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

EasyLevel::EasyLevel(bool mode) :Level(3, 2, 2, 2)
{
	this->modeAI = mode;
}

EasyLevel::~EasyLevel()
{
}
