#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

float sqrt_sum(int a, int b);
void add_arrays(const double ar1[], const double ar2[],double arsum[],int n);
int add_row(int arr[], int n);
void stcpy(char *s, char*t);


typedef struct long_lat{
  int deg;
  int min;
  char dir;
}long_lat_t;

typedef struct location{
  char place[20];
  long_lat_t longitude, latitude;
}location_t;

int verify_loc(location_t *locp);

int main(void){
  /*
  int num1=1, num2=8;
  float bruh = sqrt_sum(num1, num2);
  printf("%f\n", bruh);
  */

  /*
  char inp;
  printf("Enter a gah damn letter\n");
  inp = getchar();

  switch (inp){
      case 'a':
      printf("Apple\n");
      break;
      case 'p':
      printf("pepper\n");
      break;
      case 'r':
      printf("radish\n");
      break;
      case 't':
      printf("tomato\n");
      break;
      default:
      printf("Not a good letter to pick\n");
  }
  */

  /*
  int wind = 72;
    if(wind<25){
        printf("Not strong wind\n");
    }
    else if(wind>=25 && wind<=38){
        printf("strong wind\n");
    }
    else if(wind>=39 && wind<=54){
      printf("gale\n");
    }
    else if(wind>=55 && wind<=72){
      printf("whole gale\n");
    }
    else{
      printf("Hurricane\n");
  }
  */

  /*
  int m = 10, n = 5;
  int *mp, *np;
  mp = &m;
  np = &n;
  *mp = *mp + *np;
  *np = *mp - *np;
  printf("%d %d\n%d %d\n", m, *mp, n, *np);
  */

  /*
  double x[8] = {16.0, 12.0, 6.0, 8.0, 2.5, 12.0, 14.0, -54.5};
  int i = 5;
  printf("%d %.1f\n", i, x[i]);
  //printf("%.1f\n", x[2 * i]); //invalid, out of array bounds
  printf("%.1f\n", x[(int)x[4]]);
  */

  /*
  double c[6] = {1,2,3,4,5,6},d[6] = {7,8,9,10,11,12},e[6];
  printf("%p, %p\n", &c[2], &d[2]);
  add_arrays(&c[2],&d[2],&e[2],4);
  printf("%lf" , e[0]);
  */

  /*
  const int n = 10;
  int i;
  float hours, rate;
  float arr1[n], arr2[n];
  for(i=0; i<n;i++){
    printf("Enter employee hrs");
    scanf("%f", &hours);
    arr1[i] = hours;
    printf("Enter employee rate");
    scanf("%f", &rate);
    arr2[i] = rate;
    printf("Hours worked: %f\nPay Rate: %f\n", arr1[i], arr2[i]);
  }
  */

  /*
  const int n =10;
  int i, j;
  float hours, rate;
  float arr[n][2];

  for(i=0, j=0; i<n; j++){
      if(j>0){
        i++;
        j=0;
      }
      printf("Enter employee hours then rate:");
      scanf("%f%f", &hours, &rate);
      arr[i][j] = hours;
      arr[i][j+1] = rate;
      printf("Hours: %f\nRate: %f\n", arr[i][j], arr[i][j+1]);
  }
  */

  /*  //Dynamic allocation of 1D array
  int i;
  int *ptr; //only using a pointer for dynamic allocation

  ptr = (int *)calloc(10, sizeof(int));
  if(ptr == NULL){
    printf("There isn't any free memory");
  }
  else{
    for(i = 0; i< 10; i++){
      ptr[i] = i*2;
      printf("%d\n", ptr[i]);
    }
    free(ptr); //deallocate and return to heap
    ptr = NULL; //prevent memory leaks.
  }
  */

  /*
  //Dynamic allocation of 2D arrays with 7 row 5 cols
  int (*ptr)[5];
  int i,j;
  ptr = (int (*)[5])calloc(35, sizeof(int)); //35 = 7x5

  for(i=0; i<7;i++){
    for(j=0; j<5; j++){
      ptr[i][j] = i + j;
      printf("%d\n", ptr[i][j]);
    }
  }
  //free(ptr);
  //ptr = NULL;

  int sum;
  for(i=0;i<7;i++){
    sum = add_row(ptr[i], 5);
    printf("ptr[%d] = %d\n", i, sum);
  }
  free(ptr);
  ptr = NULL;

 */
/*
char last[20], first[20], middle[20];
char pres[20] = "Adams, John Quincy";
strncpy(first, pres, 2);
first[2] = '\0';
printf("%s\n", first);
printf(" %s\n", strcpy(last, &pres[7]));
strncpy(first, &pres[7], 2);
first[2] = '\0';
strncpy(last, &pres[14], 2);
last[2] = '\0';
printf(" %s%s\n", first, last);
*/

/*
  location_t resort = {"Hawaii\n", {158,0,'W'}, {21,30,'N'}};
  printf("%c\n", resort.longitude.dir);
  int s;
  s = verify_loc(&resort);
*/

/*
  char s[10];
  char t[10] = "Hello";
  stcpy(s,t);
  printf("%s\n", s);
*/

  return 0;
}

/*
float sqrt_sum(int a, int b){
  int sum = a+b;
  float res = sqrt(sum);
  return res;
}
*/

void add_arrays(const double ar1[], /* input - */
                const double ar2[], /* arrays being added */
                double arsum[],     /* output - sum of corresponding elements of ar1 and ar2 */
                int n)  {           /* input - number of element pairs summed */
     int i;
     for (i = 0; i < n; ++i){        /* Adds corresponding elements of ar1 and ar2 */
          arsum[i] = ar1[i] + ar2[i];
          printf("%lf\n", arsum[i]);
        }
}

int add_row(int arr[], int n){
  int sum = 0;
  int i;
  for(i=0; i<n; i++){
    sum += arr[i];
  }
  return sum;
}

int verify_loc(location_t *locp){
  printf("%p\n", &(locp->latitude));
  printf("%d\n", locp->latitude.deg);
  printf("%d\n", (*locp).latitude.deg);
  return 0;
}

void stcpy(char *s, char*t){
  while(*s++ = *t++){
    continue;
  }
}
