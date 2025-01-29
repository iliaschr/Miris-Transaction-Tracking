
# Miris

## Description

Developed a Linux-based C application for managing and analyzing financial transactions between users using a dynamic directed multi-graph structure. The program supports efficient node and edge operations, cycle detection, connectivity analysis, and memory management. Implemented a custom hash table for O(1) access and ensured modularity with separate compilation and a Makefile.

## To Do

- [Change hash table size to be the closest prime number](https://github.com/iliaschr/OSProject1/issues/1)  
  _Adjust the hash table size to improve performance by setting it to the nearest prime number._

- [Add pointer to list of incoming edges in node struct](https://github.com/iliaschr/OSProject1/issues/2)  
  _Introduce a pointer in the `node` struct for a list of incoming edges to optimize edge traversal complexity._

- [Adjust hash table allocation to handle multiple nodes per line](https://github.com/iliaschr/OSProject1/issues/3)  
  _Update hash table memory allocation to account for lines that introduce multiple nodes._

## Quick Notes on the Makefile

To use the Makefile of this project, follow the steps below:

1. **Navigate to the src folder**
   ```sh
   cd src/

2. **Run available commands using make**:

- **make all**: Builds the main executable program as well as the executable files for the tests (graph_test_exec and miris_test_exec).
- **make build_miris**: Builds and runs the main miris program. It is used for executing the main application.
- **make run**: Runs the main miris program (provided it has already been built).
- **make graph_test**: Builds and runs tests for the graph module.
- **make miris_test**: Builds and runs tests for the miris module.
- **make valgrind_graph_test**: Runs graph tests with Valgrind to detect memory leaks.
- **make valgrind_miris_test**: Runs miris tests with Valgrind for the same purpose.
- **make valgrind_test**: Runs both tests (graph and miris) with Valgrind for comprehensive memory leak checking.
- **make clean**: Cleans all generated files (.o and executables) and output files (test_output.txt, output.txt).


## Graph.h

I started by creating a general graph structure. Inside this, based on the [Lecture Code from Data Structures](https://github.com/chatziko-k08/lecture-code/tree/master), I created some functions to help manage the graph.

### About Graph.h

In graph.h, the data structures and functions related to managing the graph are declared:

- Edge: An edge of the graph, containing information about the destination, the amount, and the date.
- Node: A node, containing information about its id (unique identifier), the edges connected to it, and a flag indicating if it has been visited by a dfs function (added later when I created the miris files).
- Graph: Contains a hash table for storing the nodes.

## Graph.c

All implementations of functions for managing the graph.

### About Graph.c

1. **graph_create (Create Graph)**:
    - Creates a new graph with an initial size __initial_size__ (added later to facilitate creating graphs from a .txt file).
    - Uses __malloc__ to allocate memory for the graph and __calloc__ for the hash table, ensuring initial values are __NULL__.
    - Complexity: __O(1) for access__ and __O(n) for initialization__ (n is the size of the array).

2. **node_insert (Insert Node)**:
    - Checks if the node already exists.
    - Complexity: __O(1) for finding__ and __O(1) for node creation__.

3. **node_find (Find Node)**:
    - Returns the node with the specified id if it exists.
    - Uses find_node_in_hash_table to locate the node.
    - Complexity: __O(1) for initial access__ and __O(n) for traversing the list__.

4. **node_remove (Remove Node)**:
    - Removes the node and its connected edges.
    - First checks if there are edges to the node being deleted.
    - Complexity: __O(n) for searching edges__ and __O(1) for removal__.

5. **edge_insert (Insert Edge)**:
    - Creates a new edge between two nodes.
    - Checks the existence of the source and destination nodes before insertion.
    - Complexity: __O(1) for insertion if nodes already exist__.

6. **edge_remove (Remove Edge)**:
    - Removes a specific edge between two nodes.
    - Complexity: __O(n) for searching the edge__.

7. **edge_modify (Modify Edge)**:
    - Modifies the amount and date of an edge, checking if the nodes and edge exist.
    - Complexity: __O(n) for searching the edge__.

8. **graph_destroy (Destroy Graph)**:
    - Frees all memory allocated for the graph and its nodes.
    - Complexity: __O(n) for freeing nodes and edges__.


#### Static Functions
Although they might not be necessary, I had to include them as they help make certain functions more readable.

1. **find_node_in_hash_table**:
    - Note must be made regarding the complexity since I use malloc with initial_size in the graph. The likelihood of no space for nodes is low, so in the ideal case, the complexity is O(1).

2. **remove_edges_to_node**:
    - Removes all edges pointing to the node.
    - Uses edge_remove.
    - Complexity: __O(n)__ as it may need to traverse all nodes.

3. **free_node_edges**:
    - Frees all memory allocated for the edges of a specific node.
    - Complexity: __O(m)__, where m is the number of edges.

4. **remove_node_from_hash_table**:
    - Simple update of pointers, hence __O(1)__.

5. **create_edge**: 
    - Creates and returns a new edge.
    - Complexity: __O(1)__.

6. **find_edge**:
    - Finds an edge connecting two nodes.
    - Complexity: __O(m)__.

7. **remove_edge_from_list**:
    - Complexity: __O(m)__.

## Miris.h and Miris.c

These were created after the graph.h and graph.c files to assist in creating the functions needed for the project.

### About Miris.c

1. **miris_insert (Insert Node)**:
   - Creates a new node in the graph. If the node already exists, it returns immediately.
   - Uses node_insert function to handle the addition.
   - Complexity: __O(1)__ on average, provided the hash table is well-distributed.

2. **miris_insert2 (Insert Edge)**:
   - Creates new nodes if the source and destination nodes don't already exist.
   - Uses edge_insert to add the edge between the two nodes.
   - Complexity: __O(1)__ for finding the nodes, __O(1)__ for adding the edge if there are no collisions.

3. **miris_delete (Delete Node)**:
   - Removes a node from the graph, as well as all edges connected to it.
   - First checks if the node exists.
   - Complexity: __O(n)__ for removal, where n is the number of nodes connected to the specific node.

4. **miris_delete2 (Delete Edge)**:
   - Removes a specific edge between two nodes.
   - Checks if the source and destination nodes exist.
   - Complexity: __O(n)__ in the worst case for finding the edge.

5. **miris_modify (Modify Edge)**:
   - Modifies the amount and date of an edge if found.
   - Checks if the source and destination nodes exist.
   - Complexity: __O(n)__ for searching the edge.

6. **miris_find (Find Node)**:
   - Displays all external edges connected to a specific node.
   - Uses node_find to find the node.
   - Complexity: __O(m)__, where m is the number of edges of the node.

7. **miris_receiving (Find Incoming Edges)**:
   - Displays all edges leading to the node.
   - Scans the entire graph to find edges pointing to the specified node.
   - Complexity: __O(n*m)__, where n is the number of nodes and m is the edges.

8. **miris_circlefind (Find Circles)**:
   - Uses DFS to discover cycles in the graph starting from a specific node.
   - Uses an array to track visited nodes.
   - Complexity: __O(n + m)__, where n is the nodes and m is the edges.

9. **miris_findcircles (Find Cycles with Minimum Amount)**:
   - Applies DFS to find cycles based on a minimum amount value for the edges.
   - Complexity: __O(n + m)__, which can be affected by the number of edges.

10. **miris_traceflow (Trace Flow)**:
    - Runs DFS to trace the flow of transactions from a node.
    - Complexity: __O(n + m)__ for the full transaction path.

11. **miris_connected (Check Connection)**:
    - Checks if there is a path between two nodes.
    - Complexity: __O(n + m)__ due to DFS.

12. **miris_exit (Exit Program)**:
    - Logs the state of the graph to an output file.
    - Frees memory for nodes and edges.
    - Complexity: __O(n + m)__ due to freeing memory for nodes and edges.

### About Static Functions

1. **dfs_find_cycles**:
   - Performs Depth-First Search (DFS) to find cycles starting from a specific node.
   - Uses recursion and a path structure to track visited nodes.
   - Complexity: __O(n + m)__.

2. **dfs_find_circles_with_min_sum**:
   - Searches for cycles that meet a minimum amount value for the edges.
   - Uses similar logic as dfs_find_cycles.
   - Complexity:__O(n + m)__.

3. **dfs_traceflow**:
   - Traces the flow of transactions from the current node, printing the nodes in the path.
   - Complexity: __O(n + m)__.

4. **dfs_find_path**:
   - Finds the path from the current node to the destination.
   - Complexity: __O(n + m)__.


# Main aka. Miris the Main Program

### About Main.c

1. **load_graph (Load Graph from File)**:
   - Reads data from a file and inserts nodes and edges into the graph.
   - Uses fscanf to read the values and miris_insert and miris_insert2 to add the nodes and edges.
   - Complexity: O(n), where n is the number of lines in the file.

2. **count_lines (Count Lines)**:
   - Returns the number of lines in a file to determine the size of the graph.
   - Uses fgetc to read the file character by character.
   - Complexity: O(n), where n is the number of characters in the file.

3. **handle_insert (Handle Insert Command)**:
   - Handles the insertion of multiple nodes.
   - Uses miris_insert to add each node.
   - Complexity: O(m), where m is the number of nodes being inserted.

4. **handle_insert2 (Handle Insert Edge Command)**:
   - Handles the insertion of an edge between two nodes.
   - Uses miris_insert2 to add the edge.
   - Complexity: O(1) if there are no collisions.

5. **handle_delete (Handle Delete Command)**:
   - Handles the deletion of multiple nodes.
   - Uses miris_delete to delete each node.
   - Complexity: O(m), where m is the number of nodes being deleted.

6. **handle_delete2 (Handle Delete Edge Command)**:
   - Handles the deletion of a specific edge.
   - Uses miris_delete2 to delete the edge.
   - Complexity: O(1) if the edge exists.

7. **handle_modify (Handle Modify Edge Command)**:
   - Handles the modification of an edge.
   - Uses miris_modify to modify the edge.
   - Complexity: O(n) for searching the edge.

8. **execute_command (Execute Commands)**:
   - Handles the execution of commands entered by the user.
   - Uses the previously mentioned handler functions to execute the appropriate command.
   - Complexity: Depends on the command being executed.

9. **main (Main Function)**:
   - Manages input parameters and starts the program.
   - Loads the graph from a file if provided.
   - Displays a prompt to accept commands from the user.
   - Complexity: O(n) for loading the graph.

### About Static Functions

1. **handle_insert**:
   - Handles the insertion of nodes from user commands.

2. **handle_insert2**:
   - Handles the insertion of edges from user commands.

3. **handle_delete**:
   - Handles the deletion of nodes from user commands.

4. **handle_delete2**:
   - Handles the deletion of edges from user commands.

5. **handle_modify**:
   - Handles the modification of edges from user commands.


## Tests

I used the `acutest.h` we learned in Data Structures. The tests are fairly general and were written as I wrote the code to ensure everything was functioning correctly.

## Debugging

When I encountered segmentation faults (core dumped), I did debugging using gdb.

I also used valgrind to avoid memory leaks.

## Format

The format for each file was done using the command `clang-format -i ...`.

## Specification for miris command generated-output:
In case of unrecognized command:
    ERROR INDICATION: "Unrecognized command"

0. COMMAND: ANY
    ERROR INDICATION: "Format error: " COMMAND_NAME COMMAND OPTIONS

1.  COMMAND: i Ni [Nj Nk ...]
    	 OR
    	 insert Ni [Nj Nk ...]
    COMMENT: insert into the graph structure 1 or more nodes
             with specific INT ids.
    EXPECTED OUTPUT:
    	 "Succ:" Ni Nj ..
    ERROR INDICATION:
    	 "IssueWith:" Nj Nk Nl ..

2.  COMMAND: n Ni Nj sum date
             OR
             insert2 Ni Nj sum date
    COMMENT: introduce an edge with direction from Ni to Nj with label
             sum + date if either Ni or Nj does not exist in the graph,
             do the appropriate node insertion first.
    EXPECTED OUTPUT:
    ERROR INDICATION: "IssueWith:" Ni Nj

3.  COMMAND: d Ni [Nj Nk ...]
             OR
             delete Ni [Nj Nk ...]
    COMMENT: delete from graph listed nodes Ni, Nj, Nk, etc
    EXPECTED OUTPUT:
    ERROR INDICATION:
             "Non-existing node(s):" Ni Nk ..

4.  COMMAND: l Ni Nj
             OR
             delete2 Ni Nj
    COMMENT: remove the edge between Ni and Nj; if there are
	     more than one edges, remove one of the edges.
    EXPECTED OUTPUT:
    ERROR INDICATION:
             "Non-existing node(s):" Ni ..

5.  COMMAND: m Ni Nj sum sum1 date date1
             OR
             m(odify) Ni Nj sum sum1 date date1
    COMMENT: update the values of a specific edge between Ni and Nj
    EXPECTED OUTPUT:
    ERROR INDICATION:
             "Non-existing node(s):" Ni ..
             OR
             "Non-existing edge:" Ni Nj sum date"

6.  COMMAND: f Ni
             OR
             f(ind) Ni
    COMMENT: find all outgoing edges from Ni
    EXPECTED OUTPUT:
             Ni Nj sum1 date1
             Ni Nk sum2 date2
             Ni Nl sum3 date3
             ...
             OR
             (none) // print nothing
    ERROR INDICATION:
             "Non-existing node:" Ni

7.  COMMAND: r Ni
             OR
             receiving Ni
    COMMENT: find all incoming to Ni edges.
    EXPECTED OUTPUT:
             Nj Ni sum1 data1
             Nl Ni sum2 data2
             Nl Ni sum3 data3
             ....
             OR
             (none) // print nothing
    ERROR INDICATION:
             "Non-existing node:" Ni

8.  COMMAND: c Ni
             OR
             circlefind Ni
    COMMENT: find circles Ni is involved in if any
    EXPECTED OUTPUT:
             Ni, Nj, Nk, .., Ni
             Ni, Na, Nb, Nc, ...,Ni
             ....
             OR
             (none) // print nothing
    ERROR INDICATION:
             "Non-existing node:" Ni

9.  COMMAND: fi Ni k
             OR
             findcircles Ni k
    COMMENT: find circular relationships in which Ni is involved and
             moves at least k units of funds.
    EXPECTED OUTPUT:
             Ni, Nj, Nk, .., Ni m      // m min value found (m>=k)
             Ni, Na, Nb, Nc, ...,Ni d  // d min value found (d>=k)
             Ni, Nv, Ni o              // o min value found (o>=k)
             ....
             OR
             (none) // print nothing
    ERROR INDICATION:
            "Non-existing node:" Ni

10. COMMAND: t Ni m
             OR
             traceflow Ni m
    COMMENT: find all paths of length upto m for which a flow of transfers can
             be establsihed from Ni
    EXPECTED OUTPUT:
             Ni, Nj, Nl, Ny
             Ni, Nj, Nv
             Ni, Nj, Nv
             Ni, Na, Nc, Nb, Nx, Ny
             ....
             OR
             (none) // print nothing
    ERROR INDICATION:
            "Non-existing node:" Ni

11. COMMAND: o Ni Nj
             OR
             connected Ni Nj
    COMMENT: If exist, present a path (1) between Ni and Nj
    EXPECTED OUTPUT:
             Ni, No, .., Nm, Nj // connecting path
             OR
             (none) // print nothing
    ERROR INDICATION:
             "Non-existing node:" Ni
             "Non-existing node:" Nj

12. COMMAND: e
             OR
             exit
    COMMENT: terminate the program.
    EXPECTED OUTPUT:
             XYZ "Bytes released"
    ERROR INDICATION:
             (none)
