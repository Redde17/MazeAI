
#include "GuiHandler.h"
#include <iostream>

GuiHandler* GuiHandler::instance_{ nullptr };
std::mutex GuiHandler::mutex_;

//GuiHandler constructor
GuiHandler::GuiHandler() {
    MH = MH->GetInstance();
    size = Vector2(
        MH->getMazeMap()->getMapSize().x,
        MH->getMazeMap()->getMapSize().y
    );
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
    Vector2 oldSize(
        MH->getMazeMap()->getMapSize().x,
        MH->getMazeMap()->getMapSize().y
    );

    ImGui::Begin("Maze Settings");

    //Input field size change
    ImGui::PushItemWidth(120.f);

    //X size input field
    ImGui::Text("x:");
    ImGui::SameLine();
    ImGui::InputInt("##01", &size.x);

    ImGui::SameLine();

    //Y size input field
    ImGui::Text("y:");
    ImGui::SameLine();
    ImGui::InputInt("##02", &size.y);
    

    if (ImGui::Button("Change Size")) {
        //check if size has changed before handling the click
        if (size != oldSize) {
            onChangeMazeSizeButtonClick(size);
            oldSize = size;
        }
    }

    //cell display size


    //generation algorithm selection

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

void GuiHandler::onChangeMazeSizeButtonClick(const Vector2 newSize) {
    std::cout << "DEBUG: onChangeMazeSizeButtonClick ";
    std::cout << "x[" << newSize.x << "] ";
    std::cout << "y[" << newSize.y << "] ";
    std::cout << std::endl;
    //Create a new maze with a new size
    
    //New maze gets generated but not redrawn 

    MH->generateMazeMap(MazeHandler::Empty, newSize);

    MH->notify();
}
