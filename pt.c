#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char **argv)
{
   FILE *fp = fopen(argv[1],"r");
   char ch;
   while ((ch = fgetc(fp)) != EOF) {
   	printf("%d\n",ch);
   }
}
