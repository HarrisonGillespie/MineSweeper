#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Board.h"
#include "Cell.h"

int main()
{   
    // ImGui Stuff
    static int sizeInput[2] = { 20, 20 };
    static int bombsInput = 99;

    // Initialize Board
    Board board;

    // ImGui Stuff
    ImGui::SFML::Init(board.window);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoSavedSettings;
    window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

    auto TextCentered = [=](std::string text) {
        float windowWidth = ImGui::GetWindowSize().x;
        float textWidth = ImGui::CalcTextSize(text.c_str()).x;

        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text(text.c_str());
    };

    sf::Clock deltaClock;

    while (board.window.isOpen())
    {
        // Get Mouse Positions
        int mousePosX = static_cast<int>(sf::Mouse::getPosition(board.window).x) / board.getCellSize();
        int mousePosY = static_cast<int>(sf::Mouse::getPosition(board.window).y) / board.getCellSize();

        // Possibly reintroduce if (board.getGameOver()) board.openMenu();

        // Event Handling
        sf::Event event; 
        while (board.window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                board.window.close();
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
                    if (!board.getErrorMenu()) {
                        board.toggleMenu();
                    }
                    else {
                        board.toggleError();
                    }
                }
            }
        }
        // ImGui Update
        ImGui::SFML::Update(board.window, deltaClock.restart());

        // ImGui Menu Stuff
        if (board.getDisplayMenu() && !board.getErrorMenu()) {
            ImGui::SetNextWindowPos(ImVec2(board.window.getSize().x / 2.0f, board.window.getSize().y / 2.0f), 0, ImVec2(0.5f,0.5f));
            ImGui::Begin("Menu", NULL, window_flags);
            if (ImGui::Button("Reveal Board")) board.revealBoard();
            ImGui::SameLine();
            if (ImGui::Button("Reset Board")) board.resetBoard();
            TextCentered("Rows, Columns");
            ImGui::SliderInt2("", sizeInput, 16, 40);
            ImGui::InputInt("Bombs", &bombsInput);
            if (ImGui::Button("Resize With New Values")) board.resizeBoard(sizeInput[0], sizeInput[1], bombsInput); 
            if (ImGui::Button("Close Menu")) board.closeMenu();
            ImGui::End();
        }

        if (board.getErrorMenu()) {
            ImGui::SetNextWindowPos(ImVec2(board.window.getSize().x / 2.0f, board.window.getSize().y / 2.0f), 0, ImVec2(0.5f, 0.5f));
            ImGui::Begin("Error in Settings", NULL, window_flags);
            TextCentered("There was an error with your settings");
            TextCentered("Press Escape to close this window");
            ImGui::End();
        }

        ImGui::EndFrame();

        // Drawing the board 
        board.drawBoard(board.window);

        // Highlight cell the mouse is currently hovering over
        // if(!board.getDisplayMenu() && !board.getGameOver()) board.highlightCell(board.window, mousePosX, mousePosY);

        // Game logic -- Possibly move to onClick function 
        board.setCellsRevealed(); // Each iteration check for more revealed cells
        if (board.getCellsRevealed() >= ((board.getCol() * board.getRow()) - board.getNumBombs())) { // Greater than or equal to for debug purposes
            board.setGameWon();
        }

        // Clock stuff
        board.getClockTime(board.window);

        // ImGui Render
        ImGui::SFML::Render(board.window);

        // Disply window
        board.window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}