#ifndef CURRENCYLSD_H_INCLUDED
#define CURRENCYLSD_H_INCLUDED

struct sprice
{
  int d,s,l;
};

typedef struct sprice tprice, *pprice, **dprice;

int to_value(tprice price);
tprice to_price(int value);
char *price_disp(char *disp_str, int value);

#endif // CURRENCYLSD_H_INCLUDED
