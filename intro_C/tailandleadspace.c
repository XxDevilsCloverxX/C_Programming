#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *trim_blanks(char *trimmed, //output
const char *to_trim /*input*/);

int main(void) {
  char my_string[100], trimmed_string[100];
  printf("Enter a string with spaces before or after to be corrected:\n");
  gets(my_string);
  trim_blanks(trimmed_string, my_string);
  printf("%s", trimmed_string);
  return 0;
}

char *trim_blanks(char *trimmed,
const char *to_trim)
{
  int i=0, first_index, last_index, len, append;
  //len -1 to get index position of the string
  len = strlen(to_trim) - 1;
  //Find index of first non-space
  while(isspace(to_trim[i]))
  {
    i+=1;
  }
  first_index = i;
  //Find last index before non-spaces
  while(isspace(to_trim[len-1]))
  {
    len-=1;
  }
  last_index = len;
  append = last_index - first_index;
  //Strncpy to copy string to trimmed
  strncpy(trimmed, &to_trim[first_index], append);
  trimmed[append] = '\0';
  return trimmed;
}