#pragma once

class Cell
{
public:

	// Default Constructor
	Cell();

	// Member Set Functions
	void setMine();
	void setClicked();
	void setClickable();
	void toggleFlagged();
	void setMinesAround(unsigned a);
	void setAroundRevealed();

	// Member Output Functions
	bool getClicked();
	bool getClickable();
	bool getMine();
	bool getFlagged();
	bool getAroundRevealed();
	unsigned getMinesAround();

private:	
	bool isClicked;
	bool isMine;
	bool isFlagged;
	bool minesAroundRevealed;
	bool clickable;
	unsigned minesAround;
};

