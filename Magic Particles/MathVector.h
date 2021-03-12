#pragma once
#include <math.h>

class MathVector {

public:
	MathVector();
	MathVector(double x, double y);

	double x;
	double y;

	MathVector getUnitVector();
	double getMagnitude();

	MathVector operator+(const MathVector&);
	MathVector operator-(const MathVector&);
	MathVector operator*(const double);
	MathVector operator/(const double);
};

