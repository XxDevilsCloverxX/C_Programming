#include <stdio.h>  //ins and outs
#include <stdlib.h> //malloc
#define FILENAME "graph1.txt"  //filename to be scanned
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
  unsigned int indeg;
}node;

//data bundle for 2D array that prints table at the end, avoids declaration of 4 arrays, only has one element at a time
typedef union data{
  char vertex;    //in the queue, uploaded from dequeue
  char origin;    //from the origin queue
  int max_path;  //calculated and cumlative
  int crit:2;      //critical path setting
}data_t;

node* create_node(char vertex); //creates a node for array
edge* create_edge(char vertex, unsigned int weight);  //creates an edge variable
unsigned int find_base(node* adj_list[], char base, unsigned int size); //finds a matching base node
void insert_edge(edge**head, edge**rear, char vertex, unsigned int weight); //inserts edges into linked lists
void print_graph(node* list[], unsigned int size);  //prints a graph (adjacency list)
void destroy_graph(node* arr[], unsigned int size); //frees graph memory
void enqueue(node* queue[], node* vertex, unsigned int size, int *rear);  //queues items
node* dequeue(node* queue[], int *front, int *rear);  //removes items from queues
void topo_search(node* adj_list[], node* start, unsigned int size);  //topological sort of a graph
void print_table(data_t table[][4], int size);  //designed for pretty prints of the table

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
    //After the edge is inserted, increase the in deg of the "to" node.
    index = find_base(adj_list, to, vertex_count);
    adj_list[index]->indeg++;
    if(DEBUG){
      printf("Node %c indeg: %d\n", adj_list[index]->vertex, adj_list[index]->indeg);
    }
    if(is_directed==0){
      /*
      Condition that the graph is undirected and needs the node created for inverted values
      find the point of the other letter, edge exists from above
      */
      index = find_base(adj_list, to, vertex_count);
      insert_edge(&(adj_list[index]->head), &(adj_list[index]->rear), from, wt);
      index = find_base(adj_list, to, vertex_count);
      adj_list[index]->indeg++;
    }
  }
  //Print the graph to console
  print_graph(adj_list, vertex_count);
  //Perform a topological search from node a
  topo_search(adj_list, adj_list[0], vertex_count);
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
  newp->indeg = 0;  //initial state of indegrees
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

/* NAME: topo_search
PARAMETERS: node* adj_list[], node* start, unsigned int size
PURPOSE: perform a topological search over an acyclic directed adjacency list (graph) from a starting point.
PRECONDITION: A graph must be fully established into the adjacency list.
POSTCONDITION: Nodes are processed via queues, revealing relationships regarding connections and what nodes are interconnected. A table with relevant data is printed
*/
void topo_search(node* adj_list[], node* start, unsigned int size){
  node* queue[size], *temp, *otemp, *origin[size];
  data_t table[size][4]; //we only have 4 cols to display after queues are built
  edge* cur;  //to queue neighbors from edges
  int front=0, rear=0, ofront=0,i, j,max_path=0;
  unsigned int index;  //init crit path as 1 for first node
  //queue nodes with indeg of 0
  //table[i][0,1,..n] is ith row of node, and 0,1,...n is the data in term of vertex, origin, Max_Path, and crit_path
  for(i=0; i<size;i++){
    if(adj_list[i]->indeg ==0){
      enqueue(queue, adj_list[i], size,&rear);
      ofront++; //decrease the size of the origin queue
      table[rear-1][2].max_path=0;  //initialize max path to 0
    }
  }
  const int begin=ofront; //this variable is a read-only that will help with the critical path
  //While queue is not empty
  while(front!=rear){
    //get front of queue
    temp = dequeue(queue, &front, &rear);
    table[front-1][1].origin='-'; //init null origin
    table[front-1][3].crit =0;  //init not on crit path
    table[front-1][0].vertex = temp->vertex;  //store the vertex into the 2D array of data
    cur = temp->head; //cur will go to the first neighbor of temp

    //When dequeueing a node, check for a "parent" when conditions allow max_path increment
    if(ofront<front){
      //get the point in adj list where parent edge may occur.
      index = find_base(adj_list, origin[rear-1]->vertex,size);
      cur = adj_list[index]->head;
      while(cur->to != temp->vertex){
        cur = cur->next;
      }
      max_path = cur->weight;
      table[front-1][2].max_path=max_path;  //add max_path data
      cur = temp->head; //reset cur
    }
    //For every neighbor of node temp, find index of "to" in adj list, then at that point, decrement indeg
    while(cur!=NULL){
      index = find_base(adj_list,cur->to,size); //find index
      if(DEBUG){
        printf("%c was neighbor with indeg %d\n", adj_list[index]->vertex, adj_list[index]->indeg);
      }
      (adj_list[index]->indeg)--; //decrement indegree
      //Enqueue condition
      if(adj_list[index]->indeg == 0){
        origin[rear]=temp;
        enqueue(queue, adj_list[index], size, &rear);
      }
    cur= cur->next;
    }

    //if the search is ahead of the origin list, start dequeue.
    if(ofront<front){
      table[ofront][1].origin=origin[ofront]->vertex;
      ofront++;
    }
  }
  //queue and origin are built, find crit path from rear of origins
    //The max and critical paths are ones that trace from the rear of the origin to the first node
  rear--; //rear was 1 too big, cant use for indexing without making it smaller
  while(rear>=begin){
    table[rear][3].crit = 1;  // This is part of the critical path
    rear = find_base(queue, origin[rear]->vertex, size);
    table[rear][3].crit = 1;  // This is part of the critical path too
  }

  //rear went down to beginning
  rear = begin - rear;
  while(rear<front){
    printf("%d rear ", rear);
    index = find_base(queue, origin[rear]->vertex, size);
    printf("found %d\n", index);
    table[rear][2].max_path += table[index][2].max_path;  // This accumulates max path
    rear++;
  }
  print_table(table, size); //print the table after
}

/* NAME: print_table
PARAMETERS: data_t table[][4], int rows
PURPOSE: Take a 2D array of data from a topological sort and return the data to output
PRECONDITION: queues must have been fully constructed, and all relevant data stored into cells of the matrix
POSTCONDITION: An organized table is printed to the output for user to see.
*/
void print_table(data_t table[][4], int rows){
  int i;
  //formatting
  printf("\nTopological Search Results:\n");
  printf("\n  Vertex  Origin  Max_Path  Critical_Path\n");
  //itterate through rows, printing necessary data spacced using padding properly
  for(i=0; i<rows; i++){
    printf("%6c%8c %6c %-2d%10d\n", table[i][0].vertex, table[i][1].origin, ' ',table[i][2].max_path, table[i][3].crit);
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
                      directed graph example where
                       weights were the same:   passed :)

          iii.Graph Analysis
              1. Big O of finding indegree of each vertex:
                  For each vertex v, there were e edges assigned to it. This means big O(V+E)
                  for calculating indegree of a node V for V nodes

              2. Big O of determining critical path:
                 Big O of determining the critical path is big O(V), the number of vertices. Because worst
                 case is that the origins and the nodes that queued them are linear in the directed graph and
                 traverse up the queue

              3. Big O of the topological sort that tracks max path length and critical path is
              Big O(V+E). Chapter 13 pt 2 slide 2

              4. The Big O storage requirements for a topological sort that tracks this data
              is given to be O(4V), which reduces to O(V). There are 4 columns of data, but that
              is reduced to V vertices determing the amount of rows needed to store the data.
*/
