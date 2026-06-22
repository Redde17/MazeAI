#include <iostream>

#include "MazeHandler.h"
#include "MazeMap.h"
#include "MazeRenderer.h"
#include "GuiHandler.h"
#include "DataCollector.h"
#include "MazePath.h"
#include "PathFinder.h"

//Default settings
#define DEFAULT_SIZE_X 25
#define DEFAULT_SIZE_Y 25

//Benchmark mode for headless performance testing
//  true = ENABLED 
//  false = DISABLED
#define BENCHMARK_MODE false

//Benchmark settings
#define BENCHMARK_SIZE_X 5000
#define BENCHMARK_SIZE_Y 5000
#define BENCHMARK_RUNS 10
#define BENCHMARK_GENERATION_ALGORITHM MazeHandler::MazeGeneratorSelector::DEPTH_FIRST_SEARCH
#define BENCHMARK_PATH_FINDING_ALGORITHM PathFinder::PathFinderAlgorithm::DEPTH_FIRST_SEARCH

void runBenchmark();

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
    MazeRenderer mazeRenderer(MH->getMazeMap(), (sf::VideoMode::getDesktopMode().size.x / 4) / DEFAULT_SIZE_X);

    //Pass the mazeRenderer to the GuiHandler singleton
    GH->setMazeRenderer(&mazeRenderer);

    //Adds mazeRenderer as an observer for the mazeHandler in case of map changes for redrawing
    MH->addObserver(mazeRenderer);

    //If the program is in benchmark mode run the benchmark and close the program
    if (BENCHMARK_MODE == true) {
        runBenchmark();
        return 0;
    }

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

//Benchmark function to automatically evaluate algorithm performances over several runs
void runBenchmark() {
    MazeHandler* MH = MazeHandler::getInstance();
    DataCollector* DC = DataCollector::getInstance();

    std::cout << "\nRunning benchmark mode with current settings:"
        << "\n\tMaze size: (" << BENCHMARK_SIZE_X << ", " << BENCHMARK_SIZE_Y << ")"
        << "\n\tNumber of runs: " << BENCHMARK_RUNS
        << "\n\tGeneration type: " << BENCHMARK_GENERATION_ALGORITHM
        << "\n\tPath finding algorithm: " << BENCHMARK_PATH_FINDING_ALGORITHM
        << std::endl;

    double mazePathFindingTotalTime = 0;
    double mazeGenerationTotalTime = 0;
    for (int i = 0; i < BENCHMARK_RUNS; i++)
    {
        std::cout << "Running run [" << i << "] : ";

        //Create maze
        MH->generateMazeMap(BENCHMARK_GENERATION_ALGORITHM, Vector2(BENCHMARK_SIZE_X, BENCHMARK_SIZE_Y));

        //Solve maze
        PathFinder::findPath(
            BENCHMARK_PATH_FINDING_ALGORITHM,
            Vector2(0, 0),
            (MH->getMazeMap()->getMapSize() - Vector2(1, 1)),
            MH->getMazeMap()->getMazePath(),
            MH->getMazeMap()->getMazeNode(Vector2(0, 0))
        );

        //Retrieve times
        double mazeGenerationTime = DC->getTime(DataCollector::MAZE_GENERATION).count();
        double mazePathFindingTime = DC->getTime(DataCollector::PATH_FINDING).count();

        mazeGenerationTotalTime += mazeGenerationTime;
        mazePathFindingTotalTime += mazePathFindingTime;

        std::cout << "Completed in [" << mazeGenerationTime
            << " ms] + [" 
            << mazePathFindingTime
            << " ms] = " << (mazeGenerationTime + mazePathFindingTime) << " ms" << std::endl;
    }

    std::cout << "Benchmark terminated:"
        << "\n\tAvg generation time: " << mazeGenerationTotalTime / BENCHMARK_RUNS << "ms"
        << "\n\tAvg path finding time : " << mazePathFindingTotalTime / BENCHMARK_RUNS << "ms"
        << std::endl;
}