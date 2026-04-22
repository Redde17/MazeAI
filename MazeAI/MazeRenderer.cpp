#include "MazeRenderer.h"
#include <iostream>
#include "MazeHandler.h"
#include "DataCollector.h"

// \brief MazeRenderer empty constructor
MazeRenderer::MazeRenderer() {
	mazeMap = nullptr;
	mazeSprite = nullptr;
	tileSize = 0;
}

// \brief MazeRenderer constructor
// 
// Creates an instance of MazeRendere with a rendered
// mazeTexture from mazeMap
// 
// \param mazeMap	Pointer to the maze map to render
// \param tileSize	Size of each tile on the map, default value is 10.f
MazeRenderer::MazeRenderer(MazeMap* mazeMap, const float tileSize) {
	this->mazeMap = mazeMap;
	this->tileSize = tileSize;
	textureSize = sf::Vector2u (
		mazeMap->getMapSize().x * tileSize,
		mazeMap->getMapSize().y * tileSize
	);

	//Render all layers and merge them into mazeTexture
	renderLayers();
	//Update maze sprite
	updateSprite();
}

// \brief Get the maze sprite for drawing
// \return Pointer to a Sprite of the maze map
sf::Sprite* MazeRenderer::getMazeSprite() {
	return mazeSprite;
}

// \brief Set a new cell size value
// \param newSize value of the new size to apply to the cell
void MazeRenderer::resize(const float newSize) {
	tileSize = newSize;	//save new size

	textureSize = sf::Vector2u(
		mazeMap->getMapSize().x * tileSize,
		mazeMap->getMapSize().y * tileSize
	);

	//Render all layers and merge them into mazeTexture
	renderLayers();
	//Update maze sprite
	updateSprite();
}

//TODO: comment
void MazeRenderer::update() {
	//recreate sprite
	mazeMap = MazeHandler::getInstance()->getMazeMap();

	textureSize = sf::Vector2u(
		mazeMap->getMapSize().x * tileSize,
		mazeMap->getMapSize().y * tileSize
	);

	//Render all layers and merge them into mazeTexture
	renderLayers();
	//Update maze sprite
	updateSprite();
}

// \brief Render the bacgkround layer
void MazeRenderer::renderBackgroundLayer() {
	//Resize background layer and color it white
	backgroundTexture.resize(textureSize);
	backgroundTexture.clear(backgroundColor);
}

// \brief Render the maze path layer
void MazeRenderer::renderPathLayer() {

}

// \brief Render the maze walls layer
void MazeRenderer::renderWallsLayer() {
	sf::Vector2 mapSize(mazeMap->getMapSize().x, mazeMap->getMapSize().y);
	float wallSize = tileSize / 12;
	//Size of vertex array is: number of tiles * number of walls * number of vertices for wall
	sf::VertexArray vertexMap(sf::PrimitiveType::Triangles, mapSize.x * mapSize.y * 4 * 6);

	int vertexCounter = 0;
	MazeNode* mazeNode = nullptr;
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			mazeNode = mazeMap->getMazeNode(Vector2(x, y));
			if (!mazeNode->getNeighbour(NORTH)) {
				//if there is no neighbour to the north, render wall
				vertexMap[vertexCounter++].position = sf::Vector2f(x * tileSize, y * tileSize);
				vertexMap[vertexCounter++].position = sf::Vector2f(x * tileSize, y * tileSize + wallSize);
				vertexMap[vertexCounter++].position = sf::Vector2f((x + 1) * tileSize, y * tileSize + wallSize);

				vertexMap[vertexCounter++].position = sf::Vector2f(x * tileSize, y * tileSize);
				vertexMap[vertexCounter++].position = sf::Vector2f((x + 1) * tileSize, y * tileSize);
				vertexMap[vertexCounter++].position = sf::Vector2f((x + 1) * tileSize, y * tileSize + wallSize);
			}
			if (!mazeNode->getNeighbour(SOUTH)) {
				//if there is no neighbour to the south, render wall
				vertexMap[vertexCounter++].position = sf::Vector2f(x * tileSize, (y + 1) * tileSize - wallSize);
				vertexMap[vertexCounter++].position = sf::Vector2f(x * tileSize, (y + 1) * tileSize);
				vertexMap[vertexCounter++].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize);

				vertexMap[vertexCounter++].position = sf::Vector2f(x * tileSize, (y + 1) * tileSize - wallSize);
				vertexMap[vertexCounter++].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize - wallSize);
				vertexMap[vertexCounter++].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize);
			}
			if (!mazeNode->getNeighbour(EAST)) {
				//if there is no neighbour to the east, render wall
				vertexMap[vertexCounter++].position = sf::Vector2f((x + 1) * tileSize - wallSize, y * tileSize);
				vertexMap[vertexCounter++].position = sf::Vector2f((x + 1) * tileSize - wallSize, (y + 1) * tileSize);
				vertexMap[vertexCounter++].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize);

				vertexMap[vertexCounter++].position = sf::Vector2f((x + 1) * tileSize - wallSize, y * tileSize);
				vertexMap[vertexCounter++].position = sf::Vector2f((x + 1) * tileSize, y * tileSize);
				vertexMap[vertexCounter++].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize);
			}
			if (!mazeNode->getNeighbour(WEST)) {
				//if there is no neighbour to the west, render wall
				vertexMap[vertexCounter++].position = sf::Vector2f(x * tileSize, y * tileSize);
				vertexMap[vertexCounter++].position = sf::Vector2f(x * tileSize, (y + 1) * tileSize);
				vertexMap[vertexCounter++].position = sf::Vector2f(x * tileSize + wallSize, (y + 1) * tileSize);

				vertexMap[vertexCounter++].position = sf::Vector2f(x * tileSize, y * tileSize);
				vertexMap[vertexCounter++].position = sf::Vector2f(x * tileSize + wallSize, y * tileSize);
				vertexMap[vertexCounter++].position = sf::Vector2f(x * tileSize + wallSize, (y + 1) * tileSize);
			}
		}
	}

	vertexMap.resize(vertexCounter);
	for(int i = 0; i < vertexCounter; i++)
		vertexMap[i].color = wallColor;

	//render vertexMap into a texture
	wallsTexture.clear(sf::Color::Transparent);
	wallsTexture.resize(textureSize);
	wallsTexture.draw(vertexMap);
}

// \brief Render all the maze layers and the maze texture
void MazeRenderer::renderLayers() {
	//Render all the layers
	DataCollector* DC = DataCollector::getInstance();

	//Render bacgkround layer
	renderBackgroundLayer();

	//Render path layer
	DC->chronoTime(DataCollector::START, DataCollector::RENDERING_PATH_LAYER); //Start timing generation
	//renderPathLayer();
	DC->chronoTime(DataCollector::STOP, DataCollector::RENDERING_PATH_LAYER); //Start timing generation

	//Render wall layer
	DC->chronoTime(DataCollector::START, DataCollector::RENDERING_WALL_LAYER); //Start timing generation
	renderWallsLayer();
	DC->chronoTime(DataCollector::STOP, DataCollector::RENDERING_WALL_LAYER); //Start timing generation

	//Resize and draw all layers onto mazeTexture
	mazeTexture.resize(textureSize);
	mazeTexture.draw(sf::Sprite(backgroundTexture.getTexture()));
	//mazeTexture.draw(sf::Sprite(pathTexture.getTexture()));
	mazeTexture.draw(sf::Sprite(wallsTexture.getTexture()));
}

// \brief Updates the sprite, if needed deletes old sprite
void MazeRenderer::updateSprite() {
	if(mazeSprite)
		delete mazeSprite;	//delete old mazeSprite
	mazeSprite = new sf::Sprite(mazeTexture.getTexture());
}