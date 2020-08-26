#include "../vector/vector.hpp"
#include <limits>

template <typename T>
class Graph
{
public:
    struct Node
    {
        T data;
        mutable bool visited = false;
        mutable float distance;
        mutable int parentNodeIndex;
    };
    struct Edge  // assume undirected graph
    {
        unsigned int nodeIndex1;
        unsigned int nodeIndex2;
        float weight;
    };
public:
    void AddNode(const T &data) { mNodes.InsertLast(Node{data}); }
    void AddEdge(unsigned int nodeIndex1, unsigned int nodeIndex2, float weight = 1.0f) { mEdges.InsertLast(Edge{nodeIndex1, nodeIndex2, weight}); }

    int GetUnvisitedAdjacentNodeIndex(unsigned int nodeIndex) const;

    template <class F>
    void BreadthFirstSearch(unsigned int startNodeIndex, const F &f) const;
    template <class F>
    void DepthFirstSearch(unsigned int startNodeIndex, const F &f) const;
    template <typename F>
    void DepthFirstSearchRecursive(unsigned int nodeIndex, const F&f);

    Vector<Vector<const Node*>> DijkstraShortestPath(unsigned int startNodeIndex) const;
private:
    static const float INF;
    Vector<Node> mNodes;   // list of nodes
    Vector<Edge> mEdges;   // list of edges
};

template <typename T>
const float Graph<T>::INF = std::numeric_limits<float>::max();

template <typename T>
int Graph<T>::GetUnvisitedAdjacentNodeIndex(unsigned int nodeIndex) const
{
    for (Edge edge : mEdges)
        if (nodeIndex == edge.nodeIndex1 && !mNodes[edge.nodeIndex2].visited)
            return edge.nodeIndex2;
        else if (nodeIndex == edge.nodeIndex2 && !mNodes[edge.nodeIndex1].visited)
            return edge.nodeIndex1;

    return -1;
}

#include "../ADT/queue/queue.hpp"

template <typename T>
template <class F>
void Graph<T>::BreadthFirstSearch(unsigned int startNodeIndex, const F &f) const
{
    Queue<unsigned int> nodeQueue;

    f(mNodes[startNodeIndex]);              // visit node
    mNodes[startNodeIndex].visited = true;  // mark node as visited
    nodeQueue.Enqueue(startNodeIndex);      // insert node into queue

    while (!nodeQueue.Empty())
    {
        unsigned int currentNodeIndex = nodeQueue.Front();

        int unvisitedAdjacentNodeIndex;
        while ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNodeIndex)) != -1)
        {
            f(mNodes[unvisitedAdjacentNodeIndex]);                                  // visit node
            mNodes[unvisitedAdjacentNodeIndex].visited = true;                      // mark node as visited
            nodeQueue.Enqueue(static_cast<unsigned>(unvisitedAdjacentNodeIndex));   // insert node into queue
        }

        nodeQueue.Dequeue();
    }

    for (const Node &node : mNodes)   // reset visited flag
        node.visited = false;
}

#include "../ADT/stack/stack.hpp"

template <typename T>
template <class F>
void Graph<T>::DepthFirstSearch(unsigned int startNodeIndex, const F &f) const
{
    Stack<unsigned int> nodeStack;

    f(mNodes[startNodeIndex]);                  // visit node
    mNodes[startNodeIndex].visited = true;      // mark node as visited
    nodeStack.Push(startNodeIndex);             // push node onto stack

    while (!nodeStack.Empty())
    {
        unsigned int currentNodeIndex = nodeStack.Top();

        int unvisitedAdjacentNodeIndex;
        if ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNodeIndex)) != -1)
        {
            f(mNodes[unvisitedAdjacentNodeIndex]);                                 // visit node
            mNodes[unvisitedAdjacentNodeIndex].visited = true;                     // mark node as visited
            nodeStack.Push(static_cast<unsigned>(unvisitedAdjacentNodeIndex));     // push node onto stack
        }
        else    
            nodeStack.Pop();
    }

    for (Node const &node : mNodes)  // reset visited flag
        node.visited = false;
}

template <typename T>
template <typename F>
void Graph<T>::DepthFirstSearchRecursive(unsigned int nodeIndex, const F&f)
{
    static unsigned int level = 0U;

    level++;

    f(mNodes[nodeIndex]);                // visit node
    mNodes[nodeIndex].visited = true;    // mark node as visited

    unsigned int nextNode;
    while ((nextNode = GetUnvisitedAdjacentNodeIndex(nodeIndex)) != -1)
        DepthFirstSearchRecursive(nextNode, f);           // push node onto (implicit) stack

    level--;

    if (level == 0U)
        for (Node const &node : mNodes)  // reset visited flag
            node.visited = false;
}

#define TYPE_PARAM_HEAP_QUEUE
#include "../ADT/priority queue/priority_queue.hpp"

template <typename T>
Vector<Vector<const typename Graph<T>::Node*>> Graph<T>::DijkstraShortestPath(unsigned int startNodeIndex) const
{
    for (unsigned int i = 0; i < mNodes.Size(); i++)
    {
        mNodes[i].distance = i == startNodeIndex ? 0.0f : INF;  // all nodes but starting node start with INF as distance
        mNodes[i].visited = false;
        mNodes[i].parentNodeIndex = -1;
    }

    auto comparator = [this](unsigned int v1, unsigned int v2) -> bool { return this->mNodes[v1].distance < this->mNodes[v2].distance; };
    PriorityQueue<unsigned int, decltype(comparator)> nodePriorityQueue(comparator);

    nodePriorityQueue.Insert(startNodeIndex);                                    // insert start node into priority queue

    while (!nodePriorityQueue.Empty())
    {
        unsigned int currentNodeIndex;
        while (mNodes[currentNodeIndex = nodePriorityQueue.Peek()].visited)      // get first node in priority queue
            nodePriorityQueue.Remove();                                          // if node has been already visited remove node (duplicate)

        mNodes[currentNodeIndex].visited = true;                                 // mark node as visited

        for (Edge edge : mEdges)                                                 // find all unvisited adjacent nodes
        {
            unsigned int unvisitedAdjacentNodeIndex;

            if (edge.nodeIndex1 == currentNodeIndex && !mNodes[edge.nodeIndex2].visited)
                unvisitedAdjacentNodeIndex = edge.nodeIndex2;
            else if (edge.nodeIndex2 == currentNodeIndex && !mNodes[edge.nodeIndex1].visited)
                unvisitedAdjacentNodeIndex = edge.nodeIndex1;
            else
                continue;

            float currentDistance = mNodes[unvisitedAdjacentNodeIndex].distance;
            float newDistance = mNodes[currentNodeIndex].distance + edge.weight;

            if (newDistance < currentDistance)                                   // if distance is shorter relax edge 
            {
                mNodes[unvisitedAdjacentNodeIndex].distance = newDistance;
                mNodes[unvisitedAdjacentNodeIndex].parentNodeIndex = currentNodeIndex;
                
                nodePriorityQueue.Insert(unvisitedAdjacentNodeIndex);            // insert node into queue (duplicates)
            }
        }

        nodePriorityQueue.Remove();                                              // remove node from priority queue
    }

    Vector<Vector<typename Graph<T>::Node const*>> paths;
    paths.Resize(mNodes.Size());

    for (unsigned int i = 0; i < mNodes.Size(); i++)
    {
        if (i == startNodeIndex)
        {
            paths[i].InsertFirst(&mNodes[i]);
            continue;
        }

        unsigned int currentNodeIndex = i;
        do 
        {
            paths[i].InsertFirst(&mNodes[currentNodeIndex]);
            currentNodeIndex = mNodes[currentNodeIndex].parentNodeIndex;
        } while (currentNodeIndex != startNodeIndex);

        paths[i].InsertFirst(&mNodes[startNodeIndex]);
    }
    
    return paths;
}
