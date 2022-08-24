#ifndef SHAPE_H_
#define SHAPE_H_
#include <vector>

enum Half {Lower, Upper};

struct Point {
    double x, y;
    Point(double _x=0.0, double _y=0.0)
        : x{_x}, y{_y}
    {
    }
    bool operator == (const Point& p) const {
        return x == p.x && y == p.y;
    }
    bool operator < (const Point& p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
    bool operator > (const Point& p) const {
        return x > p.x || (x == p.x && y > p.y);
    }
    void print() const;
};

class Shape {
public:
    long id;
    Point p1, p2;
    Shape(long _id = 0)
        : id {_id}
    {
    }
    Shape(Point _p1, Point _p2, long _id = 0)
        : p1{_p1}, p2{_p2}, id{_id}
    {
    }
    virtual void print() = 0;
    virtual double slope(double x) = 0;
    virtual double second_derivative(double x) = 0;
    virtual double get_value(double x) = 0;
};

class Segment: public Shape {
public:
    Segment(Point _p1, Point _p2, long _id = 0)
        : Shape{_p1, _p2, _id}
    {
    }
    void print();
    double slope(double x);
    double second_derivative(double x);
    double get_value(double x);
};

class Circle: public Shape {
public:
    Point center;
    double radius;
    Half half;
    Circle(Point _center, double _radius, Half _half, long _id = 0)
        : Shape {_id}, center {_center}, radius {_radius}, half {_half}
    {
        p1.x = _center.x - _radius;
        p2.x = _center.x + _radius;
        p1.y = p2.y = _center.y;
    }

    void print();
    double slope(double x);
    double second_derivative(double x);
    double get_value(double x);
};

std::vector<Point> intersection(Shape *s1, Shape *s2);
bool same_object(Shape *s1, Shape *s2);
int cmp(Shape *s1, Shape *s2, double x);
void swap(Point& p1, Point& p2);
void swap(Shape*& s1, Shape*& s2);
double distance2(const Point& p1, const Point& p2);

#endif