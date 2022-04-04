#include <stdio.h>
void calculate_factorial(int num, int *factorial);
int main(void) {
  int *product, n;
  printf("Enter an integer to compute the factorial for:");
  scanf("%d", &n);
  calculate_factorial(n, product);
  return 0;
}

void calculate_factorial(int num, int *factorial){
  int i;
  for (i=1; i<=num; i+=1){
    *factorial *=i;
  }
  printf("The factorial of %d is %d.", num, *factorial);
}