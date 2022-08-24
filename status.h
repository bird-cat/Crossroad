#ifndef STATUS_H_
#define STATUS_H_
#include "shape.h"

struct Node {
    Shape *road;
    Node *left, *right;
    Node(Shape *_road = NULL, Node *_left = NULL, Node *_right = NULL)
        : road {_road}, left {_left}, right {_right}
    {
    } 
};

class BBST {
    Node *root;
    Node* get(Node *r, Shape *s, double x);
    Node* insert(Node *r, Shape *s, double x);
    Node* remove(Node *r, Shape *s, double x);
    Node* predecessor(Node *r, Node *t, Shape *s, double x);
    Node* successor(Node *r, Node *t, Shape *s, double x);
    void print(Node *r);
public:
    BBST(Node *_root = NULL)
        : root {_root}
    {
    }
    Shape* get(Shape *s, double x);
    void insert(Shape *s, double x);
    void remove(Shape *s, double x);
    Shape* predecessor(Shape *s, double x);
    Shape* successor(Shape *s, double x);
    void print();
};

#endif