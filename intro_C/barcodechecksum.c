#include <stdio.h>
int sum_odds(int ar1[]);
int sum_evens(int ar2[]);

int main(void) {
  int barcode[12];
  printf("Enter 12 digit barcode with spaces between digits.");
  int i;
  for (i=0; i<12; i+=1)
  {
    scanf("%d", &barcode[i]);
  }
  
  int sum_odd = sum_odds(barcode);
  int sum_even = sum_evens(barcode);
  int result = sum_odd + sum_even;

  int last = result %10;
  int check_digit;
  if (last == 0){
    check_digit = 0;
  }
  else{
    check_digit = -1*(last - 10);
  }
  
  if (check_digit == barcode[11]
  ){
    printf("Barcode is valid checksum return %d = %d", check_digit, barcode[11]);
  }
  else{
    printf("Barcode is invalid, Checksum returned %d != %d", check_digit, barcode[11]);
  }

}

int sum_odds(int ar1[]){
  int j, odds_sum=0;
  for (j=0; j<11; j+=2){
    odds_sum += ar1[j];
  }
  odds_sum = odds_sum *3;
  return odds_sum;
}

int sum_evens(int ar2[]){
  int k, even_sum=0;
  for (k=1; k<11; k+=2){

    even_sum += ar2[k];
  }
  return even_sum;
}