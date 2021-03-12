#include "MathVector.h"

MathVector::MathVector() {
    x = 0.0;
    y = 0.0;

}

MathVector::MathVector(double point_x, double point_y) {
    x = point_x;
    y = point_y;
}

MathVector MathVector::getUnitVector()
{
    return MathVector(x, y) / getMagnitude();
}

double MathVector::getMagnitude()
{
    return sqrt(pow(x, 2) + pow(y, 2));
}

MathVector MathVector::operator+(const MathVector& otherVector)
{
    MathVector returnVector;
    returnVector.x = x + otherVector.x;
    returnVector.y = y + otherVector.y;
    return returnVector;
}

MathVector MathVector::operator-(const MathVector& otherVector)
{
    MathVector returnVector;
    returnVector.x = x - otherVector.x;
    returnVector.y = y - otherVector.y;
    return returnVector;
}

MathVector MathVector::operator*(const double scale)
{
    MathVector returnVector;
    returnVector.x = x * scale;
    returnVector.y = y * scale;
    return returnVector;
}

MathVector MathVector::operator/(const double scale)
{
    MathVector returnVector;
    returnVector.x = x / scale;
    returnVector.y = y / scale;
    return returnVector;
}
