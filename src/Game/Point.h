#ifndef ZXC_POINT_H
#define ZXC_POINT_H

class Point final {
public:
    double x_, y_;

    Point(double x, double y);

    bool operator==(Point other) const;
    void operator+=(Point other);

    bool xInRange(double left, double right) const;

    bool yInRange(double left, double right) const;

    bool inRadius(double x, double y, double r) const;
};

#endif //ZXC_POINT_H
