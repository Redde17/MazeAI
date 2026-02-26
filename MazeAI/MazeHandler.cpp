#include "MazeHandler.h"
#include "MazeGenerator.h"

MazeHandler* MazeHandler::instance_{ nullptr };
std::mutex MazeHandler::mutex_;

//MapHandler constructor
MazeHandler::MazeHandler(){
    mazeMap = new MazeMap(Vector2(0, 0));
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

// \brief Generates a maze trough a generating algorithm
// \param mazeGenerator selector for the type of maze generation
void MazeHandler::generateMazeMap(MazeGeneratorSelector mazeGenerator, const Vector2 size, const Vector2 startPos) {
    switch (mazeGenerator){
        case MazeHandler::Empty:
            MazeGenerator::generateMazeEmpty(mazeMap, size);
            break;

        case MazeHandler::DepthFirstSearch:
            MazeGenerator::generateMazeEmpty(mazeMap, size);
            MazeGenerator::generateMazeDFS(mazeMap, startPos);
            break;

        default:
            break;
    }
}