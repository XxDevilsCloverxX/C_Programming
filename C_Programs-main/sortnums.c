#include <stdio.h>
void sort(int *ptr);
int main(void) {
  
  //create an array of 10 values
  int my_arr[10];
  int n1,n2,n3,n4,n5,n6,n7,n8,n9,n10;
  printf("Enter 10 numbers:");
  scanf("%d%d%d%d%d%d%d%d%d%d", &n1,&n2,&n3,&n4,&n5,&n6,&n7,&n8,&n9,&n10);
  /// Assign pointer values to my array
  my_arr[0] = n1;
  my_arr[1] = n2;
  my_arr[2] = n3;
  my_arr[3] = n4;
  my_arr[4] = n5;
  my_arr[5] = n6;
  my_arr[6] = n7;
  my_arr[7] = n8;
  my_arr[8] = n9;
  my_arr[9] = n10;

  sort(my_arr);
}


void sort(int* ptr)
{
    int i, j, temp;
  
    // Sort the numbers using pointers
    for (i = 0; i < 10; i+=1) {
        for (j = i + 1; j < 10; j+=1) {
          if (*(ptr + j) > *(ptr + i)) {
            temp = *(ptr + i);
            *(ptr + i) = *(ptr + j);
            *(ptr + j) = temp;
            }
        }
    }
  
    // print the numbers
    for (i = 0; i < 10; i++){
        printf("%d ", *(ptr + i));
}
}