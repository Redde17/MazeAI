#include "MazeHandler.h"

MazeHandler* MazeHandler::instance_{ nullptr };
std::mutex MazeHandler::mutex_;

//MapHandler constructor
MazeHandler::MazeHandler(){
    mazeMap = nullptr;
}

//MapHandler distructor
MazeHandler::~MazeHandler() {
    delete mazeMap;
    delete instance_;
}

// \brief Access method for the Maze Handler singleton
// \return MazeHandler* Pointer to the singleton instance
MazeHandler* MazeHandler::GetInstance() {
    //lock for multithread safety
    std::lock_guard<std::mutex> lock(mutex_);

    if (instance_ == nullptr)
        instance_ = new MazeHandler();

    return instance_;
}

// \brief Get the maze map handled by the MazeHandler
// \return MazeMap* Pointer to the maze map instance can be nullptr
MazeMap* MazeHandler::getMazeMap() {
    return mazeMap;
}

// \brief Generates an empty map with a given size
// 
// Vector2* mapSize will be deleted
// 
// \param mapSize Vector2 with map size passed by reference
void MazeHandler::generateEmptyMazeMap(const Vector2* mapSize) {
    mazeMap = new MazeMap(*mapSize);

    delete mapSize;
}

// \brief Generates an empty map with a given size
// \param mapSize Vector2 with map size passed by value
void MazeHandler::generateEmptyMazeMap(const Vector2 mapSize) {
    mazeMap = new MazeMap(mapSize);
}