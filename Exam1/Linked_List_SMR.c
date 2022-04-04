#include <stdio.h> //input and output
#include <stdlib.h> // malloc and calloc

//Let's write a linked list to spell out my name from a file using char nodes

typedef struct node{
  char letter;
  struct node *next;
}node_t;

//Define some basic LL functions
node_t *create_node(char data);
void insert_node(node_t **head, char data);
void search_and_del(node_t **head, char data, node_t **prev, node_t **cur);
void print_LL(node_t *head);

int main(void){
  //initialize my file and LL head
  FILE *infile;
  node_t *Start = NULL, *cur, *prev;
  char text, dud_letter = 't';
  infile = fopen("name.txt", "r");
  if(infile == NULL){
    printf("File does not exist.");
  }

  //Read all letters
  while (fscanf(infile, "%c" , &text) != EOF){
  insert_node(&Start, text);
}
  //Print the Linked List
  print_LL(Start);

  //Delete the extra t, if it exists
  search_and_del(&Start, dud_letter, &prev, &cur);

  //print the new LL
  print_LL(Start);

  //close file
  fclose(infile);
  return 0;
}

//To create a node with character type
node_t *create_node(char letter){
  //dynamically make a new node
  node_t *newp;
  newp = (node_t *)malloc(sizeof(node_t));
  //set the nodes members
  newp->letter = letter;
  newp->next = NULL;
  //return the node pointer so it can be used in another function
  return newp;
}

//Insert a previously created node into the LL at the end
void insert_node(node_t **head, char data){
  node_t *new_node = create_node(data);
  node_t *cur = *head;
  //if empty, insert at head
  if(*head==NULL){
    *head = new_node;
  }
  else{
    //increment until the end of LL
    while(cur->next!= NULL){
    cur = cur->next;
  }
    //link new node to the current's next member
    cur->next = new_node;
  }
}

//Print the LL to console to view
void print_LL(node_t *head){
  node_t *cur = head;
  while(cur != NULL){
    printf("%c\n", cur->letter);
    cur = cur->next;
  }
}

void search_and_del(node_t **head, char data, node_t **prev, node_t **cur){
  *cur = *head;
  *prev = NULL;
  //if Empty
  if(*head == NULL){
    printf("Empty List");
    return;
  }
  //check if deletion is at beginning
  if((*cur)->letter == data){
    *head = (*cur)->next;
    free(*cur);
    *cur = NULL;
    return;
  }
  //while not at the end or a node where the letters match
  while((*cur)!= NULL && (*cur)->letter != data){
    *prev = *cur;
    *cur = (*cur)->next;
  }
  if(*cur == NULL){
    printf("Letter not found\n");
    return;
  }
  else{
    //a letter was found, so prepare to delete cur
    (*prev)->next = (*cur)->next;
    free(*cur);
    *cur= NULL;
    printf("Node deleted\n");
    return;
  }
}
