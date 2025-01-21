#pragma once
#include "State.h"
#include "Level.h"
#include "Menu.h"
class Game : public State
{
private:
	Level level;
	State* nextState;
public:
	Game();
	void handleInput(sf::RenderWindow& window)override;
	void update()override;
	void render(sf::RenderTarget& target)override;

	State* getNextState()override;
};

