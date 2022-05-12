#include <stdio.h>  //ins and outs
#include <stdlib.h> //malloc
#define FILENAME "graph.txt"  //filename to be scanned
#define DEBUG 0 //prints debug statements

//data of edge connecting two vertices
typedef struct edge_t{
  char to;  //label of vertex edge connects to
  unsigned int weight;  //given in file
  struct edge_t* next;  //edge for linked list
}edge;

//vertex in graph
typedef struct node_t{
  unsigned int status: 2; //assigned 2 bit memory because only values 0-3 exist for status
  char vertex;  //label of node
  edge *head; //linked list head of edges
  edge *rear; //linked list rear of edges
}node;

node* create_node(char vertex); //creates a node for array
edge* create_edge(char vertex, unsigned int weight);  //creates an edge variable
unsigned int find_base(node* adj_list[], char base, unsigned int size); //finds a matching base node
void insert_edge(edge**head, edge**rear, char vertex, unsigned int weight); //inserts edges into linked lists
void print_graph(node* list[], unsigned int size);  //prints a graph (adjacency list)
void destroy_graph(node* arr[], unsigned int size); //frees graph memory
void enqueue(node* queue[], node* vertex, unsigned int size, int *rear);  //queues items
node* dequeue(node* queue[], int *front, int *rear);  //removes items from queues
void breadth_first_search(node* adj_list[], node* start, unsigned int size);  //traverses all nodes using a queue

/* NAME: main
PARAMETERS: void, none
PURPOSE: The main function will call file specified in constant, and operate on graphs
PRECONDITION: No parameters, function is called when program runs.
POSTCONDITION: An adjacency list will be created, printed, traversed via BFS and DFS, and freed
*/
int main(void){
  FILE *infile = fopen(FILENAME, "r");  //open the file to construct graph from
  if(infile==NULL){
    printf("File error");
    exit(1);
  }
  //Get attributes from graph.txt file- manual verification of graph.txt file required
  unsigned int num_vertices, is_directed;
  fscanf(infile, "%d", &num_vertices);
  const unsigned int vertex_count = num_vertices; //this had to be declared because for some reason, num_vertices kept overwritting to 0 in my loop
  if(DEBUG){
    printf("Num Vertices: %d\n", num_vertices);
  }
  fscanf(infile, "%d", &is_directed);
  if(DEBUG){
    printf("Directed: %d\n", is_directed);
  }

  //Create an array of nodes that contain label, status, and edges
  int i;
  node* base, *adj_list[vertex_count];
  char point;
  for(i=0;i<vertex_count;i++){
    fscanf(infile, "%s", &point);
    base = create_node(point);
    adj_list[i] = base;
    if (DEBUG){
      printf("adj_list[i=%d]: %c\n",i, adj_list[i]->vertex);
    }
  }
  //extract edge data and construct the linked lists
  char from, to;  //labels
  unsigned int wt, index;  //weight from file
  //Until the file reaches EOF, we scan in the ordered pairs with format from, to, weight.
  while(fscanf(infile,"%s%s%d", &from, &to, &wt) != EOF){
    //find which index to insert edge
    index = find_base(adj_list, from, vertex_count);
    insert_edge(&(adj_list[index]->head), &(adj_list[index]->rear), to, wt);
    if(is_directed==0){
      /*
      Condition that the graph is undirected and needs the node created for inverted values
      find the point of the other letter, edge exists from above
      */
      index = find_base(adj_list, to, vertex_count);
      insert_edge(&(adj_list[index]->head), &(adj_list[index]->rear), from, wt);
    }
  }
  //Print the graph to console
  print_graph(adj_list, vertex_count);
  //Perform a breadth first search from node a
  breadth_first_search(adj_list, adj_list[0], vertex_count);  //the index determines starting node.
  //reset node status:
  for(i=0; i<vertex_count;i++){
    adj_list[i]->status = 0;
  }
  //run a new search from a different point: This one is c
  breadth_first_search(adj_list, adj_list[2], vertex_count);  //the index determines starting node.
  //free the memory when graph use is done
  destroy_graph(adj_list, vertex_count);
  fclose(infile);
  return 0;
}

/* NAME: create_node
PARAMETERS: char vertex
PURPOSE: Creates nodes with vertex labels and initialize edge linked lists
PRECONDITION: Call upon first creation of a node you intend to be unique.
POSTCONDITION: A node will be initialized with no edges, 'ready' status, and a label.
*/
node *create_node(char vertex){
  //dynamic allocation of node
  node *newp = (node *)malloc(sizeof(node));
  newp ->vertex = vertex;
  newp ->head = NULL; //new node does not point anywhere
  newp ->rear = NULL;
  newp-> status = 0;  //inital "ready" status
  return(newp);
}

/* NAME: create_edge
PARAMETERS: char vertex, unsigned int weight
PURPOSE: Create edge nodes (bundles) of data that hold initial weights
PRECONDITION: Edge bundles are created where vertex is the 'to' node, no 'from' is called since they are associated through the adjacency array.
POSTCONDITION: a bundle of data that is generated for an edge between two nodes.
*/
edge *create_edge(char vertex, unsigned int weight){
  //dynamic allocation of node
  edge *newp = (edge *)malloc(sizeof(edge));
  newp->to = vertex;  //this is the 'to' vertex, not from.
  newp ->next = NULL; //self referential for linked list representations
  newp-> weight = weight; //from file, pre specified.
  return(newp);
}

/* NAME: find_base
PARAMETERS: node* adj_list[], char base, unsigned int size
PURPOSE: return the index where base occurs, if it does, in the adjacency list of base nodes.
PRECONDITION: Call when accessing the indecies of the adjacency list or processing in traversals.
POSTCONDITION: The index of the adjacency list will be returned to the call.
*/
unsigned int find_base(node* adj_list[], char base, unsigned int size){
  unsigned int i=0;
  //search for i where chars match
  while(adj_list[i]->vertex != base && i<size){
    i++;
  }
  //i exceeded size
  if (i>=size){
    printf("Error with graph.txt, please edit and rerun\n");
    exit(1);
  }
  return i;
}

/* NAME: insert_edge
PARAMETERS: edge**head, edge**rear, char vertex, unsigned int weight
PURPOSE: insert edge nodes into a linked list contained in the adjacency list
PRECONDITION: Call on the proper head pointer that is contained in the adjacency list.
POSTCONDITION: The node from the adjacency list will have the edge node inserted to the rear
*/
void insert_edge(edge**head, edge**rear, char vertex, unsigned int weight){
  //create a new node
  edge *newp = create_edge(vertex, weight); //create a node with string data, initialize as next is NULL
  //Case: Insert at Beginning
  if(*head == NULL){
    *head = *rear = newp;  //head and rear will point to the first node.
  }
  //Case: Not first node in list
  else{
    //Insertion happens at the end, so rear-> next will become newp, then rear will point to newp.
    (*rear)->next = newp;
    (*rear) = (*rear)->next;
  }
  //Head is never overwritten without it being passed as null from glist
}

/* NAME: print_graph
PARAMETERS: node* list[], unsigned int size
PURPOSE: Print the graph in a "node" or "bubbled" representation of nodes to their edges
PRECONDITION: Call when there is a need to view connections of a node and other vertices on a pre-made graph.
POSTCONDITION: The graph will be printed to terminal in the form Vertex: "('to node', weight)->" until null point
*/
void print_graph(node* list[], unsigned int size){
  //using linked list print method:
  int i=0;
  edge* cur;
  printf("Adjacency list:\n Number of vertices: %d\n", size);
  //Iterate for each item in the adjacency list
  for(i=0; i<size; i++){
    printf("%c:", list[i]->vertex);
    cur = list[i]->head;
    while(cur!= NULL){
      printf("(%c,%d)->", cur->to, cur->weight);
      cur = cur->next;
    }
    //print cur's null state to console too.s
    printf("%p", cur);
    putchar('\n');
  }
}

/* NAME: destroy_graph
PARAMETERS: node* arr[], unsigned int size
PURPOSE: free a dynamically allocated graph entirely
PRECONDITION: A graph must exist with nodes to terminate
POSTCONDITION: The graph data will be deleted, memory freed to the operating system
*/
void destroy_graph(node* arr[], unsigned int size){
  //free linked lists in array of nodes
  int i=0;
  edge*cur, *temp;  //handles
  //iterate for each vertex in the adj list
  for(i=0; i<size; i++){
    //only attempt if the edge's head is not empty
    if(arr[i]->head != NULL){
      cur = arr[i]->head;
      while(cur!= NULL){
        temp = cur->next;
        free(cur);
        cur = temp;
      }
      //prevent a leak
      free(temp);
      temp =NULL;
      cur = NULL;
    }
    else{
      if(DEBUG){
      printf(" arr[%d].head was null\n", i);
      }
      continue;
    }
}
  //free the array of nodes:
  node* slot;
  for(i=0; i<size; i++){
    slot = arr[i];
    free(slot);
    arr[i] = NULL;
  }
  slot = NULL;
  printf("\nThe graph has been obliterated.\n");
}

/* NAME: enqueue
PARAMETERS: node* queue[], node* vertex, unsigned int size, int *rear
PURPOSE: queue items into the rear of a queue
PRECONDITION: queue must have a size that prevents overflow, call to store nodes that weren't processed yet
POSTCONDITION: Nodes that weren't previously processed are added to a queue
*/
void enqueue(node* queue[], node* vertex, unsigned int size, int *rear){
  //case that queue is full, +1 because rear is offset by one on init enqueue of first node
  if(*rear >= size+1){
    printf("%d rear %d size", *rear, size);
    printf("Queue Overflow\n");
    exit(1);
  }
  if (vertex->status==0){
    vertex->status++;  //Waiting status of nodes
    //queue is inserted at the end if not full
    queue[*rear] = vertex;
    (*rear)++;  //increment the value of rear
    if (DEBUG){
      printf("Cur rear: %d\n", *rear);
    }
  }
}

/* NAME: dequeue
PARAMETERS: node* queue[], int* front, int *rear
PURPOSE: pop elements off the front of a queue for processing
PRECONDITION: queue front != rear, call to process an element from the queue
POSTCONDITION: a pointer to the first node in the queue will be returned.
*/
node* dequeue(node* queue[], int *front, int *rear){
  if(*front == *rear){
    printf("Queue underflow");
    exit(1);
  }
  node* var = queue[*front];
  var->status++;  //processed status of nodes
  (*front)++;
  return var;
}

/* NAME: breadth_first_search
PARAMETERS: node* adj_list[], node* start, unsigned int size
PURPOSE: perform a breadth first search over an adjacency list (graph) from a starting point to see how the graph is connected.
PRECONDITION: A graph must be fully established into the adjacency list. Call to see relationship from a starting node to other nodes.
POSTCONDITION: Nodes are processed via queues, revealing relationships regarding connections and what nodes are interconnected.
*/

void breadth_first_search(node* adj_list[], node* start, unsigned int size){
  printf("\nBreadth-First Search Results:\n");
  node* queue[size], *temp;
  node* origins[size-1];
  edge* cur;  //to queue neighbors from edges
  int front=0, rear=0;
  unsigned int index;
  enqueue(queue, start, size, &rear); //queue first node
  //While queue is not empty
  while(front!=rear){
    //get front of queue
    temp = dequeue(queue, &front, &rear);
    printf("%c ", temp->vertex);
    cur = temp->head;
    while(cur!= NULL){
      //find index edge matches node & queue its node
      index = find_base(adj_list, cur->to, size);
      if (DEBUG){
        printf(" *Index: %d ", index);
      }
      origins[rear-1]=temp;
      enqueue(queue, adj_list[index], size,&rear);
      cur = cur->next;  //next node
    }
  }
  //New line for the origin queue
  putchar('\n');
  putchar(' '); //indent
  //print the origin list
  int i;
  //use rear-1 because the queue may not necessarily be size-1 in directed graphs.
  for(i=0; i<rear-1;i++){
    printf(" %c", origins[i]->vertex);
  }
}


/*
                  ***   REPORT    ***
          i. Team members: None
                All functions were created, developed, and tested without
                research by myself. No external sources were used in the
                making of this program.

          ii. Test Cases and status:
                      directed graph example:   passed :)
                      undirected graph example: passed :)

          iii.Graph Analysis
              1. Big O of building the adjacency list in terms of number of vertices V and edges E:
                 Big O for building the adjacency list would be O(|V| + |E|). The vertices would make V rows to create,
                 while E edges would comprise the columns to create.

              2. Big O of storage for the adjacency list:
                 Big O of storage for the adjacency list would also be O(|V| + |E|).
                 The worst case is V^2, and V+E is a tighter bound.

              3. Big O of storage needs for breadth & depth first search from chapter 13 slides:
                 The big O of storage for a BFS would be big O(|E| + |V|) while a depth first search is lower according
                 to chapter 13 part 1 slide 37.
*/
