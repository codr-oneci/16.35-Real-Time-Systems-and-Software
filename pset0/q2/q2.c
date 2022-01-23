//Author: Codrin Oneci
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char** argv) {
      double x,y, out_value;
	char oper;
      if(argc!=4) //I expect only 3 arguments (3 strings) but I have to count the program name too so 3+1=4
	{
		printf("Invalid Input\n");
		return -1; //this is optional C convention in which return 0 means ok while return -1 will suggest error
	}
	//obtain the values for the numbers; argv[i] gives string array at location number i using the pointer
	x = atof(argv[1]); //convert string to double as sugested in the pset text
	y = atof(argv[3]); //convert string to double

	//Important note: if the command line inputs for x or y are actually not
	//numeric, the atof function outputs 0.0 so check this possibility

	int j=0,i=0,dot_count=0;
	j=strlen(argv[1]);
	if (x==0)
	{
	for (i=0;i<j;i++){
	    if (argv[1][i]!=48&&argv[1][i]!="."&&argv[1][i]!='\0')
            {
                  printf("Invalid Input\n");
                  return -1;
            }
	    if (argv[1][i]==".")
	    {
	          dot_count++;
          }
	}//48 is the ASCII value of 0; and '\0' is the termination character
	}
	if (dot_count>1)
            {
                  printf("Invalid Input\n");
                  return -1;
            }
	dot_count=0;//put the counter to zero
	j=strlen(argv[3]);
	if (y==0)
	{
	for (i=0;i<j;i++){
	    if (argv[3][i]!=48&&argv[3][i]!="."&&argv[1][i]!='\0')
            {
                  printf("Invalid Input\n");
                  return -1;
            }
	    if (argv[3][i]==".")
            {
                  dot_count++;
            }
	}
	}
	if (dot_count>1){printf("Invalid Input\n");return -1; }




	//extract the operation value
	oper=argv[2][0];

	switch(oper)
	{
		case '*':
			out_value=x*y;
			break;
		case '/':
			out_value=x/y;
			break;
		case '-':
			out_value=x-y;
			break;
            case '+':
			out_value=x+y;
			break;
		case 'x':
			out_value=x*y;
			break;
		case 'X':
			out_value=x*y;
			break;

		default:
			printf("The operation type input is not recognized. Choose from [*,/,-,+,x,X].");
			break;
	}

if(oper=='*' || oper=='/' || oper=='-' || oper=='+' || oper=='x' || oper=='X' )
      {
            printf("%f",out_value);
      }
else
      {
            return -1 ;
      }
   return 0;
}
