#include "Vector2.h"

///VECTOR2 IMPLEMENTATION///

//void Vector2::setX(const int x) {
//	this->x = x;
//}
//
//void Vector2::setY(const int y) {
//	this->y = y;
//}

void Vector2::setVector2(const int x, const int y) {
	this->x = x;
	this->y = y;
}

void Vector2::setVector2(Vector2 pos) {
	x = pos.x;
	y = pos.y;
}

// \brief Overload of operator =
void Vector2::operator=(const Vector2 other) {
	this->x = other.x;
	this->y = other.y;
}

// \brief Overload of operator !=
bool Vector2::operator!=(const Vector2 other) {
	//if (this->x != other.x || this->y != other.y)
	//	return true;
	//return false;

	return ((this->x != other.x) || (this->y != other.y));
}
// \brief Overload of operator ==
bool Vector2::operator==(const Vector2 other) {
	//if (this->x == other.x && this->y == other.y)
	//	return true;
	//return false;

	return ((this->x == other.x) && (this->y == other.y));
}

bool Vector2::operator<(const Vector2 other) const noexcept {
	return ((this->x + this->y) < (other.x + other.y));
}