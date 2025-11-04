#include "Plane.h"

bool isCellAlive(const std::vector<Cell>& cells, int x, int y) {
    for (const Cell& cell : cells) {
        if (cell.getX() == x && cell.getY() == y) {
            return true;
        }
    }
    return false;
}

int numberOfAliveNeighbors(const std::vector<Cell>& cells, int x, int y) {
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue; // Skip the cell itself
            if (isCellAlive(cells, x + dx, y + dy)) {
                count++;
            }
        }
    }
    return count;
}

void Plane::advance() {
    std::vector<Cell> newAliveCells;
    for (const Cell& cell : aliveCells) {
        int aliveNeighbors = numberOfAliveNeighbors(aliveCells, cell.getX(), cell.getY());
        if (aliveNeighbors == 2 || aliveNeighbors == 3) {
            newAliveCells.push_back(cell);
        }
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue; // Skip the cell itself
                int neighborX = cell.getX() + dx;
                int neighborY = cell.getY() + dy;
                if (!isCellAlive(aliveCells, neighborX, neighborY) && !isCellAlive(newAliveCells, neighborX, neighborY)) {
                    int neighborAliveNeighbors = numberOfAliveNeighbors(aliveCells, neighborX, neighborY);
                    if (neighborAliveNeighbors == 3) {
                        newAliveCells.push_back(Cell(neighborX, neighborY));
                    }
                }
            }
        }
    }
    aliveCells = newAliveCells;
}

void Plane::tickPause(std::shared_ptr<sf::RenderWindow> window) {
	int hoveredX = sf::Mouse::getPosition(*window).x + window->getView().getCenter().x - window->getSize().x / 2;
	int hoveredY = sf::Mouse::getPosition(*window).y + window->getView().getCenter().y - window->getSize().y / 2;
	hoveredCell = Cell(hoveredX / cellSize, hoveredY / cellSize);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (!mousePressed) {
            mousePressed = 1;
            if (isCellAlive(aliveCells, hoveredCell.getX(), hoveredCell.getY())) {
                aliveCells.erase(std::remove_if(aliveCells.begin(), aliveCells.end(),
                    [this](const Cell& cell) {
                        return cell.getX() == hoveredCell.getX() && cell.getY() == hoveredCell.getY();
                    }), aliveCells.end());
            }
            else {
                aliveCells.push_back(hoveredCell);
            }
        }
    }
    if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        mousePressed = 0;
	}
}


void Plane::tick(std::shared_ptr<sf::RenderWindow> window) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        if(!spacePressed) {
            spacePressed = 1;
            paused = !paused;
		}
	}
    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        spacePressed = 0;
	}
	if(paused) {
		tickPause(window);
    }
    else {
        if(delayCounter < 5) {
            delayCounter++;
            return;
		}
		delayCounter = 0;
		advance();
    }
}

void Plane::render(std::shared_ptr<sf::RenderWindow> window) {
    if (paused) {
        int startX = window->getView().getCenter().x - window->getView().getSize().x / 2;
        startX -= cellSize + (startX % cellSize);
        int startY = window->getView().getCenter().y - window->getView().getSize().y / 2;
        startY -= cellSize + (startY % cellSize);
        sf::Image grid(sf::Vector2u(window->getSize().x+2*cellSize, window->getSize().y + 2*cellSize));
        for (int i = 0; i < window->getView().getSize().x + 2*cellSize; i++) {
            for (int j = 0; j < window->getView().getSize().y + 2*cellSize; j++) {
                if (i % cellSize == 0 || j % cellSize == 0) {
                    grid.setPixel(sf::Vector2u(i, j), sf::Color(255, 255, 255, 63));
                }
            }
        }
        sf::Texture t(grid);
        sf::Sprite s(t);
		s.setPosition(sf::Vector2f(startX, startY));
        window->draw(s);
    }
    for (const Cell& cell : aliveCells) {
        sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize));
        cellShape.setFillColor(sf::Color::White);
        cellShape.setPosition(sf::Vector2f(cell.getX() * cellSize, cell.getY() * cellSize));
        window->draw(cellShape);
    }
    
    if(paused) {
        sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize));
        if(isCellAlive(aliveCells, hoveredCell.getX(), hoveredCell.getY()))
            cellShape.setFillColor(sf::Color(255, 0, 0, 127));
        else {
            cellShape.setFillColor(sf::Color(0, 255, 0, 127));
        }
        cellShape.setPosition(sf::Vector2f(hoveredCell.getX() * cellSize, hoveredCell.getY() * cellSize));
        window->draw(cellShape);
    }
}