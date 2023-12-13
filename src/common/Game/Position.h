#pragma once

#include "Point.h"
#include "Utils/BufferIO.h"

class Position {
public:
    Position(Point current, double angle);
    Position(const Position &) = default;
    Position &operator=(const Position &) = default;

    void refresh();

    void updateAngle(double deltaTurn);
    void updatePoint(double deltaMove);
    void updatePointIgnoreAngle(double deltaMove);

    [[nodiscard]] const Point &getPosition() const;
    void setPosition(const Point &current);
    void setPosition(double x, double y);

    [[nodiscard]] const Point &getDestination() const;
    void setDestination(const Point &destination);
    void setDestination(double x, double y);

    [[nodiscard]] double getAngle() const;
    void setAngle(double angle);

    [[nodiscard]] bool inRadius(double x, double y, double r) const;
    [[nodiscard]] bool inRadius(const Point &point, double r) const;

    void serialize(BufferIO::BufferWriter &writer);
    void deserialize(BufferIO::BufferReader &reader);

    [[nodiscard]] Point nextPosition(double deltaMove) const;

    [[nodiscard]] Point move(const Point &delta) const;
    void moveSelf(const Point &delta);

private:
    void updateDestinationAngle();
    void changeAngle(double delta);

    Point current_;
    Point initial_;
    Point destination_;
    double currentAngle_;
    double initialAngle_;
    double destAngle_;
};
