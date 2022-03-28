#pragma once

#include "Cell.h"
#include <SFML/Graphics.hpp>
#include <chrono>

class Board
{
public:
	// Constructor
	Board();

	// Internal functions
	void checkGameOver();
	void populateMines(int x, int y);
	void onClickLeft(int x, int y);
	void onClickRight(int x, int y);
	void drawBoard(sf::RenderWindow& window);
	void drawText(sf::RenderWindow& window, int x, int y);
	void getClockTime(sf::RenderWindow& window);
	void highlightCell(sf::RenderWindow& window, int x, int y);
	void findMinesAroundCell(int row, int col);
	void setAllMinesAround();
	void revealCellsAround(int row, int col);
	void revealAroundNotFlagged(int row, int col);
	void revealBoard();
	void setCellsRevealed();
	void resetBoard();
	void setGameWon();
	void setGameOver();
	bool isAround(int row, int col, int row2, int col2);
	int isAroundFlagged(int row, int col);
	int isAroundNotClicked(int row, int col);
	void flagAround(int row, int col);
	void getAllFlagged();
	void toggleMenu();
	void toggleError();
	void openMenu();
	void closeMenu();

	// Board Resizing
	void resizeBoard(int newRows, int newCols, int newBombs);
	
	// Output functions
	unsigned getCol();
	unsigned getRow();
	unsigned getCellSize();
	unsigned getNumBombs();
	unsigned getCellsRevealed();
	bool getFirstClick();
	bool getGameOver();
	bool getDisplayMenu();
	bool getErrorMenu();

	// Render window
	sf::RenderWindow window;

private:
	// Gameboard
	std::vector< std::vector<Cell> > gameBoard;

	// Board params
	int COLUMNS;
	int ROWS;
	int menuPad;
	int CELLSIZE;
	int NUMBOMBS;
	int isFlagged;
	int cellsRevealed;
	int clockTime;
	bool revealed;
	bool gameOver;
	bool firstClick;
	bool displayMenu;
	bool errorMenu;

	// Font
	sf::Font font;

	// Text
	sf::Text cellText;
	sf::Text clockText;
	sf::Text bombText;
	sf::Text smiley;

	// Time
	sf::Time time;

	// Clock
	sf::Clock clock;

	// Shapes
	sf::RectangleShape cellShape;
	sf::RectangleShape flagShape;
	sf::CircleShape smileyBackground;

	// Window stuff
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	unsigned windowHeight = modes[0].height;
	unsigned windowWidth; 
};

