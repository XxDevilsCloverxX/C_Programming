#include <stdio.h>  //for inputs and outputs
#include <time.h> //for timing the function calls
#include <stdlib.h> //for calloc

#define SIZE 10000 //dynamically allocated array size
#define SEED 0  //Seed used for random number generation
#define DEBUG 0 //Debug statements

void selection_sort(void *arr, int size, void (*swap) (void *,int,int), int (*compare) (void *,int,int));  //function args
void quick_sort(void*arr, int begin, int end, void (*swap) (void *,int,int), int (*compare) (void *,int,int));  //function args
int linear_search(void* arr, int lower_bound, int upper_bound, void* key);
int binary_search(void* arr, int lower_bound, int upper_bound, void * key);
void swap (void *arr, int i, int j);  //swap two positions in an array
int compare (void *arr, int i, int j);  //compare two positions in an array and return an index
int index_of_min (void *arr, int low, int high, int (*compare) (void *,int,int)); //get the index of min val between two points in memory
void reveal_array(void* arr, int size); //prints the array
int partition(void* arr, int beg, int end, void (*swap) (void *,int,int), int (*compare) (void *,int,int)); //for quick sort

/* NAME: main
PARAMETERS: void, none
PURPOSE: The main function will dynamically allocate random number arrays and sort them and search through them using different sorting algorithms
PRECONDITION: No parameters, function is called when program runs.
POSTCONDITION: searching and sort algorithms will operate on the rand arrays and will be timed.
*/
int main(void){
  //dynamically allocate an array using calloc for correct number of blocks
  int* nums = (int*)calloc(SIZE, sizeof(int*));
  int* nums_reset = (int*)calloc(SIZE, sizeof(int*)); //used so that the two sorting algorithms may be used on same unsorted
  //clock variables for quick sort
  clock_t start, stop;
  double cpu_time_used;

  if(nums == NULL || nums_reset == NULL){
    printf("Array failed to allocate properly\n.");
    exit(1);
  }
  int i, pos;
  time_t t = SEED;
  //Randomize the values in arr:
    //initialize random number generator
  srand((unsigned)time(&t));
  //assign random numbers
  for(i=0;i<SIZE;i++){
    nums[i] = rand()%SIZE;  //10k is the max number in the range
    nums_reset[i]= rand()%SIZE;
    //uncomment this block to use values linearly, ascending is i++ descending using i--
    /*
    nums[i] = i;
    nums_reset[i] = i;
    */
  }
  printf("Array of Random Numbers of size %d\n", SIZE);
  if(DEBUG){
    reveal_array(nums, SIZE);
  }
  //Now there is a random integer type array with data run a selection sort
  start = clock();
  selection_sort((void*)nums, SIZE, swap, compare);
  stop = clock();
  cpu_time_used = ((double)(stop - start))/ CLOCKS_PER_SEC;  //divide by this macro of the time.h to yield seconds
  printf("main - sort took %lf seconds\n", cpu_time_used);
  if(DEBUG){
    reveal_array(nums, SIZE);
  }

  if(DEBUG){
    reveal_array(nums_reset, SIZE);
  }
  //perform the quick sort
  printf("\nQuick Sort...\n");
  start = clock();
  quick_sort((void*)nums_reset, 0, SIZE, swap, compare);
  stop = clock();
  if(DEBUG){
    reveal_array(nums_reset, SIZE);
  }
  cpu_time_used = ((double)(stop - start))/ CLOCKS_PER_SEC;  //divide by this macro of the time.h to yield seconds
  printf("main - sort took %f seconds\n", cpu_time_used);

  //Perform a search on sorted array (use the last element as the key)
  int value = nums[SIZE-1];
  printf("\nLinear Search Sorted array: b[%d] = %d\n", SIZE-1, nums[SIZE-1]);
  start = clock();
  pos = linear_search((void*) nums, 0, SIZE, (void*)&value);
  stop = clock();
  cpu_time_used = ((double)(stop - start))/ CLOCKS_PER_SEC;  //divide by this macro of the time.h to yield seconds
  printf("main - sort took %f seconds\n", cpu_time_used);
  printf("index = %d, b[%d] = %d\n", pos, pos, nums[pos]);
  printf("\nLinear Search Sorted array: b[%d]+1 = %d\n", SIZE-1, nums[SIZE-1]+1);
  value +=1;

  //perform a linear seach on the sorted array for a value not in the array
  start = clock();
  pos = linear_search((void*) nums, 0, SIZE, (void*)&value);
  stop = clock();
  cpu_time_used = ((double)(stop - start))/ CLOCKS_PER_SEC;  //divide by this macro of the time.h to yield seconds
  printf("main - sort took %f seconds\n", cpu_time_used);
  printf("index = %d\n", pos);

  //perform a binary search on the same sorted array
  value-=1; //bring value back down
  printf("\nBinary Search Sorted array: b[%d] = %d\n", SIZE-1, nums[SIZE-1]);
  start = clock();
  pos = binary_search((void*) nums, 0, SIZE, (void*)&value);
  stop = clock();
  cpu_time_used = ((double)(stop - start))/ CLOCKS_PER_SEC;  //divide by this macro of the time.h to yield seconds
  printf("main - sort took %f seconds\n", cpu_time_used);
  printf("index = %d, b[%d] = %d\n", pos, pos, nums[pos]);

  //perform a binary search on a key not in the array
  value+=1;
  printf("\nBinary Search Sorted array: b[%d]+1 = %d\n", SIZE-1, nums[SIZE-1]+1);
  start = clock();
  pos = binary_search((void*) nums, 0, SIZE, (void*)&value);
  stop = clock();
  cpu_time_used = ((double)(stop - start))/ CLOCKS_PER_SEC;  //divide by this macro of the time.h to yield seconds
  printf("main - sort took %f seconds\n", cpu_time_used);
  printf("index = %d\n", pos);

  //free dynamically allocated memory
  free(nums);
  free(nums_reset);
  nums= NULL;
  nums_reset = NULL;
  return 0;
}

/* NAME: selection_sort
PARAMETERS: void *arr, int size, void (*swap) (void *,int,int), int (*compare) (void *,int,int)
PURPOSE: Perform a general selection sort on an array to sort the array in ascending order
PRECONDITION: Call when sorting an array using the selection sort algorithm
POSTCONDITION: the array called will be sorted in ascending order
*/
void selection_sort(void *arr, int size, void (*swap) (void *,int,int), int (*compare) (void *,int,int)){ //function args
   int lowindex, i, j;
   printf("\nSelection Sort...\n");
   for (i = 0; i < size-1; i++) {
     //get the index of the lowest value in the array between a range of the lower and upper bound
     lowindex = index_of_min(arr,i,size-1,compare);
     //swap if lowindex not the same as position i
     if (i != lowindex){
       swap(arr,i,lowindex);
     }
   }
}

/* NAME: quick_sort
PARAMETERS: void*arr, int begin, int end, void (*swap) (void *,int,int), int (*compare) (void *,int,int)
PURPOSE: Perform a general quick sort on an array to sort the array in ascending order
PRECONDITION: Call when sorting an array using the quick sort algorithm
POSTCONDITION: the array called will be sorted in ascending order
*/
void quick_sort(void*arr, int begin, int end, void (*swap) (void *,int,int), int (*compare) (void *,int,int)){
  int loc, *b = (int*) arr;
  if(begin<end){
    loc = partition((void*) b, begin, end, swap, compare);
    quick_sort((void *)b, begin, loc-1,swap,compare);
    quick_sort((void *)b, loc+1, end,swap,compare);
  }
}

/* NAME: partition
PARAMETERS: void* arr, int beg, int end, void (*swap) (void *,int,int), int (*compare) (void *,int,int)
PURPOSE: Find the necessary index used for the quick sort algorithm
PRECONDITION: call within the quick sort function to get the proper index
POSTCONDITION: return the proper index for quick sort
*/
int partition(void* arr, int beg, int end, void (*swap) (void *,int,int), int (*compare) (void *,int,int)){
  int left = beg, right = end-1, loc = beg, flag=0;
  int *b = (int*)arr;
  while(flag==0){
    while (b[loc] <= b[right] && loc!=right){
      right--;
    }
    if(loc==right){
      flag = 1;
    }
    else{
      swap((void*)b, loc, right);
      loc = right;
    }
    if (flag==0){
      while(b[loc]>b[left] && loc!=left){
        left++;
      }
      if (loc==left){
        flag=1;
      }
      else{
        swap((void*)b, loc, left);
        loc = left;
      }
    }
  }
  return loc;
}

/* NAME: linear_search
PARAMETERS: void* arr, int lower_bound, int upper_bound, void* key
PURPOSE: Perform a general linear search on a sorted array and return the index of a found value
PRECONDITION: Call on sorted arrays to find a position of a key within the array and return it
POSTCONDITION: The position of the key, if any, will be returned from the function
*/
int linear_search(void* arr, int lower_bound, int upper_bound, void* key){
  int pos = -1, i= lower_bound;
  int *b = (int*) arr;
  int *key2 = (int*)key;
  //loop through the array at each position to find the key
  while(i<upper_bound){
    if(b[i] == *key2){
      pos = i;
      break;
    }
    i++;  //go to next index
  }
  if(DEBUG && pos == -1){
    printf("Position not found.\n");
  }
  return pos;
}

/* NAME: binary_search
PARAMETERS: void* arr, int lower_bound, int upper_bound, void * key
PURPOSE: Perform a general binary search on an array and return the position of the key in the array
PRECONDITION: Call on a sorted array to return a position where the key exists
POSTCONDITION: The position of the key in the array is returned
*/
int binary_search(void* arr, int lower_bound, int upper_bound, void * key){
  int *b= (int*) arr;
  int pos = -1, i = lower_bound;
  int *key2 = (int *)key;
  int mid;
  while(i<=upper_bound){
    //get the middle index
    mid = (i+upper_bound)/2;
    if (b[mid]==*key2){
      pos = mid;
      return pos;
    }
    //move left if the key is greater than mid
    else if(b[mid]>*key2){
      upper_bound = mid-1;
    }
    //move right if the key is less than mid
    else{
      i = mid+1;
    }
  }
  return pos;
}

/* NAME: swap
PARAMETERS: void *arr, int i, int j
PURPOSE: swap two values in the array at points i and j
PRECONDITION: i and j are valid array indices,
POSTCONDITION: the array values at i and j are swapped
*/
void swap(void *arr, int i, int j){
  int *b = (int *) arr;
  int temp = b[i];
  b[i] = b[j];
  b[j] = temp;
}

/* NAME: compare
PARAMETERS: void *arr, int i, int j
PURPOSE: compare two keys in the array and return the res condition based on the comparison
PRECONDITION: Call when a comparison needs to be made between two points in the same array
POSTCONDITION: res, a condition, will be returned to the call
*/
int compare (void *arr, int i, int j){
  int *b = (int *) arr;
  int res = 0;
  if (b[i] < b[j])
    res = -1;
  else if (b[i] > b[j])
    res = 1;
  return (res);
}

/* NAME: index_of_min
PARAMETERS: void *arr, int low, int high, int (*compare) (void *,int,int)
PURPOSE: using a lower and upper bound, get the index of the minimum value
PRECONDITION: Call during selection sort to return the minimum value's index
POSTCONDITION: The position of the minimum value will be returned
*/
int index_of_min (void *arr, int low, int high, int (*compare) (void *,int,int)) {
 int lowind = low, i;
 for (i = low+1; i <= high; i++) {   /* check for smaller elements from low+1 to high */
   if (compare(arr,i,lowind) < 0)
     lowind = i;
  }
  return lowind;
}

/* NAME: reveal_array
PARAMETERS: void* arr, int size
PURPOSE: print an array to terminal so that a user may observe changes
PRECONDITION: Call to print the array to an output that a user will get to see
POSTCONDITION: The array is printed to the output linearly
*/
void reveal_array(void* arr, int size){
  int i, *b = (int* )arr;
  for(i=0;i<size;i++){
    printf("arr[%-7d]= %d\n", i,b[i]);
  }
}

/*
                  ***   REPORT    ***
          i. Team members: None
                All functions were created, developed, and tested without
                research by myself. No external sources were used in the
                making of this program.

          ii. Test Cases and status:
                    sorting arrays with random values - passed
                    sorting arrays with increasing / dereasing order - passed
                    searching for values at front, middle, or end of array - passed
                    searching for values not in array - passed

          iii.Search and Sort Analysis
              1. No, selection sort is independent of the original order of the elements in the array
                  (slide 33) - O(n) swaos abd O(n^2) comparisons

              2. Yes, there are arrays where quick sort will be faster or slower (less or more steps)
                  Arrays sorted in ascending or descending order will cause more steps due to how the pivot is selected
                  (slide 51)

              3. An improvement for the linear search algorithm in the chapter 14 slides could benefit from either a tighter bound,
              specifically, start from the middle of the array, take that key, and either linearly increase or decrease from that point.
              Additionally, the linear search algorithm can be rewritten to start at the beginning and end of an array, and linearly converge inward.

              4. Big O of storage for n items for each sort and search of n items:
                  quick sort     - big O(nlog_2(n))
                  selection sort - big O(n)
                  linear search  - big O(n)
                  binary search  - big O(log_2(n))
*/
