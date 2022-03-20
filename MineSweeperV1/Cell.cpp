#include "Cell.h"

Cell::Cell()
{
	isClicked = 0;
	isMine = 0;
	isFlagged = 0;
}

void Cell::setMine()
{
	isMine = 1;
}

void Cell::setClicked()
{
	isClicked = 1;
}

void Cell::toggleFlagged()
{
	isFlagged = !isFlagged;
}

bool Cell::getClicked()
{
	return isClicked;
}

bool Cell::getMine()
{
	return isMine;
}

bool Cell::getFlagged()
{
	return isFlagged;
}