#include <stdio.h>

void leap_year(int year);

int main(void) {
  
  //write a program to compare 3 integers from user input.
  int n1,n2,n3;
  printf("Enter 3 integers\n");
  scanf("%d %d %d", &n1,&n2,&n3);
  
  if (n1>n2 && n1>n3)
  {
    printf("%d is the biggest number.\n", n1);
    if (n2 < n3)
    {
      printf("%d is the smallest.\n", n2);
    }
    else if (n3<n2)
    {
      printf("%d is the smallest.\n", n3);
    }
    else
    {
      printf("There are two even integers. %d and %d.\n", n2, n3);
    }
  }
  else if (n2>n1 && n2>n3)
  {
    printf("%d is the biggest number.\n", n2);
    if (n1 < n3)
    {
      printf("%d is the smallest.\n", n1);
    }
    else if (n3<n1)
    {
      printf("%d is the smallest.\n", n3);
    }
    else
    {
      printf("There are two even integers. %d and %d.\n", n1, n3);
    }
  }
  else if (n1==n2 && n1==n3)
    {
      printf("All 3 integers %d, %d, %d are the same.\n", n1,n2,n3);
    }
  else
  {
    printf("%d is the biggest number.\n", n3);
    if (n2 < n1)
    {
      printf("%d is the smallest.\n", n2);
    }
    else if (n1<n2)
    {
      printf("%d is the smallest.\n", n1);
    }
    else
    {
      printf("There are two even integers. %d and %d.\n", n1, n2);
    }
  }
  
  int current_year;
  printf("Enter the current year.\n");
  scanf("%d", &current_year);
  leap_year(current_year);
  return 0;
}

void leap_year(int year)
{
  if (year%4==0 && year%100!=0)
  {
    printf("%d is a leap year.\n", year);
  }
  else if(year%4==0 && year%100==0 &&year%400==0)
  {
    printf("%d is a century leap year.\n", year);
  }
  else
  {
    printf("%d is not a leap year.\n", year);
  }
}
