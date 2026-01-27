#pragma once
#include <mutex>

//Singleton
class MazeHandler {
private:
	static MazeHandler* instance_;	//MapHandler singleton instance
	static std::mutex mutex_;		//Multi threading safe mechanism

protected:
    const std::string value_;

    MazeHandler();
    ~MazeHandler();
public:
    //Makes singleton not clonable and not assignable
    MazeHandler(MazeHandler& other) = delete;
    void operator=(const MazeHandler&) = delete;

    //Access method to the singleton
    static MazeHandler* GetInstance();
};

