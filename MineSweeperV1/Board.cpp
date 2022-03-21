#include "Board.h"
#include "Cell.h"
#include "font.h"
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
	NUMBOMBS = 80;
	cellsRevealed = 0;
	revealed = 0;
	firstClick = 0;
	gameOver = 0;

	// Setup font and text Settings
	size_t len = LcdSolid_VPzB_ttf_size;
	void* data = LcdSolid_VPzB_ttf;

	if (!font.loadFromMemory(data,len)) {
		std::cout << "Error loading font" << std::endl;
	}
	text.setFont(font);
	text.setCharacterSize(25);
	text.setFillColor(sf::Color::Black);

	// Initialzie board
	gameBoard.resize(ROWS,std::vector<Cell>(COLUMNS,Cell()));

	// PopulateMines
	populateMines();
	
	// Find the mines around each cell
	setAllMinesAround();
}

void Board::populateMines()
{
	std::default_random_engine gen(std::chrono::steady_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<unsigned> x(0, COLUMNS - 1);
	std::uniform_int_distribution<unsigned> y(0, ROWS - 1);

	for (unsigned i = 0; i < NUMBOMBS; i++) {
		unsigned col = x(gen);
		unsigned row = y(gen);
		if (gameBoard[row][col].getMine()) {
			i--;
		}
		else {
			gameBoard[row][col].setMine();
		}
	}
}

void Board::onClickLeft(unsigned x, unsigned y)
{
	// Event Handling
	if (!gameBoard[y][x].getClicked() && gameBoard[y][x].getClickable()) {
		if (!gameBoard[y][x].getFlagged()) {
			gameBoard[y][x].setClicked();
			if (gameBoard[y][x].getMinesAround() == 0 && !gameBoard[y][x].getMine()) {
				revealCellsAround(y,x);
			}
			else if (gameBoard[y][x].getMine()) {
				revealBoard();
			}
		}
	}
	// Testing end game func -- Later incorporate into a check end game function
	if (getCellsRevealed() == (ROWS * COLUMNS) - NUMBOMBS) {
		revealBoard();
	}
}

void Board::onClickRight(unsigned x, unsigned y)
{
	// Event handling
	if (!gameBoard[y][x].getClicked()) {
		gameBoard[y][x].toggleFlagged();
		if (gameBoard[y][x].getMine()) {
			std::cout << "MINE" << std::endl;
		}
		else {
			std::cout << gameBoard[y][x].getMinesAround() << std::endl;
		}
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
			cellShape.setPosition(1 + CELLSIZE * j, 1 + CELLSIZE * i);
			flagShape.setPosition((CELLSIZE * j) + 26, (CELLSIZE * i) + 8);
			// Draw Cell
			if (!gameBoard[i][j].getClicked()) {
				cellShape.setFillColor(sf::Color(192, 192, 192));
			}
			else if (gameBoard[i][j].getMine()) {
				cellShape.setFillColor(sf::Color::Red);
			}
			else {
				cellShape.setFillColor(sf::Color(128, 128, 128));
			}
			window.draw(cellShape);

			// Draw Flag
			if (gameBoard[i][j].getFlagged()) {
				window.draw(flagShape);
			}

			// Draw Text
			if (gameBoard[i][j].getClicked() && !gameBoard[i][j].getMine() && (gameBoard[i][j].getMinesAround() != 0)) {
				drawText(window, i, j);
			}
			// Debug showing all mines around
			/*if (!gameBoard[i][j].getMine()) {
				drawText(window, i, j);
			}*/
		}
	}
}

void Board::drawText(sf::RenderWindow& window, unsigned row, unsigned col)
{
	text.setString(std::to_string(gameBoard[row][col].getMinesAround()));
	switch (gameBoard[row][col].getMinesAround()) {
	case 0:
		text.setFillColor(sf::Color::White);
		break;
	case 1: 
		text.setFillColor(sf::Color::Blue);
		break;
	case 2:
		text.setFillColor(sf::Color::Green);
		break;
	case 3:
		text.setFillColor(sf::Color::Red);
		break;
	case 4:
		text.setFillColor(sf::Color(0, 0, 139));
		break;
	case 5:
		text.setFillColor(sf::Color(165, 42, 42));
		break;
	case 6:
		text.setFillColor(sf::Color::Cyan);
		break;
	case 7:
		text.setFillColor(sf::Color::Black);
		break;
	case 8:
		text.setFillColor(sf::Color::Yellow);
		break;
	}
	text.setPosition(18 + (CELLSIZE * col), 11 + (CELLSIZE * row));
	window.draw(text);
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
			// Checking for out of bounds
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
			gameBoard[i][j].setClickable();
			if (gameBoard[i][j].getMine()) {
				gameBoard[i][j].setClicked();
			}
		}
	}
	revealed = 1;
	gameOver = 1;
}

void Board::setCellsRevealed()
{
	unsigned counter = 0;
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (gameBoard[i][j].getClicked() && !gameBoard[i][j].getMine()) {
				counter++;
			}
		}
	}
	cellsRevealed = counter;
}

void Board::resetBoard()
{
	gameBoard.clear();
	gameBoard.resize(ROWS, std::vector<Cell>(COLUMNS, Cell()));
	populateMines();
	setAllMinesAround();
	firstClick = 0;
	cellsRevealed = 0;

	toggleGameOver();
}

void Board::toggleGameOver()
{
	gameOver = !gameOver;
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

unsigned Board::getCellsRevealed()
{
	return cellsRevealed;
}

bool Board::getFirstClick()
{
	return firstClick;
}

bool Board::getGameOver()
{
	return gameOver;
}