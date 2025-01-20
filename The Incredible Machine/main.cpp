// The Incredible Machine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include "Menu.h"

void playHuman(sf::RenderWindow& window) {
	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear(sf::Color::Blue);
		window.display();
	}
}

void playAI(sf::RenderWindow& window) {
	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear(sf::Color::Green);
		window.display();
	}
}

void levelSelector(sf::RenderWindow& window) {
	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear(sf::Color::Yellow);
		window.display();
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1200, 800), "The Incredible Machine", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(60);
	Menu menu;
	while (window.isOpen()) {
		menu.handleInput(window);
		menu.update();
		menu.render(window);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
			MenuOption selected = menu.getSelectedOption();
			switch (selected)
			{
			case PLAYER:
				playHuman(window);
				break;
			case AI:
				playAI(window);
				break;
			case LEVEL:
				levelSelector(window);
				break;
			case QUIT:
				window.close();
				break;
			default:
				break;
			}
		}
		window.display();
	}
}

