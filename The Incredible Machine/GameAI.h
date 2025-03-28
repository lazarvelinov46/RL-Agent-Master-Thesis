#pragma once

#include "State.h"
#include "Level.h"
#include "Menu.h"
#include "AgentRL.h"
#include "QTable.h"

class GameAI : public State
{
private:
	sf::RectangleShape panel;
	sf::RectangleShape playButton;
	sf::Font font;
	sf::Text playButtonText;
	sf::RectangleShape gameScreen;

	Level level;
	State* nextState;
	AgentRL* agent;

	bool isPlaying = false;

	int stateId;
	int nextStateId;
	int actionId;
	QTable table;

	void initFont();
	void initText();
	void initPanel();
	void initGameScreen();

	void selectAction();
	void updateState();
public:
	GameAI();
	~GameAI();
	void handleInput(sf::RenderWindow& window)override;
	void update(float deltaTime = 0)override;
	void render(sf::RenderTarget& target)override;

	State* getNextState()override;
};

