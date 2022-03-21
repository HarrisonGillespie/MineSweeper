#pragma once

#include "Cell.h"
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>

class Board
{
public:
	Board();

	void onClickLeft(unsigned x, unsigned y);
	void onClickRight(unsigned x, unsigned y);
	void drawBoard(sf::RenderWindow& window);
	void highlightCell(sf::RenderWindow& window, unsigned x, unsigned y);
	void findMinesAroundCell(int row, int col);
	void setAllMinesAround();
	void revealCellsAround(int row, int col);
	void revealBoard();
	
	unsigned getCol();
	unsigned getRow();
	unsigned getCellSize();
	unsigned getNumBombs();

private:
	// Gameboard
	std::vector< std::vector<Cell> > gameBoard;

	// Boards params
	unsigned COLUMNS;
	unsigned ROWS;
	unsigned CELLSIZE;
	unsigned NUMBOMBS;
	bool revealed;
};

