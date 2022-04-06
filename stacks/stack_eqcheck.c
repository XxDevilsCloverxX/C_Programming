#include <stdio.h> //for inputs and outputs
#include <stdlib.h> //for malloc
#include <ctype.h> //for isalpha

/*
AUTHORS: Silas Rodriguez
FILENAME: problem1.c
SPECIFICATION: Practice using C stacks.
FOR: CS 2413 Data Structures Section 001
*/

//size macro for buffer
#define SIZE 50

//my linked list nodes
typedef struct node{ //Linked list struct to link strings
  char bracket; //holds the bracket character
  struct node *next; //self referential.
} node_t;

//define LL stack functions
node_t *create(char bracket); // create a node
void push(node_t **top, char bracket); //push a node onto a stack
char pop(node_t **top); // free the top node & return it's value
char invert(char R_bracket); // convert right bracket to left
void peek(node_t *top); //look at the top of the stack without losing node
int check_var_and_assign(char arr[SIZE]); //when scanned from file, initial list
int is_Lbrack(char bracket); //return 1 if L brack
int is_Rbrack(char bracket); //return 1 if R brack

/* NAME: main
PARAMETERS: void, none
PURPOSE: The main function will use control statements with stack + LL operations to display info to user.
PRECONDITION: No parameters, function is called when program runs.
POSTCONDITION: An output will be printed to console that defines error type of equation and which equation.
*/
int main(void){
    //initialize file pointer, buffer, bracket holder, error_id and stack top
    FILE *infile;
    int error_id=0, i;
    node_t *top = NULL;
    char buffer[SIZE], brack; //To hold result of fgets

    //open the file
    infile = fopen("equations.txt", "r");
    if(infile == NULL){
      printf("FILE CANNOT BE READ");
      exit(1);
    }
    //while the file is not empty, scan line by line
    while (fgets(buffer, SIZE, infile) != NULL){
      //confirm first 2 case of error id
      error_id = check_var_and_assign(buffer);
      //if an error occured, print error type and continue to next statement wihtout checking error 3
      if(error_id == 1 || error_id==2){
        printf("***ERROR ID %d on %s\n", error_id, buffer);
        continue;
      }
      /*
      if error 1 or 2 wasn't found, we check for bracket mismatch
      starting with pushing left brackets into the stack
      then popping right_brackets from the stack.
      */
      i=0;
      while(buffer[i++] != '\0'){
        if(is_Lbrack(buffer[i])){
          push(&top, buffer[i]);
        }
        else if (is_Rbrack(buffer[i])){
          brack = pop(&top);
          if(brack == invert(buffer[i])){
            error_id = 0; //if reversed R brack matches, no error
          }
          else{
            error_id = 3; //any other case, something doesn't match
          }
        }
      }

      //Check if stack is empty, because this indicates mismatch parenthesis
      while(top != NULL){
        error_id = 3; //The stack wasn't clean, so error 3 (Avoids L brack with no right brack case)
        pop(&top); //clean the stack until NULL
      }
      //reveal if error id is 0 or 3 :)
      printf("***ERROR ID %d on %s\n", error_id, buffer);
  }

    fclose(infile);
    return 0;
}

/* NAME: create
PARAMETERS: bracket
PURPOSE: creates a node that will hold a character
PRECONDITION: bracket will be a character, left bracket. Call when node needs created
POSTCONDITION: a pointer to this node will be created and returned
*/
node_t *create(char bracket){
  //dynamic allocation of node
  node_t *newp = (node_t *) malloc(sizeof(node_t));
  //initialize the bracket to be the character
  newp -> bracket = bracket;
  newp ->next = NULL; //new node does not point anywhere
  return(newp);
}

/* NAME: push
PARAMETERS: **top and bracket
PURPOSE: create and push a new node with bracket character to the top of the stack
PRECONDITION: Call this function when a node must be created and pushed into a stack
POSTCONDITION: a stack, top, will have a node inserted to the front of the linked list.
*/
void push(node_t **top, char bracket){
  //create the new node
  node_t *new_node = create(bracket);
  /*
  >push the node onto the top of the stack
  >no need to check if null if always at top
  >the node going into the stack needs to link to
  node on top of the stack
  */
  new_node -> next = (*top);
  (*top) = new_node; //now the top of the stack is the new node & we are done.
}

/* NAME: peek
PARAMETERS: pointer to a stack, top
PURPOSE: View top element of the stack without removing the element
PRECONDITION: Call for sanity checks, verify empty list, and pass a pointer to the whole stack
POSTCONDITION: The address of the top node and its data will be printed to the console, or empty list.
*/
void peek(node_t *top){
  if (top == NULL){
    printf("Empty Stack\n");
  }
  else{
  printf("top of stack: %p bracket: %c\n", top, top->bracket);
  }
}

/* NAME: invert
PARAMETERS: right bracket, bracket
PURPOSE: Take a right bracket and return its reversed bracket for comparison
PRECONDITION: Right bracket must be passed, call when comparing for matching left bracket
POSTCONDITION: The reversed bracket character will be returned.
*/
char invert(char R_bracket){
  switch(R_bracket){
    case ')':
      return '(';
      break;
    case ']':
      return '[';
      break;
    case '}':
      return '{';
      break;
  }
}

/* NAME: check_var_and_assign
PARAMETERS: character array, arr
PURPOSE: Take an array storing a string and determine if it starts with a letter, a space, and an = sign.
PRECONDITION: Call after data is stored into an array to confirm if error 1 or 2 occur.
POSTCONDITION: Error 1 or 2 will be identified, 0 error otherwise.
*/
int check_var_and_assign(char arr[SIZE]){
  //check if a 1 letter variable is at beginning
  if(isalpha(arr[0])==0){ //isalpha returns 0 if non alphabet
    return 1; //error type 1
  }
  /*
  we are able to use the next format since the strings all have format ['var', ' ', '=']
  which means = will be on index 2 and var would have been one, if we ignore whitespace cleaning
  */
  else if (arr[2] != '='){ // condition to check for assignment operator
    return 2; //error type 2
  }
  else{
    return 0; //error status 0...so far
  }
}

/* NAME: is_Lbrack
PARAMETERS: bracket, character
PURPOSE: confirm if bracket is a left bracket
PRECONDITION: Call when determining a character should recieve stack operations
POSTCONDITION:1 will be returned if char is a left bracket, 0 otherwise
*/
int is_Lbrack(char bracket){
  if (bracket == '(' || bracket == '{' || bracket == '[')
    return 1;
  else
    return 0;
}

/* NAME: is_Lbrack
PARAMETERS: bracket, character
PURPOSE: confirm if bracket is a right bracket
PRECONDITION: Call when determining if a character causes stack operations
POSTCONDITION:1 will be returned if char is a right bracket, 0 otherwise
*/
int is_Rbrack(char bracket){
  if (bracket == ')' || bracket == '}' || bracket == ']')
    return 1;
  else
    return 0;
}

/* NAME: pop
PARAMETERS: node: **top
PURPOSE: remove the top element of its stack while returning the character data it contained
PRECONDITION: Call when right bracket occurs to compare with data in node, a address to a node pointer must be passed
POSTCONDITION: the node will be removed from the list, and the data it contained will be returned to the program to operate on
*/
char pop(node_t **top){
  //underflow case - don't print underflow to output
  if(*top == NULL){
    return '\0';
  }
  //take the data from top
  node_t *cur = *top;
  char data;
  data = (*top)->bracket;
  (*top) = cur-> next;
  free(cur);
  cur = NULL;
  return data;
}

/*
        *****REPORT*****
    i. Team members: None
        All functions were created, developed, and tested without
        research by myself. No external sources were used in the
        making of this program.

    ii. Test Cases and status:
        equations.txt example from the assignment: passed :)
        equations.txt with all mismatch brackets: passed :)
        equations.txt with no errors: passed :)
        equations.txt with 1 equation: passed :)
        equations.txt with 0 equations: passed :) (assuming no output needed)

    iii.Stack Linked List Analysis
        1. Example of worst-case type of equation:
            An equation of max size containing a variable in position 0, = in position 2,
            then all left brackets. This is worst case because the operations would not
            stop at case 1 and 2, and will begin to push and need cleaned. This, or a equation with
           alternating open and closing brackets that causes push and pop operations on oscillating intervals.

        2. Big O for determining n equations of max length m are valid:
            The program takes n equations of max size m, and goes through each character, m.
            This means that big O is O(nxm) because each line is accessed m times. m can be stopped
            at position 0 or 2, but we would not know and need to assume that it goes to the end.

        3. Big O of storage requirements for a stack linked list of size n:
            Big O storage for a stack linked list of size n would be O(n). This is because
            up to n elements could be pushed into the linked list from the original buffer.
*/
