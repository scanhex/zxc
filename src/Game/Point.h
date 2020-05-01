#pragma once

class Point final {
public:
    double x_, y_, z_;

    Point(double x, double y, double z = 0);

    void normalize();

    bool operator==(Point other) const;

    Point operator+(Point other) const;
    void operator+=(Point other);
    Point operator-(Point other) const;
    void operator-=(Point other);
    Point operator*(double factor) const;
    void operator*=(double factor);

    [[nodiscard]] bool xInRange(double left, double right) const;

    [[nodiscard]] bool yInRange(double left, double right) const;

    [[nodiscard]] bool inRadius(double x, double y, double r) const;

    [[nodiscard]] double normSqr() const;
    [[nodiscard]] double norm() const;
    [[nodiscard]] bool normLessThan(double v) const;
};
