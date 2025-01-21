#include "stdafx.h"
#include "Menu.h"

void Menu::initFont()
{
	if (!font.loadFromFile("assets/Fonts/ARIAL.TTF")) {
		std::cout << "ERROR: Could not load font" << std::endl;
	}
}

void Menu::initText()
{
	this->title.setFont(font);
	this->title.setString("Title");
	this->title.setCharacterSize(48);
	this->title.setPosition(100, 50);

	std::vector<std::string> menuOptions = {
		"Play as Human",
		"Play as AI",
		"Select Level",
		"Quit"
	};

	for (size_t i = 0; i < menuOptions.size(); i++) {
		sf::Text option;
		option.setFont(this->font);
		option.setString(menuOptions[i]);
		option.setCharacterSize(36);
		option.setPosition(100, 150 + i * 50);
		this->options.push_back(option);
	}
}

Menu::Menu()
{
	this->nextState = nullptr;
	this->initFont();
	this->initText();
}

void Menu::handleInput(sf::RenderWindow& window)
{
	sf::Event ev;
	while (window.pollEvent(ev)) {
		if (ev.type == sf::Event::Closed) {
			window.close();
		}
		if (ev.type == sf::Event::KeyPressed) {
			if (ev.key.code == sf::Keyboard::Up) {
				this->selectedOption = (this->selectedOption > 0) ? 
					static_cast<MenuOption>(this->selectedOption - 1) :
					static_cast<MenuOption>(this->options.size() - 1);
			}
			else if (ev.key.code == sf::Keyboard::Down) {
				this->selectedOption = static_cast<MenuOption>((this->selectedOption + 1) % this->options.size());
			}
			else if (ev.key.code == sf::Keyboard::Enter) {
				switch (selectedOption)
				{
				case PLAYER:
					this->nextState = new Game();
					break;
				case AI:
					break;
				case LEVEL:
					break;
				case QUIT:
					window.close();
					break;
				default:
					break;
				}
			}
		}
	}
}

void Menu::update()
{
	for (size_t i = 0; i < this->options.size(); i++) {
		this->options[i].setFillColor(i == this->selectedOption ? sf::Color::Blue : sf::Color::White);
	}
}

void Menu::render(sf::RenderTarget& target)
{
	target.clear();
	target.draw(this->title);
	for (const sf::Text& option : this->options) {
		target.draw(option);
	}
	
}

State* Menu::getNextState()
{
	return this->nextState;
}
