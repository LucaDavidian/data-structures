#include "graph.hpp"
#include <iostream>
#include <string>
#include <cmath>

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

template <>
float Graph<Vector2D>::GetNodeHeuristic(unsigned int nodeIndex, unsigned int endNodeIndex) const
{
    return (mNodes[endNodeIndex].mData - mNodes[nodeIndex].mData).Length();
}

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

    gi.AddEdge(0, 1);
    gi.AddEdge(0, 2);
    gi.AddEdge(0, 3);
    gi.AddEdge(0, 4);   
    gi.AddEdge(1, 4);
    gi.AddEdge(1, 5);
    gi.AddEdge(3, 6);   
    gi.AddEdge(4, 6);   

    std::cout << "breadth first search: " << '\n';   

    NodeVisitor<std::string> nv;

    gi.BreadthFirstSearch(nv, 0); 

    std::cout << "depth first search: " << '\n';   

    gi.DepthFirstSearch(nv, 0); 

    std::cout << "depth first search (recursive): " << '\n';   

    gi.DepthFirstSearchRecursive(nv, 0); 

    std::cout << "Dijkstra single source shortest path from A to G: " << '\n';

    auto shortestPaths = gi.DijkstraShortestPath(0);

    for (auto node : shortestPaths[6])
        std:: cout << node << " ";   
    std::cout << std::endl;

    std::cout << "Dijkstra single source shortest path from A to G:" << '\n';

    auto shortestPath = gi.DijkstraShortestPath(0, 6);

    for (auto node : shortestPath)
        std::cout << node << " ";
    std::cout << '\n';

    std::cout << "Dijkstra single source shortest path from A to F" << '\n';

    shortestPath = gi.DijkstraShortestPath(0, 5);

    for (auto node : shortestPath)
        std::cout << node << " ";
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

    // gv.AddEdge(0, 1, 2.0f, true);
    // gv.AddEdge(0, 3, 1.0f, true);
    // gv.AddEdge(1, 2, 3.0f, true);
    // gv.AddEdge(1, 4, 4.0f, true);
    // gv.AddEdge(2, 5, 2.0f, true);
    // gv.AddEdge(3, 6, 2.0f, true);
    // gv.AddEdge(3, 4, 3.0f, true);
    // gv.AddEdge(4, 5, 1.0f, true);
    // gv.AddEdge(4, 7, 5.0f, true);
    // gv.AddEdge(5, 8, 1.5f, true);
    // gv.AddEdge(6, 7, 0.5f, true);
    // gv.AddEdge(7, 8, 2.0f, true);
    // gv.AddEdge(7, 4, 0.1f, true);
    // gv.AddEdge(4, 1, 0.2f, true);
    // gv.AddEdge(5, 2, 0.2f, true);

    gv.AddEdge(0, 1, (gv.GetData(1) - gv.GetData(0)).Length(), true);
    gv.AddEdge(0, 3, (gv.GetData(3) - gv.GetData(0)).Length(), true);
    gv.AddEdge(1, 2, (gv.GetData(2) - gv.GetData(1)).Length(), true);
    gv.AddEdge(1, 4, (gv.GetData(4) - gv.GetData(1)).Length(), true);
    gv.AddEdge(2, 5, (gv.GetData(5) - gv.GetData(2)).Length(), true);
    gv.AddEdge(3, 6, (gv.GetData(6) - gv.GetData(3)).Length(), true);
    gv.AddEdge(3, 4, (gv.GetData(4) - gv.GetData(3)).Length(), true);
    gv.AddEdge(4, 5, (gv.GetData(4) - gv.GetData(5)).Length(), true);
    gv.AddEdge(4, 7, (gv.GetData(7) - gv.GetData(4)).Length(), true);
    gv.AddEdge(5, 8, (gv.GetData(8) - gv.GetData(5)).Length(), true);
    gv.AddEdge(6, 7, (gv.GetData(7) - gv.GetData(6)).Length(), true);
    gv.AddEdge(7, 8, (gv.GetData(8) - gv.GetData(7)).Length(), true);
    gv.AddEdge(7, 4, (gv.GetData(4) - gv.GetData(7)).Length(), true);
    gv.AddEdge(4, 1, (gv.GetData(1) - gv.GetData(4)).Length(), true);
    gv.AddEdge(5, 2, (gv.GetData(2) - gv.GetData(5)).Length(), true);

    std::cout << "Dijkstra shortest path from 0,0 to 2,0" << '\n';
    auto shortestPath2 = gv.DijkstraShortestPath(0);

    NodeVisitor<Vector2D> vv;

    for (auto nodeIndex : shortestPath2[2])
        std::cout << gv.GetData(nodeIndex) << " ";
    std::cout << '\n';

    std::cout << "Dijkstra shortest path from 0,0 to 2,0" << '\n';
    auto shortestPath3 = gv.DijkstraShortestPath(0, 2);

    for (auto nodeIndex : shortestPath3)
        std::cout << gv.GetData(nodeIndex) << " ";
    std::cout << '\n';

    std::cout << "A* shortest path from 0,0 to 2,0" << '\n';
    auto shortestPath4 = gv.AStar(0, 2);
    
    for (auto nodeIndex : shortestPath4)
        std::cout << gv.GetData(nodeIndex) << " ";
    std::cout << '\n';

    return 0;
}
