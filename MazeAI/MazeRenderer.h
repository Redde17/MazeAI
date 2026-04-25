#pragma once
#include <SFML/Graphics.hpp>

#include "MazeMap.h"
#include "IObserver.h"

// Rendere class to draw the maze on screen with SFML
class MazeRenderer : public IObserver {
private:
	sf::Color backgroundColor = sf::Color::White;
	sf::Color pathTileColor = sf::Color::Green;
	sf::Color visitedTileColor = sf::Color::Cyan;
	sf::Color defaultTileColor = sf::Color::White;
	sf::Color wallColor = sf::Color::Black;


	MazeMap* mazeMap;
	float tileSize;
	sf::Vector2u textureSize;

	sf::RenderTexture backgroundTexture;
	sf::RenderTexture pathTexture;
	sf::RenderTexture wallsTexture;
	sf::RenderTexture mazeTexture;
	sf::Sprite* mazeSprite;
public:
	// \brief MazeRenderer empty constructor
	MazeRenderer();

	// \brief MazeRenderer constructor
	// 
	// Creates an instance of MazeRendere with a rendered
	// mazeTexture from mazeMap
	// 
	// \param mazeMap	Pointer to the maze map to render
	// \param tileSize	Size of each tile on the map, default value is 10.f
	MazeRenderer(MazeMap* mazeMap, const float tileSize = 10.f);

	// \brief Get the maze sprite for drawinga
	// \return Pointer to a Sprite of the maze map
	sf::Sprite* getMazeSprite();

	// \brief Set a new cell size value
	// \param newSize value of the new size to apply to the cell
	void resize(const float newSize);

	virtual void update();
private:
	// \brief Render the bacgkround layer
	void renderBackgroundLayer();

	// \brief Render the maze path layer
	void renderPathLayer();

	// \brief Render the maze walls layer
	void renderWallsLayer();

	// \brief Render all the maze layers and the maze texture
	void renderLayers();

	// \brief Updates the sprite, if needed deletes old sprite
	void updateSprite();
};

