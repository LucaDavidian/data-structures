//#include "graph_edge_list.hpp"
//#include "graph_adjacency_matrix.hpp"
//#include "graph_adjacency_list.hpp"
#include "graph_adjacency_list2.hpp"
#include <iostream>
#include <string>
#include <cmath>

template <typename T>
class NodeVisitor
{
public:
    void operator()(const typename Graph<T>::Node &node) const
    {
        std::cout << node.data << std::endl;
    }
};

class Vector2D
{
friend std::ostream &operator<<(std::ostream &os, const Vector2D &vector)
{
    return os << "(x: " << vector.x << " y: " << vector.y << ") ---> ";
}
public:
    Vector2D(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    float X() const { return x; }
    float Y() const { return y; }

    Vector2D const operator-(const Vector2D &other) const { return Vector2D(x - other.x, y - other.y); }
    float Length() const { return std::sqrt(x * x + y * y); }
private:
    float x;
    float y;
};

std::ostream &operator<<(std::ostream &os, const Vector2D &vector);

int main(int argc, char **argv)
{
    Graph<std::string> gi;

    gi.AddNode("A");
    gi.AddNode("B");
    gi.AddNode("C");
    gi.AddNode("D");
    gi.AddNode("E");
    gi.AddNode("F");
    gi.AddNode("G");

    gi.AddEdge(0, 1, 1.0f, true);
    gi.AddEdge(0, 2, 1.0f, true);
    gi.AddEdge(0, 3, 1.0f, true);
    gi.AddEdge(0, 4, 1.0f, true);   
    gi.AddEdge(1, 4, 1.0f, true);
    gi.AddEdge(1, 5, 1.0f, true);
    gi.AddEdge(3, 6, 1.0f, true);   
    gi.AddEdge(4, 6, 2.0f, true);   

    std::cout << "breadth first search: " << '\n';   

    gi.BreadthFirstSearch(0, NodeVisitor<std::string>()); 

    std::cout << "depth first search: " << '\n';   

    gi.DepthFirstSearch(0, NodeVisitor<std::string>()); 

    std::cout << "depth first search (recursive): " << '\n';   

    gi.DepthFirstSearchRecursive(0, NodeVisitor<std::string>()); 

    std::cout << "Dijkstra single source shortest path from A: " << '\n';

    auto shortestPaths = gi.DijkstraShortestPath(0);

    for (auto path : shortestPaths)
    {
        for (auto node : path)
            std::cout << node->data << " ";

        std::cout << '\n';
    }

    std::cout << "Dijkstra single source shortest path from A to G" << '\n';

    auto shortestPath = gi.DijkstraShortestPath(0, 6);

    for (auto node : shortestPath)
        std::cout << node->data << " ";

    std::cout << '\n';

    std::cout << "Dijkstra single source shortest path from A to F" << '\n';

    shortestPath = gi.DijkstraShortestPath(0, 5);

    for (auto node : shortestPath)
        std::cout << node->data << " ";

    std::cout << '\n';

    Graph<Vector2D> gv;

    gv.AddNode(Vector2D(0.0f, 0.0f)); // node 0
    gv.AddNode(Vector2D(1.0f, 0.0f)); // node 1
    gv.AddNode(Vector2D(2.0f, 0.0f)); // node 2
    gv.AddNode(Vector2D(0.0f, 1.0f)); // node 3
    gv.AddNode(Vector2D(1.0f, 1.0f)); // node 4
    gv.AddNode(Vector2D(2.0f, 1.0f)); // node 5
    gv.AddNode(Vector2D(0.0f, 2.0f)); // node 6
    gv.AddNode(Vector2D(1.0f, 2.0f)); // node 7
    gv.AddNode(Vector2D(2.0f, 2.0f)); // node 8

    gv.AddEdge(0, 1, 2.0f, true);
    gv.AddEdge(0, 3, 1.0f, true);
    gv.AddEdge(1, 2, 3.0f, true);
    gv.AddEdge(1, 4, 4.0f, true);
    gv.AddEdge(2, 5, 2.0f, true);
    gv.AddEdge(3, 6, 2.0f, true);
    gv.AddEdge(3, 4, 3.0f, true);
    gv.AddEdge(4, 5, 1.0f, true);
    gv.AddEdge(4, 7, 5.0f, true);
    gv.AddEdge(5, 8, 1.5f, true);
    gv.AddEdge(6, 7, 0.5f, true);
    gv.AddEdge(7, 8, 2.0f, true);
    gv.AddEdge(7, 4, 0.1f, true);
    gv.AddEdge(4, 1, 0.2f, true);
    gv.AddEdge(5, 2, 0.2f, true);

    std::cout << "Dijkstra shortest path from 0,0 to 2,0" << '\n';
    auto shortestPath2 = gv.DijkstraShortestPath(0, 2);

    for (auto node : shortestPath2)
        std::cout << node->data << " ";

    std::cout << '\n';

    std::cout << "A* shortest path from 0,0 to 2,0" << '\n';
    shortestPath2 = gv.AStar(0, 2);
    
    for (auto node : shortestPath2)
        std::cout << node->data << " ";

    std::cout << '\n';

    return 0;
}
