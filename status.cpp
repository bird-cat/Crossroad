#include <cstdlib>
#include <iostream>
#include <typeinfo>

#include "status.h"

Node* BBST::get(Node *r, Shape *s, double x) {
    if (!r)
        return NULL;
    int res = cmp(s, r->road, x);
    if (res == 0)
        return r;
    if (res < 0)
        return get(r->left, s, x);
    return get(r->right, s, x);
}

Node* BBST::insert(Node *r, Shape *s, double x) {
    if (!r)
        return new Node(s);
    int res = cmp(s, r->road, x);
    if (res < 0)
        r->left = insert(r->left, s, x);
    else if (res > 0)
        r->right = insert(r->right, s, x);
    return r;
}

Node* BBST::remove(Node *r, Shape *s, double x) {
    int res = cmp(s, r->road, x);
    if (res == 0) {
        Node *tmp;
        if (!r->left) {
            tmp = r->right;
            delete r;
            return tmp;
        } else if (!r->right) {
            tmp = r->left;
            delete r;
            return tmp;
        } else {
            tmp = r->left;
            while (tmp->right)
                tmp = tmp->right;
            swap(r->road, tmp->road);
            r->left = remove(r->left, s, x);
        }
    } else if (res < 0) {
        r->left = remove(r->left, s, x);
    } else {
        r->right = remove(r->right, s, x);
    }
    return r;
}

Node* BBST::predecessor(Node *r, Node *t, Shape *s, double x) {
    int res = cmp(s, r->road, x);
    if (res == 0) {
        Node *pred = r->left;
        if (!pred)
            return t;
        while (pred->right)
            pred = pred->right;
        return pred;
    }  
    if (res < 0) {
        return predecessor(r->left, t, s, x);
    } else {
        return predecessor(r->right, r, s, x);
    }
}

Node* BBST::successor(Node *r, Node *t, Shape *s, double x) {
    int res = cmp(s, r->road, x);
    if (res == 0) {
        Node *succ = r->right;
        if (!succ)
            return t;
        while (succ->left)
            succ = succ->left;
        return succ;
    }  
    if (res < 0) {
        return successor(r->left, r, s, x);
    } else {
        return successor(r->right, t, s, x);
    }
}

void BBST::print(Node* r) {
    if (r) {
        print(r->left);
        std::cout << r->road->id << " ";
        if (typeid(*r->road) == typeid(Circle))
            std::cout << "(" << dynamic_cast<Circle*>(r->road)->half << ") ";
        print(r->right);
    }
}

// Interfaces
Shape* BBST::get(Shape *s, double x) {
    Node *node = get(root, s, x);
    if (!node)
        return NULL;
    return node->road;
}
void BBST::insert(Shape *s, double x) {
    root = insert(root, s, x);
}

void BBST::remove(Shape *s, double x) {
    root = remove(root, s, x);
}

Shape* BBST::predecessor(Shape *s, double x) {
    Node *pred, *t = NULL;
    pred = predecessor(root, t, s, x);
    if (!pred)
        return NULL;
    return pred->road;
}

Shape* BBST::successor(Shape *s, double x) {
    Node *succ, *t = NULL;
    succ = successor(root, t, s, x);
    if (!succ)
        return NULL;
    return succ->road;
}

void BBST::print() {
    print(root);
    std::cout << std::endl;
}
