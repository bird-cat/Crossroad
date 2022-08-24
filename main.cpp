#include <iostream>
#include <typeinfo>
#include <vector>
#include <queue>
#include <map>

#include "shape.h"
#include "event.h"
#include "status.h"
#include "defs.h"

using namespace std;

Shape *roads[MAX_N];

Point extend(const Point& p1, const Point& p2) {
    double c, cx, cy, dx, dy;
    dx = p2.x - p1.x;
    dy = p2.y - p1.y;
    if (dx == 0) {
        return (p1.y < p2.y)? Point(p1.x, MAX_Y): Point(p1.x, -MAX_Y);
    } else if (dx > 0) {
        cx = (MAX_X - p1.x) / dx;
    } else {
        cx = (-MAX_X - p1.x) / dx;
    }
    if (dy == 0) {
        return (p1.x < p2.x)? Point(MAX_X, p1.y): Point(-MAX_X, p2.y);
    } else if (dy > 0) {
        cy = (MAX_Y - p1.y) / dy;
    } else {
        cy = (-MAX_Y - p1.y) / dy;
    }
    c = min(cx, cy);
    return Point(p1.x + c * dx, p1.y + c * dy);
}

int count_crossroads(long n) {
    // Initialize event queue (To be modified)
    Event_Map_t event_map;
    Event_Queue_t eq;
    for (int i = 0; i < n; i++) {
        Shape *road = roads[i];
        add_event(road, road->p1, event_map, eq);
        add_event(road, road->p2, event_map, eq);
    }

    /*
    while (!eq.empty()) {
        eq.top()->p.print();
        eq.pop();
    }
    */
    /*
    for (auto e: event_map) {
        cout << e.second->roads.size() << endl;
    }
    */

    // Initialize status tree
    BBST st;

    // Count cross roads
    double cur_x, prev_x;
    Event *e;
    int count = 0;

    while (!eq.empty()) {
        cout << "In loop\n";
        e = eq.top();
        eq.pop();
        e->print();

        prev_x = cur_x;
        cur_x = e->p.x;
        // Determine intersect or not
        if (e->roads.size() > 1 &&
            !(e->roads.size() == 2 && same_object(*e->roads.begin(), *e->roads.rbegin())))
            count++;
        cout << "count = " << count << endl;

        // Determine the type of event
        int n_alive = 0;
        for (Shape *road: e->roads) {
            if (e->p < road->p2) {
                n_alive++;
            }
        }
        Shape *hi, *lo, *succ, *pred;
        hi = *e->roads.begin();
        lo = *e->roads.begin();

        if (n_alive == 0) {                              // Case 1
            // Find the highest and lowest roads at prev_x
            for (Shape *road: e->roads) {
                if (cmp(road, hi, prev_x) > 0)
                    hi = road;
                if (cmp(road, lo, prev_x) < 0)
                    lo = road;
            }
            // succ = successor(hi)
            // pred = predecessor(lo)
            succ = st.successor(hi, prev_x);
            pred = st.predecessor(lo, prev_x);

            // If succ intersect with pred, add new event
            if (succ && pred) {
                vector<Point> intersections = intersection(succ, pred);
                for (Point p: intersections) {
                    if (p > e->p) {
                        add_event(pred, p, event_map, eq);
                        add_event(succ, p, event_map, eq);
                    }
                }
            }

            // Remove all segments from st
            for (Shape *road: e->roads) {
                st.remove(road, prev_x);
            }
        } else {                                             // Case 2
            // Remove L(p) and C(p) from st
            for (Shape *road: e->roads) {
                if (road->p1 < e->p)
                    st.remove(road, prev_x);
            }
            // Insert C(p) and U(p) into st
            for (Shape *road: e->roads) {
                if (e->p < road->p2) {
                    st.insert(road, cur_x);
                }
            }
            // Find the highest and lowest roads at cur_x
            for (Shape *road: e->roads) {
                if (cmp(road, hi, cur_x) > 0)
                    hi = road;
                if (cmp(road, lo, cur_x) < 0)
                    lo = road;
            }
            // succ = successor(hi)
            // pred = predecessor(lo)
            succ = st.successor(hi, cur_x);
            pred = st.predecessor(lo, cur_x);

            // If hi intersect with succ, add new event
            if (succ) {
                cout << "succ = ";
                succ->print();
                cout << "hi =";
                hi->print();

                vector<Point> intersections = intersection(hi, succ);
                for (Point p: intersections) {
                    if (p > e->p) {
                        add_event(hi, p, event_map, eq);
                        add_event(succ, p, event_map, eq);
                    }
                }
            }

            // If lo intersect with pred, add new event
            if (pred) {
                cout << "pred = ";
                pred->print();
                cout << "lo =";
                lo->print();

                vector<Point> intersections = intersection(pred, lo);
                for (Point p: intersections) {
                    if (p > e->p) {
                        add_event(lo, p, event_map, eq);
                        add_event(pred, p, event_map, eq);
                    }
                }
            }
        }
        cout << "st = ";
        st.print();
        cout << endl;
    }
    cout << "Out loop\n";
    return count;
}

int main() {
    int S, R, L, C, N;
    cin >> S >> R >> L >> C;
    N = S + R + L + 2 * C;
    int i;
    long id = 0, radius;
    Point p1, p2;
    for (i = 0; i < S; i++) {
        cin >> p1.x >> p1.y >> p2.x >> p2.y;
        if (p2 < p1)
            swap(p1, p2);
        roads[i] = new Segment(p1, p2, id++);
    }
    for (; i < S + R; i++) {
        cin >> p1.x >> p1.y >> p2.x >> p2.y;
        Point f = extend(p1, p2);
        if (f < p1)
            swap(p1, f);
        roads[i] = new Segment(p1, f, id++);
    }
    for (; i < S + R + L; i++) {
        cin >> p1.x >> p1.y >> p2.x >> p2.y;
        Point f1 = extend(p1, p2);
        Point f2 = extend(p2, p1);
        if (f2 < f1)
            swap(f1, f2);
        roads[i] = new Segment(f1, f2, id++);
    }
    for (; i < S + R + L + 2 * C; i+=2) {
        Point center;
        cin >> center.x >> center.y >> radius;
        roads[i] = new Circle(center, radius, Lower, id);
        roads[i+1] = new Circle(center, radius, Upper, id++);
    }

/*
    for (i = 0; i < N; i++) {
        auto road = roads[i];
        if (typeid(*road) == typeid(Segment)) {
            cout << "Segment, ";
            road->print();
            cout << endl;
        } else {
            cout << "Circle, ";
            road->print();
            cout << endl;
        }
    }
*/
    cout << count_crossroads(N) << endl;
    return 0;
}