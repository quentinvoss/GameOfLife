#pragma once
#include "SFML/Graphics.hpp"

static int dx;
static int dy;
static const int speed = 4;

static class WindowHandler {
public:

	static void init() {
		dx = 0;
		dy = 0;
	}

	static void handleKeyEvents(std::shared_ptr<sf::RenderWindow> window) {
		bool updated = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			dx -= speed;
			updated = 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			dx += speed;
			updated = 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			dy -= speed;
			updated = 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			dy += speed;
			updated = 1;
		}
		if (updated) {
			sf::View view;
			view.setSize(sf::Vector2f(window->getSize()));
			view.setCenter(sf::Vector2f(window->getSize().x / 2 + dx, window->getSize().y / 2 + dy));
			window->setView(view);
		}
	}
};