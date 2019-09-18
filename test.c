#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_WORD_LEN  50
#define DEBUG 0
#define LARGE_LENGTH 100
static char one_line[LARGE_LENGTH];
static char one_line_result[LARGE_LENGTH];
void check_char(FILE *fp,char ch,char* word,int * blank_flag,int * count,int* one_line_count)
{
    if (isalpha(ch)||isdigit(ch)||ispunct(ch)) 
    {
        word[*count] = ch;
        (*count)++;
        *blank_flag=0;
    }
    else if (isspace(ch) && !(*blank_flag)) {
       if(!strcmp(word,".p")|| !strcmp(word,".b"))
       {
			  
       }
       else{
       if(*one_line_count + *count < MAX_WORD_LEN){
	       (*one_line_count)++;  // one space
       	       *one_line_count+=*count; // add word length
	       strcat(one_line,word);
       	       strcat(one_line," ");       
       }
       else
       {
            one_line[strlen(one_line)-1]='\0';//delete the last space
            strcat(one_line,"\r");
            strcat(one_line,"\n");
            //printf("%d\n",one_line[strlen(one_line)-1]);
            strcat(one_line_result,"    "); // add first 4 spaces
            strcat(one_line_result,one_line); // copy one_line to one_line_result
            memset(one_line,'\0',LARGE_LENGTH);
            *one_line_count=0;
            strcat(one_line,word);
            strcat(one_line," ");
            *one_line_count+=*count;
       }
       }
       memset(word,'\0',LARGE_LENGTH);
       *count=0;
       *blank_flag = 1;
    }
}
void read_words(FILE * fp)
{
    char ch;
    char word[LARGE_LENGTH];
    memset(word,'\0',LARGE_LENGTH);
    memset(one_line,'\0',LARGE_LENGTH);
    memset(one_line_result,'\0',LARGE_LENGTH);
    int blank_flag = 0;
    int count=0;
    int one_line_count=0;
    while ((ch = fgetc(fp)) != EOF) {
        check_char(fp,ch,word,&blank_flag,&count,&one_line_count);
    }
}
int main(int argc, char **argv)
{
    FILE *fp = fopen(argv[1],"r");
    if (!fp)
        return -1;
    read_words(fp);
    // last line operation
    strcat(one_line_result,"    ");
    one_line[strlen(one_line)-1]='\0';
    strcat(one_line,"\r");
    strcat(one_line,"\n");
    strcat(one_line_result,one_line);
    FILE *writer = fopen(argv[2],"w");
    fprintf(writer,"%s",one_line_result);
    fclose(fp);
    fclose(writer);
    return 0;
}
