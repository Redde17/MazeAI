#pragma once
#include <mutex>
#include "MazeMap.h"

//Singleton
class MazeHandler {
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
    static MazeHandler* GetInstance();

    // \brief Get the maze map handled by the MazeHandler
    // \return MazeMap* Pointer to the maze map instance
    MazeMap* getMazeMap();

    // \brief Generates an empty map with a given size
    // 
    // Vector2* mapSize will be deleted
    // 
    // \param mapSize Vector2 with map size passed by reference
    void generateEmptyMazeMap(const Vector2* mapSize);

    // \brief Generates an empty map with a given size
    // \param mapSize Vector2 with map size passed by value
    void generateEmptyMazeMap(const Vector2 mapSize);

};

