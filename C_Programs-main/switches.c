#include <stdio.h>
#include <math.h>


double sum_sqrt_3n(double x, double y, double z);
int main(void) {
  
  char ship_class;
  printf("Enter a ship ID number\n");
  scanf("\n%c", &ship_class);  
  printf("ship class is %c\n",ship_class);
  switch(ship_class){
  case 'B':
  case 'b':
    printf("battleship\n");
  break;
  case 'C':
  case 'c':
    printf("cruiser\n");
  break;
  case 'D':
  case 'd':
    printf("destroyer\n");
  break;
  case 'F':
  case 'f':
    printf("frigate\n");
  break;
  default:  // like an else
    printf("unknown\n");
  }

  char letter_grade;
  printf("Please input a letter grade.\n");
  scanf("\n%c", &letter_grade);


  switch(letter_grade)
  {
    case 'A':
    case 'a':
        printf("90-100\n");
        break;
    case 'B':
    case 'b':
        printf("80-89\n");
        break;
    case 'C':
    case 'c':
        printf("70-79");
        break;
    default:
      printf("Bruh\n");
  }
  
  
  //switch using getchar()
  char boat_class;
  printf("Enter a ship number\n");
  boat_class = getchar();  
  printf("ship class is %c\n",boat_class);
  switch(boat_class){
  case 'B':
  case 'b':
    printf("battleship\n");
  break;
  case 'C':
  case 'c':
    printf("cruiser\n");
  break;
  case 'D':
  case 'd':
    printf("destroyer\n");
  break;
  case 'F':
  case 'f':
    printf("frigate\n");
  break;
  default:  // like an else
    printf("unknown\n");
  }

  getchar(); //cleans previous input from getchar() of boatclass.

  char my_grade;
  printf("Please input a letter grade.\n");
  my_grade = getchar();
  getchar();

  switch(my_grade)
  {
    case 'A':
    case 'a':
        printf("90-100\n");
        break;
    case 'B':
    case 'b':
        printf("80-89\n");
        break;
    case 'C':
    case 'c':
        printf("70-79");
        break;
    default:
      printf("Bruh\n");
  }
  
  //for loop that initialized abc_s as "A", then increments through each character until reaching "z."
  char abc_s = 'A';
  for (abc_s = 'A'; abc_s <= 'z'; abc_s +=1){
    printf("%c is the character.\n", abc_s);
  }
  int init =0;
  int sum =0;
  
  //sum numbers 0 to 100
  for(init=0; init <=100; init+=1)
  {
    sum += init;
  }
  printf("%d", sum);
    return 0;
}

double sum_sqrt_3n(double x, double y, double z){
  double summation;
  summation = sqrt(x)+sqrt(y)+sqrt(z);
  return summation;
}
