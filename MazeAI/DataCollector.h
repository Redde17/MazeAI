#pragma once
#include <mutex>
#include <chrono>

//Class for collecting data from path finding and generation
class DataCollector {
public:
	enum ChronoControl {
		START,
		STOP
	};

	enum ChronoTarget {
		MAZE_GENERATION,
		PATH_FINDING,
		RENDERING_WALL_LAYER,
		RENDERING_PATH_LAYER,
		DRAW_TIME
	};

private:
	using TimePoint = std::chrono::steady_clock::time_point;
	using High_resolution_clock = std::chrono::high_resolution_clock;

	struct Timer {
		TimePoint start;
		TimePoint stop;

		// \brieg Get the calculated time from stop - start
		// 
		// If the start time or the stop time have not been counted return 0ms 
		// 
		// \return Calculated time in ms
		std::chrono::milliseconds getTime();
	};

	static DataCollector* instance_;	//DataCollector singleton instance
	static std::mutex mutex_;		//Multi threading safe mechanism

	//Maze generation info
	Timer mazeGenerationTime;

	//Maze path finding info
	Timer mazePathFindingTime;

	//Maze rendering info
	Timer mazeWallLayerRenderingTime;
	Timer mazePathLayerRenderingTime;

	//Maze draw times
	Timer mazeDrawTime;

protected:
	//DataCollector constructor
	DataCollector();
	//DataCollector distructor
	~DataCollector();

public:
	// \brief Access method for the DataCollector singleton
	// \return DataCollector* Pointer to the singleton instance
	static DataCollector* getInstance();

	// \brief Stop watch function to track time for various parts of the maze application
	//
	// \param command: (START, STOP) Command to start or stop time couting
	// \param target: Set target for the time counting
	void chronoTime(const ChronoControl command, const ChronoTarget target);

	// \brief Get a specified calculated time
	// 
	// \param target: Set target for the calculated time to retrieve
	// 
	// \return Calculated time in ms
	std::chrono::milliseconds getTime(const ChronoTarget target);
};

