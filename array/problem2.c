#include <stdio.h>
/*
AUTHOR: Silas Rodriguez
FILENAME: problem2.c
SPECIFICATION: Practice Concepts of C programming, complexity analysis, and abstract data types.
FOR: CS 2413 Data Structures Section 001
*/

/* NAME: main
PARAMETERS: void, no parameter
PURPOSE: This main function opens and closes a text file in read mode, uses loops to parse the data and compute summations, slope, and intercept.
Also, main sorts the data in ascending order, and prints information to the console.
PRECONDITION: void parameter suggests there is no precondition to run this program, the function is called on all program runs.
POSTCONDITION: The range of altitudes from the zone.txt file will be printed and a linear model will be printed to the console.
*/
int main (void) {
  double alt, ozone; //var alt is altitude in km. ozone is ozone mixing ratio in ppmv.
  double sum_x =0, sum_y=0; // summation of all altitude (x) and ozone mixing ratio (y) elements in file.
  double sum_xsq = 0, sum_xy=0; // summation of x elements but with squared values.
  FILE *infile; // file pointer to read and store zone.txt
  double min_altitude, max_altitude;
  int counter = 0; // variable used to count iterations of loop for n uses
  infile = fopen("zone.txt", "r");
  if (infile == NULL)
  {
    printf("Error with file.");
  }
  fscanf(infile, "%lf%lf", &alt, &ozone); //This block exists outside of the loop to initialize the min and max altitude without losing values and count of the first go.
  min_altitude = alt;
  max_altitude = min_altitude;
  sum_x += alt; // sum_x is the summation of all x elements from infile
  sum_y += ozone; // sum_y is the summation of all y elements from infile
  sum_xsq += alt * alt; // sum_xsq is the summation of all x squared elements from infile
  sum_xy += ozone * alt; // sum_xy is the summation of all x*y elements from infile
  counter +=1;
  while((fscanf(infile, "%lf%lf", &alt, &ozone)) != EOF)
  {
    min_altitude = (alt < min_altitude) ? alt:min_altitude;
    max_altitude = (alt > max_altitude) ? alt:max_altitude;
    sum_x += alt; // sum_x is the summation of all x elements from infile
    sum_y += ozone; // sum_y is the summation of all y elements from infile
    sum_xsq += alt * alt; // sum_xsq is the summation of all x squared elements from infile
    sum_xy += ozone * alt; // sum_xy is the summation of all x*y elements from infile
    counter +=1; // add a unit to counter for the n actions calculation
  }

  fclose(infile);

  double m, b; //m variable is slope and b is intercept of a linear model.
  m = (sum_x*sum_y - counter*sum_xy)/(sum_x*sum_x - counter*sum_xsq);
  b = (sum_x*sum_xy - sum_xsq * sum_y)/(sum_x*sum_x - counter*sum_xsq);

  printf("Range of altitudes in km: %.2f to %.2f\n", min_altitude, max_altitude);
  printf("Linear Model: ozone-mix-ratio = %.2f * altitude + %.2f\n", m, b);

  return (0);
}

/*
Space complexity:
1. Running externally with sizeof() function and adding all the variables, based on int = 2 bytes, double = 8 bytes, with 10 double variables
and 1 integer variable, the program requires 82 bytes of memory to run & store the data.
2. It is possible to implement this sorting algorithm without an array as shown above.

Time complexity:
1. The f(n) is just n since there is only one while loop that iterates over the file.
2. 0 <= f(n) <= O(n) where f(n) = n.
0 <= n <= cn.
0/n <= (n)/n <= cn / n.
0 <= 1 <= c, max @ n = 1 operation.
0 <= 1 <= c, so c = 1.
For n_0, 0 <= 1<= 1.
For c = 1, and n>=1, n is O(n).
f(n)=n <= O(n).
*/
