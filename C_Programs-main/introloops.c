#include <stdio.h>
int odd_sum(int length);
int even_sum(int length);

int main(void) {
  int length1, length2;
  length1 = 10;
  length2 = 10;
  //odd sum of first 10 odds
  printf("%d is odd sum of first 10 odds.\n", odd_sum(length1));

  //odd sum of first 10 evens
  printf("%d is odd sum of first 10 evens.\n", even_sum(length2));
  return 0;
}

int odd_sum(int length){
  int counter = 0;
  int sum = 0;
  int current_number = 0;

  while (counter <length)
  {
    if (current_number %2 == 1)
    {
      sum += current_number;
      counter +=1;
    }
    current_number +=1;
  }
  return sum;
}
int even_sum(int length)
{
  int counter = 0;
  int sum = 0;
  int current_number = 0;
  //must add 1 to length because 0 is counted in the counter
  while (counter<length+1)
  {
    if (current_number %2 == 0)
    {
      sum += current_number;
      counter +=1;
    }
    current_number +=1;
  }
  return sum;
}