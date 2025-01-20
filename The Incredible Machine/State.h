#pragma once

#include "stdafx.h"

class State
{
private:
public:
	virtual ~State()=default;

	virtual void handleInput(sf::RenderWindow& window)=0;
	virtual void update()=0;
	virtual void render(sf::RenderTarget& target)=0;
};

