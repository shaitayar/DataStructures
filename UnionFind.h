#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include "company.h"

template<class T>
struct UNode {
    T *obj;
    UNode *father;
    Company * leader;
    double rank;
};

template<class T>
UNode<T> *createUNode(T *obj) {
    auto *node = new UNode<T>();
    node->obj = obj;
    node->father = NULL;
    node->rank = obj->getID();
    node->leader = nullptr;
    return node;
}


template<class T>
class UnionFind {
    UNode<T> **objects;
    int *sizes;
    int size;

    double getValue_aux(UNode<T> *node, UNode<T> *root);


public:
    explicit UnionFind(int size) : size(size) {
        objects = new UNode<T> *[size + 1];
        sizes = new int[size + 1];
        objects[0] = nullptr;
        sizes[0] = -1;
        for (int i = 1; i < size + 1; ++i) {
            T *obj = new T(i);
            objects[i] = createUNode(obj);
            sizes[i] = 1;
        }
    }


    T *Find(int i);

    double getValue(int i);

    bool Union(int a, int b, double factor, int *bigger);

    bool isSameGroup(int a, int b);

    void print() {
        for (int i = 1; i < size + 1; ++i) {
            std::cout << "object: " << objects[i]->obj->getID() << " size: " << sizes[i] << " value: " << getValue(i)
                      << std::endl;
        }
    }

    void DestroyUF();

};

template<class T>
UNode<T> *FindRoot(UNode<T> *node) {
    if (!node->father) return node;
    return FindRoot(node->father);
}

template<class T>
T *UnionFind<T>::Find(int i) {
    if (i < 0 || i > size) return NULL;
    getValue(i);
    auto node = objects[i];
    auto r = FindRoot(node);
    return r->obj;
}

template<class T>
double UnionFind<T>::getValue_aux(UNode<T> *node, UNode<T> *root) {
    double counter =0;
    double sum=0;
    if (node==NULL) return 0;
    UNode<T> * iter = node;

    while (iter){
        counter+=iter->rank;
        if (iter->father==NULL){
            root=iter;
        }
        else{
            sum += iter->rank;
        }
        iter = iter->father;
    }
    iter = node;
    while (iter!=root && iter->father!=root){
        UNode<T> * temp = iter->father;
        sum-=iter->rank;
        iter->rank += sum;
        iter->father = root;
        iter = temp;
    }

    return counter;
}

template<class T>
double UnionFind<T>::getValue(int i) {
    UNode<T> *node = objects[i];
    return getValue_aux(node, NULL);

}

template<class T>
bool UnionFind<T>::isSameGroup(int a, int b) {

    UNode<T> *na = objects[a];
    UNode<T> *nb = objects[b];

    return FindRoot(na) == FindRoot(nb);

}

template<class T>
bool UnionFind<T>::Union(int a, int b, double factor, int *bigger) {
    if (isSameGroup(a, b)) return false;

    UNode<T> *na = objects[a];
    UNode<T> *nb = objects[b];

    UNode<T> *ra = FindRoot(na);
    UNode<T> *rb = FindRoot(nb);

    int ra_id = ra->obj->getID();
    int rb_id = rb->obj->getID();

    double value = 0;
    //calc values
    if (rb->leader==NULL){
        value = getValue(rb->obj->getID());
    }
    else{
        value = getValue(rb->leader->getID());
    }


    //ACQUIRER IS BIGGER THAN TARGET
    if (sizes[ra_id] >= sizes[rb_id]) {
        *bigger = ra->obj->getID();
        rb->father = ra;
        ra->rank += value * factor;
        rb->rank -= ra->rank;
        sizes[ra_id] += sizes[rb_id];
    }

        //ACQUIRER IS SMALLER THAN TARGET
    else {
        *bigger = rb->obj->getID();
        ra->father = rb;
        ra->rank += value * factor - rb->rank;
        sizes[rb_id] += sizes[ra_id];
        rb->leader = ((ra->leader!=NULL) ? ra->leader : ra->obj);
    }
    return true;
}

template<class T>
void UnionFind<T>::DestroyUF() {
    for (int i = 1; i < size + 1; ++i) {
        UNode<T> *node = objects[i];
        delete node->obj;
        node->obj = NULL;
        delete node;
        node = NULL;
    }
    delete[] objects;
    delete[] sizes;

}

#endif
