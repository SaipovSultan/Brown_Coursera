#pragma once

struct Node {
    Node(int v, Node* p)
            : value(v)
            , parent(p)
    {}

    int value;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent;
};

Node* Next(Node* me) {
    if(!me) return me;
    if(me->right){
        me = me->right;
        while(me->left){
            me = me->left;
        }
        return me;
    }
    if(me->parent && me->parent->left == me){
        return me->parent;
    }
    while(me->parent && me->parent->right == me){
        me = me->parent;
    }
    return me->parent;
}