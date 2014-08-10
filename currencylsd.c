#include <stdio.h>
#include <stdlib.h>

#include "currencylsd.h"

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
