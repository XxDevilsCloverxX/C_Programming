#include <stdio.h>
int get_min_range(int data[], int firpos, int secpos);
int main(void) {
  int arr[] ={1,2,3,4,5,6};
  int index_of_low;
  index_of_low=get_min_range(arr,0,5);
  printf("%d is index of lowest pos.", index_of_low);
  return 0;
}

int get_min_range(int data[], int firpos, int secpos){
  //finding index of smallest num
  int i, k;
  k = data[firpos];
  int index_pos;
  for(i=firpos; i<=secpos; i+=1){
    if (k>=data[i]){
      k = data[i];
      index_pos = i;
    }
  }
  return index_pos;
}