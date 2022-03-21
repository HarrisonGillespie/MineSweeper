#include "Cell.h"

Cell::Cell()
{
	isClicked = 0;
	isMine = 0;
	isFlagged = 0;
	minesAround = 0;
	minesAroundRevealed = 0;
	clickable = 1;
}

void Cell::setMine()
{
	isMine = 1;
}

void Cell::setClicked()
{
	isClicked = 1;
}

void Cell::setClickable()
{
	clickable = 0;
}

void Cell::toggleFlagged()
{
	isFlagged = !isFlagged;
}

void Cell::setMinesAround(unsigned a)
{
	minesAround = a;
}

void Cell::setAroundRevealed()
{
	minesAroundRevealed = 1;
}

bool Cell::getClicked()
{
	return isClicked;
}

bool Cell::getClickable()
{
	return clickable;
}

bool Cell::getMine()
{
	return isMine;
}

bool Cell::getFlagged()
{
	return isFlagged;
}

bool Cell::getAroundRevealed()
{
	return minesAroundRevealed;
}

unsigned Cell::getMinesAround()
{
	return minesAround;
}
