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
    window.setFramerateLimit(144);

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
        }
        
        // Drawing the board (Will be replaced later)
        board.drawBoard(window);

        // Highlight cell the mouse is currently hovering over (Keep maybe??? -- Possibly replace in the board class)
        board.highlightCell(window, mousePosX, mousePosY);

        window.display();
    }

    return 0;
}