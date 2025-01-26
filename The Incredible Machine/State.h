#pragma once

#include "stdafx.h"

class State
{
private:
public:
	virtual ~State()=default;

	virtual void handleInput(sf::RenderWindow& window)=0;
	virtual void update(float deltaTime=0)=0;
	virtual void render(sf::RenderTarget& target)=0;

	virtual State* getNextState() = 0;
};

