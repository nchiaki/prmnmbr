#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "glbldef.h"
#include "prmnmbrdef.h"

void
prime_search(unsigned long long mxvl, int fstmd, int hxot, int dgtnm)
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
  {/*指定上限値までの値について素数を求めていく*/
    prmvl = 0;

    /*除数の開始値（準備）*/
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
    {/*除数は素数判定値までの範囲*/
      if (!(bsvl % dvsr))
      {/*割切れた*/
        if (!prmvl)
          prmvl = bsvl; /*最後に割切れたとしたらこれが素数*/
        else
        {
          prmvl = 0;  /*すでに割切れている場合は素数ではないので次へ*/
          break;
        }
      }
      if (fstmd)
      {/*高速モード*/
        /*次の除数はすでに素数と確定した値から適応する*/
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
        dvsr++; /*除数は順次求めていく*/
    }
    if (prmvl)
    {/*素数が決定していたらそれを出力*/
      prmvls++;
      sprintf(prmnmbbf, prmotfmt, prmvl);
      prmdgt = strlen(prmnmbbf);
      if (dgtnm)
      {
        if (dgtnm == (prmdgt-1))
        {
          printf("%s", prmnmbbf);
        }
      }
      else
      {
        if (preprmdgt != prmdgt)
        {
          printf("\n");
          preprmdgt = prmdgt;
        }
        printf("%s", prmnmbbf);
      }
      lstprmvl = prmvl;

      if (fstmd)
      {/*高速モード時は確定した素数を次の除数として使用できるように保存する*/
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
