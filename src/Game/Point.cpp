#include "Point.h"

Point::Point(double x, double y, double z) : x_{x}, y_{y}, z_{z} {}

bool Point::operator==(Point other) const {
    return (x_ == other.x_ && y_ == other.y_);
}

void Point::operator+=(Point other) {
    x_ += other.x_;
    y_ += other.y_;
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
