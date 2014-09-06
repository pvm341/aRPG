#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "ansiutils.h"
#define ESC 27

extern FILE *nul;
const char *colours[] = {"BLACK","RED","GREEN","YELLOW","BLUE","MAGENTA","CYAN",
                        "WHITE","unknown","RESET","<end>"};

char *replace_strchr(char *str, char schr, char rchr){
  char *ptr = strchr(str,schr);
  if (ptr){
    *ptr=rchr;
  }
  return str;
}

static char *find_colour(char *ch_ptr, int *the_colour){
  char colour[10];
  int i;
  i=0;
  while( *ch_ptr && ',' != *ch_ptr && ']' != *ch_ptr){
    colour[i++] = *ch_ptr++;
  }
  colour[i]=0;
  i=-1;
  do{
    i++;
  }
  while(strcmp(colour,colours[i]) && i<END_COLOURS);
  *the_colour = (8!=i)?i:++i;
  return ch_ptr;
}

static char *get_cursor_coords(char *ip, char **row, char **col){
  int comma = 0;

  while (*ip && ']' != *ip){
    if (',' == *ip){
      comma = 1;
      *ip = '\0';
    } else if (!comma && *row==NULL && '0'<=*ip && '9' >= *ip){
      *row = ip;
    } else if (comma && *col==NULL && '0'<=*ip && '9' >= *ip){
      *col = ip;
    }
    ip++;
  }
  if (']' == *ip){
    *ip=0;
  }
  return ++ip;
}


int ansi_fprintf(FILE *file, char *fmt, ...){
  va_list args;
  int i, w;
  char *inp_str, *out_str, *ip, *op, *tip, *top;
  char *row = NULL, *col = NULL;

  va_start(args, fmt);
  w = vfprintf(nul, fmt, args);
  va_end(args);
  inp_str = (char *)calloc(w+1,sizeof(char));
  out_str = (char *)calloc(w+256,sizeof(char));
  va_start(args, fmt);
  vsprintf(inp_str,fmt,args);
  va_end(args);
  ip=inp_str;
  op=out_str;
  while(*ip){
#ifdef PERCENT_K
    // this work in progress and not yet ready
    // %k[,%k] to change the colours as in normal paramerters
    // needs some work.
    if ('%' == *ip && 'k' == *(ip+1)){
      i = va_arg(args,int);
      if ((i>=0 && i <8) || 9 == i){
        *op++ = ESC;
        *op++ = '[';
        *op++ = '3';
        *op++ = '0'+i;
      }
      ip+= 2;
      if (',' == *ip && '%' == *(ip+1) && 'k' == *(ip+2)) {
        i = va_arg(args,int);
        if ((i>=0 && i <8) || 9 == i){
          *op++ = ';';
          *op++ = '4';
          *op++ = '0'+i;
        }
        ip += 3;
      }
      *op++ = 'm';

    } else
#endif
    if ('[' == *ip){
      // save current pointers ip to tip and op to top
      // incase there is contents [ contents ] not recognised
      tip = ip;
      top = op;
      // now set at this point in the output string the start of the
      // ANSI codes
      *op++ = ESC;
      *op++ = '[';
      ip++;
      if ('@' == *ip){
        // position cursor [@row,col] base is 1 for both row and col @1,1 is
        // top left of the terminal window
        // [@5,10] will position cursor to row 5 col 10
        ip++;
        ip=get_cursor_coords(ip,&row,&col);
        if (NULL == row){
          *op++ = '1';
        } else {
          while (*row){
            *op++ = *row++;
          }
        }
        *op++ = ';';
        if (NULL == col) {
          *op++ = '1';
        } else {
          while (*col) {
            *op++ = *col++;
          }
        }
        *op++ = 'H';
      } else if ('#' == *ip){
        // clear the screen
        *op++='2';
        *op++='J';
        // position cursor top left
        *op++=27;
        *op++='[';
        *op++='1';
        *op++=';';
        *op++='1';
        *op++='H';
        ip++;
      } else {
        // colour change [ink colour(,paper colour)]
        // First ink colour
        *op++ = '3';
        // find colour and return pointer to next character in ip
        // return the value of the colour in i
        ip = find_colour(ip,&i);
        // if i is valid then set it up
        if (i<END_COLOURS){
          *op++ = '0'+i;
          // if next character is ] skip it and continue as colour change is
          // just ink colour
          if (']'== *ip){
            ip++;
          }
        } else {
          // invalid colour specified reset to saved value
          ip=tip;
          // clear the preset control characters to where they were before the
          // failed colour select
          op=top;
        }
        if (*ip && ',' == *ip){
          // there is something after the ink colour so add spearator
          *op++ = ';';
          // add paper code
          *op++ = '4';
          // find colour for paper
          ip=find_colour(++ip,&i);
          if (i<END_COLOURS){
            // if valid set it up
            *op++ = '0'+i;
            // skip the next character
            ip++;
          } else {
            // invalid colour reset to the [ and return op to that point too
            ip=tip;
            op=top;
          }
        }
        // add the find part of the colour change
        *op++ = 'm';
      }
    }
    // copy the characters that are not special
    *op++ = *ip++;
  }
  *op = *ip;
  // make sure the output string is terminated
  w=fprintf(file,"%s",out_str);
  free(inp_str);
  inp_str = NULL;
  free(out_str);
  out_str = NULL;
  return w;
}
