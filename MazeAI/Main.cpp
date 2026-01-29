#include <iostream>

#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "MazeHandler.h"
#include "MazeMap.h"

#define SIZE_X 50
#define SIZE_Y 50

int main() {
    MazeHandler* MH = MazeHandler::GetInstance();

    Vector2* mapSize = new Vector2(SIZE_X, SIZE_Y);
    MazeMap* MM = new MazeMap(*mapSize);

    //DEBUG
    std::vector<int> neighboursMap = MM->GetNeighboursMap();

    for (size_t x = 0; x < SIZE_X; x++) {
        for (size_t y = 0; y < SIZE_Y; y++) {
            std::cout << neighboursMap[MM->parsePosition(new Vector2(x, y))] << " ";
        }
        std::cout << std::endl;
    }


    sf::RenderWindow window(sf::VideoMode({ 500, 500 }), "MazeAI");

    if (!ImGui::SFML::Init(window))
        std::cout << "Error with SFML ImGui window initialization";

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        //input event handling
        while (const std::optional event = window.pollEvent())
        {
            //handles mouse events and other inputs for ImGui
            ImGui::SFML::ProcessEvent(window, event.value());

            //handles window events
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        //update
        ImGui::SFML::Update(window, deltaClock.restart());

        //Main Window Docking implementation
        ImGui::DockSpaceOverViewport();

        window.clear();

        ImGui::Begin("test");
        ImGui::Text("test test test");
        ImGui::End();

        

        //draw SFML
        //window.draw(shape);

        //render and display prepared ImGui and SFML drawing
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
