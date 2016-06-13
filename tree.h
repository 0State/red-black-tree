#ifndef tree_h
#define tree_h
#include <iostream>
#include "node.h"

template <class T>
class RBTree {
  private:
    RBNode<T> *root;
    void insert_entry(RBNode<T> *&node, const T& entry);
    void print_entries(RBNode<T> *node);
    RBNode<T>* remove_entry(RBNode<T> *node, T target, int *done);
    int is_red(RBNode<T> *node);
    void single_rotate(RBNode<T> *&node, int d);
    void double_rotate(RBNode<T> *&node, int d);
    bool find_entry(RBNode<T> *&node, const T& target);
    RBNode<T> *remove_balance(RBNode<T> *node, int d, int *done);

  public:
    RBTree() {
      root = NULL;
    }
    ~RBTree() {
      tree_clear(root);
    }
    void insert(const T& entry);
    void print();
    void find(const T& target);
    void remove(T target);
};
#endif

//**************************************************************//
//**************** Private Member Functions ********************//
//**************************************************************//

/* Helper function that checks to see if a node's color
   is red or not. Also checks to make sure node is not NULL */
template <class T>
int RBTree<T>::is_red(RBNode<T> *node) {
  /* Returns condition that node is not NULL and the color of
     the node is red (color = 1) */
  return node != NULL && node->color == 1;
}

/* Single rotate function*/
template <class T>
void RBTree<T>::single_rotate(RBNode<T> *&node, int d) {
  /* Assign temp to node's link that is not index d I.E 
     node child's sibling (EX: if index d = 1 (right) then 
     temp is assigned to the opposite node index d = 0 (left).) */
  RBNode<T> *temp = node->link[!d];
  /* Assign node's link that is not index d to temp's 
     link that is index d (I.E assign node child's sibling
     to temp's child) */
  node->link[!d] = temp->link[d];
  /* Assign temp's link that is index d (temp's child) to node */
  temp->link[d] = node;
  /* Assign node's color to red (color = 1)  */ 
  node->color = 1;
  /* Assign temp's color to black (color = 0)  */
  temp->color = 0;  
  /* Assign node to temp */
  node = temp;
}

/* Double rotate function */
template <class T>
void RBTree<T>::double_rotate(RBNode<T> *&node, int d) {
  /* Do a single rotate w/ node's link that is not index d 
     (I.E node child's sibling) */
  single_rotate(node->link[!d], !d);
  /* Do a single rotate with node and index d */
  single_rotate(node, d);
}

template <class T>
void RBTree<T>::insert_entry(RBNode<T> *&node, const T& entry) {
  /* If node is NULL then create a new node*/
  if(node == NULL)
    node = new RBNode<T>(entry);
  /* Otherwise traverse through list and do rotations */
  else {
    /* - dir = 1 if entry is greater than node->data
       - dir = 0 if entry is less than node->data */
    int dir = node->data < entry;
    /* - traverse left if dir = 0
       - traverse right if dir = 1 */
    insert_entry(node->link[dir], entry);
    /* BEGIN REBALANCING */
    /* If node's child is red then move to Case 1  */
    if(is_red(node->link[dir])) {
      /* If node's link's sibling is red then
         - set node's color to red (color = 1)
         - set node's left child's color to black (color = 0)
         - set node's right child's color to black (color = 0) */
      if(is_red(node->link[!dir])) {
        node->color = 1;
        node->link[0]->color = 0;
        node->link[1]->color = 0;    
      }
      /* Otherwise move to Cases 2 & 3 */
      else {
        /* If node's grandchild is red then do a single rotate */
        if(is_red(node->link[dir]->link[dir]))
	  single_rotate(node, !dir);
        /* Otherwise if node's other grandchild is red do a 
           double rotate */
        else if(is_red(node->link[dir]->link[!dir]))
          double_rotate(node, !dir); 
      }
    }
  }
}

/* Print helper function: prints entries in an inorder traversal */
template <class T>
void RBTree<T>::print_entries(RBNode<T> *node) {
  if(node == NULL)
    return; 
  print_entries(node->link[0]);
  std::cout << node->data << " ";
  print_entries(node->link[1]);
}

/* Find helper function: searches for target entry */
template <class T>
bool RBTree<T>::find_entry(RBNode<T> *&node, const T& target) {
  if(node == NULL)
    return false;
  else if(target == node->data)
    return true;
  else {
    int dir = node->data < target;
    return find_entry(node->link[dir], target);
  } 
}

/* Remove helper function: deletes entry */
template <class T>
RBNode<T>* RBTree<T>::remove_entry(RBNode<T> *node, T target, int *done) {
  /* If node is NULL set done to 1  */
  if(node == NULL) 
    *done = 1;
  /* Otherwise do deletion scenarios  */
  else {
    /* Initialize variable for tree direction */
    int dir;
    /* If target is equal to data then begin deletion process */
    if(node->data == target) {
      /* If node has at least 1 child then delete accordingly */
      if(node->link[0] == NULL || node->link[1] == NULL) {
        /* Assign temp node to node's NULL left child */
	RBNode<T> *temp = node->link[node->link[0] == NULL];
        /* if node is red the set done to 1 */
        if(is_red(node))
	  *done = 1;
        /* Otherwise if temp node is red then:
           - temp's color to black 
           - set done to 1*/
	else if(is_red(temp)) {
	  temp->color = 0;
	  *done = 1;
	}
	/* delete original node */
	delete node;
        /* return temp node */
	return temp;		
      }
      /* Otherwise node is a leaf and delete accordingly */
      else {
        /* Assign temp to node's left child */
	RBNode<T> *temp = node->link[0];
	/* Keep traversing right until leaf is reached */
	while(temp->link[1] != NULL) 
	  temp = temp->link[1];
	/* set the node's data to temp's data */
	node->data = temp->data;
        /* Assign target to temp's data */
	target = temp->data;
      }
    }
    /* Find direction in which tree should traverse */
    dir = node->data < target;
    /* Recurse remove_entry */
    node->link[dir] = remove_entry(node->link[dir], target, done);
    /* If node is not 1 then balance tree */
    if(!done)
      node = remove_balance(node, dir, done);
  }
  /* return original node */
  return node;
}

/* Balance tree after a node is removed */
template <class T>
RBNode<T> *RBTree<T>::remove_balance(RBNode<T> *node, int d, int *done) {
  /* Create two nodes:
     - assign u to node 
     - assign v to node's child's sibling */
  RBNode<T> *u = node;
  RBNode<T> *v = node->link[!d];
  
  /* If v is a red node then:
     - do a single rotation
     - set v to u's child's sibling */
  if(is_red(v)) {
    single_rotate(node, d);
    v = u->link[!d];
  }
  
  /* If v is not NULL then begin we need to check if v's children
     are black */
  if(v != NULL) {
    /* If v's children are black then
       - check to see if u is red. If it is set done to one. If it
         isn't if statement is skipped 
       - set u's color to black 
       - set v's color to red */
    if(!is_red(v->link[0]) && !is_red(v->link[1])) {
      if(is_red(u))
	*done = 1;
      
      u->color = 0;
      v->color = 1;
    }
    /* Otherwise v's children are red amd we assign colors */
    else {
      /* set c to u's color */
      int c = u->color;
      /* new_node is 1 if node is equal to u and 0 if it is not */
      int n = (node == u);
      /* If v's child's sibling is red then do a single rotate */
      if(is_red(v->link[!d]))
	single_rotate(u, d);
      /* Otherwise do a double rotate  */
      else 
        double_rotate(u, d);
      /* set u's color to c */ 
      u->color = c;
      /* set u's left and right children's color to black */
      u->link[0]->color = 0;
      u->link[1]->color = 0;
      
      /* if n is 1 the assign node to u */
      if(n)
	node = u;
      /* Otherwise assign node's child to u */
      else
        node->link[d] = u;

      *done = 1;
    }
  }
  /* return original node */
  return node;
}
//*************************************************************//
//**************** Public Member Functions ********************//
//*************************************************************//

/* Calls insert helper function */
template <class T>
void RBTree<T>::insert(const T& entry) {
  insert_entry(root, entry);
  root->color = 0;
}

/* Calls print helper function */
template <class T>
void RBTree<T>::print() {
  print_entries(root);
  std::cout << "\n";
}

/* Calls find helper function */
template <class T>
void RBTree<T>::find(const T& target) {
  if(find_entry(root, target))
    std::cout << target << " was found.\n";
  else
    std::cout << target << " was not found.\n";
}

/* Calls remove helper function */
template <class T>
void RBTree<T>::remove(T target) {
  /* Checks to see if target is in tree first. If it is
     call remove_entry function. */
  if(find_entry(root, target)) {
    int done = 0;
    root = remove_entry(root, target, &done);
    if(root != NULL)
      root->color = 0;
  }
  /* Otherwise print message saying entry is not in tree */
  else {
    std::cout << "Entry does not exist in tree.\n";
    return;
  }
}
