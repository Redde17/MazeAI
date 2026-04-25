#include <iostream>

#include "MazeHandler.h"
#include "MazeMap.h"
#include "MazeRenderer.h"
#include "GuiHandler.h"
#include "DataCollector.h"
#include "MazePath.h"
#include "PathFinder.h"

#define DEFAULT_SIZE_X 25
#define DEFAULT_SIZE_Y 25

int main() {
    //Get Datacollector singleton instance
    DataCollector* DC = DataCollector::getInstance();

    //Get MazeHandler singleton instance
    MazeHandler* MH = MazeHandler::getInstance();

    //Generate starting map and chrono generation time
    MH->generateMazeMap(MazeHandler::DEPTH_FIRST_SEARCH, Vector2(DEFAULT_SIZE_X, DEFAULT_SIZE_Y));

    //Get GuiHandler singleton instance
    GuiHandler* GH = GuiHandler::getInstance();

    //Instanciate a mazeRenderer
    MazeRenderer mazeRenderer(MH->getMazeMap(), 40.f);

    //Pass the mazeRenderer to the GuiHandler singleton
    GH->setMazeRenderer(&mazeRenderer);

    //Adds mazeRenderer as an observer for the mazeHandler in case of map changes for redrawing
    MH->addObserver(mazeRenderer);

    ///DEBUG
    //MazePath mazePath(Vector2(DEFAULT_SIZE_X, DEFAULT_SIZE_Y));
    /*PathFinder::findPath(
        PathFinder::DEPTH_FIRST_SEARCH,
        Vector2(0, 0),
        Vector2(DEFAULT_SIZE_X - 1, DEFAULT_SIZE_Y - 1),
        MH->getMazeMap()->getMazePath(),
        MH->getMazeMap()->getMazeNode(Vector2(0, 0))
    );

    for (int x = 0; x < DEFAULT_SIZE_X; x++){
        for (int y = 0; y < DEFAULT_SIZE_Y; y++){
            std::cout << " " << MH->getMazeMap()->getMazePath()->getPathMapValue(Vector2(x, y));
        }
        std::cout << std::endl;
    }*/
    ///DEBUG END

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