#include "stdafx.h"
#include "GameAI.h"

void GameAI::initFont()
{
	if (!font.loadFromFile("assets/Fonts/ARIAL.TTF")) {
		std::cout << "ERROR: Could not load font" << std::endl;
	}
}

void GameAI::initText()
{
	this->playButtonText.setFont(font);
	this->playButtonText.setString("PLAY");
	this->playButtonText.setCharacterSize(24);
	this->playButtonText.setPosition(1050, 50);

}

void GameAI::initPanel()
{
	this->panel.setSize(sf::Vector2f(200, 800));
	this->panel.setPosition(sf::Vector2f(1000, 0));
	this->panel.setFillColor(sf::Color::White);

	this->playButton.setSize(sf::Vector2f(100, 50));
	this->playButton.setPosition(sf::Vector2f(1050, 50));
	this->playButton.setFillColor(sf::Color::Green);
}

void GameAI::initGameScreen()
{
	this->gameScreen.setSize(sf::Vector2f(1000, 800));
	this->gameScreen.setPosition(sf::Vector2f(0, 0));
	this->gameScreen.setFillColor(sf::Color(128, 128, 128));
}

GameAI::GameAI()
{
	this->initFont();
	this->initText();
	this->initGameScreen();
	this->initPanel();
	this->level.initLevel();
	this->nextState = nullptr;
}

void GameAI::handleInput(sf::RenderWindow& window)
{
	while (!this->isPlaying) {
		level.handleInput(window);
		if (ev.type == sf::Event::Closed) {
			window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			this->nextState = new Menu();
		}

		if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2f position(ev.mouseButton.x, ev.mouseButton.y);

			if (playButton.getGlobalBounds().contains(position)) {
				std::cout << "PLAY pressed " << std::endl;
				this->isPlaying = true;
				level.setIsPlaying(this->isPlaying);
			}

			level.handleClick(position);
		}
		if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Right) {
			level.handleRightClick();
		}
	}
	//handle input on level side
}

void GameAI::update(float deltaTime)
{
	this->level.update(deltaTime);
}

void GameAI::render(sf::RenderTarget& target)
{
	target.clear(sf::Color::White);
	target.draw(this->panel);
	target.draw(this->gameScreen);
	this->level.render(target);
	target.draw(this->playButton);
	target.draw(this->playButtonText);
}

State* GameAI::getNextState()
{
	return this->nextState;
}
