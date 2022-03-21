#include "Board.h"
#include "Cell.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include <stdexcept>

// Default constructor for new Board object
Board::Board()
{
	// Set defualt board settings
	COLUMNS = 20;
	ROWS = 20;
	CELLSIZE = 50;
	NUMBOMBS = 80;
	revealed = 0;

	// Initialzie board
	gameBoard.resize(ROWS,std::vector<Cell>(COLUMNS,Cell()));

	// PopulateMines
	std::default_random_engine gen(std::chrono::steady_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<unsigned> xDist(0, COLUMNS - 1);
	std::uniform_int_distribution<unsigned> yDist(0, ROWS - 1);

	for (unsigned i = 0; i < NUMBOMBS; i++) {
		unsigned cCoord = xDist(gen);
		unsigned rCoord = yDist(gen);
		if (gameBoard[rCoord][cCoord].getMine()) {
			i--;
		}
		else {
			gameBoard[rCoord][cCoord].setMine();
		}
	}
	
	// Find the mines around each cell
	setAllMinesAround();
}

void Board::onClickLeft(unsigned x, unsigned y)
{
	// Event Handling
	if (!gameBoard[y][x].getClicked()) {
		if (!gameBoard[y][x].getFlagged()) {
			gameBoard[y][x].setClicked();
			if (gameBoard[y][x].getMinesAround() == 0) {
				revealCellsAround(y,x);
			}
		}
	}
}

void Board::onClickRight(unsigned x, unsigned y)
{
	// Event handling
	if (!gameBoard[y][x].getClicked() || revealed) {
		gameBoard[y][x].toggleFlagged();
		std::cout << gameBoard[y][x].getMinesAround() << std::endl;
	}
}

void Board::drawBoard(sf::RenderWindow& window)
{
	// Cell Shape
	sf::RectangleShape cellShape(sf::Vector2f(CELLSIZE - 1, CELLSIZE - 1));
	// "Flag" Shape
	sf::RectangleShape flagShape(sf::Vector2f((CELLSIZE / 2), (CELLSIZE / 2)));
	flagShape.rotate(45);
	flagShape.setFillColor(sf::Color::Magenta);

	// Clear the window
	window.clear(sf::Color::Black);

	// Draw the board
	for (unsigned i = 0; i < ROWS; i++) {
		for (unsigned j = 0; j < COLUMNS; j++) {
			cellShape.setPosition(1 + CELLSIZE * i, 1 + CELLSIZE * j);
			flagShape.setPosition((CELLSIZE * i) + 26, (CELLSIZE * j) + 8);
			// Draw Cell
			if (!gameBoard.at(j).at(i).getClicked()) {
				cellShape.setFillColor(sf::Color(128, 128, 128));
			}
			else if (gameBoard.at(j).at(i).getMine()) {
				cellShape.setFillColor(sf::Color::Red);
			}
			else {
				cellShape.setFillColor(sf::Color::Green);
			}
			window.draw(cellShape);

			// Draw Flag
			if (gameBoard.at(j).at(i).getFlagged()) {
				window.draw(flagShape);
			}
		}
	}
}

void Board::highlightCell(sf::RenderWindow& window, unsigned x, unsigned y)
{
	// Highlight cell the mouse is currently hovering over
	sf::RectangleShape cellShape(sf::Vector2f(CELLSIZE - 1, CELLSIZE - 1));
	cellShape.setPosition(1 + (CELLSIZE * x), 1 + (CELLSIZE * y));
	cellShape.setFillColor(sf::Color::Blue);
	window.draw(cellShape);
}

void Board::findMinesAroundCell(int row, int col)
{
	unsigned currMines = 0;

	if (!gameBoard[row][col].getMine()) {
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				// Checking for out of bounds
				if ((i == 0 && j == 0) || (i + row < 0 || j + col < 0 || i + row == ROWS || j + col == COLUMNS)) {
					continue;
				}
				else if (gameBoard[row + i][col + j].getMine()) {
					currMines++;
				}
			}
		}
	}

	gameBoard[row][col].setMinesAround(currMines);
}

void Board::setAllMinesAround()
{
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			findMinesAroundCell(i, j);
		}
	}
}

void Board::revealCellsAround(int row, int col)
{
	gameBoard[row][col].setAroundRevealed();
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if ((i == 0 && j == 0) || (i + row < 0 || j + col < 0 || i + row == ROWS || j + col == COLUMNS)) {
				continue;
			}
			else {
				gameBoard[row + i][col + j].setClicked();
				if ((gameBoard[row + i][col + j].getMinesAround() == 0) && !gameBoard[row + i][col + j].getAroundRevealed()) {
					revealCellsAround(row + i, col + j);
				}
			}
		}
	}
}

void Board::revealBoard()
{
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			gameBoard[i][j].setClicked();
		}
	}
	revealed = 1;
}

unsigned Board::getCol()
{
	return COLUMNS;
}

unsigned Board::getRow()
{
	return ROWS;
}

unsigned Board::getCellSize()
{
	return CELLSIZE;
}

unsigned Board::getNumBombs()
{
	return NUMBOMBS;
}
