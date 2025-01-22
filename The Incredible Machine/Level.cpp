#include "stdafx.h"
#include "Level.h"

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
	this->staticObjects.push_back(new StaticObject(0, 760, 1200, 40, sf::Color::Red));

	this->staticObjects.push_back(new StaticObject(100, 600, 150, 20, sf::Color::Blue));
	this->staticObjects.push_back(new StaticObject(300, 500, 150, 20, sf::Color::Blue));
	this->staticObjects.push_back(new StaticObject(600, 300, 150, 20, sf::Color::Blue));

	this->staticObjects.push_back(new StaticObject(850, 400, 20, 40, sf::Color::Green));
	this->staticObjects.push_back(new StaticObject(850, 440, 140, 20, sf::Color::Green));
	this->staticObjects.push_back(new StaticObject(970, 400, 20, 40, sf::Color::Green));
}

void Level::initResources()
{
	sf::Sprite* beltSprite = new sf::Sprite(this->beltTexture);
	beltSprite->setScale(0.5, 0.5);
	beltSprite->setPosition(1050, 150);
	sf::Sprite* gearSprite = new sf::Sprite(this->gearTexture);
	gearSprite->setScale(0.2, 0.2);
	gearSprite->setPosition(1050, 250);
	this->resources.push_back(beltSprite);
	this->resources.push_back(gearSprite);
}

Level::Level()
{
	this->initTextures();
}

void Level::initLevel()
{
	this->staticObjects.push_back(new StaticObject(0, 760, 1200, 40, sf::Color::Red));

	this->staticObjects.push_back(new StaticObject(100, 600, 150, 20, sf::Color::Blue));
	this->staticObjects.push_back(new StaticObject(300, 500, 150, 20, sf::Color::Blue));
	this->staticObjects.push_back(new StaticObject(600, 300, 150, 20, sf::Color::Blue));

	this->staticObjects.push_back(new StaticObject(850, 400, 20, 40, sf::Color::Green));
	this->staticObjects.push_back(new StaticObject(850, 440, 140, 20, sf::Color::Green));
	this->staticObjects.push_back(new StaticObject(970, 400, 20, 40, sf::Color::Green));

	this->initResources();
}

void Level::render(sf::RenderTarget& target)
{
	for (StaticObject* object : this->staticObjects) {
		object->render(target);
	}
	for (sf::Sprite* sprite : this->resources) {
		target.draw(*sprite);
	}
}

Level::~Level()
{
	for (size_t i = 0;i < this->staticObjects.size();i++) {
		delete this->staticObjects[i];
	}
	for (size_t i = 0; i < this->resources.size(); i++) {
		delete this->resources[i];
	}
}
