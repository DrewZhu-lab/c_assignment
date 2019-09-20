#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argv,char** argc){
    char c;
    while((c = getchar())!=EOF)
    {
        printf("%c",c);
    }

}
