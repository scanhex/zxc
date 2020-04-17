#include "Point.h"
#include <cmath>

Point::Point(double x, double y, double z) : x_{x}, y_{y}, z_{z} {}

void Point::normalize() {
    if (fabs(x_) < 1e-8 && fabs(y_) < 1e-8) return;
    double len = vectorLength();
    x_ /= len;
    y_ /= len;
}

bool Point::operator==(Point other) const {
    return (x_ == other.x_ && y_ == other.y_);
}

void Point::operator+=(Point other) {
    x_ += other.x_;
    y_ += other.y_;
}

Point Point::operator+(Point other) const {
    Point res = *this;
    res += other;
    return res;
}

void Point::operator-=(Point other) {
    x_ -= other.x_;
    y_ -= other.y_;
}

Point Point::operator-(Point other) const {
    Point res = *this;
    res -= other;
    return res;
}

Point Point::operator*(double factor) const {
    return Point(x_ * factor, y_ * factor);
}

void Point::operator*=(double factor) {
    x_ *= factor;
    y_ *= factor;
}

bool Point::xInRange(double left, double right) const {
    return (x_ >= left && x_ <= right);
}

bool Point::yInRange(double left, double right) const {
    return (y_ >= left && y_ <= right);
}

bool Point::inRadius(double x, double y, double r) const {
    return ((x - x_) * (x - x_) + (y - y_) * (y - y_) <= r * r);
}

double Point::vectorLength() const {
    return std::sqrt(x_ * x_ + y_ * y_);
}

bool Point::vectorLengthIsLessThan(double v) const {
    return (x_ * x_ + y_ * y_ < v * v);
}
