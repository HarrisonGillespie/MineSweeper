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

	// Member Output Functions
	bool getClicked();
	bool getMine();
	bool getFlagged();

private:	
	bool isClicked;
	bool isMine;
	bool isFlagged;
};

