#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {

  char MMOC[100], Warehouse[100], ID[100], Qualifiers[100];
  int i=0, counter=0;
  printf("Enter a MMOC Product Code\n");
  gets(MMOC);

  // While loop to count first index positions of Warehouse String & cpy
  while(isalpha(MMOC[i]))
  {
    i+=1;
  }
  strncpy(Warehouse, MMOC, i);
  Warehouse[i] = '\0';

  //Initialize the initial index to be i.
  int hold = i;
  while(isdigit(MMOC[i]))
  {
    counter +=1;
    i+=1;
  }
  strncpy(ID, &MMOC[hold], counter);
  ID[counter] = '\0';

  //Now with the last index of the ID, the rest is Qualifications & we can copy strings:
  strcpy(Qualifiers, &MMOC[i]);
  int len = strlen(Qualifiers);
  Qualifiers[len] = '\0';

  printf("Warehouse : %s\n", Warehouse);
  printf("Product ID : %s\n", ID);
  printf("Qualifiers : %s\n", Qualifiers);
  return 0;
}