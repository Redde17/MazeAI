#pragma once
#include <mutex>

#include "MazeMap.h"
#include "IObservable.h"

//Singleton for accessing the maze
class MazeHandler : public Observable {
public:
    enum MazeGeneratorSelector {
        EMPTY,
        DEPTH_FIRST_SEARCH
    };

private:
	static MazeHandler* instance_;	//MapHandler singleton instance
	static std::mutex mutex_;		//Multi threading safe mechanism

    MazeMap* mazeMap;
protected:
    const std::string value_;

    MazeHandler();
    ~MazeHandler();
public:
    //Makes singleton not clonable and not assignable
    MazeHandler(MazeHandler& other) = delete;
    void operator=(const MazeHandler&) = delete;

    // \brief Access method for the Maze Handler singleton
    // \return MazeHandler* Pointer to the singleton instance
    static MazeHandler* getInstance();

    // \brief Get the maze map handled by the MazeHandler
    // \return MazeMap* Pointer to the maze map instance
    MazeMap* getMazeMap();

    // \brief Generates a maze trough a generating algorithm
    // 
    // \param mazeGenerator: Selector for the type of maze generation
    // \param size: Size of the map to generate
    // \param startPos: Starting position for some generation algorithms, default value (0, 0)
    void generateMazeMap(MazeGeneratorSelector mazeGenerator, const Vector2 size, const Vector2 startPos = Vector2(0, 0));
};

