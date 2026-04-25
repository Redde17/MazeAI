#pragma once
#include <imgui.h>
#include <imgui-SFML.h>

#include "MazeHandler.h"
#include "DataCollector.h"
#include "MazeRenderer.h"
#include "PathFinder.h"

// Singleton for user interface class handler
class GuiHandler {
private:
	static GuiHandler* instance_;	//GuiHandler singleton instance
	static std::mutex mutex_;		//Multi threading safe mechanism

	//Items for maze generation algorithm drop down menu selector
	const char* generationSelectorItems[2] = {
		"Empty",
		"Depth First Search"
	};

	//Items for path finding algorithm drop down menu selector
	const char* searchSelectorItems[2] = {
		"None",
		"Depth First Search"
	};

	MazeHandler* MH;
	MazeRenderer* MR;
	DataCollector* DC;

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
	static GuiHandler* getInstance();

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

	// \brief Draws the window with all the path generation settings
	void drawPathSettings();

	// \brief Draws the window with the maze render
	// \param *sprite: Reference to the sprite to be drawn inside the window
	void drawRenderWindow(sf::Sprite* sprite);

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

	// \brief Event on the click of Find Path button
	// 
	// On find path button click, find a path to the current maze
	// 
	// \param mazeGenereatorSelector: Path finder algorithm for the search
	void onPathFindingButtonClick(const PathFinder::PathFinderAlgorithm pathFinderAlgorithm);
};