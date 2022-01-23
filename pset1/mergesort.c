#include<stdlib.h>
#include<stdio.h>
#include<string.h>

void swap(char ** elem1, char ** elem2) {
    char * temp = *elem1;
    *elem1 = *elem2;
    *elem2 = temp;
}

void Merge(char ** argv, int l, int m, int r)
{ //l,m,r are left,middle, right indexes in the argv array
    int i=0, j=0, k=l, n1 = m - l + 1, n2 =  r - m;
  //l is the merged subarray future injection index, i and j are L and R indexes that will be used for merging
    //Create now two temporary memory string arrays, such that we know the initial orders
    char **L; char **R; //L must have size n1, R must be n2
    L=malloc((n1+1)*sizeof(char*));R=malloc((n2+1)*sizeof(char*));

    //Introduce relevant data in the arrays L and R
    for (i = 0; i < n1; i++)
        L[i] = argv[l + i];
    for (j = 0; j < n2; j++)
        R[j] = argv[m + 1+ j];
      i=0;j=0;
    while (i < n1 && j < n2)
    {
        if (strcmp(L[i], R[j])<0)
        {    swap(&argv[k], &L[i]);//I swap the address
            i++;
        }
        else
        {   swap(&argv[k], &R[j]);
            //argv[k] = R[j]; tried this doesn't work
            j++;
        }
        k++;
    }

    //If elements of L remained unused yet, I copy them
    while (i < n1)
    {    swap(&argv[k], &L[i]);
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

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
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


int main(int argc, char ** argv)
{

    MergeSort(argv, 1, argc-1); //I remember that argv[0] is the name of the program, so start with index 1, go to index argc-1
    for (int i=1; i < argc; i++)
        printf("%s ", argv[i]);
    return 0;
}
