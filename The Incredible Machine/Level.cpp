#include "stdafx.h"
#include "Level.h"

Level::Level()
{
}

void Level::initLevel()
{
	this->staticObjects.push_back(new StaticObject(0, 760, 1200, 40, sf::Color::Red));

	this->staticObjects.push_back(new StaticObject(100, 600, 150, 20, sf::Color::Blue));
	this->staticObjects.push_back(new StaticObject(300, 500, 150, 20, sf::Color::Blue));
	this->staticObjects.push_back(new StaticObject(600, 300, 150, 20, sf::Color::Blue));

	this->staticObjects.push_back(new StaticObject(800, 100, 20, 200, sf::Color::Red));
	this->staticObjects.push_back(new StaticObject(820, 100, 80, 20, sf::Color::Red));
}

void Level::render(sf::RenderTarget& target)
{
	for (StaticObject* object : this->staticObjects) {
		object->render(target);
	}
}

Level::~Level()
{
	for (size_t i = 0;i < this->staticObjects.size();i++) {
		delete this->staticObjects[i];
	}
}
