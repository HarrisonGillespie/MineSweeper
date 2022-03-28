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
	// Default Board Parameters
	COLUMNS = 20;
	ROWS = 20;
	NUMBOMBS = 99;
	menuPad = 1;
	isFlagged = 0;
	cellsRevealed = 0;
	revealed = 0;
	firstClick = 1;
	gameOver = 0;
	clockTime = 0;
	displayMenu = 0;
	errorMenu = 0;

	// Algorithm to find cell size from window size and rows/columns
	windowHeight = (0.90f * windowHeight);
	windowHeight -= (windowHeight % 100);
	CELLSIZE = windowHeight / ROWS; // Integer Divison to truncate
	windowHeight = (CELLSIZE * (ROWS + menuPad)) + 1; // Re-multiply in case value was truncated, adding 1 for the black outline
	windowWidth = (CELLSIZE * COLUMNS) + 1; // Adding 1 for the black outline

	// Create the window
	window.create(sf::VideoMode(windowWidth, windowHeight), "MineSweeper", sf::Style::Close);
	window.setFramerateLimit(60);

	// Setup font and text Settings
	size_t len = LcdSolid_VPzB_ttf_size;
	void* data = LcdSolid_VPzB_ttf;

	font.loadFromMemory(data, len);

	cellText.setFont(font);
	cellText.setCharacterSize(CELLSIZE * 0.5f);
	// cellText.setOrigin(CELLSIZE / 4.0f, CELLSIZE / 4.0f);
	cellText.setFillColor(sf::Color::Black);

	clockText.setFont(font);
	clockText.setCharacterSize(CELLSIZE);
	clockText.setFillColor(sf::Color::Red);
	clockText.setString("000");

	bombText.setFont(font);
	bombText.setCharacterSize(CELLSIZE);
	bombText.setFillColor(sf::Color::Red);
	bombText.setPosition(0 + (window.getSize().x * 0.05f), 0);
	bombText.setString("099");

	smiley.setFont(font);
	smiley.setCharacterSize(CELLSIZE * 0.6f);
	smiley.setFillColor(sf::Color::Black);
	smiley.setString(":|");
	smiley.setOrigin((CELLSIZE * 0.6f) * 0.5f, (CELLSIZE * 0.6f) * 0.5f);
	smiley.rotate(90);
	smiley.scale(1,1.1);

	// Setup Shapes
	cellShape.setSize(sf::Vector2f(CELLSIZE - 1, CELLSIZE - 1));

	flagShape.setSize(sf::Vector2f((CELLSIZE / 2), (CELLSIZE / 2)));
	flagShape.setOrigin(sf::Vector2f((CELLSIZE / 4), (CELLSIZE / 4)));
	flagShape.rotate(45);
	flagShape.setFillColor(sf::Color::Red);

	smileyBackground.setRadius((CELLSIZE - 9) / 2.0f);
	smileyBackground.setOrigin((CELLSIZE - 9) / 2.0f, 0);
	smileyBackground.setFillColor(sf::Color::Yellow);

	// Initialzie board
	gameBoard.resize(ROWS,std::vector<Cell>(COLUMNS,Cell()));
}

void Board::checkGameOver()
{
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (gameBoard[i][j].getClicked() && gameBoard[i][j].getMine()) {
				setGameOver();
			}
		}
	}
}

void Board::populateMines(int x, int y)
{
	std::default_random_engine gen(std::chrono::steady_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> xR(0, COLUMNS - 1);
	std::uniform_int_distribution<int> yR(0, ROWS - 1);

	for (int i = 0; i < NUMBOMBS; i++) {
		int col = xR(gen);
		int row = yR(gen);
		if (gameBoard[row][col].getMine() || isAround(y,x,row,col) || (row == y && col == x)) {
			i--;
		}
		else {
			gameBoard[row][col].setMine();
		}
	}

	setAllMinesAround();
}

void Board::onClickLeft(int x, int y)
{
	int relativeY = y - menuPad;
	// Event Handling

		// Menu
	if (y == 0) {
		; // Nothing for now
	}
		// Board
	else if(!displayMenu) {
		if (getFirstClick()) {
			populateMines(x, relativeY);
			smiley.setString(":)");
			firstClick = 0;
			clockTime = 0;
			clock.restart();
		}

		if (!gameBoard[relativeY][x].getClicked() && gameBoard[relativeY][x].getClickable()) {
			if (!gameBoard[relativeY][x].getFlagged()) {
				gameBoard[relativeY][x].setClicked();
				if (gameBoard[relativeY][x].getMinesAround() == 0 && !gameBoard[relativeY][x].getMine()) {
					revealCellsAround(relativeY, x);
				}
			}
		}
		else if (gameBoard[relativeY][x].getClicked() && gameBoard[relativeY][x].getClickable() && !gameBoard[relativeY][x].getMine()) {
			if (isAroundFlagged(relativeY, x) >= gameBoard[relativeY][x].getMinesAround()) {
				revealAroundNotFlagged(relativeY, x);
			}
		}
	}

	checkGameOver();
}

void Board::onClickRight(int x, int y)
{
	int relativeY = y - menuPad;
	// Event handling

	if (y == 0) {
		; // Nothing for now
	}
	else if (!displayMenu) {
		if (!getFirstClick()) {
			if (!gameBoard[relativeY][x].getClicked() && !gameOver) {
				gameBoard[relativeY][x].toggleFlagged();
				if (gameBoard[relativeY][x].getMine()) {
					// DEBUG: std::cout << "MINE" << std::endl;
				}
				else {
					// DEBUG: std::cout << gameBoard[y][x].getMinesAround() << std::endl;
				}
			}
			else if (gameBoard[relativeY][x].getClicked() && gameBoard[relativeY][x].getClickable() && !gameBoard[relativeY][x].getMine()) {
				if (isAroundNotClicked(relativeY, x) == gameBoard[relativeY][x].getMinesAround()) {
					flagAround(relativeY, x);
				}
			}
		}
	}
}

void Board::drawBoard(sf::RenderWindow& window)
{
	// Clear the window
	window.clear(sf::Color::Black);

	// Draw the board
	for (unsigned i = 0; i < ROWS; i++) {
		for (unsigned j = 0; j < COLUMNS; j++) {
			cellShape.setPosition(1 + CELLSIZE * j, (1 + CELLSIZE * i) + CELLSIZE);
			flagShape.setPosition((CELLSIZE * j) + (CELLSIZE * 0.5f), (CELLSIZE * i) + (CELLSIZE * 1.5f));
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

			// Draw Bomb Counter
			getAllFlagged();
			if (NUMBOMBS - isFlagged < 0) {
				if (std::to_string(isFlagged - NUMBOMBS).size() < 3) {
					bombText.setString((std::to_string(isFlagged - NUMBOMBS).insert(0, 2 - std::to_string(isFlagged - NUMBOMBS).size(), '0')).insert(0,1,'-'));
				}
			}
			else {
				if (std::to_string(NUMBOMBS - isFlagged).size() < 3) {
					bombText.setString(std::to_string(NUMBOMBS - isFlagged).insert(0, 3 - std::to_string(NUMBOMBS - isFlagged).size(), '0'));
				}
				else {
					bombText.setString(std::to_string(NUMBOMBS - isFlagged));
				}
			}
			// std::cout << NUMBOMBS << std::endl;
			window.draw(bombText);
			
			// Draw Smiley Face
			smiley.setPosition((window.getSize().x / 2.0f) + 2, CELLSIZE * 0.4f);
			smileyBackground.setPosition(window.getSize().x / 2.0f, 5);
			window.draw(smileyBackground);
			window.draw(smiley);
		}
	}
}

void Board::drawText(sf::RenderWindow& window, int row, int col)
{
	cellText.setString(std::to_string(gameBoard[row][col].getMinesAround()));
	switch (gameBoard[row][col].getMinesAround()) {
	case 0:
		cellText.setFillColor(sf::Color::White);
		break;
	case 1: 
		cellText.setFillColor(sf::Color::Blue);
		break;
	case 2:
		cellText.setFillColor(sf::Color::Green);
		break;
	case 3:
		cellText.setFillColor(sf::Color::Red);
		break;
	case 4:
		cellText.setFillColor(sf::Color(0, 0, 139));
		break;
	case 5:
		cellText.setFillColor(sf::Color(165, 42, 42));
		break;
	case 6:
		cellText.setFillColor(sf::Color::Cyan);
		break;
	case 7:
		cellText.setFillColor(sf::Color::Black);
		break;
	case 8:
		cellText.setFillColor(sf::Color::Yellow);
		break;
	}
	cellText.setPosition((CELLSIZE * (col + 0.4f)) - 2, (CELLSIZE * row) + (CELLSIZE * 1.2f));
	window.draw(cellText);
}

void Board::getClockTime(sf::RenderWindow& window)
{
	time = clock.getElapsedTime();
	clockText.setPosition(0 + (window.getSize().x * 0.85f), 0);
	if (!firstClick && !gameOver) {
		clockText.setFillColor(sf::Color::Green);
		clockTime = static_cast<int>(time.asSeconds());
		if (std::to_string(clockTime).size() < 3) {
			clockText.setString(std::to_string(clockTime).insert(0, 3 - std::to_string(clockTime).size(), '0'));
		}
		else {
			clockText.setString(std::to_string(clockTime));
		}
		// DEBUG: std::cout << clockOut << std::endl;
	}
	else {
		clockText.setFillColor(sf::Color::Red);
	}

	window.draw(clockText);
}

void Board::highlightCell(sf::RenderWindow& window, int x, int y)
{
	// Highlight cell the mouse is currently hovering over
	if (x < COLUMNS && y > 0 && y < ROWS + menuPad) {
		cellShape.setPosition(1 + (CELLSIZE * x), 1 + (CELLSIZE * y));
		cellShape.setFillColor(sf::Color::Blue);
		window.draw(cellShape);
	}
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

void Board::revealAroundNotFlagged(int row, int col)
{
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			// Checking for out of bounds
			if ((i == 0 && j == 0) || (i + row < 0 || j + col < 0 || i + row == ROWS || j + col == COLUMNS)) {
				continue;
			}
			else if(!gameBoard[row + i][col + j].getFlagged()) {
				gameBoard[row + i][col + j].setClicked();
				if ((gameBoard[row + i][col + j].getMinesAround() == 0) && !gameBoard[row + i][col + j].getMine()) revealCellsAround(row + i, col + j);
				checkGameOver();
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
	revealed = 0;
	firstClick = 1;
	isFlagged = 0;
	clockText.setString("000");
	smiley.setString(":|");
	bombText.setFillColor(sf::Color::Red);
	cellsRevealed = 0;
	gameOver = 0;
	displayMenu = 0;
}

void Board::setGameWon()
{
	gameOver = 1;
	smiley.setString(":D");
	bombText.setString("000");
	bombText.setFillColor(sf::Color::Green);
}

void Board::setGameOver()
{
	gameOver = 1;
	smiley.setString(":(");
	revealBoard();
}

bool Board::isAround(/*First Click Cell*/int row, int col, /*Cell to check if around*/int row2, int col2)
{
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			// Checking for out of bounds
			if ((i == 0 && j == 0) || (i + row2 < 0 || j + col2 < 0 || i + row2 == ROWS || j + col2 == COLUMNS)) {
				continue;
			}
			else if((row == row2 + i) && (col == col2 + j)) {
				return true;
			}
		}
	}
	return false;
}

int Board::isAroundFlagged(int row, int col)
{
	int count = 0;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			// Checking for out of bounds
			if ((i == 0 && j == 0) || (i + row < 0 || j + col < 0 || i + row == ROWS || j + col == COLUMNS)) {
				continue;
			}
			else if (gameBoard[row + i][col + j].getFlagged()) {
				count++;
			}
		}
	}
	return count;
}

int Board::isAroundNotClicked(int row, int col)
{
	int count = 0;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			// Checking for out of bounds
			if ((i == 0 && j == 0) || (i + row < 0 || j + col < 0 || i + row == ROWS || j + col == COLUMNS)) {
				continue;
			}
			else if (!gameBoard[row + i][col + j].getClicked()) {
				count++;
			}
		}
	}
	return count;
}

void Board::flagAround(int row, int col)
{
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			// Checking for out of bounds
			if ((i == 0 && j == 0) || (i + row < 0 || j + col < 0 || i + row == ROWS || j + col == COLUMNS)) {
				continue;
			}
			else if (!gameBoard[row + i][col + j].getFlagged() && !gameBoard[row + i][col + j].getClicked()) {
				gameBoard[row + i][col + j].toggleFlagged();
			}
		}
	}
}

void Board::getAllFlagged()
{
	int totalFlagged = 0;

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (gameBoard[i][j].getFlagged()) {
				totalFlagged++;
			}
		}
	}
	
	isFlagged = totalFlagged;
}

void Board::toggleMenu()
{
	displayMenu = !displayMenu;
}

void Board::toggleError()
{
	errorMenu = !errorMenu;
}

void Board::openMenu()
{
	displayMenu = 1;
}

void Board::closeMenu()
{
	displayMenu = 0;
}

void Board::resizeBoard(int newRows, int newCols, int newBombs)
{
	int calcCellsize;

	if (newBombs > ((newRows * newCols) - 9)) {
		displayMenu = 0;
		errorMenu = 1;
		std::cout << "Error bombs check" << std::endl;
		return;
	}

	windowHeight = modes[0].height; 
	windowHeight = (0.90f * windowHeight);
	windowHeight -= (windowHeight % 100);
	calcCellsize = windowHeight / newRows; // Integer Divison to truncate
	windowHeight = (calcCellsize * (newRows + menuPad)) + 1; // Re-multiply in case value was truncated, adding 1 for the black outline
	windowWidth = (calcCellsize * newCols) + 1; // Adding 1 for the black outline

	if ((windowWidth + 1) > modes[0].width) {
		displayMenu = 0;
		errorMenu = 1;
		std::cout << "Error wdith check" << std::endl;
		std::cout << "Calc width: " << windowWidth << " Monitor width: " << window.getSize().x << std::endl;
		return;
	}

	ROWS = newRows;
	COLUMNS = newCols;
	NUMBOMBS = newBombs;
	CELLSIZE = calcCellsize;

	window.setSize(sf::Vector2u(windowWidth,windowHeight));
	window.setView(sf::View(sf::FloatRect(0,0,windowWidth,windowHeight)));
	window.setPosition(sf::Vector2i((modes[0].width / 2) - (window.getSize().x / 2), (modes[0].height / 2) - (window.getSize().y / 2)));

	cellShape.setSize(sf::Vector2f(CELLSIZE - 1, CELLSIZE - 1));
	flagShape.setSize(sf::Vector2f((CELLSIZE / 2), (CELLSIZE / 2)));
	flagShape.setOrigin(sf::Vector2f((CELLSIZE / 4), (CELLSIZE / 4)));
	smileyBackground.setRadius((CELLSIZE - 9) / 2.0f);
	smileyBackground.setOrigin((CELLSIZE - 9) / 2.0f, 0);

	cellText.setCharacterSize(CELLSIZE / 2);
	clockText.setCharacterSize(CELLSIZE);
	bombText.setCharacterSize(CELLSIZE);
	bombText.setPosition(0 + (window.getSize().x * 0.05f), 0);
	smiley.setCharacterSize(CELLSIZE * 0.6f);
	smiley.setOrigin((CELLSIZE * 0.6f) * 0.5f, (CELLSIZE * 0.6f) * 0.5f);

	resetBoard();
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

bool Board::getDisplayMenu()
{
	return displayMenu;
}

bool Board::getErrorMenu()
{
	return errorMenu;
}