#include "MazeRenderer.h"
#include <iostream>
#include "MazeHandler.h"

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

	generateMazeTexture();
	mazeSprite = new sf::Sprite(mazeTexture.getTexture());
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

	generateMazeTexture();	//generate new mazeTexture with new size

	delete mazeSprite;	//delete old mazeSprite
	mazeSprite = new sf::Sprite(mazeTexture.getTexture());	//create new sprite from new mazeTexture
}

//TODO: comment
void MazeRenderer::update() {
	//recreate sprite
	mazeMap = MazeHandler::GetInstance()->getMazeMap();

	generateMazeTexture();

	delete mazeSprite; //delete old mazeSprite
	mazeSprite = new sf::Sprite(mazeTexture.getTexture()); //create new sprite from new mazeTexture
}

// \brief Generates a maze texture from the maze map
void MazeRenderer::generateMazeTexture() {
	sf::Vertex* tile;
	float wallSize = tileSize / 12;
	MazeNode* mazeNode;

	sf::Vector2 mapSize(mazeMap->getMapSize().x, mazeMap->getMapSize().y);
	sf::VertexArray vertexMap(sf::PrimitiveType::Triangles);	//vertex map containing all the map
	vertexMap.resize(mapSize.x * mapSize.y * (6 * 5));	//vertex map size is map size * 6 * number of possible walls

	bool swap = true;

	int counter  = 0;
	int vertexCounter = 0;
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			//gets the pointer to the first vertex of a tile inside the vertexMap
			tile = &vertexMap[(x * mapSize.y + y) * 6 * 5];

			//define tile vertices for a single tile with size tileSize * tileSize
			tile[counter].position = sf::Vector2f(x * tileSize, y * tileSize);
			tile[++counter].position = sf::Vector2f(x * tileSize, (y + 1) * tileSize);
			tile[++counter].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize);

			tile[++counter].position = sf::Vector2f(x * tileSize, y * tileSize);
			tile[++counter].position = sf::Vector2f((x + 1) * tileSize, y * tileSize);
			tile[++counter].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize);

			//draw walls
			mazeNode = mazeMap->getMazeNode(Vector2(x, y));
			if (!mazeNode->getNeighbour(NORTH)) {
				//draw north wall
				tile[++counter].position = sf::Vector2f(x * tileSize, y * tileSize);
				tile[++counter].position = sf::Vector2f(x * tileSize, y * tileSize + wallSize);
				tile[++counter].position = sf::Vector2f((x + 1) * tileSize, y * tileSize + wallSize);

				tile[++counter].position = sf::Vector2f(x * tileSize, y * tileSize);
				tile[++counter].position = sf::Vector2f((x + 1) * tileSize, y * tileSize);
				tile[++counter].position = sf::Vector2f((x + 1) * tileSize, y * tileSize + wallSize);

				//set black color for walls
				for (size_t i = counter - 5; i < counter + 1; i++)
					tile[i].color = sf::Color::Black;
			}
			if (!mazeNode->getNeighbour(SOUTH)) {
				//draw south wall
				tile[++counter].position = sf::Vector2f(x * tileSize, (y + 1) * tileSize - wallSize);
				tile[++counter].position = sf::Vector2f(x * tileSize, (y + 1) * tileSize);
				tile[++counter].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize);

				tile[++counter].position = sf::Vector2f(x * tileSize, (y + 1) * tileSize - wallSize);
				tile[++counter].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize - wallSize);
				tile[++counter].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize);

				//set black color for walls
				for (size_t i = counter - 5; i < counter + 1; i++)
					tile[i].color = sf::Color::Black;
			}
			if (!mazeNode->getNeighbour(EAST)) {
				//draw east wall
				tile[++counter].position = sf::Vector2f((x + 1) * tileSize - wallSize, y * tileSize);
				tile[++counter].position = sf::Vector2f((x + 1) * tileSize - wallSize, (y + 1) * tileSize);
				tile[++counter].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize);

				tile[++counter].position = sf::Vector2f((x + 1) * tileSize - wallSize, y * tileSize);
				tile[++counter].position = sf::Vector2f((x + 1) * tileSize, y * tileSize);
				tile[++counter].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize);

				//set black color for walls
				for (size_t i = counter - 5; i < counter + 1; i++)
					tile[i].color = sf::Color::Black;
			}
			if (!mazeNode->getNeighbour(WEST)) {
				//draw west wall
				tile[++counter].position = sf::Vector2f(x * tileSize, y * tileSize);
				tile[++counter].position = sf::Vector2f(x * tileSize, (y + 1) * tileSize);
				tile[++counter].position = sf::Vector2f(x * tileSize + wallSize, (y + 1) * tileSize);

				tile[++counter].position = sf::Vector2f(x * tileSize, y * tileSize);
				tile[++counter].position = sf::Vector2f(x * tileSize + wallSize, y * tileSize);
				tile[++counter].position = sf::Vector2f(x * tileSize + wallSize, (y + 1) * tileSize);

				//set black color for walls
				for (size_t i = counter - 5; i < counter + 1; i++)
					tile[i].color = sf::Color::Black;
			}
			vertexCounter += counter + 1;
			counter = 0;
		}
	}

	//std::cout << "DEBUG: vertexCounter[" << vertexCounter << "]" << std::endl;
	//std::cout << "DEBUG: Expected vortex count[" << mapSize.x * mapSize.y * (6 * 5) << "]" << std::endl;

	//render vertexMap into a texture
	sf::Vector2u mazeTextureSize(
		mazeMap->getMapSize().x * tileSize,
		mazeMap->getMapSize().y * tileSize
	);

	mazeTexture.resize(mazeTextureSize);
	mazeTexture.draw(vertexMap);
}