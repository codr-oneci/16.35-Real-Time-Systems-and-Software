#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Node structure
typedef struct Node {
    struct Node * upper;
    struct Node * lower;
    char * item;
} node;

void insert(node * root, char * str) {
    // Is the tree empty? If so return a new node (important step for the initialization!)
    if (root == NULL)
    {//return newNode(str);
        struct Node *aux = new Node;
        aux->item = str;
        aux->lower = aux->upper = NULL; //there's only one element, so we initialize NULL pointers
        return temp; }
    //Use recursion for string insertion
    if (strcmp(root->item,str)>0)
        root->lower  = insert(root->lower, str);
    else if (strcmp(str,root->item)>0)
        root->upper = insert(root->upper, str);


}

void print_tree(node * root) {
  /** Y O U R   C O D E   H E R E  */
}

void free_tree(node * root) {
    if (root != NULL) {
        free_tree(root->upper);
        free_tree(root->lower);
        free(root);
    }
}

int main (int argc, char ** argv) {
    node * root = malloc(sizeof(node));
    root->upper = NULL;
    root->lower = NULL;
    root->item = "";
    for (int i = 1; i < argc; i++){
        insert(root,argv[i]);
    }
    print_tree(root);
    free_tree(root);
}
