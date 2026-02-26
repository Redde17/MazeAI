#pragma once
#include "IObserver.h"
#include <vector>

//Observable interface
class IObservable {
public:
	virtual ~IObservable() {}
	virtual void addObserver(IObserver& observer) = 0;
	virtual void removeObserver(IObserver& observer) = 0;
	virtual void notify() = 0;
};

//Observable class with implementation of necessary functions for the Observer pattern desgin
class Observable : public IObservable {
	using RefObserver = std::reference_wrapper<IObserver>;
private:
	std::vector<RefObserver> observers;
public:
	// \brief Observable empty destructor
	virtual ~Observable() {}

	// \brief Add an observer to the notification vector
	// \param Observer reference to the observer to be added to the notification vector
	virtual void addObserver(IObserver& observer);

	// \brief Remove an observer to the notification vector
	// \param Observer reference to the observer to be removed from the notification vector
	virtual void removeObserver(IObserver& observer);

	// \brief Notify all the observers subscribed to the observable object
	virtual void notify();
};
