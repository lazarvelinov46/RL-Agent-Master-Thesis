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

Level::Level()
{
}

void Level::initLevel()
{
	this->initFont();

	this->initTextures();

	this->initStaticObjects();

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
		sf::Vector2f alignedPosition = this->alignToGrid(mousePosition);
		if (alignedPosition.x < 900 && alignedPosition.y < 700) {
			this->selectedResource->setPosition(alignedPosition);
			this->staticObjects.push_back(new StaticObject(*this->selectedResource));
			this->resourceNumbersText[this->selectedResoureceIndex].setString(std::to_string(--this->resourceNumbers[this->selectedResoureceIndex]));
		}
		this->clickedResource = false;
		this->selectedResoureceIndex = -1;
		std::cout << "STAVLJENO " << this->selectedResource->getGlobalBounds().left << this->selectedResource->getGlobalBounds().top
			<< this->selectedResource->getGlobalBounds().height << this->selectedResource->getGlobalBounds().width << std::endl;
		
	}
}

void Level::update()
{

}

void Level::render(sf::RenderTarget& target)
{
	
	for (StaticObject* object : this->staticObjects) {
		object->render(target);
	}
	for (Resource* res : this->resources) {
		target.draw(*res->sprite);
	}
	for (sf::Text t : this->resourceNumbersText) {
		target.draw(t);
	}
	if (this->clickedResource) {
		target.draw(*this->selectedResource);
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
}
