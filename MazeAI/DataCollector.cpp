#include "DataCollector.h"
#include <iostream>

DataCollector* DataCollector::instance_{ nullptr };
std::mutex DataCollector::mutex_;

///Timer struct implementation///

// \brieg Get the calculated time from stop - start
// 
// If the start time or the stop time have not been counted return 0ms 
// 
// \return Calculated time in ms
std::chrono::milliseconds DataCollector::Timer::getTime() {
	using Ms = std::chrono::milliseconds;

	//Check if start and stop have been counted, if not return 0ms
	if (!stop.time_since_epoch().count() || !start.time_since_epoch().count())
		return std::chrono::duration_cast<Ms>(start.time_since_epoch().zero());
	
	return std::chrono::duration_cast<Ms>(stop - start);
}


///DataCollector class implementation///
//DataCollector constructor
DataCollector::DataCollector() {
}

//DataCollector distructor
DataCollector::~DataCollector() {
}

// \brief Access method for the DataCollector singleton
// \return DataCollector* Pointer to the singleton instance
DataCollector* DataCollector::getInstance() {
	//lock for multithread safety
	std::lock_guard<std::mutex> lock(mutex_);

	if (instance_ == nullptr)
		instance_ = new DataCollector();

	return instance_;
}

// \brief Stop watch function to track time for various parts of the maze application
//
// \param command: (START, STOP) Command to start or stop time couting
// \param target: Set target for the time counting
void DataCollector::chronoTime(const ChronoControl command, const ChronoTarget target) {
	switch (target){
		case MAZE_GENERATION:
			if(command == START)
				mazeGenerationTime.start = High_resolution_clock::now();
			else
				mazeGenerationTime.stop = High_resolution_clock::now();
			break;
		
		case PATH_FINDING:
			if (command == START)
				mazePathFindingTime.start = High_resolution_clock::now();
			else
				mazePathFindingTime.stop = High_resolution_clock::now();
			break;

		case RENDERING_WALL_LAYER:
			if (command == START)
				mazeWallLayerRenderingTime.start = High_resolution_clock::now();
			else
				mazeWallLayerRenderingTime.stop = High_resolution_clock::now();
			break;

		case RENDERING_PATH_LAYER:
			if (command == START)
				mazePathLayerRenderingTime.start = High_resolution_clock::now();
			else
				mazePathLayerRenderingTime.stop = High_resolution_clock::now();
			break;

		case DRAW_TIME:
			if (command == START)
				mazeDrawTime.start = High_resolution_clock::now();
			else
				mazeDrawTime.stop = High_resolution_clock::now();
			break;

		default:
			break;
	}
}

// \brief Get a specified calculated time
// 
// \param target: Set target for the calculated time to retrieve
// 
// \return Calculated time in ms
std::chrono::milliseconds DataCollector::getTime(const ChronoTarget target) {
	switch (target) {
	case MAZE_GENERATION:
		return mazeGenerationTime.getTime();

	case PATH_FINDING:
		return mazePathFindingTime.getTime();

	case RENDERING_WALL_LAYER:
		return mazeWallLayerRenderingTime.getTime();

	case RENDERING_PATH_LAYER:
		return mazePathLayerRenderingTime.getTime();

	case DRAW_TIME:
		return mazeDrawTime.getTime();

	default:
		return mazeGenerationTime.getTime().zero();
	}
}