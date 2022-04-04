/*  
Counting occurrences of a letter in a string.  
*/  
#include <stdio.h>  
int count(char ch, const char *str);  
int main(void)  
{  
  char str[80]; /* string to be processed */    
  char target; /* character counted */    
  int my_count;    
  printf("Enter up to 79 characters.\n");    
  gets(str); /* read in the string */    
  printf("Enter the character you want to count: ");    
  scanf("%c", &target);    
  my_count = count(target, str);  
  printf("The number of occurrences of %c in\n\"%s\"\nis %d\n",    
  target, str, my_count);  
  return (0);      
}   
 /*  
 * Counts the number of times ch occurs in string str.  
 * Pre: Letter ch and string str are defined.  
 */  
int count(char ch, const char *str)  
{  
  int ans;  
  if (str[0] == '\0')
  { /* simple case */  
  ans = 0;
  }
  else
  { /* redefine problem using recursion */  
    if (ch == str[0])
    {/* first character must be counted */    
    ans = 1 + count(ch, &str[1]);
    printf("Current num of occurences: %d\n", ans);
    }
    else
    {
    /* first character is not counted */    
    ans = count(ch, &str[1]);
    printf("Current num of occurences: %d\n", ans);
    }
  }
  return ans;
} 