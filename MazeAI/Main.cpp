#include <iostream>

#include "MazeHandler.h"
#include "MazeMap.h"
#include "MazeRenderer.h"
#include "GuiHandler.h"

#define DEFAULT_SIZE_X 25
#define DEFAULT_SIZE_Y 25

int main() {
    //Get instance of MazeHandler singleton
    MazeHandler* MH = MazeHandler::GetInstance();
    
    //generate a maze map
    MH->generateMazeMap(MazeHandler::DepthFirstSearch, Vector2(DEFAULT_SIZE_X, DEFAULT_SIZE_Y));

    //Get gui handler singleton reference
    GuiHandler* GH = GuiHandler::GetInstance();

    MazeRenderer mazeRenderer(MH->getMazeMap(), 40.f);

    GH->setMazeRenderer(&mazeRenderer);

    //Adds mazeRenderer as an observer for the mazeHandler in case of map changes for redrawing
    MH->addObserver(mazeRenderer);

    //Define window with settings
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    //set window size to half the size of the monitor
    sf::VideoMode windowSize = sf::VideoMode({ 
        sf::VideoMode::getDesktopMode().size.x / 2,
        sf::VideoMode::getDesktopMode().size.y / 2 
        });

    //create window with windowSize and settings
    sf::RenderWindow window(windowSize, "MazeAI", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(160);
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window))
        std::cout << "Error with SFML ImGui window initialization";

    //ImGui docking flag 
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

        //Draw all the ImGui windows
        GH->drawAll(mazeRenderer.getMazeSprite());

        //Clear window for next frame render
        window.clear();
        //render and display prepared ImGui and SFML drawing
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}