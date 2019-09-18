#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEBUG 0
#define MAX_LINE_LENGTH 100
#define BREAK_LINE_COMMAND  ".b"
#define BLANK_LINE_COMMAND  ".p"
#define LEFT_SHIFT_COMMAND  ".l"
#define LINE_WIDTH_COMMAND  ".w"
/****************************************************************/

/* function prototypes */
void _process_file(FILE * fp);
void _process_char(FILE *fp,char ch,char* word,int * blank_flag,int * _word_count,int* _line_count,int *p_flag,int *b_flag);
void _process_break_command();
void _process_blank_command();
void _process_left_shift_command();
void _process_line_width_command(FILE * fp,int * _line_count);
void _process_common_word(int *_line_count,int *_word_count,char *word);
int _word_is_command(char * word);
/****************************************************************/
static int LENGTH_LIMIT=50; // maximum line length 
static int LEFT_SHIFT=4;    // shift space
static char _one_line[MAX_LINE_LENGTH]; // one line words smaller than MAX_LINE_LENGTH
static char _result[MAX_LINE_LENGTH];
static char _number[3];
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

          if (!strcmp(word,BREAK_LINE_COMMAND) && !(*b_flag))  // word == .b  -> break current line
            {
               if(each_char == '\n'|| each_char=='\r') // if the command followed by other word . it's just command word not a command
                {
                  _process_break_command();
                  *b_flag=1; // avoid consecutive break
                  *p_flag=0; // reset p_flag
                  *_line_count=0; // reset the one_line_count
                }
                else
                {
                   _process_common_word(_line_count,_word_count,word);
                   *p_flag=0;
                   *b_flag=0;
                }
                
            }
            
            if (!strcmp(word,BLANK_LINE_COMMAND) && !(*p_flag)) // word == .p -> leave new line
            {
                if(each_char == '\n'|| each_char=='\r') // if the command followed by other word . it's just command word not a command
                {
                  _process_blank_command();
                  *p_flag=1;
                  *b_flag=0; // reset b_flag;
                  *_line_count=0; // reset the one_line_count
                }
                else
                { // this part is same as the next branch . 
                    // Just leave these lines here for better understanding
                  _process_common_word(_line_count,_word_count,word);
                  *p_flag=0;
                  *b_flag=0;
                }
            }
            if(!strcmp(word,LEFT_SHIFT_COMMAND))
            {
              _process_left_shift_command();
              *p_flag=0;
              *b_flag=0;
            }
            if(!strcmp(word,LINE_WIDTH_COMMAND))
            { 
              _process_line_width_command(fp,_line_count);
              *p_flag=0;
              *b_flag=0;
            }

       }
       else
       {
         *p_flag=0;
         *b_flag=0;
         _process_common_word(_line_count,_word_count,word);
       }
       
       memset(word,'\0',MAX_LINE_LENGTH);
       *_word_count=0;
       *blank_flag = 1;
    }
}
int _word_is_command(char * word){
    return !strcmp(word,BLANK_LINE_COMMAND) || !strcmp(word,BREAK_LINE_COMMAND)|| !strcmp(word,LEFT_SHIFT_COMMAND) || !strcmp(word,LINE_WIDTH_COMMAND);
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
void _process_left_shift_command(){

}
void _process_line_width_command(FILE *fp,int * _line_count)
{
   strcat(_one_line,"\r");
   strcat(_one_line,"\n");
   strcat(_result,"    "); // add first 4 spaces
   strcat(_result,_one_line); // copy one_line to one_line_result
   memset(_one_line,'\0',MAX_LINE_LENGTH);
   *_line_count=0;
   fgetc(fp); // skip current char which is space
   char number_char;
   int number_count=0;
   int width_number;
   while((number_char = fgetc(fp)) != ' '){
       _number[number_count]=number_char;
       number_count++;
   }
   width_number = atoi(_number);
   LENGTH_LIMIT = width_number;
   memset(_number,'\0',3); // initialize result array
}
void _process_common_word(int *_line_count,int *_word_count,char *word){
    if(*_line_count + *_word_count < LENGTH_LIMIT)
     {
         (*_line_count)++;  // one space
         *_line_count+=*_word_count; // add word length
         strcat(_one_line,word);
         strcat(_one_line," ");       
     }
     else
     {
        _one_line[strlen(_one_line)-1]='\0';//delete the last space
        strcat(_one_line,"\r");
        strcat(_one_line,"\n");
        strcat(_result,"    "); // add first 4 spaces
        strcat(_result,_one_line); // copy one_line to one_line_result
        memset(_one_line,'\0',MAX_LINE_LENGTH);
        *_line_count=0;
        strcat(_one_line,word);
        strcat(_one_line," ");
        *_line_count+=*_word_count;
     }

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
    memset(_number,'\0',3); // initialize result array
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
        printf("cant oepn file");
        return -1;
    }
    _process_file(fp);
    // process last line
    strcat(_result,"    ");
    _one_line[strlen(_one_line)-1]='\0';
    strcat(_one_line,"\r");// add first 4 spaces
    strcat(_one_line,"\n");
    strcat(_result,_one_line);
    FILE *writer = fopen(argv[2],"w");
    fprintf(writer,"%s",_result);
    fclose(fp);
    fclose(writer);
    return 0;
}
