#include "BoundedVector2.h"

///BOUNDED VECTOR2 IMPLEMENTATION///

// \brief Sets the bounds for the vector
// 
// The bounds are limits to where the vector can move
// Lower bound is x and upper bound is y
// 
// \param xBounds	Bounds for the x axis
// \param yBounds	Bounds for the y axis
void BoundedVector2::setBounds(Vector2 xBounds, Vector2 yBounds) {
	this->xBounds = xBounds;
	this->yBounds = yBounds;
}

// \brief Creates a new vector moved to a specified direction
// 
// \param direction Value that indicates the direction of movement for the vector
// 
// \return New moved vector
Vector2* BoundedVector2::moveTo(Direction direction) {
	if (!canMoveTo(direction))
		return nullptr;

	switch (direction) {
	case NORTH:
		return new Vector2(x, y - 1);

	case SOUTH:
		return new Vector2(x, y + 1);

	case EAST:
		return new Vector2(x + 1, y);

	case WEST:
		return new Vector2(x - 1, y);

	default:
		return nullptr;
	}
}

// \brief Checks if the vector can move to the specified direction
// 
// \param direction Value that indicates the direction of movement for the vector
// 
// \return bool Result of the check
bool BoundedVector2::canMoveTo(Direction direction) {
	switch (direction) {
	case NORTH:
		if ((y - 1) < yBounds.x || (y - 1) > yBounds.y - 1)
			return false;
		break;

	case SOUTH:
		if ((y + 1) < yBounds.x || (y + 1) > yBounds.y - 1)
			return false;
		break;

	case EAST:
		if ((x + 1) < xBounds.x || (x + 1) > xBounds.y - 1)
			return false;
		break;

	case WEST:
		if ((x - 1) < xBounds.x || (x - 1) > xBounds.y - 1)
			return false;
		break;

	default:
		return false;
	}
	return true;
}

// \brief Overload of operator =
void BoundedVector2::operator=(const BoundedVector2 other) {
	this->x = other.x;
	this->y = other.y;
	this->xBounds = other.xBounds;
	this->yBounds = other.yBounds;
}