#include "stdafx.h"
#include "Game.h"

Game::Game()
{
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
}

State* Game::getNextState()
{
	return nullptr;
}
