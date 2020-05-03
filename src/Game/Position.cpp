#define _USE_MATH_DEFINES

#include "Position.h"
#include <cmath>
#include <algorithm>

Position::Position(Point current, double angle) : current_{current},
                                                  destination_{current},
                                                  currentAngle_{angle},
                                                  destAngle_{angle} {}

void Position::update(double deltaTurn, double deltaMove) {
    if (current_ == destination_) return;
    updateAngle(deltaTurn);
    updatePoint(deltaMove);
}

void Position::updateAngle(double deltaTurn) {
    if (std::abs(destAngle_ - currentAngle_) <= M_PI) {
        if (std::abs(destAngle_ - currentAngle_) < deltaTurn) {
            currentAngle_ = destAngle_;
        } else {
            changeAngle(destAngle_ > currentAngle_ ? deltaTurn : -deltaTurn);
        }
    } else {
        if (2 * M_PI - std::abs(destAngle_ - currentAngle_) < deltaTurn) {
            currentAngle_ = destAngle_;
        } else {
            changeAngle(destAngle_ > currentAngle_ ? -deltaTurn : deltaTurn);
        }
    }
}

void Position::updatePoint(double deltaMove) {
    double delta = std::abs(destAngle_ - currentAngle_);
    if (std::min(delta, 2 * M_PI - delta) > 0) return;

    Point vector = destination_ - current_;
    if (vector.normLessThan(deltaMove)) {
        current_ = destination_;
    } else {
        vector.normalize();
        vector *= deltaMove;
        current_ += vector;
    }
}

void Position::updateDestinationAngle() {
    Point vector = destination_ - current_;
    destAngle_ = std::acos(vector.y_ / vector.norm());
    if (vector.x_ > 0) {
        destAngle_ = 2 * M_PI - destAngle_;
    }
}


// setters and getters

const Point &Position::getPosition() const { return current_; }

void Position::setPosition(const Point &current) {
    current_ = current;
    updateDestinationAngle();
}
void Position::setPosition(double x, double y) {
    current_ = Point(x, y);
    updateDestinationAngle();
}

const Point &Position::getDestination() const { return destination_; }

void Position::setDestination(const Point &destination) {
    destination_ = destination;
    updateDestinationAngle();
}

void Position::setDestination(double x, double y) {
    destination_ = Point(x, y);
    updateDestinationAngle();
}

double Position::getAngle() const { return currentAngle_; }

void Position::changeAngle(double delta) {
    currentAngle_ += delta;
    if (currentAngle_ >= 2 * M_PI) currentAngle_ -= 2 * M_PI;
    if (currentAngle_ <= 0) currentAngle_ += 2 * M_PI;
}
