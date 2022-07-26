#ifndef WET2_RANKTREE_H
#define WET2_RANKTREE_H

#include <iostream>
#include <math.h>

using std::ostream;
using std::max;

template<class T, class L>
struct RNode {
    T *obj;
    RNode *father;
    RNode *left_son;
    RNode *right_son;
    int height;
    long long rank;
    long long rank_subtree;
    int size_subtree;

};

template<class T, class L>
class RankTree {
    RNode<T, L> *root;
    L compare;
    int size;

    /**HELP FUNCTIONS*/
    RNode<T, L> *addNode(RNode<T, L> *node, T *obj, bool *is_success);

    RNode<T, L> *RightRotate(RNode<T, L> *r);

    RNode<T, L> *LeftRotate(RNode<T, L> *r);

    RNode<T, L> *RemoveNode(RNode<T, L> *node, T *Data, bool is_obj);

    RNode<T, L> *balance(RNode<T, L> *node);

    int inorderBackRemoveExtra(RNode<T, L> *node, int nodes_to_remove);

public:
    RankTree() = default;

    explicit RankTree(L compare) : root(NULL), compare(compare), size(0) {};

    ~RankTree() = default;

    void DestroyTree(bool is_obj);

    int calcHeight(RNode<T, L> *node);

    void insert(T *obj);

    void deleteNode(T *data, bool is_obj);

    RNode<T, L> *RRRotate(RNode<T, L> *r);

    RNode<T, L> *LLRotate(RNode<T, L> *r);

    RNode<T, L> *LRRotate(RNode<T, L> *r);

    RNode<T, L> *RLRotate(RNode<T, L> *r);

    RNode<T, L> *find(int NodeID);

    RNode<T, L> *find_rank_index(int data, long long *calc_rank, int *index);

    void SumOfBumpGradeBetweenTopWorkers(int m, void **sumBumpGrade);

    RNode<T, L> *getMaxNode();

    void print() const;

    void printToList(int **, int sizet) const;

    void inorder(RNode<T, L> *) const;

    int getSize() const { return size; }

    void getNMax(int, int **);

    void getMatch(int MinEmployeeID, int MaxEmployeeID, int MinSalary,
                  int MinGrade,
                  int *TotalNumOfEmployees, int *NumOfEmployees);

    void treeToArr(T **arr);

    void arrToTree(T **arr, int sizet);

    RNode<T, L> *buildEmptyTree(int height_neede, RNode<T, L> *parent);

    void print2D();

};

template<class T, class L>
long long get_rank(RNode<T, L> *node) {
    if (node == NULL) return 0;
    return node->rank;
}

template<class T, class L>
long long get_rank_subtree(RNode<T, L> *node) {
    if (node == NULL) return 0;
    return node->rank_subtree;
}

template<class T, class L>
int size_subtree(RNode<T, L> *node) {
    if (node == NULL) return 0;
    return node->size_subtree;
}


template<class T, class L>
void print2DUtil(RNode<T, L> *root, int space) {
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    print2DUtil(root->right_son, space);

    // Print current node after space
    // count
    std::cout << std::endl;
    for (int i = 10; i < space; i++)
        std::cout << "   ";
    std::cout << root->obj->getSalary()<<","<<root->rank << ",id:" << root->obj->getID() << ",ranksub:" << root->rank_subtree << ",size:"
              << root->size_subtree << "\n";

    // Process left child
    print2DUtil(root->left_son, space);
}
// Wrapper over print2DUtil()

template<class T, class L>
void RankTree<T, L>::print2D() {
    // Pass initial space count as 0
    print2DUtil(root, 0);
}


/*** RankTree Implementation */
template<class T, class L>
int getNMaxAux(RNode<T, L> *root, int **Employees, int n, int counter) {
    if (root == NULL || counter >= n) return counter;
    counter = getNMaxAux(root->left_son, Employees, n, counter);
    if (counter < n) {
        (*Employees)[counter++] = root->obj->getMax();
    }
    counter = getNMaxAux(root->right_son, Employees, n, counter);
    return counter;
}

template<class T, class L>
void RankTree<T, L>::getNMax(int n, int **Employees) {
    getNMaxAux(root, Employees, n, 0);
}

template<class T, class L>
RNode<T, L> *RankTree<T, L>::getMaxNode() {
    RNode<T, L> *iter = root;
    while (iter && iter->right_son) {
        iter = iter->right_son;
    }
    return iter;
}

template<class T, class L>
int calcBF(RNode<T, L> *node) {
    if (node->left_son && node->right_son) {
        return node->left_son->height - node->right_son->height;
    } else if (node->left_son) {
        return node->left_son->height + 1;
    } else if (node->right_son) {
        return -(node->right_son->height + 1);
    } else return 0;
};


template<class T, class L>
RNode<T, L> *RankTree<T, L>::balance(RNode<T, L> *node) {
    if (!node) return NULL;
    if (calcBF(node) == 2) {
        if (calcBF(node->left_son) >= 0) {
            return LLRotate(node);
        } else if (calcBF(node->left_son) == -1) {
            return LRRotate(node);
        }
    } else if (calcBF(node) == -2) {
        if (calcBF(node->right_son) <= 0) {
            return RRRotate(node);
        } else if (calcBF(node->right_son) == 1) {
            return RLRotate(node);
        }
    }
    return node;
}

template<class T, class L>
int is_balanced(RNode<T, L> *current) {
    int BF = calcBF(current);
    return (BF >= -1 && BF <= 1);
}

template<class T, class L>
RNode<T, L> *createNode(T *obj) {
    auto *node = new RNode<T, L>();
    node->obj = obj;
    node->father = NULL;
    node->left_son = NULL;
    node->right_son = NULL;
    node->height = 0;

    //rank
    node->rank = obj->getRank();
    node->rank_subtree = 0;
    node->size_subtree = 1;

    return node;
}

template<class T, class L>
void RankTree<T, L>::insert(T *obj) {
    bool is_success = false;
    root = addNode(root, obj, &is_success);
    size++;
}

template<class T, class L>
void RankTree<T, L>::deleteNode(T *data, bool is_obj) {

    if (!data) return;
    root = RemoveNode(root, data, is_obj);
    size--;
}

template<class T, class L>
RNode<T, L> *find_minimal(RNode<T, L> *node) {
    RNode<T, L> *iter = node;
    while (iter->left_son) {
        iter = iter->left_son;
    }
    return iter;
}


/***
 * Recursive RemoveNode function
 * complexity: O(logn)
 */
template<class T, class L>
RNode<T, L> *RankTree<T, L>::RemoveNode(RNode<T, L> *node, T *data, bool is_obj) {
    if (node == NULL) return node;
    //if node key is bigger than NodeID - then left subtree
    if (compare(node->obj, data) > 0) {
        node->left_son = RemoveNode(node->left_son, data, is_obj);
    } else if (compare(node->obj, data) < 0) {
        node->right_son = RemoveNode(node->right_son, data, is_obj);
    }
        //find the Node to delete
    else {
        if (!node->left_son || !node->right_son) {
            //node has one or no sons
            RNode<T, L> *temp = node->right_son ? node->right_son : node->left_son;
            T *t_obj = node->obj;
            //no sons
            if (!temp) {
                temp = node;
                node = NULL;
            } else {
                node->obj = temp->obj;
                node->right_son = temp->right_son;
                node->left_son = temp->left_son;
                node->rank = temp->rank;
            }
            delete temp;
            if (is_obj) {
                delete t_obj;
                t_obj = NULL;
            }
        }
            //node has two sons
        else {
            //o(log (current node height))
            RNode<T, L> *temp = find_minimal(node->right_son);
            if (is_obj) {
                delete node->obj;
                node->obj = NULL;
            }
            node->obj = temp->obj;
            node->rank = temp->rank;
            node->right_son = RemoveNode(node->right_son, node->obj, false);
        }
    }

    if (node == NULL) return node;
    //update
    node->size_subtree = size_subtree(node->right_son) + size_subtree(node->left_son) + 1;
    node->rank_subtree = get_rank_subtree(node->left_son) + get_rank_subtree(node->right_son) + get_rank(node);

    node->height = max(calcHeight(node->left_son), calcHeight(node->right_son)) + 1;
    return balance(node);
}


template<class T, class L>
int RankTree<T, L>::calcHeight(RNode<T, L> *node) {
    if (!node) return -1;
    else return node->height;
}

template<class T, class L>
RNode<T, L> *RankTree<T, L>::addNode(RNode<T, L> *node, T *obj, bool *is_success) {
    if (node == NULL) {
        node = createNode<T, L>(obj);
        *is_success = true;
    } else if (compare(obj, node->obj) > 0) {
        node->right_son = addNode(node->right_son, obj, is_success);
        (node->right_son)->father = node;
    } else if (compare(obj, node->obj) < 0) {
        node->left_son = addNode(node->left_son, obj, is_success);
        (node->left_son)->father = node;

    }

        //illegal, node already exist
    else {
        return node;
    }
//update subtree size
    node->size_subtree = size_subtree(node->right_son) + size_subtree(node->left_son) + 1;
    node->rank_subtree = get_rank_subtree(node->left_son) + get_rank_subtree(node->right_son) + get_rank(node);

//update height
    node->height = max(calcHeight(node->left_son), calcHeight(node->right_son)) + 1;
    return balance(node);
}

template<class T, class L>
RNode<T, L> *RankTree<T, L>::RRRotate(RNode<T, L> *r) {
    return LeftRotate(r);
}

template<class T, class L>
RNode<T, L> *RankTree<T, L>::LLRotate(RNode<T, L> *r) {
    return RightRotate(r);
}


template<class T, class L>
RNode<T, L> *RankTree<T, L>::RightRotate(RNode<T, L> *r) {
    RNode<T, L> *pb = r->left_son;
    RNode<T, L> *pc = pb->right_son;
    if (r->father) {
        if ((r->father)->left_son == r) {
            r->father->left_son = pb;
            pb->father = r->father;
        } else {
            r->father->right_son = pb;
            pb->father = r->father;
        }
    }
    pb->right_son = r;
    this->root = pb;
    if (r->father == NULL) {
        root->father = NULL;
    }

    r->left_son = pc;
    r->father = pb;
    if (pc) pc->father = r;

    //update height
    r->height = max(calcHeight(r->left_son), calcHeight(r->right_son)) + 1;
    pb->height = max(calcHeight(pb->left_son), calcHeight(pb->right_son)) + 1;

    //update rank
    r->rank_subtree = get_rank_subtree(r->left_son) + get_rank_subtree(r->right_son) + get_rank(r);
    pb->rank_subtree = get_rank_subtree(pb->left_son) + get_rank_subtree(pb->right_son) + get_rank(pb);

    //update size
    r->size_subtree = size_subtree(r->left_son) + size_subtree(r->right_son) + 1;
    pb->size_subtree = size_subtree(pb->left_son) + size_subtree(pb->right_son) + 1;

    return root;
}

template<class T, class L>
RNode<T, L> *RankTree<T, L>::LeftRotate(RNode<T, L> *r) {
    RNode<T, L> *pb = r->right_son;
    RNode<T, L> *pc = pb->left_son;
    if (r->father) {
        if (r->father->left_son == r) {
            r->father->left_son = pb;
            pb->father = r->father;
        } else {
            r->father->right_son = pb;
            pb->father = r->father;
        }
    }
    pb->left_son = r;
    this->root = pb;
    if (r->father == NULL) {
        root->father = NULL;
    }
    r->right_son = pc;
    r->father = pb;
    if (pc) pc->father = r;


    //update height
    r->height = max(calcHeight(r->left_son), calcHeight(r->right_son)) + 1;
    pb->height = max(calcHeight(pb->left_son), calcHeight(pb->right_son)) + 1;

    //update rank
    r->rank_subtree = get_rank_subtree(r->left_son) + get_rank_subtree(r->right_son) + get_rank(r);
    pb->rank_subtree = get_rank_subtree(pb->left_son) + get_rank_subtree(pb->right_son) + get_rank(pb);

    //update size subtree
    r->size_subtree = size_subtree(r->left_son) + size_subtree(r->right_son) + 1;
    pb->size_subtree = size_subtree(pb->left_son) + size_subtree(pb->right_son) + 1;

    return root;
}

template<class T, class L>
RNode<T, L> *RankTree<T, L>::LRRotate(RNode<T, L> *r) {
    LeftRotate(r->left_son);
    return RightRotate(r);
}

template<class T, class L>
RNode<T, L> *RankTree<T, L>::RLRotate(RNode<T, L> *r) {
    RightRotate(r->right_son);
    return LeftRotate(r);
}

template<class T, class L>
RNode<T, L> *RankTree<T, L>::find(int NodeID) {
    RNode<T, L> *iter = this->root;
    while (iter && compare(iter->obj, NodeID)) {
        if (compare(iter->obj, NodeID) > 0) {
            iter = iter->left_son;
        } else iter = iter->right_son;
    }
    return iter;
}

template<class T, class L>
RNode<T, L> *RankTree<T, L>::find_rank_index(int data, long long *calc_rank, int *index) {
    RNode<T, L> *iter = this->root;
    *calc_rank = 0;
    *index = 0;
    while (iter) {
        //turn left subtree
        if (iter->obj->getSalary()> data) {
            iter = iter->left_son;
        }
            //turn right subtree
        else if (iter->obj->getSalary()<= data) {
            *calc_rank += get_rank_subtree(iter->left_son) + get_rank(iter);
            *index += size_subtree(iter->left_son) + 1;
            iter = iter->right_son;
        }
    }

    return iter;
}

template<class T, class L>
int sumRanksByIndex(RNode<T, L> *root, int index) {
    int counter = 0;
    RNode<T, L> *iter = root;

    int current = size_subtree(iter->left_son) + 1;

    while (iter) {

        //left tree
        if (current > index) {
            iter = iter->left_son;
            current += -size_subtree(iter) + size_subtree(iter->left_son);
        }
            //right tree
        else if (current < index) {
            counter += get_rank_subtree(iter->left_son) + get_rank(iter);
            iter = iter->right_son;
            current += size_subtree(iter->left_son) + 1;

        }
            //equals
        else {
            counter += get_rank_subtree(iter->left_son) + get_rank(iter);
            return counter;
        }
    }
    return counter;
}

template<class T, class L>
void RankTree<T, L>::SumOfBumpGradeBetweenTopWorkers(int m, void **sumBumpGrade) {
    long long tmax = get_rank_subtree(root);
    long long tmin=0;
    if (size-m>0){
       tmin = sumRanksByIndex(root, size - m);
    }
    **(long long **) sumBumpGrade = tmax - tmin;
}


template<class T, class L>
void RankTree<T, L>::inorder(RNode<T, L> *p) const {
    if (p == NULL) return;
    inorder(p->left_son);
    p->obj->print();
    inorder(p->right_son);
}

template<class T, class L>
int inorder_back(RNode<T, L> *p, int **keys, int index, int size) {
    if (p == NULL || index >= size) return index;
    index = inorder_back(p->right_son, keys, index, size);
    if (index < size) {
        (*keys)[index++] = p->obj->getID();
    }
    index = inorder_back(p->left_son, keys, index, size);

    return index;
}

template<class T, class L>
void RankTree<T, L>::print() const {
    inorder(root);
}

template<class T, class L>
void RankTree<T, L>::printToList(int **keys, int sizet) const {
    inorder_back(root, keys, 0, sizet);
}


template<class T, class L>
void DestroyTreeAux(RNode<T, L> *node, bool is_obj) {
    if (!node) return;
    DestroyTreeAux(node->left_son, is_obj);
    DestroyTreeAux(node->right_son, is_obj);
    if (is_obj) {
        delete node->obj;
        node->obj = NULL;
    }
    delete node;
    node = NULL;
}

template<class T, class L>
void RankTree<T, L>::DestroyTree(bool is_obj) {
    DestroyTreeAux(root, is_obj);
    this->root = NULL;
}

template<class T, class L>
RNode<T, L> *FindMin(RNode<T, L> *root, int MinEmployeeID) {
    RNode<T, L> *iter = root;
    while (iter->left_son->obj->getID() >= MinEmployeeID) {
        iter = iter->left_son;
    }
    return iter;
}

template<class T, class L>
void inorderRange(RNode<T, L> *node, int MinEmployeeID, int MaxEmployeeID, int MinSalary, int MinGrade,
                  int *TotalNumOfEmployees, int *NumOfEmployees) {
    if (node == NULL) return;

    //search left
    if (node->obj->getID() > MaxEmployeeID) {
        inorderRange(node->left_son, MinEmployeeID, MaxEmployeeID, MinSalary, MinGrade, TotalNumOfEmployees,
                     NumOfEmployees);
    }
        //search right
    else if (node->obj->getID() < MinEmployeeID) {
        inorderRange(node->right_son, MinEmployeeID, MaxEmployeeID, MinSalary, MinGrade, TotalNumOfEmployees,
                     NumOfEmployees);
    } else if (node->obj->getID() >= MinEmployeeID && node->obj->getID() <= MaxEmployeeID) {
        *TotalNumOfEmployees += 1;
        if (node->obj->getSalary() >= MinSalary && node->obj->getGrade() >= MinGrade) *NumOfEmployees += 1;
        inorderRange(node->left_son, MinEmployeeID, MaxEmployeeID, MinSalary, MinGrade, TotalNumOfEmployees,
                     NumOfEmployees);
        inorderRange(node->right_son, MinEmployeeID, MaxEmployeeID, MinSalary, MinGrade, TotalNumOfEmployees,
                     NumOfEmployees);

    } else return;
}


template<class T, class L>
void RankTree<T, L>::getMatch(int MinEmployeeID, int MaxEmployeeID, int MinSalary,
                              int MinGrade,
                              int *TotalNumOfEmployees, int *NumOfEmployees) {
    inorderRange(root, MinEmployeeID, MaxEmployeeID, MinSalary, MinGrade, TotalNumOfEmployees, NumOfEmployees);
}

template<class T, class L>
int treeToArrAUX(RNode<T, L> *root, T **arr, int index) {
    if (root == NULL) return index;
    index = treeToArrAUX(root->left_son, arr, index);
    (arr)[index++] = root->obj;
    index = treeToArrAUX(root->right_son, arr, index);

    return index;
}

template<class T, class L>
void RankTree<T, L>::treeToArr(T **arr) {
    treeToArrAUX(root, arr, 0);
}


template<class T, class L>
RNode<T, L> *RankTree<T, L>::buildEmptyTree(int height_needed, RNode<T, L> *parent) {
    if (height_needed <= 0) return NULL;
    auto node = new RNode<T, L>();
    node->height = height_needed - 1;
    node->father = parent;
    node->left_son = buildEmptyTree(height_needed - 1, node);
    node->right_son = buildEmptyTree(height_needed - 1, node);
    return node;
}

template<class T, class L>
int RankTree<T, L>::inorderBackRemoveExtra(RNode<T, L> *node, int nodes_to_remove) {
    if (node == NULL || nodes_to_remove <= 0) return nodes_to_remove;
    nodes_to_remove = inorderBackRemoveExtra(node->right_son, nodes_to_remove);
    nodes_to_remove = inorderBackRemoveExtra(node->left_son, nodes_to_remove);
    if (node->height == 0) {
        if (node->father->left_son == node) node->father->left_son = NULL;
        else if (node->father->right_son == node) node->father->right_son = NULL;
        delete node;
        node = NULL;
        return nodes_to_remove - 1;
    }

    //update height
    node->height = max(calcHeight(node->left_son), calcHeight(node->right_son)) + 1;

    return nodes_to_remove;
}

template<class T, class L>
int inorderFillTree(RNode<T, L> *node, T **arr, int sizet, int index) {
    if (node == NULL || index >= sizet) return index;
    index = inorderFillTree(node->left_son, arr, sizet, index);
    node->obj = arr[index];
    node->rank = arr[index++]->getRank();
    index = inorderFillTree(node->right_son, arr, sizet, index);
    return index;
}

template<class T, class L>
void postorderUpdateRate(RNode<T, L> *node) {
    if (!node) return;
    postorderUpdateRate(node->left_son);
    postorderUpdateRate(node->right_son);

    node->size_subtree = size_subtree(node->right_son) + size_subtree(node->left_son) + 1;
    node->rank_subtree = get_rank_subtree(node->left_son) + get_rank_subtree(node->right_son) + get_rank(node);

}


template<class T, class L>
void RankTree<T, L>::arrToTree(T **arr, int sizet) {
    int height_needed = 0;
    int current_nodes = 0;
    int nodes_to_remove = 0;

    if (sizet == 0) return;
    else if (sizet == 1) {
        height_needed = 1;
        nodes_to_remove = 0;
    } else if (sizet == 2) {
        height_needed = 2;
        nodes_to_remove = 1;
    } else {
        height_needed = ceil(log2(sizet + 1));
        current_nodes = pow(2, height_needed);
        nodes_to_remove = current_nodes - sizet - 1;
    }
    root = buildEmptyTree(height_needed, NULL);
    inorderBackRemoveExtra(root, nodes_to_remove);
    inorderFillTree(root, arr, sizet, 0);
    postorderUpdateRate(root);
    this->size = sizet;
}


#endif
