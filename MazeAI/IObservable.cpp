#include "IObservable.h"

// \brief Add an observer to the notification vector
// \param Observer reference to the observer to be added to the notification vector
void Observable::addObserver(IObserver& observer) {
	observers.push_back(observer);
}

// \brief Remove an observer to the notification vector
// \param Observer reference to the observer to be removed from the notification vector
void Observable::removeObserver(IObserver& observer) {
	observers.erase(std::remove_if(
		observers.begin(), 
		observers.end(), 
		[&observer](const RefObserver& obj) -> bool {
			return &obj.get() == &observer;
		})
	);
}

// \brief Notify all the observers subscribed to the observable object
void Observable::notify() {
	for (RefObserver observer : observers)
		observer.get().update();
}