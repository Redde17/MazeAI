#pragma once
#include <imgui.h>
#include <imgui-SFML.h>

#include "MazeHandler.h"
#include "MazeRenderer.h"

// Singleton for user interface class handler
class GuiHandler {
private:
	static GuiHandler* instance_;	//GuiHandler singleton instance
	static std::mutex mutex_;		//Multi threading safe mechanism

	//Items for drop down menu selector
	const char* generationSelectorItems[2] = {
		"Empty",
		"Depth First Search"
	};

	MazeHandler* MH;
	MazeRenderer* MR;
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
	// \return GuiHandler*: Pointer to the singleton instance
	static GuiHandler* GetInstance();

	// \brief Setter for the maze renderer
	// \param mazeRenderer: Pointer to the maze renderer to set
	void setMazeRenderer(MazeRenderer* mazeRenderer);

	// \brief Getter of the maze renderer
	// \return MazeRenderer*: Pointer to the maze renderer used by the Gui Handler
	MazeRenderer* getMazeRenderer();

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
	// \brief Event on the click of Generate Maze button
	// 
	// On generate maze button click generate a new maze with selected generator and with new size 
	// 
	// \param newSize: Size to use for the new maze
	// \param mazeGenereatorSelector: Generation type for the new maze
	void onGenerateMazeButtonClick(const Vector2 newSize, MazeHandler::MazeGeneratorSelector mazeGenereatorSelector);
};

