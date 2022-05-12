#include <stdio.h> //for inputs and outputs
#include <stdlib.h> //for malloc and exit
#include <string.h> //for strncmp, strncpy, strndup

#define SIZE 100

typedef struct node{ //Linked list struct to link strings
  char data[SIZE]; //holds string type data
  int freq; //contains frequency of word in LL
  struct node *next; //self referential.
} node_t;

//define LL operations
node_t *create_node(char *data);
void insert(node_t **head, char *data);
void display_LL(node_t *head, FILE *fp);
int is_dups(node_t *head, char *data);
void bubble_sort(node_t *head);
int LL_len(node_t *head);
void del_node(node_t **head, char *stopbuf);

/* NAME: main
PARAMETERS: void, none
PURPOSE: The main function will operate files, start the linked list, then call functions to operate on that linked list.
PRECONDITION: No parameters, function is called when program runs.
POSTCONDITION: A linked list will be made, printed to a text file, modified, then printed to a seperate file.
*/
int main(void){
  //declare pointers to linked list, files and strings buffers for file reads.
  FILE *infile, *outfile, *stopin, *stopout; //file pointers to read and write text
  char buffer[SIZE], stopbuffer[SIZE]; //current word accessed from infile
  node_t *head = NULL; //pointers of linked list

  //open the files to be read and written.
  infile = fopen("text.txt", "r");
  outfile = fopen("concordance.txt", "w");
  stopin = fopen("stopwords.txt", "r");
  stopout = fopen("concordance_wo_stop_words.txt", "w");

  //check if the files exist.
  if (infile == NULL){
    printf("ERROR: File does not exist or No words in file...\n");
    exit(1);
  }

  //Create nodes for ALL words in file and link list
  while((fscanf(infile, "%s", buffer)) != EOF){
    insert(&head, buffer);
  }

  //sort the existing list then write to outfile
  bubble_sort(head);
  display_LL(head, outfile);

  //delete nodes using the words from stop file.
  while((fscanf(stopin, "%s", stopbuffer)) != EOF){
    del_node(&head, stopbuffer);
  }
  //display new linked list to another outfile.
  display_LL(head, stopout);

  //notify user runtime is finished and close all files.
  printf("Files successfully written.\n");
  fclose(infile);
  fclose(outfile);
  fclose(stopin);
  fclose(stopout);
  return 0;
}

/* NAME: create_node
PARAMETERS: data (string/ char array)
PURPOSE: creates a linked list node
PRECONDITION: data must be a string, call when you want an unlinked node created
POSTCONDITION: A node with data "data" and frequency initialized to 1 is created and it points to null return the address to the node.
*/
node_t *create_node(char *data){
  //dynamic allocation of node
  node_t *newp = (node_t *) malloc(sizeof(node_t));
  char *reset = data; //points to an array that is duped
  reset = strndup(data, SIZE);
  strncpy(newp->data, reset, SIZE); //data becomes the string stored in data. Since buffer would be appened and overwrite list, we duplicate it safely
  newp ->freq = 1; //this is the first instance of creation
  newp ->next = NULL; //new node does not point anywhere
  return(newp);
}

/* NAME: insert
PARAMETERS: **head (First node of linked list) and *data(string data to be used in node creation)
PURPOSE: inserts a new node at the end of the linked list. If data has a duplicate, do not create.
PRECONDITION: head must be the first node, node must be the address of the node to be added. Call to create and auto link nodes.
POSTCONDITION: Linked list "head" will have an non-duplicated node appeneded to it.
*/
void insert(node_t **head, char *data){
  //use function to confirm existence of node with data:
  if (is_dups(*head, data)){
    return; // if a node with data exists, do not create and link node.
  }
  //no duplicate was found.
  else{
    //create a new node
    node_t *newp = create_node(data); //create a node with string data, initialize as next is NULL
    node_t *cur; //point to current node in LL
    if(*head == NULL){ //if head is empty, link head to first node
      *head = newp;
    }
    else{ //otherwise, go to end of linked list and link the node at the end
      cur = *head;
      while(cur->next!=NULL){ // go to end
        cur = cur->next;
      }
      cur-> next = newp; //the last node's link goes to the new node
    }
  }
}

/* NAME: display_LL
PARAMETERS: *head (address to first node in the linked list head)
PURPOSE: print the linked list to an output file one node at a time.
PRECONDITION: head must be the address to the first node and output file must exist and be open. Call when wish to print LL.
POSTCONDITION: Linked list "head" will be written to a file.
*/
void display_LL(node_t *head, FILE *fp){
  //create a pointer to head and define the length of the current linked list.
  node_t *cur = head;
  int count = LL_len(head);
  //use fprintf to write to file.
  fprintf(fp, "There are %d distinct words in the text file:\n", count);
  if (cur == NULL){ //if the list is empty, show that.
    fprintf(fp, "Linked List is empty");
  }
  else{ //otherwise, print LL node by node until empty.
    do{
      fprintf(fp, "%s %d\n", cur->data, cur->freq);
      cur = cur->next;
    }while (cur != NULL);
  }
}

/* NAME: LL_len
PARAMETERS: head (address to first node in the linked list head)
PURPOSE: Calculate the length of length list head and return the number of nodes.
PRECONDITION: Linked list must be passed on function call, call when the length of linked list is needed.
POSTCONDITION: The LL will be traversed and the count of nodes will be returned.
*/
int LL_len(node_t *head){
  //initialize node count to zero
  int count = 0;
  //point to head
  node_t *cur = head;
  //parse the LL until at last node, tracking number of nodes.
  while(cur!=NULL){
    count +=1;
    cur = cur->next;
  }
  return count;
}

/* NAME: is_dups
PARAMETERS: *head (address to first node in the linked list head), data is the string to be passed.
PURPOSE: Parse a LL until a node containing the string that matches data occurs. If one does, increase frequency attribute at that node.
PRECONDITION: Call before creation of a node to confirm a node with the data does not exist. Data must be a string and the linked list must exist.
POSTCONDITION: 1 will be returned if a node with a match for data existed (to prevent creation) and the frequency of that node will go up one instead.
returns 0 otherwise to allow creation of new node.
*/
int is_dups(node_t *head, char *data){
  //point to head
  node_t *cur = head;
  //if empty, return 0 because a node must exist.
  if (cur == NULL){
    return 0; // lets pilot program know to create node since no nodes exits.
}
  else {
    //increments until node with match is found.
    while (cur != NULL) {
      if(strncmp(cur->data, data, sizeof(SIZE)) == 0){
        cur->freq +=1; //increment word freq at node in list.
        return 1; // exits and lets pilot know not to create node.
        }
      cur = cur->next;
      }
    return 0; //No duplicates exist; pilot program creates new node.
  }
}

/* NAME: bubble_sort
PARAMETERS: *head (address to first node in the linked list head)
PURPOSE: sort an existing linked list alphabetically.
PRECONDITION: head must be an existing linked list.
POSTCONDITION: Linked list "head" will be sorted alphabetically using an n^2 operation
*/
void bubble_sort(node_t *head){
  //introduce pointers to nodes, and a temp node for data swaps rather than position swaps.
  node_t *i, *j, temp;
  //if empty, exit the function
  if (head == NULL){
    return;
  }
  //one pointer starts earlier than the other
  i = head;
  j = head ->next;
  //nested loop. at each i itteration, j will start after i until null. Loop until i hits null.
  for(i=head; i!=NULL; i=i->next){
    for(j = i->next; j!=NULL; j=j->next){
      //if node j is less than i, bring node j data to i and vice versa,
      if (strncmp(j->data, i->data, sizeof(temp.data)) <0){
        strncpy(temp.data, j->data, sizeof(temp.data));
        temp.freq = j->freq;
        strncpy(j->data, i->data, sizeof(j->data));
        j->freq = i->freq;
        strncpy(i->data, temp.data, sizeof(i->data));
        i->freq = temp.freq;
      }
    }
  }
}

/* NAME: del_node
PARAMETERS: **head is the first node of the list. (*head is address of first node and head is pointer to list.)
data is the string the function searches nodes for to find which node to delete.
PURPOSE: Delete a node from a linked list and relink its previous to its next.
PRECONDITION: head must be an existing linked list with no duplicates. Call when a node is being "ignored." Or removed otherwised.
POSTCONDITION: The node with string that matches data will be deleted from linked list, head, and have its memory freed without leak.
*/
void del_node(node_t **head, char *data){
  //point to head.
  node_t *cur=*head, *prev;
  //if head has the data
  if (cur != NULL && (strncmp(cur->data, data, SIZE)) == 0) {
        *head = cur->next; // change head
        free(cur); // free old head
        return;
    }
    //search for string "data" while tracking previous node.
  while (cur != NULL && (strncmp(cur->data, data, SIZE)) !=0) {
      prev = cur;
      cur = cur->next;
  }

  // If key was not present in linked list
  if (cur == NULL){
      return;
    }
    //otherwise,
  // Unlink the node from linked list and link prev to next.
  prev->next = cur->next;
  free(cur); // Free memory
  cur = NULL;
}

/*

**********************Report***********************

ii.
    1. text.txt and stopwords.txt – passed :)
    2. text.txt with one word and empty stopwords.txt – passed :)
    3. text.txt with one stop word and stopwords.txt with that stop word – passed :)
    4. empty text.txt and nonempty stopwords.txt – passed :)
    5. text.txt with > 500 total words and stopwords.txt with > 10 total words – passed :) (Scary test, used 2 pasted scholarship essays for it lol)

iii.
    1. The worst case for inserting n words into the ordered linked list is if the words are in alphabetical order

    2.  Big O time complexity for creating an ordered linked list of n words is n^2 in this program. The slowest step is insertion of alphabetical words

    3. Big O storage requirements for the linked list of size n is n where n is the number of words to be stored into linked lists.

*/
