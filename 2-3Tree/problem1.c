#include <stdio.h>  //For inputs and output functions
#include <stdlib.h> //For malloc and exit
#include <string.h> //For strncmp and strncpy
#include <ctype.h>  //For is upper and tolower

/*
AUTHOR: Silas Rodriguez
FILENAME: problem1.c
SPECIFICATION: Practice using 2-3 Trees
FOR: CS 2413 Data Structures Section 001
*/

#define KEYS 2  //max number of keys in a 2-3 tree node
#define SIZE 50 //max characters in an string

//Bundle data item: string and frequency
typedef struct data_node {
  char key[SIZE];
  int freq;
}
data_t;

//2-3 tree node with number of keys, array of data bundles, and array of self-pointers
typedef struct node {
  int num_keys; //number of keys in the data array
  data_t data[KEYS+1];  //data array with 1 extra for splits
  struct node *tree_p[KEYS+1+1];  //pointer array with 1 extra for splits
}tree_t;

//Helper Functions & Recursive Child Function
void print_tree (tree_t *tree, FILE *out);  //helper to recursive 2-3 recur print
void recur_print (tree_t *tree, FILE *out); //print the 2-3 tree recursively in order traversal
void insert (tree_t **root, data_t val);  //helper function for recur_insert function
void recur_insert (tree_t **root, data_t val, int *split, data_t *mid, tree_t **newp); //recursively insert a key into a 2-3 tree

//Non-Tree Operations & Non-Helper Tree Operations
tree_t *search_23tree (tree_t *, char word[]);  //recursive search function to find an element in a 2-3 tree
tree_t *create_node (); //creates and initializes an empty 2-3 tree node
void clean_str(char word[]);  //converts capital letters to lowercase
int count_keys(tree_t *tree); //return the number of keys from all nodes in a tree
int get_ht(tree_t *tree); //Return the Level of a 2-3 tree
int count_nodes(tree_t *tree);  //Count how many nodes exist in a 2-3 tree
void tree_check(tree_t *tree, char word[]); //Terminal output to peek at tree properties

/* NAME: main
PARAMETERS: void, none
PURPOSE: The main function will operate files, start the 2-3 tree, then call functions to operate on tree.
PRECONDITION: No parameters, function is called when program runs.
POSTCONDITION: A 2-3 tree will be made and operated on by pre-defined functions, then searched for two words- output shown to terminal.
Files will be opened, read and written, then closed.
*/
int main(void){
    //Initialize file pointers
    FILE *infile, *outfile;
    infile =fopen("text.txt", "r");
    outfile=fopen("concordance.txt", "w");

      //Create an array of nulls to copy to for the check
    char check_word[SIZE];
    memset(check_word, '\0', sizeof(check_word));

    //Initialize root, a data_t bundle, and the bundles with frequency 1
    tree_t *root = NULL;
    data_t buffer;
    buffer.freq=1;

    //check if the file exists.
    if (infile == NULL || outfile == NULL){
      printf("ERROR: File does not exist...\n");
      exit(1);  //terminate
    }

    //Create nodes for ALL words in file and link list
    while((fscanf(infile, "%s", buffer.key)) != EOF){
      //convert the string to lowercase:
      clean_str(buffer.key);
      //create and insert the node into the tree:
      insert(&root, buffer);
    }

    //Write the 2-3 tree in-order to outfile
    print_tree(root, outfile);

    //peek tree properties and search for word 1
    strncpy(check_word, "concordance", SIZE);
    tree_check(root, check_word);

    //peek tree properties and search for word 2
    strncpy(check_word, "water", SIZE);
    tree_check(root, check_word);

    //close files
    fclose(infile);
    fclose(outfile);
    return 0;
}

/* NAME: insert
PARAMETERS: **root, val
PURPOSE: Helper function to handle inserting into a 2-3 tree and repeated data
PRECONDITION: Call when a key needs to be inserted into the 2-3 tree.
POSTCONDITION: The key will be inserted or have frequency increased at the matching key node. Data for node will be updated here
*/
void insert (tree_t **root, data_t val){
  //Initialize mid bundle, split setting, and pointers
  data_t mid;
  int split;
  tree_t *newp;
  tree_t *ptr, *temp;
  int i;
  // empty tree - create new root node and insert key
  if (*root == NULL) {
    newp = create_node();
    newp->data[0] = val;
    newp->num_keys = 1;
    *root = newp;
  }
  // non-empty tree - create a new root if a split occurred
  else {
    temp = search_23tree(*root,val.key);  //temp is a pointer so that its keys can be accessed later for repeat key case
    //if a repeat doesnt exist
    if (temp == NULL){
      split = 0;
      recur_insert(root,val,&split,&mid,&ptr);
      //if a split needed to happen, create a new node and copy the data over
      if (split){
        newp = create_node();
        newp->data[0] = mid;
        newp->tree_p[0] = *root;
        newp->tree_p[1] = ptr;
        newp->num_keys = 1;
        *root = newp;
      }
    }
    //If a repeat exists, use the node properties to pick the correct key, and increase its frequency
    else{
      for (i = 0; i < temp->num_keys && strncmp(val.key, temp->data[i].key, SIZE) > 0; i++);  //Go to the correct index
        if (i < temp->num_keys && strncmp(val.key, temp->data[i].key, SIZE)==0){  //if the values match
          temp->data[i].freq++;
        }
    }
  }
}

/* NAME: recur_insert
PARAMETERS: **root, val, *split, *mid, **newp
PURPOSE: Recursively insert a key into a 2-3 tree, update split setting and current data being handled
PRECONDITION: Call when a key needs to be inserted into the 2-3 tree at internal or leaf nodes
POSTCONDITION: The key will be inserted and splits will occur, mids propogate up, etc, as needed to satisfy 2-3 property
*/
void recur_insert (tree_t **root, data_t val, int *split, data_t *mid, tree_t **newp){
  int i,j;
  tree_t *np;
  //Insert the value at a leaf node (left pointer doesn't exist)
  if ((*root)->tree_p[0] == NULL){
    //If the new key is smaller, organize the values
    for (i = (*root)->num_keys; i > 0 && (strncmp(val.key, (*root)->data[i-1].key, SIZE)<0); i--){
      (*root)->data[i] = (*root)->data[i-1];
      (*root)->tree_p[i+1] = (*root)->tree_p[i];
    }
    //At i, copy the value in and update other values
    (*root)->data[i] = val;
    (*root)->tree_p[i+1] = NULL;
    (*root)->num_keys++;

    //If overflow, split the leaf node
    if ((*root)->num_keys == KEYS+1){
      *split = 1; //split setting becomes 1
      *newp = create_node();
      //Update new node data
      (*newp)->data[0] = (*root)->data[KEYS];
      (*newp)->num_keys = 1;
      *mid = (*root)->data[1];
      (*root)->num_keys = 1;
    }
  }
  //internal node case
  else{
    // determine which tree_p pointer to traverse
    for (i = 0; i < (*root)->num_keys && (strncmp(val.key, (*root)->data[i].key, SIZE)>0); i++);
    recur_insert(&((*root)->tree_p[i]), val, split, mid, newp);

    if (*split) {   // a split occurred on the call to recur_insert
      *split = 0;
      // insert *mid and *newp into **root
      for (i = (*root)->num_keys; i > 0 && (strncmp(mid->key,(*root)->data[i-1].key,SIZE) <0); i--){
      (*root)->data[i] = (*root)->data[i-1];
      (*root)->tree_p[i+1] = (*root)->tree_p[i];
      }
      (*root)->data[i] = *mid;
      (*root)->tree_p[i+1] = *newp;
      (*root)->num_keys++;
      //Overflow in the node
      if ((*root)->num_keys == KEYS+1) {     // split internal node
        *split = 1;
        *newp = create_node();
        //update empty node
        (*newp)->data[0] = (*root)->data[KEYS];
        (*newp)->tree_p[0] = (*root)->tree_p[2];
        (*newp)->tree_p[1] = (*root)->tree_p[3];
        (*newp)->num_keys = 1;
        *mid = (*root)->data[1];
        (*root)->num_keys = 1;
      }
    }
  }
}

/* NAME: create_node
PARAMETERS: void
PURPOSE: Create an empty tree node with null data
PRECONDITION: Call when createing an empty node to have data written into later
POSTCONDITION: A node with null data handles will be created
*/
tree_t *create_node(){
  //dynamic allocation of memory
  tree_t *newp = (tree_t *)malloc(sizeof(tree_t));
  int i;
  //Every possible value will be initialized to Null Properties
  for (i = 0; i < KEYS; i++) {
    memset(newp->data[i].key, '\0', sizeof(newp->data[i].key)); //initialize a null array
    newp->tree_p[i] = NULL; //new node has no child nodes
    newp->data[i].freq=0; //new node datas have no frequency if they don't exist
  }
  newp->tree_p[i] = NULL; //last pointer needs to be NULL
  newp->num_keys = 0; //no keys exist yet
  return(newp);
}

/* NAME: search_23tree
PARAMETERS: *root, char word[] (string)
PURPOSE: Traverse a 2-3 tree and return a poitner to a node containing the proper key
PRECONDITION: Call when verifying the existence of a key within a 2-3 tree.
POSTCONDITION: A pointer to the node will be returned if the node has a matching key, null otherwise.
*/
tree_t *search_23tree(tree_t *root, char word[]){
  tree_t *p = NULL;
  //If the root is not empty
  if (root != NULL){
    int i;
    //Find the correct index, verify if key matches word, and create a handle if it does.
    for (i = 0; i < root->num_keys && strncmp(word, root->data[i].key, SIZE) > 0; i++);
      if (i < root->num_keys && strncmp(word, root->data[i].key, SIZE)==0){  /* check for num in root->data[i] */
        p = root;
      }
      else
        p = search_23tree(root->tree_p[i],word);  // keep searching down root->tree_p[i]
  }
  return (p);
}

/* NAME: print_tree
PARAMETERS: *tree, *out (file)
PURPOSE: Formats the top line of a output file with write priviledges, then calls recursive print
PRECONDITION: Call to print a 2-3 using in order but have a format with data first.
POSTCONDITION: Out will have data written to the top before the recur_print function writes to the file.
*/
void print_tree(tree_t *tree, FILE *out){
  int total_nodes = count_keys(tree);
  fprintf(out, "There are %d unique words in the file:\n", total_nodes);
  recur_print(tree, out);
}

/* NAME: recur_print
PARAMETERS: *tree, *out (file)
PURPOSE: Recursively use an in-order traversal on a 2-3 tree to write contents in increasing order to the file.
PRECONDITION: Called from helper function to write the 2-3 tree.
POSTCONDITION: The data bundles in a 2-3 tree will be printed in ascending order based on the key.
*/
void recur_print(tree_t *tree, FILE *out){
  //Tree must not be empty
  if (tree != NULL) {
    //Traverse Left & print first data value
    recur_print(tree->tree_p[0], out);
    fprintf(out, "%s %d\n", tree->data[0].key, tree->data[0].freq);

    //Traverse the Middle Branch and print second data value if it exists
    recur_print(tree->tree_p[1], out);
    if (tree->num_keys ==2){
      fprintf(out, "%s %d\n", tree->data[1].key, tree->data[1].freq);
      //Traverse the right subtree if there was a mid tree
      recur_print(tree->tree_p[2], out);  //Only way this happens is if two keys exist.
    }
  }
}

/* NAME: clean_str
PARAMETERS: word[] (string)
PURPOSE: Convert capital letters to lowercase letters
PRECONDITION: Call before a string is copied so that capital letters don't exist.
POSTCONDITION: All capital letters would be lowercased.
*/
void clean_str(char word[]){
  int i;
  for(i=0; word[i]; i++){
    if (isupper(word[i])){  //If the word is uppercased, convert to lower
      word[i] = tolower(word[i]);
    }
  }
  word[i] = '\0'; //Set the last character to be null
}

/* NAME: count_keys
PARAMETERS: *tree
PURPOSE: count the keys in the 2-3 tree keys:=unique keys
PRECONDITION: Call when needing to show the unqiue words in the tree
POSTCONDITION: The sum of all the number of keys per node will be returned.
*/
int count_keys(tree_t *tree){
  int i, temp =0;
  //empty tree
  if (tree == NULL){
    return temp;
  }
  else{
    //traverse each subtree and add the keys total
    temp = tree->num_keys;
    for(i = 0; i<=tree->num_keys; i++){
      temp += count_keys(tree->tree_p[i]);
    }
    return temp;
  }
}

/* NAME: get_ht
PARAMETERS: *tree
PURPOSE: Get the height of a 2-3 tree using the balanced property of a 2-3 tree
PRECONDITION: Call on a 2-3 tree when its height property is desired.
POSTCONDITION: The levels of the tree will be returned. (Height)
*/
int get_ht(tree_t *tree){
  //empty tree
  if(tree==NULL){
    return 0;
  }
  //Because tree is balanced, just traverse left nodes and add 1 each time
  return(1+ get_ht(tree->tree_p[0]));
}

/* NAME: count_nodes
PARAMETERS: *tree
PURPOSE: Traverse to each node in a tree, and count it
PRECONDITION:Call when the tree size is desired
POSTCONDITION: The number of the nodes in a tree will be returned
*/
int count_nodes(tree_t *tree){
  int i, temp =0;
  //empty tree
  if (tree == NULL){
    return temp;
  }
  else{
    //Count the first node
    temp = 1;
    //For every key, there is 1 extra pointer, visit it and add 1.
    for(i = 0; i<=tree->num_keys; i++){
      temp += count_nodes(tree->tree_p[i]);
    }
    return temp;
  }
}

/* NAME: tree_check
PARAMETERS: *tree, word[] (string)
PURPOSE: Display a 2-3 tree's properties to the console as well as search for a key.
PRECONDITION: Call when wanting to peek at a tree's properties and search for a key that may be in the tree
POSTCONDITION: The tree's height and size will be logged, then the key will log if it is found or not, as well as the frequency.s
*/
void tree_check(tree_t *tree, char word[]){
  //Get a handle on the node with the matching key
  tree_t *temp=search_23tree(tree, word);
  int i;
  //Print initial checks
  printf("\nTree Check:\n");
  printf("  Tree Height: %d Levels\n", get_ht(tree));
  printf("  Tree Size: %d Nodes\n", count_nodes(tree));
  //No match was found
  if(temp == NULL){
    printf("  %s is not found\n", word);
  }
  else{
    //Traverse to the index i that may be the key, then at that index, compare if keys match. If yes, print data bundle.
    for (i = 0; i < temp->num_keys && strncmp(word, temp->data[i].key, SIZE) > 0; i++);
      if (i < temp->num_keys && strncmp(word, temp->data[i].key, SIZE)==0){
        printf("  %s found and has frequency %d\n", word, temp->data[i].freq);
      }
    }
}

/*
                                  *****Report*****
          i. Team members: None
              All functions were created, developed, and tested without
              research by myself. No external sources were used in the
              making of this program.

          ii. Test Cases and status:
              text.txt example from the assignment: passed :)
              text.txt with one word: passed :)
              text.txt with no words: passed :)
              text.txt with 500+ Words: passed :) - Taken from an essay I wrote

          iii.2-3 Search Tree Analysis
              1. The worst case for inserting n words into a 2-3 tree would be one where the nodes are half full. This increases the number of nodes
                  required to store the data of N elements. This would likely happen if the words were sorted alphabetical in ascending or descending
                  order already.

              2. Big O of the best case for inserting n words into a 2-3 search tree:
                  Big O of the best case for inserting n words into a 2-3ST would be big O(log_2(n)), this is because the search for a position for
                  insertion takes log_2(n) operations on average. The best case would occur on an unsorted file.

              3. Big O of storage requirements for a 2-3 search tree with n keys:
                  The big O of storage for a 2-3 with n keys would be O(n). Up to n nodes would need to be created as n-> infinity.(linear insertion)

              4. For the example text.txt file, the tree height is 7. What would the minimum height be in
                the best case?
                  In the best case, the tree height would be 1, there would be a single node and no child nodes.
                  **For an empty text.txt file, the height would be 0 and this would also be a best case.

*/
