#include "acutest.h"
#include "../Graph/graph.h"

// Test Graph Creation
void test_create(void) {
    Graph graph = graph_create(10);
    TEST_ASSERT(graph != NULL);
    TEST_ASSERT(graph->table_size == 10);
    for (size_t i = 0; i < graph->table_size; i++) {
        TEST_ASSERT(graph->hash_table[i] == NULL);
    }
    graph_destroy(graph);
}

// Test Node Insertion
void test_node_insert(void) {
    Graph graph = graph_create(10);
    Node node1 = node_insert(graph, "Node1");
    TEST_ASSERT(node1 != NULL);
    TEST_ASSERT(strcmp(node1->id, "Node1") == 0);

    // Test duplicate insertion
    Node duplicate = node_insert(graph, "Node1");
    TEST_ASSERT(duplicate == node1);

    Node node2 = node_insert(graph, "Node2");
    TEST_ASSERT(node2 != NULL);
    TEST_ASSERT(strcmp(node2->id, "Node2") == 0);

    graph_destroy(graph);
}

// Test Node Retrieval
void test_node_find(void) {
    Graph graph = graph_create(10);
    node_insert(graph, "Node1");
    node_insert(graph, "Node2");

    Node found = node_find(graph, "Node1");
    TEST_ASSERT(found != NULL);
    TEST_ASSERT(strcmp(found->id, "Node1") == 0);

    Node not_found = node_find(graph, "NonExistent");
    TEST_ASSERT(not_found == NULL);

    graph_destroy(graph);
}

// Test Node Removal
void test_node_remove(void) {
    Graph graph = graph_create(10);
    node_insert(graph, "Node1");
    node_insert(graph, "Node2");

    node_remove(graph, "Node1");
    TEST_ASSERT(node_find(graph, "Node1") == NULL);

    node_remove(graph, "Node2");
    TEST_ASSERT(node_find(graph, "Node2") == NULL);

    // Test removing non-existing node
    node_remove(graph, "NonExistent");

    graph_destroy(graph);
}

// Test Edge Insertion
void test_edge_insert(void) {
    Graph graph = graph_create(10);
    node_insert(graph, "Node1");
    node_insert(graph, "Node2");

    Edge edge = edge_insert(graph, "Node1", "Node2", 100, "2023-10-25");
    TEST_ASSERT(edge != NULL);
    TEST_ASSERT(edge->amount == 100);
    TEST_ASSERT(strcmp(edge->date, "2023-10-25") == 0);

    graph_destroy(graph);
}

// Test Edge Removal
void test_edge_remove(void) {
    Graph graph = graph_create(10);
    node_insert(graph, "Node1");
    node_insert(graph, "Node2");

    edge_insert(graph, "Node1", "Node2", 100, "2023-10-25");
    edge_remove(graph, "Node1", "Node2");

    // Check if edge is removed
    Node node1 = node_find(graph, "Node1");
    TEST_ASSERT(node1->edges == NULL);

    graph_destroy(graph);
}

// Test Edge Modification
void test_edge_modify(void) {
    Graph graph = graph_create(10);
    node_insert(graph, "Node1");
    node_insert(graph, "Node2");

    edge_insert(graph, "Node1", "Node2", 100, "2023-10-25");
    bool modified = edge_modify(graph, "Node1", "Node2", 100, 200, "2023-10-25", "2023-10-26");
    TEST_ASSERT(modified == true);

    // Check if edge is modified
    Node node1 = node_find(graph, "Node1");
    Edge edge = node1->edges;
    TEST_ASSERT(edge->amount == 200);
    TEST_ASSERT(strcmp(edge->date, "2023-10-26") == 0);

    graph_destroy(graph);
}

// Registering the tests
TEST_LIST = {
    {"test_create", test_create},
    {"test_node_insert", test_node_insert},
    {"test_node_find", test_node_find},
    {"test_node_remove", test_node_remove},
    {"test_edge_insert", test_edge_insert},
    {"test_edge_remove", test_edge_remove},
    {"test_edge_modify", test_edge_modify},
    {NULL, NULL}
};
