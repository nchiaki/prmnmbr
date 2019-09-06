/*
**  通常モード：指定の値までの除数はその値までの順次とする
**  高速モード：指定の値までの除数はその値までの素数とする（指定地に達しない分は、以降。順次数を使用する）
*/
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>

#include "queuedef.h"
#include "funcdef.h"

void usage(char *iam)
{
  char  *mynm;

  mynm = basename(iam);
  printf("%s <upperLimit> [fast] [hex] [digit <displayDigit>]\n", mynm);
}
int
main(int ac, char *av[])
{
  unsigned long long mxvl;
  int   ix, fstmd, hxot, dgtnm;

  if (ac <= 1)
  {
    usage(av[0]);
    exit(0);
  }
  mxvl = 0;
  fstmd = 0;
  hxot = 0;
  dgtnm = 0;
  for (ix=1; ix<ac; ix++)
  {
    if (!strcmp(av[ix], "fast"))
    {
      fstmd = 1;
    }
    else if (!strcmp(av[ix], "hex"))
    {
      hxot = 1;
    }
    else if (!strcmp(av[ix], "digit"))
    {
      ix++;
      dgtnm = atoi(av[ix]);
    }
    else if (!strcmp(av[ix], "help"))
    {
      usage(av[0]);
      exit(0);
    }
    else
    {
      mxvl = atoll(av[ix]);
    }
  }
  prime_search(mxvl, fstmd, hxot, dgtnm);
}
