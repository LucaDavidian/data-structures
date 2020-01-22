//#include "graph_adjacency_list.hpp"
#include "graph_adjacency_matrix.hpp"
#include <string>

int main(int argc, char **argv)
{
    Graph<std::string,10> graph;   
    //Graph<std::string> graph;   

    graph.AddVertex("A");
    graph.AddVertex("B");
    graph.AddVertex("C");
    graph.AddVertex("D");
    graph.AddVertex("E");

    graph.AddEdge(0, 1);
    graph.AddEdge(2, 1);
    graph.AddEdge(0, 3);
    graph.AddEdge(4, 3);

    graph.DepthFirstSearch(0);
    graph.DepthFirstSearchRecursive(0);
    graph.BreadthFirstSearch(0);

    Graph<std::string,10> graph2;  
    //Graph<std::string> graph2;

    graph2.AddVertex("A");
    graph2.AddVertex("B");
    graph2.AddVertex("C");
    graph2.AddVertex("D");
    graph2.AddVertex("E");
    graph2.AddVertex("F");
    graph2.AddVertex("G");
    graph2.AddVertex("H");
    graph2.AddVertex("I");

    graph2.AddEdge(0, 1);
    graph2.AddEdge(0, 2);
    graph2.AddEdge(0, 3);
    graph2.AddEdge(0, 4);
    graph2.AddEdge(1, 5);
    graph2.AddEdge(5, 7);
    graph2.AddEdge(3, 6);
    graph2.AddEdge(6, 8);

    graph2.BreadthFirstSearch(0);

    // Graph<std::string,10> graph3;

    // graph3.AddVertex("A");
    // graph3.AddVertex("B");
    // graph3.AddVertex("C");
    // graph3.AddVertex("D");
    // graph3.AddVertex("E");

    // graph3.AddEdge(0, 1);
    // graph3.AddEdge(0, 2);
    // graph3.AddEdge(0, 3);
    // graph3.AddEdge(0, 4);
    // graph3.AddEdge(1, 2);
    // graph3.AddEdge(1, 3);
    // graph3.AddEdge(1, 4);
    // graph3.AddEdge(2, 3);
    // graph3.AddEdge(2, 4);
    // graph3.AddEdge(3, 4);

    // PRINT("minimum spanning tree: ");
    // graph3.MinimumSpanningTreeDFS(0);
    // PRINTLN("");

    // Graph<std::string,10> graph4;

    // graph4.AddVertex("A");
    // graph4.AddVertex("B");
    // graph4.AddVertex("C");
    // graph4.AddVertex("D");
    // graph4.AddVertex("E");

    // graph4.AddEdge(0, 2, 1, true);
    // graph4.AddEdge(1, 0, 1, true);
    // graph4.AddEdge(1, 4, 1, true);
    // graph4.AddEdge(3, 4, 1, true);
    // graph4.AddEdge(4, 2, 1, true);    

    // PRINTLN("connectivity table (DFS directed graph): ");
    // for (int i = 0; i < graph4.Size(); i++)
    // {
    //     PRINT("starting from "); PRINT(i); PRINT(": ");
    //     graph4.DepthFirstSearch(i);
    //     PRINTLN("");
    // }
    // PRINTLN("");

    // unsigned int closure[10][10];
    // graph4.TransitiveClosure(closure);

    // PRINT("A is connected to C: "); PRINTLN(closure[0][2] == 1 ? "yes" : "no");  // yes
    // PRINT("A is connected to E: "); PRINTLN(closure[0][4] == 1 ? "yes" : "no");  // no
    // PRINT("B is connected to C: "); PRINTLN(closure[1][2] == 1 ? "yes" : "no");  // yes
    // PRINT("D is connected to A: "); PRINTLN(closure[3][0] == 1 ? "yes" : "no");  // no
    // PRINTLN("");

    Graph<std::string,5> graph5;
    //Graph<std::string> graph5;

    graph5.AddVertex("A", 10);
    graph5.AddVertex("B", 5);
    graph5.AddVertex("C", 4);
    graph5.AddVertex("D", 4);
    graph5.AddVertex("E", 0);

    graph5.AddEdge(0, 1, 50, true);
    graph5.AddEdge(0, 3, 80, true);
    graph5.AddEdge(1, 2, 60, true);
    graph5.AddEdge(1, 3, 90, true);
    graph5.AddEdge(2, 4, 40, true); 
    graph5.AddEdge(3, 2, 20, true);
    graph5.AddEdge(3, 4, 70, true);
    graph5.AddEdge(4, 1, 50, true); 

    PRINTLN("Dijkstra's shortest path algorithm: ");
    graph5.SingleSourceShortestPath(0);
    PRINTLN("");

    PRINTLN("A* shortest path algorithm: ");
    graph5.A_Star(0, 4);
    PRINTLN("");

    PRINTLN("Dijkstra's shortest path algorithm: ");
    graph5.SingleSourceShortestPath(4);
    PRINTLN("");

    return 0;
}