#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Board.h"
#include "Cell.h"

int main()
{   
    // Initialize object containing current settings
    Board board;

    // Setup SFML Window
    sf::RenderWindow window(sf::VideoMode(1001, 1001), "MineSweeper", sf::Style::Close);
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        // Get Mouse Positions
        unsigned mousePosX = static_cast<unsigned>(sf::Mouse::getPosition(window).x) / board.getCellSize();
        unsigned mousePosY = static_cast<unsigned>(sf::Mouse::getPosition(window).y) / board.getCellSize();

        // Event Handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            // Mouse Events
            else if (event.type == sf::Event::MouseButtonReleased) {
                // Left Mouse Button Event
                if (event.mouseButton.button == sf::Mouse::Left) {
                    board.onClickLeft(mousePosX, mousePosY);
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    board.onClickRight(mousePosX, mousePosY);
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (!board.getGameOver()) {
                        board.revealBoard();
                    }
                    else {
                        board.resetBoard();
                    }
                }
            }
        }
        
        // Drawing the board 
        board.drawBoard(window);

        // Highlight cell the mouse is currently hovering over 
        board.highlightCell(window, mousePosX, mousePosY);

        // Game logic
        board.setCellsRevealed(); // Each iteration check for more revealed cells
        if (board.getCellsRevealed() >= ((board.getCol() * board.getRow()) - board.getNumBombs())) { // Greater than or equal to for debug purposes
            board.revealBoard();
            board.setGameOver();
        }

        // Clock stuff
        board.getClockTime();

        // Disply window
        window.display();
    }

    return 0;
}