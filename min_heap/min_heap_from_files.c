#include <stdio.h>  //use of stream
#include <stdlib.h> //exit
#define FILE_COUNT 2  //n files
#define LEN 25  //max length of file names
#define DEBUG 0 //print statements to terminal

//data budle for this assignment
typedef struct data{
  int input;
  int file_num;
}data_t;

//declare heap functions
void heap_insert(data_t heap[], data_t data, int n);
data_t heap_pop(data_t heap[], int n);

/*
AUTHOR: Silas Rodriguez
FILENAME: problem1.c
SPECIFICATION: Practice using heaps
FOR: CS 2413 Data Structures Section 001
*/

/* NAME: main
PARAMETERS: void, none
PURPOSE: The main function will operate files, start the heap, then call functions to operate on heap.
PRECONDITION: No parameters, function is called when program runs.
POSTCONDITION: A min heap will be generated, and evuentually contents from n sorted integer .txt files will be written to an outfile in ascending order.
*/
int main(void){
  //Declare initial variables
  data_t heap[FILE_COUNT], data, extracted;
  FILE * files[FILE_COUNT];
  int file_flags[FILE_COUNT], i, j=0, heap_size=0;
  char file_name[LEN];
  FILE *infile, *outfile;
  outfile = fopen("merged.txt", "w");

  //Check if the outfile exists, exit program if it does, files opened will be closed automatically
  if (outfile == NULL){
    printf("OUTFILE ERROR\n");
    exit(1);
  }
  //This loop will create the array of file pointers & file flags, and checks if they exist- initialize heap
  for(i=1; i<=FILE_COUNT; i++){
    //create a file name
    sprintf(file_name, "%d.txt", i);
    if(DEBUG){
      printf("%s\n", file_name);
    }
    //open file with appropriate name
    infile= fopen(file_name, "r");
    //check if file opened correctly, if not, exit with error and files will automatically close
    if (infile == NULL){
      printf("INFILE ERROR\n");
      exit(1);
    }
    //store the open file in an array
    files[i-1]= infile;
    if(DEBUG){
      printf("%p\n", infile);
  }
    //signal the array that file has been opened
    file_flags[i-1] = 1;
    //initialize the heap using file 1st data
    if(fscanf(infile, "%d", &(data.input)) != EOF){  //store the data into the input if not empty
      data.file_num = i;  //1st, 2nd, etc file at i, when used to reference files array, -1 will be needed
      //Construct the heap
      heap_insert(heap, data, heap_size); //we can use i-1 for the size of the heap because the heap will grow up to the FILE_COUNT
      heap_size++;  //Increase the heap size
  }
    else{
      //file was empty, so go ahead and close one file_flag and ignore entry
      file_flags[j] = 0;
      j++; //j will remember which file flag we are on
    }
  }

  if(DEBUG){
    printf("%d is starting heap size\n", heap_size);
  }
  /*
  Build the merged.txt file: The best way to track this would be to use a while loop that constantly checks if the last file flag is closed. - Flag index not corresponding to file
  Conditions: The heap size will decrease after pops, and increase with inserts.
  */
  while(file_flags[FILE_COUNT-1] != 0){
    //Pop the top data and write it to the merged.txt
    extracted = heap_pop(heap, heap_size);
    heap_size--;
    fprintf(outfile, "%d\n", extracted.input);
    if(DEBUG){
      fprintf(outfile, "--From file %d\n", extracted.file_num);
    }
    //using extracted node, scan next number in from the same file & insert into the array, if one exists
    if(fscanf(files[extracted.file_num-1], "%d", &(data.input))!= EOF){ //remember file num will be adjusted from earlier
    data.file_num = extracted.file_num; //adjust the file number to match so that data is polled correctly
    heap_insert(heap, data, heap_size);
    heap_size++;
  }
    else{
      if(DEBUG){
        printf("%d is heap size\n", heap_size);
      }
    //designate an empty file, and print the
    file_flags[j] = 0;
    j++;
  }
  }
  //This segment will close all of the files
  for (i=0; i<FILE_COUNT; i++){
    if (files[i] != NULL){
      fclose(files[i]);
    }
  }
  //Close the merge.txt
  fclose(outfile);
  //Let the user know that the file was built successfully.
  printf("merged.txt successfully constructed\n");
  return 0;
}

/* NAME: heap_insert
PARAMETERS: heap, data bundle, and size (n)
PURPOSE: heap insert will insert a data bundle at the end of a heap based on the current heap size and percolate up the min heap
PRECONDITION: heap cannot be full. (n cannot be greater than num of files). call when inserting a value into a heap
POSTCONDITION: The value will be inserted into the min heap and the heap invariant will be satisfied, after calling the funciton, increase the heap size externally!
*/
void heap_insert(data_t heap[], data_t data, int n){
  //Define variables
  int parent = (int)(n-1)/2;  //Parent cell index in array
  data_t temp;  //for swapping

  //Insert data at the end of the heap
  heap[n] = data;
  if (DEBUG){
    printf("%d of file %d inserted\n", data.input, data.file_num);
  }
  /*
  Satisty the heap invariant by comparing the inserted child to it's parent-
  swap if child is less than parent and repeat until the parent is less than or n =0
  */
  while(n>=0 && heap[n].input<heap[parent].input){
    /*
    This for loop runs while n is not at the beginning
    and the heap property is unsatified
    */
    //Swap the value of the parent and child, updating parent and child
    temp = heap[parent];
    heap[parent] = heap[n];
    heap[n] = temp;
    n = parent;
    parent = (int)(n-1)/2;
  }
}

/* NAME: heap_pop
PARAMETERS: heap, heap size
PURPOSE: heap pop will return the top node of the heap so that it can be operated on, while still satisfying the min heap axioms
PRECONDITION: heap must not be empty when called, call when taking the top data for operations (like writing to external file)
POSTCONDITION: The min value (from the top of the heap) will be returned to the calling function for operations, min heap invarient will still be satisfied in doing so
*/
data_t heap_pop(data_t heap[], int size){
  //declare a dummy variable to return at the end and a swap
  data_t top = heap[0], temp;
  int i=0, left=1, right=2;
  //swap the first and last node in the heap
  heap[0] = heap[size-1];
  heap[size-1] = top;
  //  "Delete" the last node:
  size--;
  //while in bound
  while(right<=size){
    //case 1: parent has two child
    if (right<size){
      //if the new parent makes a heap, return the data no swaps
      if(heap[i].input<heap[left].input && heap[i].input <heap[right].input){
        return top;
      }
      //if the parent does not satisfy the heap prop. check if left is less than right; if so, bring left up
      if(heap[left].input <= heap[right].input){
        temp = heap[i];
        heap[i] = heap[left];
        heap[left] = temp;
        i=left;
      }
      //left wasn't bigger, and heap property wasn't satisfied. bring right up
      else{
        temp = heap[i];
        heap[i] = heap[right];
        heap[right] = temp;
        i=right;
      }
    }
    else{ //parent has 1 child
      //check if the left value is not top and is less than parent, then swap.
      if (heap[left].input != top.input && heap[left].input < heap[i].input){
        temp = heap[i];
        heap[i] = heap[left];
        heap[left] = temp;
      }
      i=left;
    }
    left = 2*i+1;
    right = left+1;
  }
  //Return the top old top of the heap
  return top;
}

/*
                              *****  Report  *****
          i. Team members: None
              All functions were created, developed, and tested without
              research by myself. No external sources were used in the
              making of this program.

          ii. Test Cases and status:
          1. example 1.txt, 2.txt, merged.txt built – passed :)
          2. 3<=n<=12 files of sorted integers, merged.txt built – passed :) Used up to 7 files
          3. one of the m.txt files of sorted integers is empty, merged.txt built – passed :0
          4. one of the m.txt files of sorted integers is missing, merged.txt built – passed B) -exited properly
          5. one of the m.txt files of sorted integers containted numbers present in other file(s) - passed :P

          iii.Binary Heap Analysis

          1. Big O of inserting all m integers in files 1.txt to n.txt into a binary min heap of size n
                Big O of n for inserting m integer in files 1 to n would be O(m*log(n)) [base 2] because the structure is similar to a tree that as you traverse,
                exponentially many nodes are skipped to reach the nth position.
          2. Big O of the storage requirements for a binary min heap of size n
                Big O of storage requirements for a binary min heap of size n will be O(n), since the elements are stored in an array.
          3. How could this program be extended to perform an external file merge sort on one file
          of m unordered integers?
                I could think of one good solution:
                  1) The program could be modified to read in an unordered integer file
                      and write to different files the individual numbers (1_out.txt, 2_out.txt, ... n_out.txt), rescan those files, and they will be sorted. from this program on one file.
                      The mainchange is that the FILE_COUNT macro constant will be changed to the m unordered integers.
*/
