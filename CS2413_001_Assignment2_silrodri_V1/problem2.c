#include <stdio.h>
/*
AUTHOR: Silas Rodriguez
FILENAME: problem2.c
SPECIFICATION: Practice using C arrays and Strings
FOR: CS 2413 Data Structures Section 001
*/
#define SIZE 100 //allow for strings up to 100 characters.
int optimized_search(char string[SIZE], char substring[SIZE]);
/* NAME: main
PARAMETERS: void, none
PURPOSE: The main function initializes the test string arrays and calls the optimized_search function
PRECONDITION: No parameters, function is called when program runs.
POSTCONDITION: The function assigns the base_position variable to the output of the search function, then prints the base index where the
search string can be found in the original string.
*/
int main(void){
  char origin_string[SIZE]= "cut cutting cuts carts and cats\0", search_phrase[SIZE] = "cutt\0"; // arrays for original string and phrase to search
  int base_position; //first index position of found substring, -1 if false
  printf("Origin string is: %s\n", origin_string);
  printf("Searching for: %s\n", search_phrase);
  base_position = optimized_search(origin_string, search_phrase);
  if (base_position == -1){
    printf("No match found :(\n");
  }
  else{
    printf("Match Found at index %d!\n", base_position);
  }
  return 0;
}

/* NAME: optimized_search
PARAMETERS: two character arrays (strings)
PURPOSE: Locating the base index where the substring occurs in the original string, if existent.
PRECONDITION: Both arguments must be strings (char arrays)
POSTCONDITION: The function returns -1 if the substring could not be found or the base index of the
origin string where the substring had its first match.
*/
int optimized_search(char string[SIZE], char substring[SIZE]){
  int i, j=0, init=0, flag=0;
  //index variables, init stores base index while flag allows writing to init memory cell.
  if(substring[j] == '\0'){
    printf("Fatal Error: Search is empty.\n");
    return -1;
  }
  for(i = 0; i<SIZE; i++){
    if(string[i] == substring[j]){ //if match true
      flag++;
      init = (flag == 1) ? i:init;
      j++;
      if(substring[j] == '\0'){
        return init;
      }
    }
    else{ //if match fails
      j = 0; //substring returns to first case
      flag = 0; //flag allows new init points
      if(string[i] == '\0'){
        return -1;
      }
      else{
      if(string[i] == string[init]){ // check if fail point was equal to init point, otherwise, i increments normally.
        init++; //Prevents infinite loops from occuring on 1st letter differences. No other purpose, is reset later.
        i--; // decrement i before increment
        continue;
      }
      else{
        continue; // increment normally
        }
      }
    }
  }
  return -1; // only returns if loop cannot find matches.
}

/*
i.

The program, running as is, returns:
Origin string is: cut cutting cuts carts and cats
Searching for: cutt
Match Found at index 4!

If the search is empty, the program errors and returns no match.
If the search does not exist, such as "cutter", the program returns no match found.

ii.

Part 1) The worst case for the brute force method discussed in the assignment would be that
for a substring of size m at the end of a size n string, would be that the substring is the same length
of of the primary string, meaning Big-O time complexity would be O(n^2). (Two for loops would itterate for i and j of the Strings)

Part 2) The best case string of size n and substring of size m would be a case where m is empty or a single character
that is equivalent to the first character of the original array, or where the substring exists at the beginning. The big-O time complexity for this case would be O(n) because
the program would only have to run n times to verify the substring. Or f(n) = 1 for the character instance, where f(n) <= O(n)
*/
