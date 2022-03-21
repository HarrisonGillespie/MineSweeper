#pragma once

#include "Cell.h"
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>

class Board
{
public:
	Board();

	// Member set functions
	void populateMines();
	void onClickLeft(unsigned x, unsigned y);
	void onClickRight(unsigned x, unsigned y);
	void drawBoard(sf::RenderWindow& window);
	void drawText(sf::RenderWindow& window, unsigned x, unsigned y);
	void highlightCell(sf::RenderWindow& window, unsigned x, unsigned y);
	void findMinesAroundCell(int row, int col);
	void setAllMinesAround();
	void revealCellsAround(int row, int col);
	void revealBoard();
	void setCellsRevealed();
	void resetBoard();
	void toggleGameOver();
	
	// Member output functions
	unsigned getCol();
	unsigned getRow();
	unsigned getCellSize();
	unsigned getNumBombs();
	unsigned getCellsRevealed();
	bool getFirstClick();
	bool getGameOver();

private:
	// Gameboard
	std::vector< std::vector<Cell> > gameBoard;

	// Boards params
	unsigned COLUMNS;
	unsigned ROWS;
	unsigned CELLSIZE;
	unsigned NUMBOMBS;
	unsigned cellsRevealed;
	bool revealed;
	bool gameOver;
	bool firstClick;

	// Font
	sf::Font font;
	// Text
	sf::Text text;
};

