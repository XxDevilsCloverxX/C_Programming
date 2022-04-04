#include <stdio.h>

/*Problem 1:
Write a program that estimates the temperature in a freezer (in celcius) given the elapsed time (hours) since a power failure.
Temperature, T, is given by:

T = [4t^2 / (t+2)] - 20

where t is the time since the power failure. Program should prompt the user to enter how long it has been since the start of the power failure in hours and minutes.*/

int main(void) {
  
  int hours, minutes;
  //define inputs as int var
  printf("Please enter whole hours since power failure as an integer:\n");
  scanf("%d", &hours);
  printf("Please enter whole minutes since power failure as an integer:\n");
  scanf("%d", &minutes);
  //prompt and read integer inputs from user

  double total_time, min_to_hrs;
  //defining floats for total time and our conversion from minutes to hours.
  double min_int_to_float;
  min_int_to_float = minutes;
  min_to_hrs = min_int_to_float/60;
  total_time = hours + min_to_hrs;
  //convert integer of minutes to double for conversion of mins to fraction of hours

  double Temperature;
  Temperature = ((4*(total_time * total_time))/(total_time +2))-20;

  printf("%.2f is the estimated temperature after %d hours and %d minutes of power failure.\n", Temperature, hours, minutes);
  return 0;
}