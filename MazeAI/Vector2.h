#pragma once
#include "MazeNode.h"

//Struct for bidimensional vector rappresentation
struct Vector2 {
public:
	int x;
	int y;

	Vector2() {
		x = y = 0;
	}

	Vector2(const int x, const int y) {
		this->x = x;
		this->y = y;
	}

	//Setters
	//void setX(const int x);
	//void setY(const int y);
	void setVector2(const int x, const int y);
	void setVector2(Vector2 pos);

	// \brief Overload of operator =
	void operator=(const Vector2 other);
	// \brief Overload of operator !=
	bool operator!=(const Vector2 other);
	// \brief Overload of operator ==
	bool operator==(const Vector2 other);
	// \brief Overload of operator <
	bool operator<(const Vector2 other) const noexcept;
	// \brief Overload of operator +
	Vector2 operator+(const Vector2 other);
	// \brief Overload of operator -
	Vector2 operator-(const Vector2 other);
};