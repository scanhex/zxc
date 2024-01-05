#include "Point.h"

#include <algorithm>
#include <cmath>

Point::Point(double x, double y, double z) : x_{x}, y_{y}, z_{z} {}

void Point::normalize() {
    if (fabs(x_) < 1e-8 && fabs(y_) < 1e-8) {
        return;
    }
    double len = norm();
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

bool Point::inRadius(const Point &point, double r) const {
    return inRadius(point.x_, point.y_, r);
}

double Point::normSqr() const {
    return x_ * x_ + y_ * y_;
}

double Point::norm() const {
    return std::sqrt(normSqr());
}

bool Point::normLessThan(double v) const {
    return (normSqr() < v * v);
}

void Point::serialize(BufferIO::BufferWriter &writer) const {
    // z?
    writer.writeDouble(x_);
    writer.writeDouble(y_);
}

void Point::deserialize(BufferIO::BufferReader &reader) {
    // z?
    x_ = reader.readDouble();
    y_ = reader.readDouble();
}

bool Point::isEnoughDistance(const Point &first, double r1, const Point &second, double r2) {
    return (r1 + r2) * (r1 + r2) <=
           (first.x_ - second.x_) * (first.x_ - second.x_) + (first.y_ - second.y_) * (first.y_ - second.y_);
}

double Point::getDistance(const Point &first, const Point &second) {
    return (first.x_ - second.x_) * (first.x_ - second.x_) + (first.y_ - second.y_) * (first.y_ - second.y_);
}

void Point::fitInMap() {
    x_ = std::clamp(x_, -MAP_SIZE, MAP_SIZE);
    y_ = std::clamp(y_, -MAP_SIZE, MAP_SIZE);
}
