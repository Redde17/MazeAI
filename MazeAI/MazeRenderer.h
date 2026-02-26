#pragma once
#include <SFML/Graphics.hpp>
#include "MazeMap.h"
#include "IObserver.h"

// Rendere class to draw the maze on screen with SFML
class MazeRenderer : public IObserver {
private:
	MazeMap* mazeMap;
	sf::RenderTexture mazeTexture;
	sf::Sprite* mazeSprite;
	float tileSize;

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

	// \brief Get the maze sprite for drawing
	// \return Pointer to a Sprite of the maze map
	sf::Sprite* getMazeSprite();

	// \brief Set a new cell size value
	// \param newSize value of the new size to apply to the cell
	void resize(const float newSize);

	virtual void update();

private:
	// \brief Generates a maze texture from the maze map
	void generateMazeTexture();
};

