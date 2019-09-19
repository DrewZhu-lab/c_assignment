#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define DEBUG 0
#define MAX_LINE_LENGTH 100
#define FILE_MAX_SIZE 99999
#define BREAK_LINE_COMMAND  ".b"
#define BLANK_LINE_COMMAND  ".p"
#define LEFT_SHIFT_COMMAND  ".l"
#define LINE_WIDTH_COMMAND  ".w"
#define CENTER_LINE_COMMAND ".c"
/****************************************************************/

/* function prototypes */
void _process_file(FILE * fp);
void _process_char(FILE *fp,char ch,char* word,int * blank_flag,int * _word_count,int* _line_count,int *p_flag,int *b_flag);
void _process_break_command();
void _process_blank_command();
void _process_left_shift_command(FILE * fp,int * _line_count);
void _process_line_width_command(FILE * fp,int * _line_count);
void _process_common_word(int *_line_count,int *_word_count,char *word);
void _process_center_command(FILE *fp,int *_line_count);
int _word_is_command(char * word);
/****************************************************************/
static int LENGTH_LIMIT=50; // maximum line length 
static int LEFT_SHIFT=4;    // shift space
static int CONSECUTIVE_MARGIN_CHANGE=0; // should add new line or not
static char _one_line[MAX_LINE_LENGTH]; // one line words smaller than MAX_LINE_LENGTH
static char _result[FILE_MAX_SIZE];
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
               if(CONSECUTIVE_MARGIN_CHANGE){
                   CONSECUTIVE_MARGIN_CHANGE=0;
               }
               if(each_char == '\n'|| each_char=='\r') 
               // if the command followed by other word . it's just command word not a command
                {
                  _process_break_command();
                  *b_flag=1; // avoid consecutive break
                  *_line_count=0; // reset the one_line_count
                }
                else
                {
                   _process_common_word(_line_count,_word_count,word);
                   *p_flag=0;
                   *b_flag=0;
                }
                
            }
            
        else  if (!strcmp(word,BLANK_LINE_COMMAND) && !(*p_flag)) 
            // word == .p -> leave new line
            {
               if(CONSECUTIVE_MARGIN_CHANGE){
                   CONSECUTIVE_MARGIN_CHANGE=0;
               }
                if(each_char == '\n'|| each_char=='\r') 
                // if the command followed by other word . it's just command word not a command
                {
                  _process_blank_command();
                  *p_flag=1;
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

        else  if(!strcmp(word,LEFT_SHIFT_COMMAND))
            {
              _process_left_shift_command(fp,_line_count);
              *p_flag=0;
              *b_flag=0;
            }

        else if(!strcmp(word,LINE_WIDTH_COMMAND))
            { 
              _process_line_width_command(fp,_line_count);
              *p_flag=0;
              *b_flag=0;
            }
        else if(!strcmp(word,CENTER_LINE_COMMAND))
            {
              _process_center_command(fp,_line_count);
            }
       }
       else
       {
         if(CONSECUTIVE_MARGIN_CHANGE)
         {
            CONSECUTIVE_MARGIN_CHANGE=0;
         }
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
    return !strcmp(word,BLANK_LINE_COMMAND) || !strcmp(word,BREAK_LINE_COMMAND)|| !strcmp(word,LEFT_SHIFT_COMMAND) || !strcmp(word,LINE_WIDTH_COMMAND) || !strcmp(word,CENTER_LINE_COMMAND);
}
/**  break current line
    the next word would be the next newline.
**/
void _process_break_command()
{
    _one_line[strlen(_one_line)-1]='\0';//delete the last space
    strcat(_one_line,"\r");// add extra end char for old lines 
    strcat(_one_line,"\n");// add extra end char for old lines 
    for(int i=0;i<LEFT_SHIFT;++i){ // add shift spaces for new lines
        strcat(_result," ");
    }
    strcat(_result,_one_line); // copy one_line to one_line_result
    memset(_one_line,'\0',MAX_LINE_LENGTH); // clear current line 
}
/** leave blank line
    the next word would be the next newline.
**/
void _process_blank_command(){
    _one_line[strlen(_one_line)-1]='\0';//delete the last space
    strcat(_one_line,"\r");// add extra end char for old lines 
    strcat(_one_line,"\n");// add extra end char for old lines 
    strcat(_one_line,"\r");// add extra end char for old lines 
    strcat(_one_line,"\n");// add extra end char for old lines 
    for(int i=0;i<LEFT_SHIFT;++i){ // add shift spaces for new lines
        strcat(_result," ");
    }
    strcat(_result,_one_line); // copy one_line to one_line_result
    memset(_one_line,'\0',MAX_LINE_LENGTH); // clear current line 
}
/** center the current line
    the next word would be the next newline.
**/
void _process_center_command(FILE *fp,int *_line_count)
{
    if(_one_line[0]!='\0')
    {
        
        _one_line[strlen(_one_line)-1]='\0';//delete the last space
        strcat(_one_line,"\r");
        strcat(_one_line,"\n");
        for(int i=0;i<LEFT_SHIFT;++i){ // add shift spaces for old lines
                strcat(_result," ");
        }
        strcat(_result,_one_line); // copy one_line to one_line_result
        memset(_one_line,'\0',MAX_LINE_LENGTH);
        *_line_count=0;
      
    }
    
    char line_char;
    int  tmp_line_count=0;
    char _temp_line[MAX_LINE_LENGTH];
    memset(_temp_line,'\0',MAX_LINE_LENGTH);
    printf("*****************\n");
    while(line_char=fgetc(fp))
    {
        printf("current char is %c\n",line_char);
        if(line_char==13|| line_char==10){
            break;
       }
       _temp_line[tmp_line_count]=line_char;
       ++tmp_line_count;
    }
    if(tmp_line_count + LEFT_SHIFT>LENGTH_LIMIT){
        memset(_temp_line,'\0',MAX_LINE_LENGTH);
    }
    else
    {
        // to do .....center the line
        
        int left_indent = (LENGTH_LIMIT-tmp_line_count)/2;
        strcat(_temp_line,"\r");
        strcat(_temp_line,"\n");
        for(int i=0;i<left_indent;++i){ // add shift spaces for old lines
                strcat(_result," ");
        }
        strcat(_result,_temp_line);
    }
}
void _process_left_shift_command(FILE *fp,int * _line_count){
    _one_line[strlen(_one_line)-1]='\0';//delete the last space
    if(!CONSECUTIVE_MARGIN_CHANGE)
    {
        strcat(_one_line,"\r");
        strcat(_one_line,"\n");
        strcat(_one_line,"\r");
        strcat(_one_line,"\n");
        CONSECUTIVE_MARGIN_CHANGE=1;
    }
    strcat(_result,_one_line); // copy one_line to one_line_result
    memset(_one_line,'\0',MAX_LINE_LENGTH);
     *_line_count=0;
    char number_char;
    int number_count=0;
    int shift_number;
    while(number_char=fgetc(fp))
    {
       
       if(number_char==13|| number_char==10){
            break;
       }
       _number[number_count]=number_char;
       number_count++;
    }
    shift_number = atoi(_number);
    LEFT_SHIFT = shift_number;
    memset(_number,'\0',3); // initialize result array
}
void _process_line_width_command(FILE *fp,int * _line_count)
{
   _one_line[strlen(_one_line)-1]='\0';//delete the last space
   if(!CONSECUTIVE_MARGIN_CHANGE)
   {
        strcat(_one_line,"\r");
        strcat(_one_line,"\n");
        strcat(_one_line,"\r");
        strcat(_one_line,"\n");
        CONSECUTIVE_MARGIN_CHANGE=1;
   }
   for(int i=0;i<LEFT_SHIFT;++i){ // add shift spaces for old lines
        strcat(_result," ");
   }
   strcat(_result,_one_line); // copy one_line to one_line_result
   memset(_one_line,'\0',MAX_LINE_LENGTH);
   *_line_count=0;
   char number_char;
   int number_count=0;
   int width_number;
   while(number_char=fgetc(fp))
   {
       if(number_char==13|| number_char==10){
            break;
       }
       _number[number_count]=number_char;
       number_count++;
   }
   width_number = atoi(_number);
   LENGTH_LIMIT = width_number;
   memset(_number,'\0',3); // initialize result array

}
void _process_common_word(int *_line_count,int *_word_count,char *word)
{
    if(*_line_count + *_word_count <= LENGTH_LIMIT)
     {
         (*_line_count)++;  // one space
         *_line_count+=*_word_count; // add word length
         strcat(_one_line,word);//copy the word into one_line
         strcat(_one_line," "); //copy empty one empty space into_line
     }
     else
     {
	_one_line[strlen(_one_line)-1]='\0';//delete the last space
	strcat(_one_line,"\r");
        strcat(_one_line,"\n");
        for(int i=0;i<LEFT_SHIFT;++i){ // add shift spaces for old lines
                strcat(_result," ");
        }
        strcat(_result,_one_line); // copy one_line to one_line_result
        memset(_one_line,'\0',MAX_LINE_LENGTH);
        *_line_count=0;
        strcat(_one_line,word);
        strcat(_one_line," ");
        *_line_count+=*_word_count;
	(*_line_count)++;
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
    memset(_result,'\0',FILE_MAX_SIZE); // initialize result array
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
                   &_b_flag
                   );
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
     _one_line[strlen(_one_line)-1]='\0'; //delete the last space

    strcat(_one_line,"\r");
    strcat(_one_line,"\n");
    for(int i=0;i<LEFT_SHIFT;++i){ // add shift spaces for old lines
            strcat(_result," ");
    }
    strcat(_result,_one_line);
    FILE *writer = fopen(argv[2],"w");
    fprintf(writer,"%s",_result);
    
    fclose(fp);
    fclose(writer);
    return 0;
}
