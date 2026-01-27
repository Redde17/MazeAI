#include <iostream>

#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "MazeHandler.h"

int main() {
    MazeHandler* MH = MazeHandler::GetInstance();



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
