#include <stdio.h>  //For printf and scanf
#include <stdlib.h> //For exit, malloc and free
#include <string.h> //For strncmp and tolower
#include <ctype.h>  //For isupper

#define SIZE 50 //Max String length is size 50, including NULL

typedef struct node{
  struct node *leftp;  //left subtree
  char word[SIZE];     //Word contained in node
  int freq;            //Word's occurence when inserting
  struct node *rightp; //right subtree
}node_t;

//tree and array functions:
  //Recursive Functions
void insert(node_t **root, char word[]);  //insert into a tree
void in_order(node_t *root, FILE *outp); //Write the concordance.txt
int count_nodes(node_t *root);  //count the nodes in the BST
int get_ht(node_t *root); //Count levels in BST
void search(node_t *root, char word[]); //Look for a node with a matching word
void del_tree(node_t *tree); //clean a tree at runtime

  //Normal Functions (May contain a recursive operation)
void clean_str(char word[]);  //lowercases string
node_t *create_node(char word[]); //create a node of tree
void build_output(node_t *root, FILE *outp); //Write the concordance.txt
void terminal_output(node_t *root, char word[]); //Print terminal output from searches

/* NAME: main
PARAMETERS: void, none
PURPOSE: The main function will operate files, start the tree, then call functions to operate on tree.
PRECONDITION: No parameters, function is called when program runs.
POSTCONDITION: A tree will be made and operated on by pre-defined functions, then searched for two words- output shown to terminal.
Files will be opened, read and written, then closed.
*/
int main(void){
//declare pointers to tree, files and strings buffer for file reads.
FILE *infile, *outfile; //file pointers to read and write text
char buffer[SIZE], check[SIZE]; //current word accessed from infile and a word to search for
node_t *root = NULL; //root pointer for tree

//open the files to be read and written.
infile = fopen("text.txt", "r");
outfile = fopen("concordance.txt", "w");

//check if the file exists.
if (infile == NULL){
  printf("ERROR: File does not exist...\n");
  exit(1);
}

//Create nodes for ALL words in file and link list
while((fscanf(infile, "%s", buffer)) != EOF){
  //convert the string to lowercase:
  clean_str(buffer);
  //create and insert the node into the tree:
  insert(&root, buffer);
}

//Write the concordance file
build_output(root, outfile);

//Search for a word that is there and output to terminal
memset(check, '\0', sizeof(check)); //makes the array all null characters
strncpy(check, "concordance", SIZE);  //copy the word concorance into check string
terminal_output(root, check);

//Search for a word that is not there and output to terminal
strncpy(check, "water", SIZE);
terminal_output(root, check);
//notify user runtime is finished.
printf("File successfully written.\n");

//Free the memory the tree occupies
del_tree(root);

//Close files
fclose(infile);
fclose(outfile);
return 0;
}

/* NAME: clean_str
PARAMETERS: word[], string
PURPOSE: lowercases all the letters of a string
PRECONDITION: A string is passed, call when there may be some capitalized letters in a word.
POSTCONDITION: Every capitalized character in the array will be lowercased at the same index
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

/* NAME: create_node
PARAMETERS: word[], string
PURPOSE: Allocates memory for a node_t variable with initial data
PRECONDITION: String is passed, call when creating a node for a tree
POSTCONDITION: A node_t node is created with NULL left and right pointers, freq 1, and word data as the string passed
*/
node_t *create_node(char word[]){
  //create a pointer to a dynamically allocated new node
  node_t *newp = (node_t *) malloc(sizeof(node_t));
  //Assign this new node's L and R subtrees to be NULL, and freq to be 1
  newp->leftp = NULL;
  newp->rightp = NULL;
  newp->freq = 1;
  //Now copy the word into the word member
  strncpy(newp->word, word, SIZE);
  //set the last char to null
  newp->word[SIZE] = '\0';
  return (newp);
}

/* NAME: insert
PARAMETERS: a node_t pointer to a pointer (address to a node_t pointer), and a string
PURPOSE: creates and inserts a node_t node into a tree data structure
PRECONDITION: Call when there is data to be inserted into a tree.
POSTCONDITION: A node will be properly inserted into a Binary Search Tree.
*/
void insert(node_t **root, char word[]){
  node_t *newp;

  //Case 1: Root is empty
  if(*root == NULL){
    *root = create_node(word);
  }
  //Case 2: Root not empty, word is the same
  else if(strncmp(word, (*root)->word, SIZE) == 0){
    (*root)->freq +=1;
  }
  //Case 3: Root not empty, word is less than root
  else if(strncmp(word, (*root)->word, SIZE) < 0){
    insert(&((*root)->leftp), word);
  }
  //Case 4: Root not empty, word is greater than root
  else{
    insert(&((*root)->rightp), word);
  }

  /*
  Only need to recurse through L and R subtrees because the 1st two
  conditions check if a node is created or frequency is added.
  */
}

/* NAME: in_order
PARAMETERS: root (of a tree), and a file to write to.
PURPOSE: use an in-order traversal method to print nodes in order to an output file
PRECONDITION: A Binary search tree is passed and a file in write mode is passed. Call when a file needs the BST printed in order by value.
POSTCONDITION: The output file will have an alphabetical output with frequencies, even though the tree is not linearly alphabetical
*/
void in_order(node_t *root, FILE *outp){
  /*
  To write the concordance and use alphabetical, I will need to use
  in-order traversal algorithm
  */
  if(root != NULL){
    in_order(root->leftp, outp);
    fprintf(outp, "%s %d\n", root->word, root->freq);
    in_order(root->rightp, outp);
  }
  //If the root is null, then the function will do nothing.
}

/* NAME: count_nodes
PARAMETERS: root, a tree
PURPOSE: visit each node in the tree once and count the node
PRECONDITION: A root must be established, call when the size of the tree is needed
POSTCONDITION: The number of nodes in the tree is returned
*/
int count_nodes(node_t *root){
  //Empty tree
  if (root==NULL){
    return 0;
  }
  //Count left subtree and right subtree, adds 1 each node until 0. Adds 1 at end for root.
  else{
    return(count_nodes(root->leftp)+ count_nodes(root->rightp)+1);
  }
}

/* NAME: build_output
PARAMETERS: root (a tree), and an output file
PURPOSE: properly formats a file by avoiding a recursion call on first line prints
PRECONDITION: root is a tree, and outp is a file with write priviledge. Call when there is a need
to have text before the recursive function in_order.
POSTCONDITION: Lines are written to the output file once and in_order is called.
*/
void build_output(node_t *root, FILE *outp){
  //get count
  int total_nodes = count_nodes(root);
  //print first line
  fprintf(outp, "There are %d unique words in the concordance file:\n", total_nodes);
  //print alphabetical tree
  in_order(root, outp);
}

/* NAME: get_ht
PARAMETERS: root, a tree
PURPOSE: Return the height of a tree
PRECONDITION: A tree must exist, call when the height of the tree is needed.
POSTCONDITION: The levels of the tree will be returned.
*/
int get_ht(node_t *root){
  int Lht, Rht; //Left and right heights
  //Case 1: Tree empty
  if(root ==NULL){
    return 0;
  }
  //Case 2: Tree has nodes
  else{
    Lht = get_ht(root->leftp);
    Rht = get_ht(root->rightp);
    if (Lht > Rht){
      return (Lht +1);
    }
    else{
      return (Rht +1);
    }
  }
}

/* NAME: search
PARAMETERS: root(a tree), and word(a string)
PURPOSE: Search recursively through a tree for a node with the matching word
PRECONDITION: root is an existing tree or subtree, and word is a string to search for.
POSTCONDITION: The terminal will display if the node with a matching word is found and its frequency
*/
void search(node_t *root, char word[]){
  //Case 1: Root is empty
  if(root == NULL){
    printf("%s is not found.\n", word);
  }
  //Case 2: root contains word:
  else if(strncmp(root->word, word, SIZE) == 0){
    printf("%s is found and has frequency %d\n", word, root->freq);
  }
  //Case 3: word is bigger than the root
  else if (strncmp(word, root->word, SIZE) > 0) {
    search(root->rightp, word);
  }
  //Case 4: word is less than the root
  else{
    search(root->leftp, word);
  }
}

/* NAME: terminal_output
PARAMETERS: root, a tree, and word, a string
PURPOSE: properly format a terminal output by printing single lines of text, then calling a recursive function
PRECONDITION: A word to search for is passed and a root to a tree is passed. Call when displaying info for the user to see
POSTCONDITION: A tree check will be done to show tree height, node count, and if the searched word was found in console.
*/
void terminal_output(node_t *root, char word[]){
  //initialize numeric variables
  int total_nodes = count_nodes(root), ht = get_ht(root);
  //print non-recursive information
  printf("Tree Check:\n");
  printf("  Tree Height:  %d\n", ht);
  printf("  Tree Size  : %d nodes\n", total_nodes);
  //print using recursive search
  search(root, word);
  printf("\n");
}

/* NAME: del_tree
PARAMETERS: root, a tree
PURPOSE: Delete a tree and return memory to the system
PRECONDITION: Tree shoudln't be null, call when freeing up the memory containing a tree
POSTCONDITION: The tree will be cleaned and set to null.
*/
void del_tree(node_t *tree){
  if (tree != NULL){
    del_tree(tree->leftp);
    del_tree(tree->rightp);
    free(tree);
    tree = NULL; //avoid memory leak.
  }
  //process right and left subtree, free nodes using postorder
}

/*
        *****REPORT*****
    i. Team members: None
        All functions were created, developed, and tested without
        research by myself. No external sources were used in the
        making of this program.

    ii. Test Cases and status:
        text.txt example from the assignment: passed :)
        text.txt with one word: passed :)
        text.txt with no words: passed :)
        text.txt with 500+ Words: passed :) - Taken from an essay I wrote

    iii.Binary Search Tree Analysis
        1. Example of best-case for inserting n words into a BST:
            All the words are the same-> The operations would be to check if the word exists, it does, increase frequency.
            Storage would also be O(1) because only one node_t variable and pointer would be needed.
            **If the words are different, the best case would be words that fill the tree in order at each level (left subtree then right).

        2. Big O of the best case for inserting n words into BST:
            Big O of the best case for inserting n words into a BST would be big O(1), because the operation would just be increasing the frequency
            because for every item, you have to at least increase the frequency of the item.
            **Otherwise, the big O would be O(log(n)) because the insert operation is not linear,
            but is shortened by traversing left and right subtrees. And for every level, log_2(nodes) are traversed.

        3. Big O of storage requirements for a BST size n:
            The big O of storage for a BST of size n would be O(n). Up to n nodes would need to be created as n-> infinity.

        4. For the example text.txt file, the tree height is 7. What would the minimum height be in
          the best case?
            In the best case, the tree height would be 1, there would be a single node and no child nodes.
            **For an empty text.txt file, the height would be 0 and this would also be a good case.
*/
