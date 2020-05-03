#pragma once

#include "Point.h"


class Position {
public:
    Position(Point current, double angle);

    void update(double deltaTurn, double deltaMove);
    void updateAngle(double deltaTurn);
    void updatePoint(double deltaMove);

    [[nodiscard]] const Point &getPosition() const;
    void setPosition(const Point &current);
    void setPosition(double x, double y);

    [[nodiscard]] const Point &getDestination() const;
    void setDestination(const Point &destination);
    void setDestination(double x, double y);

    [[nodiscard]] double getAngle() const;
    [[nodiscard]] bool inRadius(double x, double y, double r) const;
    [[nodiscard]] bool inRadius(const Point& point, double r) const;

private:
    void updateDestinationAngle();
    void changeAngle(double delta);

    Point current_;
    Point destination_;
    double currentAngle_;
    double destAngle_;
};
