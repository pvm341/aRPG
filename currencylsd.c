#include <stdio.h>
#include <stdlib.h>

#include "currencylsd.h"
// Thanks to the following websites to confirm my knowledge
// http://gwydir.demon.co.uk/jo/units/money.htm
// http://www.learnenglish.de/culture/britishmoney.html

struct smoney {
int value;
int pos;  // 0=after value 1s/1d 0 , 1=before value £1
char *abbrev;
char *real_name;
char *slang_terms[3];
};

typedef struct smoney tmoney, *pmoney, amoney[], **dmoney;

amoney money_array =  {
                        {     1, 0, "d",     "penny", {"","",""}},
                        {     2, 0,  "",  "tuppence", {"","",""}},
                        {     3, 0,  "", "thrupence", {"","",""}},
                        {     4, 0,  "",     "groat", {"","",""}},
                        {     6, 0,  "",  "sixpence", {"tanner","",""}},
                        {    12, 0, "s",  "shilling", {"bob","",""}},
                        {    24, 0,  "",    "florin", {"","",""}},
                        {    30, 0,  "", "halfcrown", {"halfdollar","",""}},
                        {    60, 0,  "",     "crown", {"Oxford","dollar",""}},
                        {   240, 1, "£",     "pound", {"quid","nicker",""}},
                        {   252, 0,  "",    "guinea", {"","",""}},
                        {  1200, 0,  "",     "fiver", {"lady","",""}},
                        {  2400, 0,  "",    "tenner", {"","",""}},
                        {  4800, 0,  "",          "", {"score","",""}},
                        {  6000, 0,  "",          "", {"pony","",""}},
                        { 24000, 0,  "",          "", {"ton","",""}},
                        {120000, 0,  "",          "", {"monkey","",""}},
                        {240000, 0,  "",          "", {"grand","",""}}
                      };

char *price_formats[] = {"£%*d %2ds %2dd", "%d/%d", "%d/-"};
int to_value(tprice price){
  return 240*price.l + 12*price.s+price.d;
}

tprice to_price(int value){
  tprice price;
  price.l = value / 240;
  value %=240;
  price.s = value /12;
  price.d = value % 12;
  return price;
}

int digits(int v){
  int d = 1;
  if (v>9) {
    d += digits(v/10);
  }
  return d;
}

char *price_disp(char *disp_str, int value){
  tprice price = to_price(value);
  int w = digits(price.l);
  sprintf(disp_str,"£%*d %2ds %2dd",w,price.l,price.s,price.d);
  return disp_str;
}
