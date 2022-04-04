#include <stdio.h>
#include <string.h>
#include <ctype.h>

int count_alpha(char arr[],int n);
int count_digit(char arr[], int n);
int count_special(char arr[], int n);
void return_least(char arr[], int n);

int main(void) {
  char arr[100];
  printf("Enter a string to sort:");
  gets(arr);
  int alpha, numeric, special;
  int size;
  size = strlen(arr);

  numeric = count_digit(arr,size);
  special = count_special(arr, size);
  alpha = count_alpha(arr,size);
  printf("%s = input.\n", arr);
  printf("%d digits\n", numeric);
  printf("%d alphabet\n", alpha);
  printf("%d special\n", special);
 

  //Write a program to count and return least occuring character.
  char least_string[100];
  printf("Enter a string: ");
  gets(least_string);
  int this_len = strlen(least_string);
  return_least(least_string, this_len);
}

int count_digit(char arr[], int n){
  char num[] = {'1','2','3','4','5','6','7','8','9','0'};
  int counter = 0, i, j;
  for (i=0; i<n; i+=1){
    for(j=0; j<10; j+=1){
      if (arr[i]==num[j]){
        counter +=1;
      }
    }
  }
  return counter; 
}

int count_alpha(char arr[], int n){
  int i, counter=0;
  for (i=0; i<n; i+=1)
  {
    if (arr[i]>='A' && arr[i]<='z'){
      counter+=1;
    }
  }
  return counter; 
}

int count_special(char arr[], int n){
  int i, counter=0;
  
  for (i=0; i<n; i+=1)
  {
    int null =0;
    if (isdigit(arr[i])){
      null+=1;
    }
    else if (isalpha(arr[i])){
      null +=1;
    }
    else{
      counter+=1;
    }
  }

  return counter; 
}

void return_least(char arr[], int n){
  int i, j, counter;
  int freq[100];
  char string[100];
  for (i=0; i<n; i+=1)
  {
    counter =0;
    for (j=0; j<n; j+=1)
    {
      if (arr[i] == arr[j])
      {
        counter +=1;
      }
    }
    freq[i] = counter;
  }
  int smallest;
  smallest = freq[0];
   int loop;
   for(loop = 1; loop < n; loop++) {
      if( smallest > freq[loop] ) 
         smallest = freq[loop];
   }
   int string_index = 0;
  for (i=0; i<n; i+=1)
  {
    if (freq[i]==smallest){
      string[string_index] = arr[i];
      string_index +=1;
    }
  }
  printf("\n%s are the least frequent with frequency %d", string, smallest);
}