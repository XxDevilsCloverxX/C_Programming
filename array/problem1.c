#include <stdio.h>

void fun1(double *smp, double *lgp); //smp is smaller pointer and lgp is larger pointer

/* NAME: main
PARAMETERS: void, no parameter
PURPOSE: initialize variables, prompt user for input, call function fun1, and return text to the console for user.
PRECONDITION: void parameter suggests there is no precondition to run this program, the function is called on all program runs.
POSTCONDITION: After the main function is called on program build and run, the inputs taken from user will be sorted in ascending order under the same variable names then printed.
*/
int main (void) {
  double num1, num2, num3;
  printf("Enter 3 numbers separated by blanks >");
  scanf("%lf%lf%lf",&num1,&num2,&num3);
  fun1(&num1,&num2);
  fun1(&num1,&num3);
  fun1(&num2,&num3);
  printf("The numbers are: %.2f %.2f %.2f\n", num1, num2, num3);
  return (0);
}

/* NAME: fun1
PARAMETERS: *smp, *lgp, double type pointers
PURPOSE: The function compares two values of pointers and swaps them if value of smp is greater than lgp
PRECONDITION: The parameters and arguments must be pointer type, or the values would not update. Function is called when you want to compare two values.
POSTCONDITION: After function call, value of lgp will be greater than smp.
*/
void fun1 (double *smp, double *lgp) {
  double temp;
  if (*smp > *lgp) {
    temp=*smp;
    *smp=*lgp;
    *lgp=temp;
  }
}

/* 1.
b. The program takes three user input numbers as double type. The program then compares
the inputs and sorts them in ascending order (Least to Greatest).
Input: 8.0 2.1 5.1
Output: The numbers are: 2.10 5.10 8.00
c. When changing the fun1 parameter list and removing the &
from the arguments of fun1 calls, as well as the pointer refrences from the fun1 body per the email, the code returns the inputs unsorted but in the order
the user entered them as. This is because without the pointer type, the variables are not updated because the function does not return anything, including
the sorting changes.
*/
