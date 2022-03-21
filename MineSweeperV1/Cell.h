#pragma once

class Cell
{
public:

	// Default Constructor
	Cell();

	// Member Set Functions
	void setMine();
	void setClicked();
	void toggleFlagged();
	void setMinesAround(unsigned a);
	void setAroundRevealed();

	// Member Output Functions
	bool getClicked();
	bool getMine();
	bool getFlagged();
	bool getAroundRevealed();
	unsigned getMinesAround();

private:	
	bool isClicked;
	bool isMine;
	bool isFlagged;
	bool minesAroundRevealed;
	unsigned minesAround;
};

