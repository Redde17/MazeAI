#include "MazeHandler.h"

MazeHandler* MazeHandler::instance_{ nullptr };
std::mutex MazeHandler::mutex_;

//MapHandler constructor
MazeHandler::MazeHandler(){
    //free memory
}

//MapHandler distructor
MazeHandler::~MazeHandler() {
    //free memory
}

MazeHandler* MazeHandler::GetInstance() {
    //lock for multithread safety
    std::lock_guard<std::mutex> lock(mutex_);

    if (instance_ == nullptr)
        instance_ = new MazeHandler();

    return instance_;
}