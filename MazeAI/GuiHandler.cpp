
#include "GuiHandler.h"
#include <iostream>

GuiHandler* GuiHandler::instance_{ nullptr };
std::mutex GuiHandler::mutex_;

//GuiHandler constructor
GuiHandler::GuiHandler() {
    MH = MH->GetInstance();
    //size = Vector2(
    //    MH->getMazeMap()->getMapSize().x,
    //    MH->getMazeMap()->getMapSize().y
    //);
}

//GuiHandler distructor
GuiHandler::~GuiHandler() {
    delete instance_;
}

// \brief Access method for the Gui Handler singleton
// \return GuiHandler* Pointer to the singleton instance
GuiHandler* GuiHandler::GetInstance() {
    //lock for multithread safety
    std::lock_guard<std::mutex> lock(mutex_);

    if (instance_ == nullptr)
        instance_ = new GuiHandler();

    return instance_;
}

// \brief Draws all the UI elements that do not require data connection
// \param *sprite: Reference to the sprite to be drawn inside the RenderWindow
void GuiHandler::drawAll(sf::Sprite* sprite) {
    drawRenderWindow(sprite);
    drawMazeSettings();
    drawDebugWindow();
}

// \brief Draws the window with all the maze settings
void GuiHandler::drawMazeSettings() {
    //Size input variables
    static Vector2 size(
        MH->getMazeMap()->getMapSize().x,
        MH->getMazeMap()->getMapSize().y
    );

    //Generation algorithm selector variables
    static const char* current_item = generationSelectorItems[0];
    static int generationSelection = 0;

    ImGui::Begin("Maze Settings");

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
    
    ///Generation algorithm selector
    ImGui::PopItemWidth();
    ImGui::Text("\nMaze generation method:");

    //Drop down menu with generationSelectorItems[] for items
    if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
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

    //cell display size

    //Generate Maze button
    ImGui::Text("\n");
    if (ImGui::Button("Generate Maze")) {
        switch (generationSelection) {
            case 0:
                onGenerateMazeButtonClick(size, MazeHandler::Empty);
                break;
            case 1:
                onGenerateMazeButtonClick(size, MazeHandler::DepthFirstSearch);
                break;
            default:
                onGenerateMazeButtonClick(size, MazeHandler::Empty);
                break;
        }
    }
    ImGui::End();
}

// \brief Draws the window with the maze render
// \param *sprite: Reference to the sprite to be drawn inside the window
void GuiHandler::drawRenderWindow(sf::Sprite* sprite) {
    ImGui::Begin("Maze", NULL, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Image(*sprite);
    ImGui::End();
}

// \brief Draws the debg window
void GuiHandler::drawDebugWindow() {
    ImGui::Begin("Debug Window");
    ImGui::Text("Current Maze Size:");
    ImGui::Text("x:[%d] y:[%d]", MH->getMazeMap()->getMapSize().x, MH->getMazeMap()->getMapSize().y);
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
