#include "GuiHandler.h"
#include <iostream>


GuiHandler* GuiHandler::instance_{ nullptr };
std::mutex GuiHandler::mutex_;

//GuiHandler constructor
GuiHandler::GuiHandler() {
    MH = MazeHandler::getInstance();
    MR = nullptr;
    DC = DataCollector::getInstance();
}

//GuiHandler distructor
GuiHandler::~GuiHandler() {
    delete instance_;
}

// \brief Access method for the Gui Handler singleton
// \return GuiHandler* Pointer to the singleton instance
GuiHandler* GuiHandler::getInstance() {
    //lock for multithread safety
    std::lock_guard<std::mutex> lock(mutex_);

    if (instance_ == nullptr)
        instance_ = new GuiHandler();

    return instance_;
}

// \brief Setter for the maze renderer
// \param mazeRenderer: Pointer to the maze renderer to set
void GuiHandler::setMazeRenderer(MazeRenderer* mazeRenderer) {
    MR = mazeRenderer;
}

// \brief Getter of the maze renderer
// \return MazeRenderer*: Pointer to the maze renderer used by the Gui Handler
MazeRenderer* GuiHandler::getMazeRenderer() {
    return MR;
}

// \brief Draws all the UI elements that do not require data connection
// \param *sprite: Reference to the sprite to be drawn inside the RenderWindow
void GuiHandler::drawAll(sf::Sprite* sprite) {
    drawRenderWindow(sprite);
    drawMazeSettings();
    drawPathSettings();
    drawDebugWindow();
}

// \brief Draws the window with all the maze settings
void GuiHandler::drawMazeSettings() {
    ImGui::Begin("Maze Settings");

    //Size input variables
    static Vector2 size(
        MH->getMazeMap()->getMapSize().x,
        MH->getMazeMap()->getMapSize().y
    );

    ///Input field size change
    ImGui::PushItemWidth(120.f);
    ImGui::Text("Maze size:");
    //X size input field
    ImGui::Text("x:");
    ImGui::SameLine();
    ImGui::InputInt("##01", &size.x);
    ImGui::SameLine();

    //Y size input field
    ImGui::Text("y:");
    ImGui::SameLine();
    ImGui::InputInt("##02", &size.y);
    

    //Generation algorithm selector variables
    static const char* current_item = generationSelectorItems[0];
    static int generationSelection = 0;

    ///Generation algorithm selector
    ImGui::PopItemWidth();
    ImGui::Text("\nMaze generation method:");

    //Drop down menu with generationSelectorItems[] for items
    if (ImGui::BeginCombo("##generation_combo", current_item)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(generationSelectorItems); n++) {
            bool is_selected = (current_item == generationSelectorItems[n]);
            if (ImGui::Selectable(generationSelectorItems[n], is_selected)) {
                current_item = generationSelectorItems[n];
                generationSelection = n;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    //Generate Maze button
    ImGui::Text("\n");
    if (ImGui::Button("Generate Maze")) {
        switch (generationSelection) {
            case 0: //Option: Empty
                onGenerateMazeButtonClick(size, MazeHandler::EMPTY);
                break;
            case 1: //Option: Depth First Search
                onGenerateMazeButtonClick(size, MazeHandler::DEPTH_FIRST_SEARCH);
                break;
            default:
                onGenerateMazeButtonClick(size, MazeHandler::EMPTY);
                break;
        }
    }

    //Cell display size for the maze
    static float renderSize = 40.f;
    static float oldRenderSize = renderSize;

    ImGui::Text("\nMaze zoom:");
    ImGui::SliderFloat("##FloatSlider", &renderSize, 1.0f, 50.0f);

    if (renderSize != oldRenderSize) {
        //render size changed
        MR->resize(renderSize);

        oldRenderSize = renderSize;
    }

    ImGui::End();
}

// \brief Draws the window with all the path generation settings
void GuiHandler::drawPathSettings() {
    ImGui::Begin("Path Settings");
    //Generation algorithm selector variables
    static const char* current_item = searchSelectorItems[0];
    static int pathFinderSelection = 0;

    ///Generation algorithm selector
    ImGui::Text("\nPath search algorithm:");

    //Drop down menu with generationSelectorItems[] for items
    if (ImGui::BeginCombo("##search_combo", current_item)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(searchSelectorItems); n++) {
            bool is_selected = (current_item == searchSelectorItems[n]);
            if (ImGui::Selectable(searchSelectorItems[n], is_selected)) {
                current_item = searchSelectorItems[n];
                pathFinderSelection = n;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    //Search path button
    ImGui::Text("\n");
    if (ImGui::Button("Search path")) {
        switch (pathFinderSelection) {
        case 0: //Option: Empty
            onPathFindingButtonClick(PathFinder::NONE);
            break;
        case 1: //Option: Depth First Search
            onPathFindingButtonClick(PathFinder::DEPTH_FIRST_SEARCH);
            break;
        default:
            onPathFindingButtonClick(PathFinder::NONE);
            break;
        }
    }
    ImGui::End();
}

// \brief Draws the window with the maze render
// \param *sprite: Reference to the sprite to be drawn inside the window
void GuiHandler::drawRenderWindow(sf::Sprite* sprite) {
    DC->chronoTime(DataCollector::START, DataCollector::DRAW_TIME);

    ImGui::Begin("Maze", NULL, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Image(*sprite);
    ImGui::End();

    DC->chronoTime(DataCollector::STOP, DataCollector::DRAW_TIME);
}

// \brief Draws the debg window
void GuiHandler::drawDebugWindow() {
    ImGui::Begin("Debug Window");
    ImGui::Text("Current Maze Size:");
    ImGui::Text("\tx:[%d] y:[%d]", MH->getMazeMap()->getMapSize().x, MH->getMazeMap()->getMapSize().y);

    ImGui::Text("\nTimers: ");
    ImGui::Text("\tMaze generation: [%d ms]", DC->getTime(DataCollector::MAZE_GENERATION).count());
    ImGui::Text("\tPath finding: [%d ms]", DC->getTime(DataCollector::PATH_FINDING).count());
    ImGui::Text("\tRendering: [%d ms]", DC->getTime(DataCollector::RENDERING_WALL_LAYER).count() + DC->getTime(DataCollector::RENDERING_PATH_LAYER).count());
    ImGui::Text("\t\tWall layer: [%d ms]", DC->getTime(DataCollector::RENDERING_WALL_LAYER).count());
    ImGui::Text("\t\tPath Layer: [%d ms]", DC->getTime(DataCollector::RENDERING_PATH_LAYER).count());
    ImGui::Text("\tDraw Time: [%d ms]", DC->getTime(DataCollector::DRAW_TIME).count());

    ImGui::End();
}

// \brief Event on the click of Generate Maze button
// 
// On generate maze button click generate a new maze with selected generator and with new size 
// 
// \param newSize: Size to use for the new maze
// \param mazeGenereatorSelector: Generation type for the new maze
void GuiHandler::onGenerateMazeButtonClick(const Vector2 newSize, MazeHandler::MazeGeneratorSelector mazeGenereatorSelector) {
    //generate new Maze with passed parameters from the UI
    MH->generateMazeMap(mazeGenereatorSelector, newSize);

    //Notify observers of MazeHandler of change
    MH->notify();
}

// \brief Event on the click of Find Path button
// 
// On find path button click, find a path to the current maze
// 
// \param mazeGenereatorSelector: Path finder algorithm for the search
void GuiHandler::onPathFindingButtonClick(const PathFinder::PathFinderAlgorithm pathFinderAlgorithm) {
    //TODO: Add support for starting position and finish position selection
    PathFinder::findPath(
        pathFinderAlgorithm,
        Vector2(0, 0),
        (MH->getMazeMap()->getMapSize() - Vector2(1, 1)),
        MH->getMazeMap()->getMazePath(),
        MH->getMazeMap()->getMazeNode(Vector2(0, 0))
    );

    MH->notify();
}