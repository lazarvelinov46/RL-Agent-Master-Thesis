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
	//Title configuration
	this->title.setFont(font);
	this->title.setString("Title");
	this->title.setCharacterSize(48);
	this->title.setPosition(100, 50);

	//Menu option string
	std::vector<std::string> menuOptions = {
		"Play as Human",
		"Play as AI",
		"Easy",
		"Quit"
	};


	//Creates sf::Text element for each menu entry
	for (size_t i = 0; i < menuOptions.size(); i++) {
		sf::Text option;
		option.setFont(this->font);
		option.setString(menuOptions[i]);
		option.setCharacterSize(36);
		option.setPosition(100, 150 + i * 50);
		this->options.push_back(option);
	}
	this->selectedOption = MenuOption::PLAYER;
}

Menu::Menu()
{
	this->nextState = nullptr;
	this->selectedDifficulty = LevelDifficulty::EASY;
	this->initFont();
	this->initText();
}

Menu::~Menu()
{
	delete this->nextState;
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
				//Move selection up
				this->selectedOption = (this->selectedOption > 0) ? 
					static_cast<MenuOption>(this->selectedOption - 1) :
					static_cast<MenuOption>(this->options.size() - 1);
			}
			else if (ev.key.code == sf::Keyboard::Down) {
				//Move selection down
				this->selectedOption = static_cast<MenuOption>((this->selectedOption + 1) % this->options.size());
			}
			else if (ev.key.code == sf::Keyboard::Enter) {
				//Change state or close window based on selected item
				switch (selectedOption)
				{
				case PLAYER:
					this->nextState = new Game(this->selectedDifficulty);
					break;
				case AI:
					this->nextState = new GameAI(this->selectedDifficulty);
					break;
				case LEVEL:
					this->selectedDifficulty = (LevelDifficulty)((this->selectedDifficulty + 1) % LevelDifficulty::NUM);
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

void Menu::update(float deltaTime)
{
	//Highlight currently selected option
	for (size_t i = 0; i < this->options.size(); i++) {
		this->options[i].setFillColor(i == this->selectedOption ? sf::Color::Blue : sf::Color::White);
	}
	switch (this->selectedDifficulty)
	{
	case EASY:
		this->options[2].setString("Easy");
		break;
	case MEDIUM:
		this->options[2].setString("Medium");
		break;
	case HARD:
		this->options[2].setString("Hard");
		break;
	default:
		break;
	}
}

void Menu::render(sf::RenderTarget& target)
{
	//Draw elements
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
