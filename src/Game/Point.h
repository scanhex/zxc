#pragma once

class Point final {
public:
    double x_, y_;

    Point(double x, double y);

    bool operator==(Point other) const;
    void operator+=(Point other);

    [[nodiscard]] bool xInRange(double left, double right) const;

    [[nodiscard]] bool yInRange(double left, double right) const;

    [[nodiscard]] bool inRadius(double x, double y, double r) const;
};
