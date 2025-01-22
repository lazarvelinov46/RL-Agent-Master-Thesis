#include "stdafx.h"
#include "Game.h"

void Game::initFont()
{
	if (!font.loadFromFile("assets/Fonts/ARIAL.TTF")) {
		std::cout << "ERROR: Could not load font" << std::endl;
	}
}

void Game::initText()
{
	this->playButtonText.setFont(font);
	this->playButtonText.setString("PLAY");
	this->playButtonText.setCharacterSize(24);
	this->playButtonText.setPosition(975, 50);

}

void Game::initPanel()
{
	this->panel.setSize(sf::Vector2f(200, 800));
	this->panel.setPosition(sf::Vector2f(1000, 0));
	this->panel.setFillColor(sf::Color::White);

	this->playButton.setSize(sf::Vector2f(200, 50));
	this->playButton.setPosition(sf::Vector2f(950, 50));
	this->playButton.setFillColor(sf::Color::Green);
}

Game::Game()
{
	this->initFont();
	this->initText();
	this->initPanel();
	this->level.initLevel();
	this->nextState = nullptr;
}

void Game::handleInput(sf::RenderWindow& window)
{
	sf::Event ev;
	while (window.pollEvent(ev)) {
		if (ev.type == sf::Event::Closed) {
			window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			this->nextState = new Menu();
		}
	}
}

void Game::update()
{
	//dynamic objects
}

void Game::render(sf::RenderTarget& target)
{
	target.clear();
	this->level.render(target);
	target.draw(this->panel);
	target.draw(this->playButton);
	target.draw(this->playButtonText);
}

State* Game::getNextState()
{
	return nullptr;
}
