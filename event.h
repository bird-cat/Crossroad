#ifndef EVENT_H_
#define EVENT_H_

#include <vector>
#include <set>
#include <map>
#include <queue>
#include "shape.h"

struct Event {
    Point p;
    std::set<Shape*> roads;
    Event() {}
    Event(Point _p, std::set<Shape*> _roads = {});
    bool operator < (const Event& e) const;
    bool operator > (const Event& e) const;
    bool operator == (const Event& e) const;
    void print();
};

struct Event_Comparator {
    bool operator() (const Event* e1, const Event* e2);
};

typedef std::map<Point, Event*> Event_Map_t;
typedef std::priority_queue<Event*, std::vector<Event*>, Event_Comparator> Event_Queue_t;

void add_event(Shape *road, const Point& p,
    Event_Map_t& event_map, Event_Queue_t& eq);



#endif