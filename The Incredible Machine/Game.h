#pragma once
#include "State.h"
#include "Level.h"
#include "Menu.h"

/*

*/
class Game : public State
{
private:
	sf::RectangleShape panel;
	sf::RectangleShape playButton;
	sf::Font font;
	sf::Text playButtonText;

	Level level;
	State* nextState;

	void initFont();
	void initText();
	void initPanel();
public:
	Game();
	void handleInput(sf::RenderWindow& window)override;
	void update()override;
	void render(sf::RenderTarget& target)override;

	State* getNextState()override;
};

