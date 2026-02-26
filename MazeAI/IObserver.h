#pragma once

//Observer interface for observer pattern design
class IObserver {
public:
	virtual ~IObserver() {}
	virtual void update() = 0;
};

