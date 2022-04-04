#include <stdio.h>
int main(void) {

  int n;
  
  printf("Please enter a num.\n");
  scanf("%d", &n);
  int printable_n = n;
  int extracted_digit, check_sum;
  do 
  {
    //%10 returns remainder of rightmost
    extracted_digit = n%10;
    // divide by 10 for reducing the num.
    n = n/10;
    printf("%d, ", extracted_digit);
    check_sum += extracted_digit;

  }while(n>0);
  printf("added is %d", check_sum);
  if (check_sum%9 ==0)
  {
    printf("\n%d is divisible by 9.", printable_n);
  }
  else
  {
    printf("\n%d is not divisible by 9.", printable_n);
  }

  int j, num, count, counter, sum; 
  
  printf("\nSum of first 10 prime numbers is: \n"); 
  for(num = 1; counter < 10; num+=1)
  {
    count = 0;
    for (j = 2; j <= num/2; j++)
    {
  	if(num%j == 0)
  	{
     	  count++;
  	  break;
    }
    }
    if(count == 0 && num != 1 )
    {
	  sum+=num;
    counter+=1;
    printf("%d, ", num);
    }  
  }
  printf("added is %d.", sum);
  return 0;
}