#define _USE_MATH_DEFINES

#include "Position.h"

#include <algorithm>
#include <cmath>

Position::Position(Point current, double angle)
    : current_{current},
      initial_{current},
      destination_{current},
      currentAngle_{angle},
      initialAngle_{angle},
      destAngle_{angle} {}

void Position::refresh() {
    current_ = destination_ = initial_;
    currentAngle_ = destAngle_ = initialAngle_;
}

void Position::update(double deltaTurn, double deltaMove) {
    if (current_ == destination_) {
        return;
    }
    updatePoint(deltaMove);
    //  updateAngle(deltaTurn);
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
    if (current_ == destination_) {
        return;
    }
    double delta = std::abs(destAngle_ - currentAngle_);
    if (std::min(delta, 2 * M_PI - delta) > 0) {
        return;
    }

    Point vector = destination_ - current_;
    if (vector.normLessThan(deltaMove)) {
        current_ = destination_;
    } else {
        vector.normalize();
        vector *= deltaMove;
        moveSelf(vector);
    }
}

void Position::updatePointIgnoreAngle(double deltaMove) {
    Point vector = destination_ - current_;
    if (vector.normLessThan(deltaMove)) {
        current_ = destination_;
    } else {
        vector.normalize();
        vector *= deltaMove;
        moveSelf(vector);
    }
}

void Position::updateDestinationAngle() {
    Point vector = destination_ - current_;
    if (vector == Point{0, 0}) {
        return;
    }
    destAngle_ = std::acos(vector.y_ / vector.norm());
    if (vector.x_ > 0) {
        destAngle_ = 2 * M_PI - destAngle_;
    }
}

void Position::serialize(BufferIO::BufferWriter &writer) {
    current_.serialize(writer);
    destination_.serialize(writer);
    writer.writeDouble(currentAngle_);
    writer.writeDouble(destAngle_);
}

void Position::deserialize(BufferIO::BufferReader &reader) {
    current_.deserialize(reader);
    destination_.deserialize(reader);
    currentAngle_ = reader.readDouble();
    destAngle_ = reader.readDouble();
}

// setters and getters

const Point &Position::getPosition() const {
    return current_;
}

void Position::setPosition(const Point &current) {
    current_ = current;
    updateDestinationAngle();
}

void Position::setPosition(double x, double y) {
    current_ = Point(x, y);
    updateDestinationAngle();
}

const Point &Position::getDestination() const {
    return destination_;
}

void Position::setDestination(const Point &destination) {
    destination_ = destination;
    updateDestinationAngle();
}

void Position::setDestination(double x, double y) {
    destination_ = Point(x, y);
    updateDestinationAngle();
}

double Position::getAngle() const {
    return currentAngle_;
}

void Position::setAngle(double angle) {
    currentAngle_ = angle;
    destAngle_ = angle;
}

void Position::changeAngle(double delta) {
    currentAngle_ += delta;
    if (currentAngle_ >= 2 * M_PI) {
        currentAngle_ -= 2 * M_PI;
    }
    if (currentAngle_ <= 0) {
        currentAngle_ += 2 * M_PI;
    }
}

bool Position::inRadius(double x, double y, double r) const {
    return current_.inRadius(x, y, r);
}

bool Position::inRadius(const Point &point, double r) const {
    return current_.inRadius(point, r);
}

Point Position::nextPosition(double deltaMove) const {
    if (current_ == destination_) {
        return current_;
    }
    double delta = std::abs(destAngle_ - currentAngle_);
    if (std::min(delta, 2 * M_PI - delta) > 0) {
        return current_;
    }

    Point vector = destination_ - current_;
    if (vector.normLessThan(deltaMove)) {
        return destination_;
    } else {
        vector.normalize();
        vector *= deltaMove;
        return move(vector);
    }
}

Point Position::move(const Point &delta) const {
    Point res = current_ + delta;
    res.fitInMap();
    return res;
}

void Position::moveSelf(const Point &delta) {
    current_ += delta;
    current_.fitInMap();
}
