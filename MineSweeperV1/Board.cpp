#include "Board.h"
#include "Cell.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>

// Default constructor for new Board object
Board::Board()
{
	// Set defualt board settings
	COLUMNS = 20;
	ROWS = 20;
	CELLSIZE = 50;
	NUMBOMBS = 40;

	// Initialzie board
	gameBoard.resize(ROWS,std::vector<Cell>(COLUMNS,Cell()));

	// PopulateMines
	std::default_random_engine gen(std::chrono::steady_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<unsigned> xDist(0, COLUMNS - 1);
	std::uniform_int_distribution<unsigned> yDist(0, ROWS - 1);

	for (unsigned i = 0; i < NUMBOMBS; i++) {
		unsigned cCoord = xDist(gen);
		unsigned rCoord = yDist(gen);
		if (gameBoard.at(rCoord).at(cCoord).getMine()) {
			i--;
		}
		else {
			gameBoard.at(rCoord).at(cCoord).setMine();
		}
	}
}

void Board::onClickLeft(unsigned x, unsigned y)
{
	// Event Handling
	if (!gameBoard[y][x].getClicked()) {
		if (!gameBoard[y][x].getFlagged()) {
			gameBoard[y][x].setClicked();
		}
	}
}

void Board::onClickRight(unsigned x, unsigned y)
{
	// Event handling
	if (!gameBoard[y][x].getClicked()) {
		gameBoard[y][x].toggleFlagged();
	}
}

void Board::drawBoard(sf::RenderWindow& window)
{
	// Cell Shape
	sf::RectangleShape cellShape(sf::Vector2f(CELLSIZE - 1, CELLSIZE - 1));
	// "Flag" Shape
	sf::RectangleShape flagShape(sf::Vector2f((CELLSIZE / 2), (CELLSIZE / 2)));
	flagShape.setFillColor(sf::Color::Magenta);

	// Clear the window
	window.clear(sf::Color::Black);

	// Draw the board
	for (unsigned i = 0; i < ROWS; i++) {
		for (unsigned j = 0; j < COLUMNS; j++) {
			cellShape.setPosition(1 + CELLSIZE * i, 1 + CELLSIZE * j);
			flagShape.setPosition((CELLSIZE * i) + 13, (CELLSIZE * j) + 13);
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
