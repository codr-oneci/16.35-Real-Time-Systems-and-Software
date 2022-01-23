//Author: Codrin Oneci
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char** argv)
{ int p=1;//assume symmetry
 int count_characters=0; //this variable will store a count of the total number of characters

 if (argc<=1)
      { printf(" is NOT a palindrome!\n");
return 0;}//In the case that the program does not receive at least an input string (word)

 for (int i=1; i<argc; i++)
      {
            count_characters+=strlen(argv[i]);
      }



char *concat_str=malloc(count_characters);//in this array I concatenate all the words

for (int i=1; i<argc; i++)
      {
            strcat(concat_str,argv[i]);
      }
//at this point, I have stored all the relavant data in a single char array called concat_str

int l=strlen(concat_str);//length of the string with concatenated words
int i=0;
//create a lowercase string now from the initial string of concatenated words
//observe that in the example that you gave us the program is case insensitive
char *concat_str_l=malloc(count_characters);
while(concat_str[i])
      {
            concat_str_l[i]=tolower(concat_str[i]);
            i++;
      }

for(i = 0; i < l; i++)
	{if(concat_str_l[i] != concat_str_l[l - i - 1]) //check palindrome symmetry
		{
                  p = 0;
                  break;
		} //if not symmetric, it is not a palindrome so p=0
	}

char *input_str=malloc(count_characters+argc);//in this array I concatenate all the words with spaces too for the printing
for (int i=1; i<argc; i++)
      {     strcat(input_str,argv[i]);
            if (i<argc-1)
            {
                  strcat(input_str," ");
            }
      }

if (p==1)
    printf("\"%s\" is a palindrome!\n",input_str);
else
      printf("\"%s\" is NOT a palindrome!\n",input_str);
    return 0;
}

