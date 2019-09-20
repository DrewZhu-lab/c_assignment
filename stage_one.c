#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEBUG 0
#define MAX_LINE_LENGTH 100
#define BREAK_LINE ".b"
#define BLANK_LINE ".p"
/****************************************************************/

/* function prototypes */
void _process_file(FILE * fp);
void _process_char(FILE *fp,char ch,char* word,int * blank_flag,int * _word_count,int* _line_count,int *p_flag,int *b_flag);
void _process_break_command();
void _process_blank_command();
int _word_is_command(char * word);
/****************************************************************/
static int LENGTH_LIMIT=50; // maximum line length 
static int LEFT_SHIFT=4;    // shift space
static char _one_line[MAX_LINE_LENGTH]; // one line words smaller than MAX_LINE_LENGTH
static char _result[MAX_LINE_LENGTH];
void _process_char(FILE *fp,char each_char,char* word,int * _word_count,int * blank_flag,int* _line_count,int *p_flag,int *b_flag)
{

    if (isalpha(each_char)||isdigit(each_char)||ispunct(each_char)) 
    {
        word[*_word_count] = each_char;
        (*_word_count)++;
        *blank_flag=0;
    }
    else if (isspace(each_char) && !(*blank_flag)) // end of one word
    {
       if(_word_is_command(word))
       {
       }
       else
       {
         *p_flag=0;
         *b_flag=0;

         if(*_line_count + *_word_count < LENGTH_LIMIT)
         {
            strcat(_one_line,word);
            strcat(_one_line," ");   
            (*_line_count)++;  // one space
            *_line_count+=*_word_count; // add word length
                
         }
         else
         {
          	_one_line[strlen(_one_line)-1]='\0';//delete the last space
          	strcat(_one_line,"\r");// add first 4 spaces
            strcat(_one_line,"\n");
		        strcat(_result,"    ");
          	strcat(_result,_one_line); // copy one_line to one_line_result
          	memset(_one_line,'\0',MAX_LINE_LENGTH);
          	*_line_count=0;
            // current word for new line
          	strcat(_one_line,word);
          	strcat(_one_line," ");
          	*_line_count+=*_word_count;
         }
       }
       
       memset(word,'\0',MAX_LINE_LENGTH);
       *_word_count=0;
       *blank_flag = 1;
    }
}
int _word_is_command(char * word){
    return !strcmp(word,BLANK_LINE) || !strcmp(word,BREAK_LINE)|| !strcmp(word,".l") || !strcmp(word,".w");
}
/**  break current line
    the next word would be the next newline.
**/
void _process_break_command()
{
    _one_line[strlen(_one_line)-1]='\0';//delete the last space
    strcat(_one_line,"\r\n");// add extra end char for old lines 
    for(int i=0;i<LEFT_SHIFT;++i){ // add shift spaces for new lines
        strcat(_one_line," ");
    }
    strcat(_result,_one_line); // copy one_line to one_line_result
    memset(_one_line,'\0',MAX_LINE_LENGTH); // clear current line 
}
/** leave blank line
    the next word would be the next newline.
**/
void _process_blank_command(){
    _one_line[strlen(_one_line)-1]='\0';//delete the last space
    strcat(_one_line,"\r\n\r\n");// add extra end char for old lines 
    for(int i=0;i<LEFT_SHIFT;++i){
        strcat(_one_line," ");
    }
    strcat(_result,_one_line); // copy one_line to one_line_result
    memset(_one_line,'\0',MAX_LINE_LENGTH); // clear current line 
}
void _process_file(FILE * fp)
{
    char each_char;
    char word[MAX_LINE_LENGTH];
    int _word_count=0;
    int _blank_flag=0;
    int _line_count=0;
    int _p_flag=0;
    int _b_flag=0;
    memset(word,'\0',MAX_LINE_LENGTH); // initialize word array
    memset(_one_line,'\0',MAX_LINE_LENGTH); // initialize line array
    memset(_result,'\0',MAX_LINE_LENGTH); // initialize result array
    while ((each_char = fgetc(fp)) != EOF) 
    {
        _process_char(fp,
                   each_char,
                   word,
                   &_word_count,
                   &_blank_flag,
                   &_line_count,
                   &_p_flag,
                   &_b_flag);
    }
}
int main(int argc, char **argv)
{
    FILE *fp = fopen(argv[1],"r");
    if (!fp)
    {
        printf("cant open file");
        return -1;
    }
    _process_file(fp);
    // process last line
    strcat(_result,"    ");
    _one_line[strlen(_one_line)-1]='\0';
    strcat(_one_line,"\r");// add first 4 spaces
    strcat(_one_line,"\n");
    strcat(_result,_one_line);
    //FILE *writer = fopen(argv[2],"w");
    //fprintf(writer,"%s",_result);
    fclose(fp);
    printf("%s\n",_result);
    //fclose(writer);
    return 0;
}
