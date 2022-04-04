#include <stdio.h>
double cost(double initcost, double annualfuel, double annualelecrate, double annualtax);

double Celsius(double depth);
double Fahrenheit(double Temp_inC);

int main(void) {
  
  //Problem 1
  //define our inputs as double
  double initcost, annualfuel, elecrate, taxrate, TotalCost;

  //prompt user to provide input values
  printf("Please input initial house cost.\n");
  scanf("%lf", &initcost);
  printf("Please input Annual fuel cost.\n");
  scanf("%lf", &annualfuel);
  printf("Please input annual electric rate.\n");
  scanf("%lf", &elecrate);
  printf("Please input annaul tax rate.\n");
  scanf("%lf", &taxrate);

  TotalCost = cost(initcost,annualfuel, elecrate, taxrate);

  printf("%.2lf is the total cost of the house in 5 years.\n", TotalCost);

  //Problem 2
  
  //define depth as a float
  double depth, Temp_inC, TempinF;
  //prompt user for depth
  printf("Please enter the depth in KM.");
  scanf("%lf", &depth);
  
  //calling function to calculate temp in C and F
  Temp_inC = Celsius(depth);
  TempinF = Fahrenheit(Temp_inC);

  printf("%.2lf is Temperature Celsius at %lf km.\n", Temp_inC,depth);
  printf("%.2lf is Temperature in Farenheit at %lf km.", TempinF, depth);
  return 0;
}

double cost(double initcost, double annualfuel, double elecrate, double taxrate)
{
  double TotalCost, AnnualElec, AnnualTax;

  AnnualElec = annualfuel * elecrate;
  AnnualTax = initcost * taxrate;
  TotalCost = initcost + (AnnualTax + annualfuel + AnnualElec)*5;

  return TotalCost;
}

double Celsius(double depth)
{
  double Temp_inC;
  Temp_inC = 10*(depth)+20;
  return Temp_inC;
}
double Fahrenheit(double Temp_inC)
{
  double Temp_inF;
  Temp_inF = 1.8*(Temp_inC)+32;
  return Temp_inF;
}