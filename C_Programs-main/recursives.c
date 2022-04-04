#include <stdio.h>
#include <string.h>

int multiply(int a, int b);
int divide(int c, int d);

int main(void) {
  int user_num, n, product=0;
  //Scan user inputs
  printf("Enter an integer");
  scanf("%d", &user_num);
  printf("Multiply by what?");
  scanf("%d", &n);
  //Call the recursive function
  product = multiply(user_num, n);
  printf("%d is the product of the recursive function.\n", product);
  
  //scan dividing inputs
  int c, d, quotient;
  printf("Enter 2 numbers, larger first.");
  scanf("%d%d", &c, &d);
  //call quotient function
  quotient = divide(c, d);
  printf("%d is the quotient of the recursive function\n", quotient);

  // try to reverse a list of characters
  char arr[100];
  printf("Enter a string to reverse");
  gets(arr);
  printf("%s is the string. \n", arr);

  return 0;
}

int multiply(int a,int b)
{
  static int product=0,i=0;
  
  if(i < a)
  {  
    product += b;
    i+=1;
    multiply(a,b);
  }

    return product;}

int divide( int c, int d)
{
  static int quotient =0, i=0;
  if (i<c)
  {
    quotient += 1;
    i+=d;
    divide(c, d);
  }
  return quotient;
}
