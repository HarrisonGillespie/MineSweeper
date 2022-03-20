#pragma once

class currSettings 
{
public:
    currSettings();

    void newSettings(
        unsigned cols,
        unsigned rows,
        unsigned size,
        unsigned bombs);

    unsigned getWindowHeight();
    unsigned getWindowWidth();
    unsigned getCellSize();
    unsigned getCols();
    unsigned getRows();
    unsigned getBombs();
private:
    unsigned COLUMNS;
    unsigned ROWS;
    unsigned CELLSIZE;
    unsigned NUMBOMBS;
};

