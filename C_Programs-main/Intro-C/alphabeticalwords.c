#include<stdio.h>
#include<string.h>
int main()
{
  int words;
  printf("Enter how many words to be sorted:");
  scanf("%d", &words);
  char str[words][50], t[50];
  int i, j;
  printf("Enter %d words serperately; Using all caps or all lower:\n", words);
  for(i=0; i<words; i+=1){
    scanf("%s", str[i]);
  }
  for(i=1; i<words; i+=1)
  {
    for(j=1; j<words; j+=1)
    {
      if(strcmp(str[j-1], str[j])>0)
      {
        strcpy(t, str[j-1]);
        strcpy(str[j-1], str[j]);
        strcpy(str[j], t);
        }
      }
   }
  printf("\nWords in alphabetical order:\n");
  for(i=0; i<words; i+=1){
    printf("%s\n", str[i]);
    }
  getchar();
  return 0;
}
