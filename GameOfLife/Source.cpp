#include <iostream>
#include <chrono>
#include "SFML/Graphics.hpp"
#include "Plane.h"
#include "WindowHandler.h"

const int FRAMERATE = 60;
constexpr unsigned int FRAME_DURATION = int(1000000 / FRAMERATE);

int main(int argumentCount, char* arguments[]) {
	int cellSize = 10;
	if (argumentCount == 1) {
		std::cout << "Using default cell size of 10 pixels." << std::endl;
	}
	else if(argumentCount == 2) {
		cellSize = std::stoi(arguments[1]);
	}
	else {
		std::cout << "Usage: GameOfLife [cellSize]" << std::endl;
		return 1;
	}


	WindowHandler::init();

	unsigned lag = 0;

	std::chrono::time_point<std::chrono::steady_clock> previous_time;

	previous_time = std::chrono::steady_clock::now();

	std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(sf::Vector2u(int(1000/cellSize) * cellSize, int(1000 / cellSize) * cellSize )), "Life");

	Plane plane(cellSize);

	window->setKeyRepeatEnabled(true);
	//Main Loop
	while (window->isOpen()) {



		//The difference in time between the current frame and the previous frame
		unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();

		//Add the difference to the lag
		lag += delta_time;

		//Updating the current time for the next frame.
		previous_time += std::chrono::microseconds(delta_time);

		while (FRAME_DURATION <= lag)
		{
			lag -= FRAME_DURATION;
			while (const std::optional event = window->pollEvent()) {
				if (event->is<sf::Event::Closed>()) {
					window->close();
				}
			}
			if (FRAME_DURATION > lag) {
				window->clear();
				WindowHandler::handleKeyEvents(window);
				plane.tick(window);
				plane.render(window);
				window->display();
			}
		}
	}
	return 0;
}