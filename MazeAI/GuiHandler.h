#pragma once
#include <imgui.h>
#include <imgui-SFML.h>

#include "MazeHandler.h"

// Singleton for user interface class handler
class GuiHandler {
private:
	static GuiHandler* instance_;	//GuiHandler singleton instance
	static std::mutex mutex_;		//Multi threading safe mechanism

	MazeHandler* MH;
	Vector2 size;
protected:
	//GuiHandler constructor
	GuiHandler();
	//GuiHandler distructor
	~GuiHandler();
public:
	//Makes singleton not clonable and not assignable
	GuiHandler(GuiHandler& other) = delete;
	void operator=(const GuiHandler&) = delete;

	// \brief Access method for the Gui Handler singleton
	// \return GuiHandler* Pointer to the singleton instance
	static GuiHandler* GetInstance();

	// \brief Draws all the UI elements
	// \param *sprite: Reference to the sprite to be drawn inside the RenderWindow
	void drawAll(sf::Sprite* sprite);

	// \brief Draws the window with all the maze settings
	void drawMazeSettings();

	// \brief Draws the window with the maze render
	// \param *sprite: Reference to the sprite to be drawn inside the window
	static void drawRenderWindow(sf::Sprite* sprite);

	// \brief Draws the debg window
	void drawDebugWindow();

private:
	void onChangeMazeSizeButtonClick(const Vector2 newSize);
};

