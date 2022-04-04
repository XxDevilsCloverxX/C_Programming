#include <stdio.h>
void swap(int *num1, int *num2);
void ordering(int *snum, int *mnum, int *lnum);
int main(void) {
  int n1, n2;
  printf("Enter two numbers:");
  scanf("%d%d", &n1, &n2);
  printf("\n %d and %d swap to become: ", n1, n2);
  swap(&n1,&n2);
  printf("%d and %d.\n", n1, n2);
  
  int bignum, midnum, smallnum;
  printf("Enter 3 numbers.");
  scanf("%d%d%d", &bignum, &midnum, &smallnum);
  ordering(&smallnum, &midnum, &bignum);
  printf("%d is the biggest of the 3, %d is the smallest, while %d is the remainder.", bignum, smallnum, midnum);
  return 0;
}

void swap(int *num1, int *num2){
  int t = *num1;
  *num1 = *num2;
  *num2 = t;
}

void ordering(int *snum, int *mnum, int *lnum)
{
  //finding largest num
  int swap_varl;
  if (*snum>*lnum)
  {
    swap_varl = *lnum;
    *lnum = *snum;
    *snum = swap_varl;
  }
  if(*mnum >*lnum)
  {
    swap_varl = *lnum;
    *lnum = *mnum;
    *mnum = swap_varl;
  }
  //finding smallest num
  int swap_vars;
  if (*snum>*lnum)
  {
    swap_varl = *snum;
    *snum = *lnum;
    *lnum = swap_varl;
  }
  if(*snum >*mnum)
  {
    swap_varl = *snum;
    *snum = *mnum;
    *mnum = swap_varl;
  }
}