/*
**  通常モード：指定の値までの除数はその値までの順次とする
**  高速モード：指定の値までの除数はその値までの素数とする（指定地に達しない分は、以降。順次数を使用する）
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "queuedef.h"

QUE prime_root = {&prime_root, &prime_root};

typedef struct prime_value_block {
  QUE _queue;
  unsigned long long  _prmvl;
} PRMVLBCK;

void
prime_search(unsigned long long mxvl, int fstmd, int hxot)
{
  unsigned long long  bsvl;
  unsigned long long  dvsr;
  unsigned long long  prmvl, lstprmvl, prmvls;
  PRMVLBCK  *pmvlbp;
  int qisempty, preprmdgt,prmdgt;
  char  prmnmbbf[64];
  char  *prmdcfmt = "%llu ", *enddcfmt = "\nPrime No. total %llu last %llu\n";
  char  *prmhxfmt = "0%llx ", *endhxfmt = "\nPrime No. total %llu last 0%llx\n";
  char  *prmotfmt, *endotfmt;

  if (hxot)
  {
    prmotfmt = prmhxfmt;
    endotfmt = endhxfmt;
  }
  else
  {
    prmotfmt = prmdcfmt;
    endotfmt = enddcfmt;
  }

  preprmdgt = 0;
  qisempty = 0;
  prmvls = 0;
  lstprmvl = 0;
  for (bsvl=2; bsvl<=mxvl; bsvl++)
  {
    prmvl = 0;

    if (fstmd)
    {/*高速モード*/
      pmvlbp = (PRMVLBCK *)NEXT(&prime_root);
      if (pmvlbp == (PRMVLBCK *)&prime_root)
      {/*解析済み素数未登録状態*/
        dvsr = 2;
        qisempty = 1;
      }
      else
        dvsr = pmvlbp->_prmvl;
    }
    else
      dvsr = 2;

    while (dvsr <= bsvl)
    {
      if (!(bsvl % dvsr))
      {
        if (!prmvl)
          prmvl = bsvl;
        else
        {
          prmvl = 0;
          break;
        }
      }
      if (fstmd)
      {
        if (qisempty)
          dvsr++;
        else
        {
          pmvlbp = (PRMVLBCK *)NEXT(pmvlbp);
          if (pmvlbp == (PRMVLBCK *)&prime_root)
          {
            dvsr++;
            qisempty = 1;
          }
          else
            dvsr = pmvlbp->_prmvl;
        }
      }
      else
        dvsr++;
    }
    if (prmvl)
    {
      prmvls++;
      sprintf(prmnmbbf, prmotfmt, prmvl);
      prmdgt = strlen(prmnmbbf);
      if (preprmdgt != prmdgt)
      {
        printf("\n");
        preprmdgt = prmdgt;
      }
      printf("%s", prmnmbbf);

      lstprmvl = prmvl;

      if (fstmd)
      {
        pmvlbp = (PRMVLBCK *)malloc(sizeof(PRMVLBCK));
        if (pmvlbp)
        {
          pmvlbp->_prmvl = prmvl;
          ENQUEUE(&prime_root, pmvlbp);
          qisempty = 0;
        }
        else
        {
          fprintf(stderr, "%s", strerror(errno));
        }
      }
    }
  }
  printf(endotfmt, prmvls, lstprmvl);
}

int
main(int ac, char *av[])
{
  unsigned long long mxvl;
  int   ix, fstmd, hxot;

  if (1 < ac)
    mxvl = atoll(av[1]);
  else
    mxvl = 0;

  fstmd = 0;
  hxot = 0;
  for (ix=2; ix<ac; ix++)
  {
    if (!strcmp(av[ix], "fast"))
    {
      fstmd = 1;
    }
    if (!strcmp(av[ix], "hex"))
    {
      hxot = 1;
    }
  }
  prime_search(mxvl, fstmd, hxot);
}
