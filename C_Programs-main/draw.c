#include <stdio.h>
#include <math.h>
//Defining my own functions. Must be outside main
void draw_eyes(void);
void draw_frown(void);
void draw_eyebrows(void);
void draw_sad(void);

int main(void) {
  
  int dollars, pennies, dimes, quarters, nickels;
  int total_cents, total_dollars, change;
  printf("Enter number of dollars:");
  scanf("%d", &dollars);
  printf("Enter number of pennies:");
  scanf("%d", &pennies);
  printf("Enter number of dimes:");
  scanf("%d", &dimes);
  printf("Enter number of quarters:");
  scanf("%d", &quarters);
  printf("Enter number of nickels:");
  scanf("%d", &nickels);

  total_cents = 100*dollars + 10*dimes+ 5*nickels + 25*quarters + pennies;
  printf("%d total cents", total_cents);
  total_dollars = total_cents / 100;
  change = total_cents % 100;
  printf("\n%d dollars and %d cents", total_dollars, change);
  printf("\n$%d.%d total", total_dollars, change);
  

  
  int i = 1;
  printf("i is %d \n", --i);
  // i is modified before execution (i-1)
  printf("i is %d \n", i);
  i = 1;
  printf("i is %d \n", i--);
  //i is modified after execution i, then i-1
  printf("i is %d \n", i);
  

  
  double pi = 3.14159; //constant pi
  double hole_diameter; //diameter of hole
  double edge_diameter; //diameter of washer
  double thickness; // thickness of washers
  double density; //density of washer wt/volume
  double quantity; //how many washers
  double weight; //total weight
  double hole_radius; //radius of the hole in a washer
  double edge_radius; //radius of the washer
  double rim_area; //area of the washer without the hole
  double unit_weight; //weight per washer

  printf("Inner diameter of hole in cm:");
  scanf("%lf", &hole_diameter);
  printf("Diameter of edge in cm:");
  scanf("%lf", &edge_diameter);
  printf("Thickness:");
  scanf("%lf", &thickness);
  printf("Quantity:");
  scanf("%lf", &quantity);
  printf("Unit Weight:");
  scanf("%lf", &unit_weight); 
  

  
  double n1,n2,sq_rt, sqrt2, sum_sqrt;
  printf("Enter first number\n");
  scanf("%lf", &n1);
  printf("Enter second number\n");
  scanf("%lf", &n2);

  sq_rt = sqrt(n1);
  sqrt2 = sqrt(n2);
  sum_sqrt = sq_rt + sqrt2;
  printf("%.3f, is the sum sqrt of %.1f and %.1f\n", sum_sqrt, n1, n2);
  printf("%.3f and %.3f are the sqrts added\n", sq_rt, sqrt2);
  
  
  draw_sad();
  

  return 0;
}

void draw_eyes(void)
{
  {
  printf("         ***      ***\n");
  printf("        *   *    *   *\n");
  printf("        *   *    *   *\n");
  printf("         *** |  | ***\n"); 
  printf("             |  |\n");
}
}

void draw_frown(void)
{
  printf("              /\\\n");
  printf("             /  \\\n");
  printf("            /    \\\n");
}

void draw_eyebrows(void)
{
  printf("       <<<<<</   \\>>>>>>>\n");
  printf("      <<<<<<<     >>>>>>>>\n");
}

void draw_sad(void)
{
  draw_eyebrows();
  draw_eyes();
  draw_frown();
  printf("\n 'I'm big sad but at least I know C Functions'");
}