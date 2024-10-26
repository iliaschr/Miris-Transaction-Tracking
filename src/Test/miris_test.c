#include "../Miris/miris.h"
#include "acutest.h"

// Helper function for color output
#define COLOR_BLUE "\033[1;34m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RESET "\033[0m"

// Test miris_insert
void test_miris_insert(void) {
    printf(COLOR_BLUE "Calling miris_insert for Node1 and Node2...\n" COLOR_RESET);
    Graph test_graph = graph_create(INIT_SIZE);
    TEST_ASSERT(miris_insert(test_graph, "Node1"));
    TEST_ASSERT(miris_insert(test_graph, "Node2"));

    Node found_node1 = node_find(test_graph, "Node1");
    Node found_node2 = node_find(test_graph, "Node2");
    TEST_ASSERT(found_node1 != NULL);
    TEST_ASSERT(found_node2 != NULL);
    
    miris_exit(test_graph, "test_output.txt");
    printf(COLOR_GREEN "miris_insert completed successfully\n" COLOR_RESET);
}

// Test miris_insert2 (insert edge between nodes)
void test_miris_insert2(void) {
    printf(COLOR_BLUE "Calling miris_insert2 to add edge from Node1 to Node2...\n" COLOR_RESET);
    Graph test_graph = graph_create(INIT_SIZE);
    miris_insert(test_graph, "Node1");
    miris_insert(test_graph, "Node2");
    
    TEST_ASSERT(miris_insert2(test_graph, "Node1", "Node2", 100, "2023-10-26"));

    Node node1 = node_find(test_graph, "Node1");
    TEST_ASSERT(node1->edges != NULL);
    TEST_ASSERT(node1->edges->amount == 100);
    TEST_ASSERT(strcmp(node1->edges->date, "2023-10-26") == 0);

    miris_exit(test_graph, "test_output.txt");
    printf(COLOR_GREEN "miris_insert2 completed successfully\n" COLOR_RESET);
}

// Test miris_delete (delete node)
void test_miris_delete(void) {
    printf(COLOR_BLUE "Calling miris_delete to remove Node1...\n" COLOR_RESET);
    Graph test_graph = graph_create(INIT_SIZE);
    miris_insert(test_graph, "Node1");
    miris_insert(test_graph, "Node2");
    
    TEST_ASSERT(miris_delete(test_graph, "Node1"));
    TEST_ASSERT(node_find(test_graph, "Node1") == NULL);
    TEST_ASSERT(node_find(test_graph, "Node2") != NULL);

    miris_exit(test_graph, "test_output.txt");
    printf(COLOR_GREEN "miris_delete completed successfully\n" COLOR_RESET);
}

// Test miris_delete2 (delete edge)
void test_miris_delete2(void) {
    printf(COLOR_BLUE "Calling miris_delete2 to remove edge from Node1 to Node2...\n" COLOR_RESET);
    Graph test_graph = graph_create(INIT_SIZE);
    miris_insert(test_graph, "Node1");
    miris_insert(test_graph, "Node2");
    miris_insert2(test_graph, "Node1", "Node2", 100, "2023-10-26");
    
    TEST_ASSERT(miris_delete2(test_graph, "Node1", "Node2"));
    TEST_ASSERT(node_find(test_graph, "Node1")->edges == NULL);

    miris_exit(test_graph, "test_output.txt");
    printf(COLOR_GREEN "miris_delete2 completed successfully\n" COLOR_RESET);
}

// Test miris_modify (modify edge)
void test_miris_modify(void) {
    printf(COLOR_BLUE "Calling miris_modify to update edge between Node1 and Node2...\n" COLOR_RESET);
    Graph test_graph = graph_create(INIT_SIZE);
    miris_insert(test_graph, "Node1");
    miris_insert(test_graph, "Node2");
    miris_insert2(test_graph, "Node1", "Node2", 100, "2023-10-26");
    
    TEST_ASSERT(miris_modify(test_graph, "Node1", "Node2", 100, 200, "2023-10-26", "2023-10-27"));

    Node node1 = node_find(test_graph, "Node1");
    Edge edge = node1->edges;
    TEST_ASSERT(edge->amount == 200);
    TEST_ASSERT(strcmp(edge->date, "2023-10-27") == 0);

    miris_exit(test_graph, "test_output.txt");
    printf(COLOR_GREEN "miris_modify completed successfully\n" COLOR_RESET);
}

// Test miris_find (find outgoing edges)
void test_miris_find(void) {
    printf(COLOR_BLUE "Calling miris_find to list outgoing edges of Node1...\n" COLOR_RESET);
    Graph test_graph = graph_create(INIT_SIZE);
    miris_insert(test_graph, "Node1");
    miris_insert(test_graph, "Node2");
    miris_insert2(test_graph, "Node1", "Node2", 100, "2023-10-26");

    TEST_ASSERT(miris_find(test_graph, "Node1"));
    TEST_ASSERT(!miris_find(test_graph, "NonExistentNode"));

    miris_exit(test_graph, "test_output.txt");
    printf(COLOR_GREEN "miris_find completed successfully\n" COLOR_RESET);
}

// Test miris_receiving (find incoming edges)
void test_miris_receiving(void) {
    printf(COLOR_BLUE "Calling miris_receiving to find incoming edges to Node1...\n" COLOR_RESET);

    Graph test_graph = graph_create(INIT_SIZE);
    miris_insert(test_graph, "Node1");
    miris_insert(test_graph, "Node2");
    miris_insert2(test_graph, "Node2", "Node1", 150, "2023-10-26");

    TEST_ASSERT(miris_receiving(test_graph, "Node1"));
    TEST_ASSERT(!miris_receiving(test_graph, "NonExistentNode"));

    miris_exit(test_graph, "test_output.txt");
    printf(COLOR_GREEN "miris_receiving completed successfully\n" COLOR_RESET);
}

// Test miris_circlefind (find cycles)
void test_miris_circlefind(void) {
    printf(COLOR_BLUE "Calling miris_circlefind to detect cycles involving Node1...\n" COLOR_RESET);
    Graph test_graph = graph_create(INIT_SIZE);
    miris_insert(test_graph, "Node1");
    miris_insert(test_graph, "Node2");
    miris_insert2(test_graph, "Node1", "Node2", 100, "2023-10-26");
    miris_insert2(test_graph, "Node2", "Node1", 100, "2023-10-27");

    TEST_ASSERT(miris_circlefind(test_graph, "Node1"));
    TEST_ASSERT(!miris_circlefind(test_graph, "NonExistentNode"));

    miris_exit(test_graph, "test_output.txt");
    printf(COLOR_GREEN "miris_circlefind completed successfully\n" COLOR_RESET);
}

// Test miris_findcircles (find circular relationships with minimum sum)
void test_miris_findcircles(void) {
    printf(COLOR_BLUE "Calling miris_findcircles to find circular relationships with minimum sum for Node1...\n" COLOR_RESET);
    Graph test_graph = graph_create(INIT_SIZE);
    miris_insert(test_graph, "Node1");
    miris_insert(test_graph, "Node2");
    miris_insert2(test_graph, "Node1", "Node2", 150, "2023-10-26");
    miris_insert2(test_graph, "Node2", "Node1", 150, "2023-10-27");

    TEST_ASSERT(miris_findcircles(test_graph, "Node1", 100));
    TEST_ASSERT(!miris_findcircles(test_graph, "NonExistentNode", 100));

    miris_exit(test_graph, "test_output.txt");
    printf(COLOR_GREEN "miris_findcircles completed successfully\n" COLOR_RESET);
}

// Test miris_traceflow (find paths up to a certain length)
void test_miris_traceflow(void) {
    printf(COLOR_BLUE "Calling miris_traceflow to find paths up to length 2 from Node1...\n" COLOR_RESET);
    Graph test_graph = graph_create(INIT_SIZE);
    miris_insert(test_graph, "Node1");
    miris_insert(test_graph, "Node2");
    miris_insert(test_graph, "Node3");
    miris_insert2(test_graph, "Node1", "Node2", 100, "2023-10-26");
    miris_insert2(test_graph, "Node2", "Node3", 100, "2023-10-27");

    TEST_ASSERT(miris_traceflow(test_graph, "Node1", 2));
    TEST_ASSERT(!miris_traceflow(test_graph, "NonExistentNode", 2));

    miris_exit(test_graph, "test_output.txt");
    printf(COLOR_GREEN "miris_traceflow completed successfully\n" COLOR_RESET);
}

// Test miris_connected (check connection between two nodes)
void test_miris_connected(void) {
    printf(COLOR_BLUE "Calling miris_connected to check if Node1 and Node3 are connected...\n" COLOR_RESET);
    Graph test_graph = graph_create(INIT_SIZE);
    miris_insert(test_graph, "Node1");
    miris_insert(test_graph, "Node2");
    miris_insert(test_graph, "Node3");
    miris_insert2(test_graph, "Node1", "Node2", 100, "2023-10-26");
    miris_insert2(test_graph, "Node2", "Node3", 100, "2023-10-27");

    TEST_ASSERT(miris_connected(test_graph, "Node1", "Node3"));
    TEST_ASSERT(!miris_connected(test_graph, "Node1", "NonExistentNode"));

    miris_exit(test_graph, "test_output.txt");
    printf(COLOR_GREEN "miris_connected completed successfully\n" COLOR_RESET);
}

// Registering the tests
TEST_LIST = {
    {"test_miris_insert", test_miris_insert},
    {"test_miris_insert2", test_miris_insert2},
    {"test_miris_delete", test_miris_delete},
    {"test_miris_delete2", test_miris_delete2},
    {"test_miris_modify", test_miris_modify},
    {"test_miris_find", test_miris_find},
    {"test_miris_receiving", test_miris_receiving},
    {"test_miris_circlefind", test_miris_circlefind},
    {"test_miris_findcircles", test_miris_findcircles},
    {"test_miris_traceflow", test_miris_traceflow},
    {"test_miris_connected", test_miris_connected},
    {NULL, NULL}
};
