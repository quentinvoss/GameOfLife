#pragma once
class Cell {
private:
	int x;
	int y;
public:
	Cell(int toX, int toY){
		x = toX;
		y = toY;
	}
	int getX() const { return x; }
	int getY() const { return y; }
};