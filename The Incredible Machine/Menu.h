#pragma once

#include "State.h"


enum MenuOption{PLAYER,AI,LEVEL,QUIT};


class Menu : public State
{
private:
	sf::Font font;
	sf::Text title;
	std::vector<sf::Text> options;
	MenuOption selectedOption;

	void initFont();
	void initText();
public:
	Menu();
	//getters
	const MenuOption getSelectedOption()const { return this->selectedOption; }
	void handleInput(sf::RenderWindow& window)override;
	void update()override;
	void render(sf::RenderTarget& target)override;
};

