#include "GlobalVars.h"

currSettings::currSettings() {
    COLUMNS = 20;
    ROWS = 20;
    CELLSIZE = 50;
    NUMBOMBS = 40;
}

unsigned currSettings::getWindowHeight() {
    return COLUMNS * CELLSIZE;
}

unsigned currSettings::getWindowWidth() {
    return ROWS * CELLSIZE;
}

unsigned currSettings::getCellSize() {
    return CELLSIZE;
}

unsigned currSettings::getCols() {
    return COLUMNS;
}

unsigned currSettings::getRows() {
    return ROWS;
}

unsigned currSettings::getBombs() {
    return NUMBOMBS;
}

void currSettings::newSettings(
    unsigned cols,
    unsigned rows,
    unsigned size,
    unsigned bombs) {
    COLUMNS = cols;
    ROWS = rows;
    CELLSIZE = size;
    NUMBOMBS = bombs;
}