#include <stdio.h>
#include <string.h>

int recursive_plus2(int n);
int palindrome(char pal[80]);

int main(void) {
  //Palindrome Code
  char my_str[100];
  int is_pal, len, start=0;
  printf("Enter a string with no spaces or punctuation to check if it is a palindrome:");
  gets(my_str);
  len = strlen(my_str);
  is_pal = palindrome(my_str); 
  // Returns : 0 -> False / 1 -> True
  printf("%d\n", is_pal);
  
  //Recursive add for *2
  int x, calls;
  printf("Enter the number of recursive calls to return a value for:");
  scanf("%d", &calls);
  x = recursive_plus2(calls);
  printf("%d", x);
  return 0;
}

int recursive_plus2(int n)
{
  char str_to_print[100];
  int sum;
  if (n <= 0)
  {
    printf("recursive_plus2(%d) = ", n);
    return 0;
  }
  else 
  {
    printf("recursive_plus2(%d) \n+", n);
    sum = recursive_plus2(n-1) + 2;
  }
  return sum;
}

int palindrome(char pal[80])
{
    char check[80];
    if(strlen(pal) == 1)
    {
        return 1;
    }
    if(strlen(pal) == 2 && pal[0] == pal[1])
    {
        return 1;
    }
    if(pal[0] != pal[strlen(pal)-1])
    {
        printf("%c is not the same as %c", pal[0],pal[strlen(pal)-1] );
        return 0;
    }
    strncpy(check, &pal[1], strlen(pal)-2);
    check[strlen(pal)-2]='\0';

    return palindrome(check);
}