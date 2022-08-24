#include "event.h"
#include <iostream>

Event::Event(Point _p, std::set<Shape*> _roads)
    : p{_p}, roads{_roads}
{
}

bool Event::operator < (const Event& e) const {
    return p < e.p;
}

bool Event::operator > (const Event& e) const {
    return p > e.p;
}

bool Event::operator == (const Event& e) const {
    return p == e.p;
}

void Event::print() {
    p.print();
    for (auto s: roads) {
        s->print();
    }
}

bool Event_Comparator::operator() (const Event* e1, const Event* e2) {
    return *e1 > *e2;
}

void add_event(Shape *road, const Point& p,
    Event_Map_t& event_map, Event_Queue_t& eq)
{
    if (event_map.find(p) == event_map.end()) {
        event_map[p] = new Event(p);
        eq.push(event_map[p]);
        /*
        std::cout << "Insert\n";
        std::cout << "Points visited\n";
        for (auto e: event_map) {
            e.first.print();
        }
        */
    }
    if (event_map[p]->roads.find(road) == event_map[p]->roads.end()) {
        event_map[p]->roads.insert(road);
    }
}