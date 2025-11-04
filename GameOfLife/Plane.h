#pragma once
#include "Cell.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>






class Plane {
private:
	std::vector<Cell> aliveCells;
	bool paused = 1;
	bool spacePressed = 0;
	bool mousePressed = 0;
	Cell hoveredCell = Cell(-1, -1);
	int delayCounter = 0;
	int cellSize = 20;
public:
	Plane(int toCellSize) {
		cellSize = toCellSize;
	}
	void tick(std::shared_ptr<sf::RenderWindow> window);
	void advance();
	void tickPause(std::shared_ptr<sf::RenderWindow> window);
	void render(std::shared_ptr<sf::RenderWindow> window);
};