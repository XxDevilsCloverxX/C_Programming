#include <stdio.h>

/*
AUTHOR: Silas Rodriguez
FILENAME: problem1.c
SPECIFICATION: Practice using C 2D arrays
FOR: CS 2413 Data Structures Section 001
*/
#define ROW 6
#define COL 7
void find_peaks(int arr[ROW][COL]);

/* NAME: main
PARAMETERS: void, none
PURPOSE: The main function initializes the 2D array and calls the find_peaks function
PRECONDITION: No parameters, function is called when program runs.
POSTCONDITION: The function find_peaks is called, and the main function returns 0 to check for completion.
*/
int main(void){
  int two_dimen_arr[ROW][COL] = {{5039, 5127, 5238, 5259, 5248, 5310, 5299}, {5150, 5392, 5410, 5401, 5320, 5820, 5321}, {5290, 5560, 5490, 5421, 5530, 5831, 5210}, {5110, 5429, 5430, 5411, 5459, 5630, 5319}, {4920, 5129, 4921, 5821, 4722, 4921, 5129}, {5023, 5129, 4822, 4872, 4794, 4862, 4245}}; //2D array from assignment
  find_peaks(two_dimen_arr); // function call to print output.
  return 0;
}

/* NAME: find_peaks
PARAMETERS: arr[ROW][COL], 2D array of ROW rows and COL columns containing integers
PURPOSE: The function parses the array and identifies peaks in relative positions.
PRECONDITION: The function must be called with a 2D array argument of ROW rows and COL Columns, called when finding peaks within 2D array data.
POSTCONDITION: The function should return prints of the amount of peaks in the 2D array as well as their index locations.
*/
void find_peaks(int arr[ROW][COL])
{
  int inner_row_size = ROW-1, inner_col_size = COL-1; //sizes of array minus border because we only check inner side of matrix
  int total_cells = inner_row_size * inner_col_size; // number of elements accessed in matrix
  int i, j, k=0, peaks=0; // index vars, k < total_cells, peaks counts number of peaks
  int accessed_value, north, south, east, west; // values accessed in array[row][column]
  int index_arr[ROW][COL] = {0}; // empty array for index printing
  /*
  initialize 6x7 matrix loop on inner bound, total cells will become inner bound
  */
  for(i=1, j=1; k < total_cells; j++, k++)
  {
    //check if j is within bound, COL works because index starts at 0.
    if (j == COL){
      i++; //increment i row
      j = 1; // reset j col
    }
    accessed_value = *(*(arr + i) +j);
    north = *(*(arr + (i-1)) +j); // value above two_dimen_arr[i][j]
    south = *(*(arr + (i+1)) +j); // value below two_dimen_arr[i][j]
    east = *(*(arr + i) +(j+1)); // value right two_dimen_arr[i][j]
    west = *(*(arr + i) +(j-1)); // value left two_dimen_arr[i][j]
    if ((accessed_value > north) && (accessed_value > south) && (accessed_value > east) && (accessed_value > west)){
      peaks++;
      index_arr[i][j] = 1; // store 1's for peak found as true, all other are null
    }
  }
  printf("%d Peaks found:\n", peaks);
  for(i = 1, j=1, k=0; k<total_cells; j++, k++){
    if (j == COL){
      i++; //increment i row
      j = 1; // reset j col
    }
    if (index_arr[i][j] == 1){ //we do not check border index, so those nonzero values will not be checked or influence print
      printf("Location at row %d and col %d: %d\n", i, j, arr[i][j]);
    }
  }
}

/*
Part d:

i. The output of this program is:
3 Peaks found:
Location at row 2 and col 1: 5560
Location at row 2 and col 5: 5831
Location at row 4 and col 3: 5821

ii. To store the data into the array, the Big-O storage
complexity is O(mxn), because its a 2D array of m rows and n columns. There are 2, but Big-O notation does not use constant multiples.
Additionally, the peaks and location variables are just a variable with int types, and only take O(1) space.
Therefore, the whole program takes the higher order and requires O(mxn) space complexity to run.

iii. For this program, two linear loops are implemented and parsed by data rather than rows and columns. Therefore, Big-O notation is O(n) because
not all all of the data is parsed with the loops, but an f(n) < O(n) rule applies
and that would make the time complexity of this dependent on the linear loops for Big-O O(n) where f(n) is some variation of 2n.
*/
