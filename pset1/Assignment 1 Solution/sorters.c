// Various implementations of sorting algorithms.
#include "sorters.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void swap(char ** elem1, char ** elem2) {
    char * temp = *elem1;
    *elem1 = *elem2;
    *elem2 = temp;
}

void bubble_sort(int argc, char ** args) {
    for (int i = 0; i < argc; i++) {
        for (int j = 0; j < argc - i - 1; j++) {
            if (strcmp(args[j], args[j + 1]) > 0) {
                swap(&args[j], &args[j + 1]);
            }
        }
    }
}

void bogo_sort(int argc, char ** args) {
    int num_swaps = 10;
    int seed = 2;
    srand(seed);
    // Idea: randomly shuffle. If the list is sorted, return. If not, try again.
    bool is_sorted = false;
    while (!is_sorted) {
        // Shuffle the list.
        for (int i = 0; i < num_swaps; i++) {
            int index1 = rand() % argc;
            int index2 = rand() % argc;
            swap(&args[index1], &args[index2]);
        }
        // Update is_sorted depending on list.
        is_sorted = true;
        for (int i = 0; i < argc - 1; i++) {
            if (strcmp(args[i], args[i + 1]) > 0) {
                is_sorted = false;  break;
            }
        }
    }
}


void Merge(char ** argv, int l, int m, int r)
{
      //l,m,r are left,middle, right indexes in the argv array
      int i=0, j=0, k=l, n1 = m - l + 1, n2 =  r - m;
      //l is the merged subarray future injection index, i and j are L and R indexes that will be used for merging
      //Create now two temporary memory string arrays, such that we know the initial orders
      char **L; char **R; //L must have size n1, R must be n2
      L=malloc((n1+1)*sizeof(char*));
      R=malloc((n2+1)*sizeof(char*));

      //Introduce relevant data in the arrays L and R
      for (i = 0; i < n1; i++)
            L[i] = argv[l + i];
      for (j = 0; j < n2; j++)
            R[j] = argv[m + 1+ j];

      //reinitialize for use in while loop
      i=0;
      j=0;
      while (i < n1 && j < n2)
      {
            if (strcmp(L[i], R[j])<0)
            {     swap(&argv[k], &L[i]);//I swap the address
                  i++;
            }
            else
            {     swap(&argv[k], &R[j]);
                  j++;
            }
            k++;
      }

    //If elements of L remained unused yet, I copy them
      while (i < n1)
      {     swap(&argv[k], &L[i]);
            i++;
            k++;
      }

      //Same as above for R
      while (j < n2)
      {
            swap(&argv[k], &R[j]);
            j++;
            k++;
      }
}


/* l is left index, r is right index of the
   sub-array of argv to be sorted */
void MergeSort(char ** argv, int l, int r)
{
      if (l < r)
      {
            int m = (l+r)/2;
            // Divide et Impera
            MergeSort(argv, l, m);
            MergeSort(argv, m+1, r);
            Merge(argv, l, m, r);
      }
}



void merge_sort(int argc, char ** args){
      MergeSort(args, 0, argc-1);//left index is 0, right index is argc-1
}

