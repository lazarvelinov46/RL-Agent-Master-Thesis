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
	this->playButtonText.setPosition(1050, 50);

}

void Game::initPanel()
{
	this->panel.setSize(sf::Vector2f(200, 800));
	this->panel.setPosition(sf::Vector2f(1000, 0));
	this->panel.setFillColor(sf::Color::White);

	this->playButton.setSize(sf::Vector2f(100, 50));
	this->playButton.setPosition(sf::Vector2f(1050, 50));
	this->playButton.setFillColor(sf::Color::Green);
}

void Game::initGameScreen()
{
	this->gameScreen.setSize(sf::Vector2f(1000, 800));
	this->gameScreen.setPosition(sf::Vector2f(0, 0));
	this->gameScreen.setFillColor(sf::Color(128,128,128));
}

Game::Game()
{
	this->initFont();
	this->initText();
	this->initGameScreen();
	this->initPanel();
	this->level.initLevel();
	this->nextState = nullptr;
}

void Game::handleInput(sf::RenderWindow& window)
{
	sf::Event ev;
	while (window.pollEvent(ev)) {

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
			}

			level.handleClick(position);
		}
	}
	//handle input on level side
}

void Game::update(float deltaTime)
{
	this->level.update(deltaTime);
}

void Game::render(sf::RenderTarget& target)
{
	target.clear();
	target.draw(this->panel);
	target.draw(this->gameScreen);
	this->level.render(target);
	target.draw(this->playButton);
	target.draw(this->playButtonText);
}

State* Game::getNextState()
{
	return this->nextState;
}
