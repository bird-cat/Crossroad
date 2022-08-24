#include <algorithm>
#include <cmath>
#include <typeinfo>
#include <iostream>
#include <iomanip>

#include "shape.h"
#include "defs.h"

void Point::print() const {
    std::cout << "(" << x << ", " << y << ")" << std::endl;
}

void Segment::print() {
    std::cout << id << ": (" <<  std::setprecision(17) << p1.x << ", " << p1.y << ")" << " " << "(" << p2.x << ", " << p2.y << ")\n";
}

void Circle::print() {
    std::cout << id << ": (" << p1.x << ", " << p1.y << ")" << " " << "(" << p2.x << ", " << p2.y << ")";
    if (half == Upper)
        std::cout << " Upper\n";
    else
        std::cout << " Lower\n";
}

double Segment::slope(double x)
{   
    if (p1.x == p2.x)
        return INF;
    return (p2.y - p1.y) / (p2.x - p1.x);
}

double Segment::second_derivative(double x)
{
    return 0.0;
}

double Segment::get_value(double x)
{
    if (p1.x == p2.x)
        return p1.y;
    double dx, dy;
    dx = p2.x - p1.x;
    dy = p2.y - p1.y;
    return (x - p1.x) * dy / dx + p1.y;
}

double Circle::slope(double x)
{
    double y = get_value(x);
    return -(x - center.x) / (y - center.y);
}

double Circle::second_derivative(double x)
{
    double y, dy;
    y = get_value(x);
    dy = y - center.y;
    return - (radius * radius) / (dy * dy * dy);
}

double Circle::get_value(double x)
{
    double dx = x - center.x;
    if (half == Upper)
        return center.y + sqrt(radius * radius - dx * dx);
    return center.y - sqrt(radius * radius - dx * dx);
}

std::vector<Point> intersection(Shape *s1, Shape *s2)
{
    std::vector<Point> intersections;
    if (same_object(s1, s2))
        return intersections;
    // Segment - Segment
    if (typeid(*s1) == typeid(Segment) && typeid(*s2) == typeid(Segment)) {
        Point p;
        double dx1, dx2, dy1, dy2, delta;
        dx1 = s1->p2.x - s1->p1.x;
        dx2 = s2->p2.x - s2->p1.x;
        dy1 = s1->p2.y - s1->p1.y;
        dy2 = s2->p2.y - s2->p1.y;
        delta = dy2 * dx1 - dy1 * dx2;
        if (delta == 0)
            return intersections;
        p.x = (dx1 * dx2 * (s1->p1.y - s2->p1.y) + dx1 * dy2 * s2->p1.x - dx2 * dy1 * s1->p1.x) / delta;
        p.y = s1->p1.y + (p.x - s1->p1.x) * dy1 / dx1;
        if (p.x >= std::max(s1->p1.x, s2->p1.x) && p.x <= std::min(s1->p2.x, s2->p2.x))
            intersections.push_back(p);
    }
    // Circle - Circle
    else if (typeid(*s1) == typeid(Circle) && typeid(*s2) == typeid(Circle)) {
        Point p1, p2;
        Circle *c1 = dynamic_cast<Circle*>(s1);
        Circle *c2 = dynamic_cast<Circle*>(s2);
        double rr, a, D;
        rr = distance2(c1->center, c2->center);
        a = (c1->radius * c1->radius - c2->radius * c2->radius) / rr;
        D = 2 * (c1->radius * c1->radius + c2->radius * c2->radius) / rr - a * a - 1;
        if (D >= 0) {
            p1.x = (c1->center.x + c2->center.x + a * (c2->center.x - c1->center.x) - sqrt(D) * (c2->center.y - c1->center.y)) / 2;
            p2.x = (c1->center.x + c2->center.x + a * (c2->center.x - c1->center.x) + sqrt(D) * (c2->center.y - c1->center.y)) / 2;
            p1.y = (c1->center.y + c2->center.y + a * (c2->center.y - c1->center.y) - sqrt(D) * (c1->center.x - c2->center.x)) / 2;
            p2.y = (c1->center.y + c2->center.y + a * (c2->center.y - c1->center.y) + sqrt(D) * (c1->center.x - c2->center.x)) / 2;
            if (c1->half == Upper && c2->half == Upper) {
                if (p1.y > c1->center.y && p1.y > c2->center.y)
                    intersections.push_back(p1);
                if (p2.y > c1->center.y && p2.y > c2->center.y)
                    intersections.push_back(p2);
            } else if (c1->half == Upper && c2->half == Lower) {
                if (p1.y > c1->center.y && p1.y <= c2->center.y)
                    intersections.push_back(p1);
                if (p2.y > c1->center.y && p2.y <= c2->center.y)
                    intersections.push_back(p2);
            } else if (c1->half == Lower && c2->half == Upper) {
                if (p1.y <= c1->center.y && p1.y > c2->center.y)
                    intersections.push_back(p1);
                if (p2.y <= c1->center.y && p2.y > c2->center.y)
                    intersections.push_back(p2);
            } else {
                if (p1.y <= c1->center.y && p1.y <= c2->center.y)
                    intersections.push_back(p1);
                if (p2.y <= c1->center.y && p2.y <= c2->center.y)
                    intersections.push_back(p2);
            }
        }
    }
    // Segment - Circle
    else {
        if (typeid(*s1) == typeid(Circle))
            swap(s1, s2);
        Point p1, p2;
        Circle *c2 = dynamic_cast<Circle*>(s2);
        double m, c, D;
        m = (s1->p2.y - s1->p1.y) / (s1->p2.x - s1->p1.x);
        c = (s1->p1.y - c2->center.y) - m * (s1->p1.x - c2->center.x);
        D = 4 * (m * m * c * c - (1 + m * m) * (c * c - c2->radius * c2->radius));
        if (D >= 0) {
            p1.x = (-2 * m * c - sqrt(D)) / (2 * (m * m + 1)) + c2->center.x;
            p2.x = (-2 * m * c + sqrt(D)) / (2 * (m * m + 1)) + c2->center.x;
            p1.y = m * (p1.x - s1->p1.x) + s1->p1.y;
            p2.y = m * (p2.x - s1->p1.x) + s1->p1.y;
            if (c2->half == Upper) {
                if (p1.y > c2->center.y)
                    intersections.push_back(p1);
                if (p2.y > c2->center.y && D > 0)
                    intersections.push_back(p2);
            } else {
                if (p1.y <= c2->center.y)
                    intersections.push_back(p1);
                if (p2.y <= c2->center.y && D > 0)
                    intersections.push_back(p2);
            }
        }
    }
    return intersections;
}

bool same_object(Shape *s1, Shape *s2)
{
    return s1->id == s2->id;
}

int cmp(Shape *s1, Shape *s2, double x) {
    // Equal
    if (s1 == s2)
        return 0;

    // Compare y-interception
    double y1, y2;
    y1 = s1->get_value(x);
    y2 = s2->get_value(x);
    if (y2 - y1 > EPSILON) // y1 < y2
        return -1;
    if (y1 - y2 > EPSILON) // y2 > y1
        return 1;

    // Compare slope
    double m1, m2;
    m1 = s1->slope(x);
    m2 = s2->slope(x);
    if (m1 < m2)
        return -1;
    if (m1 > m2)
        return 1;

    // Compare second direvative
    double r1, r2;
    r1 = s1->second_derivative(x);
    r2 = s2->second_derivative(x);
    if (r1 < r2)
        return -1;
    if (r1 > r2)
        return 1;
}

void swap(Shape*& s1, Shape*& s2) {
    Shape *tmp = s1;
    s1 = s2;
    s2 = tmp;
}

void swap(Point& p1, Point& p2) {
    Point tmp = p1;
    p1 = p2;
    p2 = tmp;
}

double distance2(const Point& p1, const Point& p2) {
    double dx, dy;
    dx = p2.x - p1.x;
    dy = p2.y - p1.y;
    return dx * dx + dy * dy;
}