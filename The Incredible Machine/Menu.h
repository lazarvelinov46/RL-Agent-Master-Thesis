#pragma once

#include "Game.h"
#include "GameAI.h"


enum MenuOption{PLAYER,AI,LEVEL,QUIT};


class Menu : public State
{
private:
	sf::Font font;
	sf::Text title;
	std::vector<sf::Text> options;
	MenuOption selectedOption;
	State* nextState;

	void initFont();
	void initText();
public:
	Menu();
	//getters
	const MenuOption getSelectedOption()const { return this->selectedOption; }
	void handleInput(sf::RenderWindow& window)override;
	void update(float deltaTime=0)override;
	void render(sf::RenderTarget& target)override;
	State* getNextState()override;
};

