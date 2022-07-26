#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H

#include "RankTree.h"

#define M_FACTOR (10)
#define DUPL (2)
#define TOP (1.5)
#define LOW (0.5)

template<class T, class L>
struct Hnode {
    RankTree<T, L> *tree;
};

template<class T, class L>
Hnode<T, L> *createHNode(const L &compare) {
    Hnode<T, L> *node = new Hnode<T, L>();
    node->tree = new RankTree<T, L>(compare);
    return node;
}

template<class T, class L>
class HashTable {
    Hnode<T, L> **objects;
    L compare;
    int M;
    int N;

    int hash(int key);

public:
    explicit HashTable(L compare) : compare(compare), M(M_FACTOR), N(0) {
        objects = new Hnode<T, L> *[M];
        for (int i = 0; i < M; ++i) {
            objects[i] = createHNode<T, L>(compare);
        }
    };

    ~HashTable() = default;

    void DestroyHashTable(bool is_obj);

    void Add(T *obj);

    void Remove(int key, bool is_obj);

    T *Get(int key);

    void resize_bigger();

    void resize_smaller();

    //void print();

    void HashToArray(T **arr);
};

template<class T, class L>
void HashTable<T, L>::DestroyHashTable(bool is_obj) {
    if (objects == NULL) return;
    for (int i = 0; i < M; ++i) {
        objects[i]->tree->DestroyTree(is_obj);
        delete objects[i]->tree;
        objects[i]->tree = NULL;
        delete objects[i];

    }
    delete[] objects;
    objects = NULL;
    N = 0;
}


template<class T, class L>
int HashTable<T, L>::hash(int key) {
    return key % M;
}

template<class T, class L>
void HashTable<T, L>::Add(T *obj) {
    int key = obj->getID();
    int hashes_key = hash(key);

    //ADD TO ARRAY
    objects[hashes_key]->tree->insert(obj);
    N += 1;


    double load_factor = double(N / M);
    //resize
    if (load_factor >= TOP) {
        resize_bigger();
    }

}

template<class T, class L>
void HashTable<T, L>::Remove(int key, bool is_obj) {
    int hashed = hash(key);
    RankTree<T, L> *tree = objects[hashed]->tree;
    auto temp = tree->find(key);
    tree->deleteNode(temp->obj, is_obj);
    N--;
    double load_factor = double(N / M);
    if (load_factor <= LOW && M > M_FACTOR) {
        resize_smaller();
    }
}

template<class T, class L>
T *HashTable<T, L>::Get(int key) {
    int hashed = hash(key);
    RankTree<T, L> *tree = objects[hashed]->tree;
    auto *n = tree->find(key);
    if (!n) return NULL;
    return n->obj;
}

template<class T, class L>
void HashTable<T, L>::resize_bigger() {
    //multiply by DUPL
    M *= DUPL;
    Hnode<T, L> **new_array = new Hnode<T, L> *[M];
    for (int i = 0; i < M; ++i) {
        new_array[i] = createHNode<T, L>(compare);
    }
    for (int i = 0; i < M / DUPL; ++i) {
        RankTree<T, L> *ptree = objects[i]->tree;
        int tree_size = ptree->getSize();
        T **arr = (T **) malloc(sizeof(**arr) * tree_size);
        ptree->treeToArr(arr);
        for (int j = 0; j < tree_size; ++j) {
            int id = arr[j]->getID();
            int hashed = hash(id);
            new_array[hashed]->tree->insert(arr[j]);
        }
        free(arr);
    }
    Hnode<T, L> **temp = this->objects;
    this->objects = new_array;

    //delete old with no destroy
    for (int i = 0; i < M / DUPL; ++i) {
        temp[i]->tree->DestroyTree(false);
        delete temp[i]->tree;
        temp[i]->tree = NULL;
        delete temp[i];
    }
    delete[] temp;
    temp=NULL;

}

template<class T, class L>
void HashTable<T, L>::resize_smaller() {
    //divide by DUPL
    M /= DUPL;
    Hnode<T, L> **new_array = new Hnode<T, L> *[M];
    for (int i = 0; i < M; ++i) {
        new_array[i] = createHNode<T, L>(compare);
    }
    for (int i = 0; i < M * DUPL; ++i) {
        RankTree<T, L> *ptree = objects[i]->tree;
        int tree_size = ptree->getSize();
        T **arr = (T **) malloc(sizeof(**arr) * tree_size);
        ptree->treeToArr(arr);
        for (int j = 0; j < tree_size; ++j) {
            int id = arr[j]->getID();
            int hashed = hash(id);
            new_array[hashed]->tree->insert(arr[j]);
        }
        free(arr);
    }
    Hnode<T, L> **temp = this->objects;
    this->objects = new_array;

    //delete old with no destroy
    for (int i = 0; i < M * DUPL; ++i) {
        (temp[i])->tree->DestroyTree(false);
        delete temp[i]->tree;
        temp[i]->tree = NULL;
        delete temp[i];

    }
    delete[] temp;
    temp=NULL;
}

/*template<class T, class L>
void HashTable<T, L>::print() {
    double factor = (double) N / M;
    std::cout.precision(3);
    std::cout << "M: " << this->M << " N: " << this->N << " factor: " << std::showpoint << factor << std::endl;
    for (int i = 0; i < M; ++i) {
        objects[i].print2D();
        std::cout << "------" << std::endl;
    }
}*/


template<class T, class L>
void HashTable<T, L>::HashToArray(T **arr) {
    for (int i = 0, j = 0; i < M && j < N; ++i) {
        RankTree<T, L> *ptree = objects[i]->tree;
        int size = ptree->getSize();
        if (size > 0) {
            ptree->treeToArr(arr + j);
            j += size;
        }
    }
}


#endif //WET2_HASHTABLE_H
