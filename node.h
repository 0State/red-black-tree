#ifndef node_h
#define node_h
#include <iostream>
template <class T>
class RBNode {
  public:
    T data;
    /* - red = 1
       - black = 0 */
    int color;
    /* - index 0 is the left node
       - index 1 is the right node*/
    RBNode *link[2];
    
    RBNode(const T& d = T(), RBNode *l = NULL, 
           RBNode *r = NULL, int c = 1) {
	data = d;
	link[0] = l;
        link[1] = r;
        color = c;
    }
};
#endif

template <class T>
void tree_clear(RBNode<T> *&node) {
    if(node != NULL) {
        tree_clear(node->link[0]);
        tree_clear(node->link[1]);
        delete node;
        node = NULL;
    }
}
