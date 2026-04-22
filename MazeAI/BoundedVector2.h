#pragma once
#include "Vector2.h"

//Struct extending vector2 by adding upper and lower bounds to the axis
struct BoundedVector2 : public Vector2 {
protected:
	Vector2 xBounds;
	Vector2 yBounds;

	using Vector2::Vector2;

public:
	// \brief Sets the bounds for the vector
	// 
	// The bounds are limits to where the vector can move
	// Lower bound is x and upper bound is y
	// 
	// \param xBounds: Bounds for the x axis
	// \param yBounds: Bounds for the y axis
	void setBounds(Vector2 xBounds, Vector2 yBounds);

	// \brief Creates a new vector moved to a specified direction
	// 
	// \param direction: Value that indicates the direction of movement for the vector
	// 
	// \return New moved vector
	Vector2* moveTo(Direction direction);

	// \brief Checks if the vector can move to the specified direction
	// 
	// \param direction: Value that indicates the direction of movement for the vector
	// 
	// \return bool Result of the check
	bool canMoveTo(Direction direction);

	// \brief Overload of operator =
	void operator=(const BoundedVector2 other);
};



