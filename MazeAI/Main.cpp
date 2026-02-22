#include <iostream>

#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "MazeHandler.h"
#include "MazeMap.h"
#include "MazeRenderer.h"

#define SIZE_X 25
#define SIZE_Y 25

void drawImGui(sf::Sprite* sprite);
void sideBar();
void mainRenderWindow(sf::Sprite* sprite);
void debugWindow(std::string intMazeMap);

MazeHandler* MH = MazeHandler::GetInstance();

int main() {
    MH->generateEmptyMazeMap(Vector2(SIZE_X, SIZE_Y));
    MH->generateMazeMap(MazeHandler::DepthFirstSearch);
    MazeRenderer mazeRenderer(MH->getMazeMap(), 40.f);

    //DEBUG
    std::string intMazeMap;
    std::vector<int> mazeMap = MH->getMazeMap()->getMazeMap();
    int counter = 0;
    for (int x : mazeMap) {
        if (counter == SIZE_Y * 3) {
            intMazeMap.append("\n");
            //std::cout << std::endl;
            counter = 0;
        }
        if (x)
            intMazeMap.append("#");
        else
            intMazeMap.append(" ");
        counter++;
    }


    //Define window with settings
    sf::ContextSettings settings;
    //settings.antiAliasingLevel = 8;

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
        drawImGui(mazeRenderer.getMazeSprite());
        debugWindow(intMazeMap);

        //Clear window for next frame render
        window.clear();
        //render and display prepared ImGui and SFML drawing
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

//ImGui windows
void drawImGui(sf::Sprite* sprite) {
    sideBar();
    mainRenderWindow(sprite);
}

void sideBar() {
    ImGui::Begin("test");
    ImGui::Text("test test test");
    ImGui::End();
}

void mainRenderWindow(sf::Sprite* sprite) {
    ImGui::Begin("Maze", NULL, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Image(*sprite);
    ImGui::End();
}

void debugWindow(std::string intMazeMap) {
    ImGui::Begin("Debug Window");
    ImGui::Text(intMazeMap.data());
    ImGui::End();
}