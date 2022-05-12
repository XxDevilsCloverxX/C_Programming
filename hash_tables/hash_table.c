#include <stdio.h>  //for inputs and outputs
#include <ctype.h>  //for cleaning words
#include <string.h> //for strncmp
#include <stdlib.h> //for calloc

#define TABLE_SIZE 1024 //prime number table size for hash function
#define STR_SIZE 35 //Max character length of a string
#define FILE_COUNT 12 //n files to check
#define SEARCH_QUERY "query.txt"

/*
AUTHOR: Silas Rodriguez
FILENAME: problem1.c
SPECIFICATION: Practice using hash tables
FOR: CS 2413 Data Structures Section 001
*/

//document id linked list
typedef struct docs{
  unsigned long int id; //id contained in node
  struct docs *next;  //self referential for doc ids
}doc_id;

//hash table nodes
typedef struct data_t{
  char *str; //word contained in beginning node
  doc_id *head; //linked list of associated document numbers
}data;

//array type for searching
typedef struct array_bundles{
  unsigned long int doc_id; //document id number
  unsigned long int terms;  //how many terms found in doc
}search_data;

unsigned long int hash(char *string);  // a hash function that converts a given string into a index
unsigned long int linear_probe_hash(char *string, unsigned long int i);  //linear probe collision resolution
void clean_word(char string[]); //cleans a string from punctuation and capitals
data *create_node(char *string, unsigned long int file_number); //create a bundle of data from a given word
doc_id *create_list(unsigned long int file_number); //create a linked list of bundles of file_number data
void insert(char *string, unsigned long int file_number, data* hash_table[], unsigned long int *collisions); //insert values into a hash table
void print_table(data *hash_table[]); //reveal the hash table
void input_query(char *query, data *hash_table[]); // take a file and search a hash table for keys
unsigned long int linear_search(search_data *doc_list, int doc_num,int size); //out of a small table of size FILE SIZE, find a matching doc id
void print_search(search_data *doc_list, unsigned long int size); //print the results of the search


/*
NAME: main
PARAMETERS: void, none
PURPOSE: The function opens and operates on files, and calls hash table functions to operate on struct arrays
PRECONDITION: No parameters, function is called when program runs.
POSTCONDITION: A hash table is built using document contents. Searched for a query using the macro above.
*/
int main(void){
  FILE* infile;
  char file_name[STR_SIZE]; //for opening multiple files
  char buffer[STR_SIZE];  //for storing strings to be cleaned
  data *table[TABLE_SIZE] = {0};  //initialize an array of null pointers
  unsigned long int collides = 0;
  infile = fopen("document1.txt", "r");
  //working on 1 file
  int i=0;

  //Build the hash table:
  for(i=1; i<=FILE_COUNT; i++){
    //create a file name
    sprintf(file_name, "document%d.txt", i);

    //open file with appropriate name
    infile= fopen(file_name, "r");
    //check if file opened correctly, if not, exit with error and files will automatically close
    if (infile == NULL){
      printf("INFILE ERROR\n");
      exit(1);
    }
    //insert file contents to hash table
    while(fscanf(infile, "%s", buffer)> 0){
      clean_word(buffer); //clean the word of upper and punct
      //printf("%s\n", buffer); //uncomment for viewing words
      insert(buffer, i, table, &collides);
    }
    //close the current file
    fclose(infile);
  }
  printf("There were %ld collisions.\n", collides);
  //uncomment to print the hash table
  //print_table(table);

  //perform a look up in the hash table
  FILE *search = fopen(SEARCH_QUERY, "r");  //open the query file
  while(fgets(buffer, STR_SIZE, search)){
    printf("Processing Query: %s", buffer);
    //the buffer has been generated, now perform a search
    input_query(buffer, table);
  }
  //close the file
  fclose(search);
  return 0;
}

/*
NAME: hash
PARAMETERS: char *string
PURPOSE: Gernerate a key using an algorithm that operates on the string
PRECONDITION: clean strings contain an arr of chars, call when hashing to an index of an array
POSTCONDITION: A hash key will generate for a given word, while be deterministic.
*/
unsigned long int hash(char *string){
  unsigned long int h = 0, i=0;
  //itterate through all characters in string
  while (string[i] != '\0'){
    //type cast the character to its ascii value
    h = h*97+(int)string[i];
    i++;
  }
  h = h % TABLE_SIZE;
  return h;
}
/*
NAME: linear_probe_hash
PARAMETERS: char *string, unsigned long int i
PURPOSE: Linearly move through an array to generate a new key one position over
PRECONDITION: Call when a new hash key is needed due to collisions
POSTCONDITION: A new hash key will be generated that is independent of the first
*/
unsigned long int linear_probe_hash(char *string, unsigned long int i){
  unsigned long int h_prime = (hash(string) + i ) % TABLE_SIZE;
  return (h_prime);
}
/*
NAME: clean_word
PARAMETERS: char string[]
PURPOSE: remove any non-lowercase alphabetic characters from a string
PRECONDITION: any string can be passed to be cleaned
POSTCONDITION: string will only contain lowercase characters
*/
void clean_word(char string[]){
  //create a destination string
  char *src=string, *dest=string;
  int len = strlen(src), i;
  //itterate through all characters
  while(*src){
    //skip copy if punct or not a letter range ASCII
    if(ispunct(*src)|| (int)*src <65 || (int)*src > 122){
      src++;
    }
    //capital ascii
    else if(isupper((unsigned char)*src))
       {
          /* Make it lowercase */
          *dest++ = tolower((unsigned char)*src);
          src++;
       }
       else if (src == dest){
          /* Increment both pointers without copying */
          src++;
          dest++;
       }
       else{
          /* Copy character */
          *dest++ = *src++;
       }
    }
    *dest = 0;
}
/*
NAME: create_node
PARAMETERS: string, file_number
PURPOSE: creates a bundle of data that contains a base key and a linked list of file numbers the key belongs to
PRECONDITION: string is cleaned, and file number is the 1st occurence of the word
POSTCONDITION: return a pointer to this base node
*/
data *create_node(char *string, unsigned long int file_number){
  //dynamic allocation of node
  data *newp = (data *) malloc(sizeof(data));
  if(newp == NULL){
    printf("Failed to allocate");
    exit(1);
  }
  char *reset = string; //points to an array that is duped
  reset = strndup(string, STR_SIZE);
  newp->str = reset;
  //create a linked list of file nums
  newp->head = create_list(file_number);  //initial pointer created from passed file_number
  return newp;
}
/*
NAME: create_list
PARAMETERS: unsigned long int file_number
PURPOSE: creates a linked list of document ids
PRECONDITION: Call on first creation of a node using create node, but otherwise call seperately
POSTCONDITION: a pointer to the LL node is returned with specified data
*/
doc_id *create_list(unsigned long int file_number){
  //dynamic allocation of node
  doc_id *newp = (doc_id *)malloc(sizeof(doc_id));
  newp->id = file_number; //id gets the file number
  newp->next = NULL;
  return(newp);
}
/*
NAME: insert
PARAMETERS: string, file_number, data* hash_table[], *collisions
PURPOSE: Insert information into a hash table while tracking collisions that occur
PRECONDITION: Call when building the hash table from files.
POSTCONDITION: a hash table will be constructed using the input data
*/
void insert(char *string, unsigned long int file_number, data* hash_table[], unsigned long int *collisions){
  data *bundle = NULL;  //initial bundle pointer for nodes
  doc_id *cur = NULL, *rear = NULL; //inital pointers for file numbers LL
  //generate hash key:
  unsigned long int key = hash(string);
  //case 1: hash table at index is empty
  if(hash_table[key]==NULL){
    //create a node to fill:
    bundle = create_node(string, file_number);
    hash_table[key] = bundle; //first point of hash table recieves the bundle
  }
  //case 2: string hashed to this index, string is equivalent to existing value
  else if(strncmp(hash_table[key]->str, string,STR_SIZE) == 0){
    cur = hash_table[key]->head;  //get the head of the pointer
    //find a point where cur meets the end or file numbers match
    while(cur->next != NULL && cur->id != file_number){
      cur = cur->next;
    }
    //only append if the last id wasn't also matching file number
    if(cur->next == NULL && cur->id != file_number){
      rear = create_list(file_number);  //create a new node
      cur->next = rear; //append to the list
    }
  }

  //case 3: a string hashed to this value, but the keys are not equivalent:
  else{
    (*collisions)++;  //track collisions, collisions in this case refer to hashing to a value, not linear probing collisions
    unsigned long int j = 0;  //beginning of adder for linear probe
    //find a point linearly that is not occupied, or another matching set:
    while(hash_table[key]!= NULL && strncmp(hash_table[key]->str, string, STR_SIZE)){
      key = linear_probe_hash(string, j); //new key generated
      j++;
    }
    //printf("cluster %ld\n", j); //cluster size
    //Sub case 1: the slot is empty:
    if(hash_table[key] == NULL){
      bundle = create_node(string, file_number);  //create a node to store into table
      hash_table[key] = bundle; //first point of hash table recieves the bundle
    }
    else{ //subcase 2: a matching string was found
      cur = hash_table[key]->head;  //get the head of the pointer
      //find a point where cur meets the end or file numbers match
      while(cur->next != NULL && cur->id != file_number){
        cur = cur->next;
      }
      //only append if the last id wasn't also matching file number
      if(cur->next == NULL && cur->id != file_number){
        rear = create_list(file_number);  //create a new node
        cur->next = rear; //append to the list
      }
    }
  }
}
/*
NAME: print_table
PARAMETERS: data *hash_table[]
PURPOSE: Print the contents of the hash table
PRECONDITION: Hash table must already be built. _Helpful debug tool
POSTCONDITION: Hash table will be linearly printed.
*/
void print_table(data *hash_table[]){
  printf("Hash Table:\n");
  doc_id *cur= NULL;
  int i=0;
  for(i=0;i<TABLE_SIZE;i++){
    if(hash_table[i] !=NULL){
      cur=hash_table[i]->head;  //get first pointer
      printf("%s: ", hash_table[i]->str);
      while(cur!=NULL){
          printf("-> (Doc %ld)", cur->id);
          cur = cur->next;
      }
      putchar('\n');
    }
  }

}
/*
NAME: input_query
PARAMETERS: query string, data * hash_table[]
PURPOSE: search a hash table for a set of terms
PRECONDITION: query is a clean set of words, hash table is built.
POSTCONDITION: the hash table will be searched, and the resulting docs will stored in an array
*/
void input_query(char *query, data *hash_table[]){
  //multi-word strings are passed into query and need to be broken to tokens:
  char delim[2] = {" "};  //how to seperate tokens
  char *token = strtok(query, delim); //get first token
  unsigned long int key=0, begin = 0, search_index =0;  //key for hash, begin to prevent inf loop(if somethign doesnt exist), search index for linear file search
  int j = 1;
  unsigned long int finds=0; //finds is size of the list array
  search_data list[FILE_COUNT]; //an array for all the files a search query pulls up
  doc_id *cur =NULL;  //for adding all finds into a list

  //initialize terms counts to 0
  int i=0;
  for(i=0; i<FILE_COUNT; i++){
    list[i].terms = 0;
  }

  //prepped at points for edits >>>>>>>>START HERE<<<<<<<<<<<<
  while(token != NULL){
    //clean the token
    clean_word(token);
    //generate a hash key for the token:
    key = hash(token);
    begin = key;  //to handle non-existent values in table
    //check if hash index in table is empty:
    if(hash_table[key]!= NULL){
      //check if intial value is a matching value -> done
      if(strncmp(token, hash_table[key]->str, STR_SIZE) == 0){
        //A matching base node was found, use cur loop to add all docs to the array:
        cur = hash_table[key]->head;
        while(cur!= NULL){
          //Find a matching document # in list, if any, increase the document's found quantity (a new one may be created)
          search_index = linear_search(list, cur->id, finds);
          //case 1: position was found (previous node created):
          if(search_index != -1){
            list[search_index].terms++; //increment the found terms in the list by 1
            //printf("**%ld\n", list[search_index].terms);
          }
          //case 2: no point was found (need to create the node)
          else{
            list[finds].terms++;  //add the first found term
            list[finds].doc_id = cur->id; //give the list node its id
            finds++; //if a new node was created, we have finds documents that have a match
          }
          cur = cur->next;  //go to next node
        }
      }
      // if the inital match is not correct, linear probe until a full loop is made or the value is found
      else{
        key = linear_probe_hash(token, j);  //generate a new key
        j++;
        while(key!=begin&&hash_table[key]!=NULL){
          //only perform a check if the value exists
          if(hash_table[key]!= NULL){
            if (strncmp(token, hash_table[key]->str, STR_SIZE) == 0){
              //A matching base node was found, use cur loop to add all docs to the array:
              cur = hash_table[key]->head;
              while(cur!= NULL){
                //Find a matching document # in list, if any, increase the document's found quantity (a new one may be created)
                search_index = linear_search(list, cur->id, finds);
                //case 1: position was found (previous node created):
                if(search_index != -1){
                  list[search_index].terms++; //increment the found terms in the list by 1
                }
                //case 2: no point was found (need to create the node)
                else{
                  list[finds].terms++;  //add the first found term
                  list[finds].doc_id = cur->id; //give the list node its id
                  finds++; //if a new node was created, we have finds documents that have a match
                }
                cur = cur->next;  //go to next node
              }
              break;  // a match was found, so break early without incrementing
            }
          }
          key = linear_probe_hash(token, j);  //generate a new key
          j++;
        }
        //value not found otherwise
      }
    }
    token = strtok(NULL, delim);  //get the next token
  }
  //after all tokens are processed, print the results
  print_search(list, finds);
}

/*
NAME: linear_search
PARAMETERS: search_data *doc_list, doc_num, size
PURPOSE: search an array within input query to find a matching doc number
PRECONDITION: Only call within input query or wherever a specific array mentioned occurs. doc_num is number to match
POSTCONDITION: return position of matching doc _id, -1 otherwise
*/
unsigned long int linear_search(search_data *doc_list, int doc_num,int size){
  unsigned long int i;
  for(i=0;i<size;i++){
    if(doc_list[i].doc_id == doc_num){
      return i; //match is found and the proper index is returned
    }
  }
  return -1; //failure to find match
}

/*
NAME: print_search
PARAMETERS: search_data *doc_list, size
PURPOSE: print the results of the search performed on a query- showing which documents contain words from the query
PRECONDITION: doc_list is completed at the time of call, size is the size of doc_list
POSTCONDITION: the results of a queried search call will be displayed to an output
*/
void print_search(search_data *doc_list, unsigned long int size){
  int i;
  char *terms=NULL;
  printf(" %3ld Document(s)\n", size);
  if(size == 0){
    printf("%4cNo documents contain the term\n", ' ');
  }
  for(i=0;i<size;i++){
    if(doc_list[i].terms == 1){
      terms = "term";
    }
    else{
      terms = "terms";
    }
    printf("    Document %3ld -%2c%ld %s\n", doc_list[i].doc_id, ' ',doc_list[i].terms, terms);
  }
  putchar('\n'); //spacing
}

/*
                  ***   REPORT    ***
          i. Team members: None
                All functions were created, developed, and tested with research
                by myself. One external source was used in the
                making of this program: https://stackoverflow.com/questions/1841758/how-to-remove-punctuation-from-a-string-in-c

          ii. Test Cases and status:
                    1. 1 document excerpt file and hash table - passed  :)
                    2. 2 to 12 document excerpt files and hash table – passed :)
                    3. searching for words in the hash table – passed :)
                    4. searching for words not in the hash table – passed :)
                    5. searching for words in and not in the hash table – passed :)

          iii. Hashing Analysis
              1. 1024 collisions: 326 collisions not counting linear probing collisions
                 2048 collisions: 170 collisions not counting linear probing collisions

              2. 1031 collisions: 363 collisions not counting linear probing collisions
                 2039 collisions: 236 collisions not counting linear probing collisions.

              3. I chose 1024 because the collisions were a lot less than primes and
               still balanced for space (half of 2048).

              4. I went with the one from the assignment so that the collisions and keys
              would be relatively close / exact with what would be checked. But if I wrote my
              own hash function, I would have chosen to add all the ascii values in the string,
              divide by my favorite number (8), then mod TABLE_SIZE. No particular reason, just like that.

              5. The largest primary cluster I had from this hash table was 26.

              6. m files, n words in each file, q queries.
                 Hashing keys: O(1) per word, O(mxn) for n words in m files
                 Linear Probing: O(n) per word, but m times for files: O(mxn) ->worst case
                 Building the hash table total: O(mxn) because hashing keys is fast, but linear probing is slower.

                 Hashing keys: O(1) per word in query. O(qxn) for n words in q queries.
                 linear probing (during the hash table search): O(qxn) -> for q queries there could be up to n words linear probing
                 linear searching (finding matching file ids) total: O(qxn). For q files, n words must be searched. Hashing is fast step,
                 but it repeats for n words of q queries.

                 so total, I am getting O(n*(m+q)).

              7. Big O_storage is:
                  hash_table construction: O(mxn), m files of n words. mxn = total spaces needed.

                  hash_table searching: O(q+m) - One array is used equivalent in size to the numeber of file
                  constructing the hash table. And a query string, q is used to perform this search

*/
