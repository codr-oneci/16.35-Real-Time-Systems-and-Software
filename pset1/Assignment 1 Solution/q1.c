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
      // BASE CASE: is the tree empty (not containing an item)?
      if (strcmp(root->item, "")==0)
      {
            root->item = str; //if so putthe current word in it as item
      }
      //Use recursion for string insertion
      if (strcmp(root->item,str)>0)
      {
            if (root->lower==NULL)//if root->lower points nowhere, initialize it
            {
                  root->lower = malloc(sizeof(node));//allocate memory
                  root->lower->upper = NULL;
                  root->lower->lower = NULL;
                  root->lower->item = "";//this is the type of item initialization requested by the problem
            }
            insert(root->lower, str);//now that root->lower exists, insert item in it
      }

      else if (strcmp(str,root->item)>0)
      {
            if (root->upper==NULL)//if root->upper points nowhere, initialize it
            {
                  root->upper = malloc(sizeof(node)); //allocate memory
                  root->upper->upper = NULL;
                  root->upper->lower = NULL;
                  root->upper->item = "";//this is the type of item initialization requested by the problem
            }
            insert(root->upper, str);//now that root->upper exists, insert item in it
      }
}

void print_tree(node * root)
{
  if (root!=NULL) //print only non-empty structures
  {
      print_tree (root->lower);
      printf("%s  ", root->item);
      print_tree (root->upper);
  }

}

void free_tree(node * root)
{
    if (root != NULL)
    {
        free_tree(root->upper);
        free_tree(root->lower);
        free(root);
    }
}

int main (int argc, char ** argv)
{
    node * root = malloc(sizeof(node));
    root->upper = NULL;
    root->lower = NULL;
    root->item = "";
    for (int i = 1; i < argc; i++)
      {
        insert(root,argv[i]);
      }
    print_tree(root);
    free_tree(root);
}
