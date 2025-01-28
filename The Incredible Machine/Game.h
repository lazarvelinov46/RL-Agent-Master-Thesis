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
	sf::RectangleShape gameScreen;

	Level level;
	State* nextState;

	bool isPlaying = false;

	void initFont();
	void initText();
	void initPanel();
	void initGameScreen();
public:
	Game();
	void handleInput(sf::RenderWindow& window)override;
	void update(float deltaTime=0)override;
	void render(sf::RenderTarget& target)override;

	State* getNextState()override;
};

