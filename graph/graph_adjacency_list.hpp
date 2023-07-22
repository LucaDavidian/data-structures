#include "../vector/vector.hpp"
#include <limits>

template <typename T, typename Heuristic>
class Graph;

// euclidean distance (underestimating)
class EuclideanHeuristic
{
public:
    template <typename T, typename Heuristic>
    float operator()(Graph<T, Heuristic> const *graph, unsigned int nodeIndex, unsigned int endNodeIndex) const
    {
        T diff = graph->mNodes[nodeIndex].data - graph->mNodes[endNodeIndex].data;
        return diff.Length() * 0.01f;
    }
};

template <typename T, typename Heuristic = EuclideanHeuristic>
class Graph
{
friend Heuristic;

public:
    struct Node   
    {
        T data;
        mutable bool visited;
        mutable float distance;
        mutable int parentNodeIndex;
        mutable int parentEdgeIndex;
        mutable float heuristic;
    };

    struct Edge  
    {
        unsigned int sourceNodeIndex;        
        unsigned int destinationNodeIndex;    
        float weight;
    };

public:
    using Path = Vector<Graph::Node const *>;

public:
    void AddNode(const T &data);
    
    void AddEdge(unsigned int nodeIndex1, unsigned int nodeIndex2, float weight = 1.0f, bool directed = false);

    int GetUnvisitedAdjacentNodeIndex(unsigned int nodeIndex) const;

    template <typename F>
    void BreadthFirstSearch(unsigned int startNodeIndex, const F&f);
    
    template <typename F>
    void DepthFirstSearch(unsigned int startNodeIndex, const F&f);
    
    template <typename F>
    void DepthFirstSearchRecursive(unsigned int nodeIndex, const F&f);

    Vector<Path> DijkstraShortestPath(unsigned int startNodeIndex) const; 
    
    Path DijkstraShortestPath(unsigned int startNodeIndex, unsigned int endNodeIndex) const; 
    
    Path AStar(unsigned int startNodeIndex, unsigned int endNodeIndex) const;

private:
    static const float INF;
    Vector<Node> mNodes;                           // ordered list of nodes
    Vector<Edge> mEdges;                           // ordered list of edges
    Vector<Vector<unsigned int>> mAdjacencyList;   // ordered list of lists of references (indices) to edges in the edge list
};

template <typename T, typename Heuristic>
const float Graph<T, Heuristic>::INF = std::numeric_limits<float>::max();

template <typename T, typename Heuristic>
void Graph<T, Heuristic>::AddNode(const T &data)
{
    mNodes.InsertLast(Node{data});
    mAdjacencyList.InsertLast(Vector<unsigned int>());  // to each node corresponds a list of edges
}

template <typename T, typename Heuristic>
void Graph<T, Heuristic>::AddEdge(unsigned int nodeIndex1, unsigned int nodeIndex2, float weight, bool directed)
{
    if (nodeIndex1 < mNodes.Size() && nodeIndex2 < mNodes.Size())
    {
        mEdges.InsertLast(Edge{nodeIndex1, nodeIndex2, weight});             // insert edge in list
        mAdjacencyList[nodeIndex1].InsertLast(mEdges.Size() - 1);       // insert reference to edge in adjacency list
    
        if (!directed)   // if edge is undirected add another edge in opposite direction
        {
            mEdges.InsertLast(Edge{nodeIndex2, nodeIndex1, weight});
            mAdjacencyList[nodeIndex2].InsertLast(mEdges.Size() - 1);  
        }
    }
}

template <typename T, typename Heuristic>
int Graph<T, Heuristic>::GetUnvisitedAdjacentNodeIndex(unsigned int nodeIndex) const
{
    for (unsigned int edgeIndex : mAdjacencyList[nodeIndex])
        if (!mNodes[mEdges[edgeIndex].destinationNodeIndex].visited)
            return mEdges[edgeIndex].destinationNodeIndex;

    return -1;
}

#include "../ADT/queue/queue.hpp"

template <typename T, typename Heuristic>
template <typename F>
void Graph<T, Heuristic>::BreadthFirstSearch(unsigned int startNodeIndex, const F&f)
{
    Queue<unsigned int> nodeQueue;

    f(mNodes[startNodeIndex]);                  // visit node
    mNodes[startNodeIndex].visited = true;      // mark node as visited
    nodeQueue.Enqueue(startNodeIndex);          // insert note into queue

    while (!nodeQueue.Empty())
    {
        unsigned int currentNode = nodeQueue.Front();

        int unvisitedAdjacentNodeIndex;
        while ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNode)) != -1)
        {
            f(mNodes[unvisitedAdjacentNodeIndex]);                                     // visit node
            mNodes[unvisitedAdjacentNodeIndex].visited = true;                         // mark node as visited
            nodeQueue.Enqueue(static_cast<unsigned>(unvisitedAdjacentNodeIndex));      // insert note into queue
        }

        nodeQueue.Dequeue();
    }

    for (Node const &node : mNodes)   // reset visited flag
        node.visited = false;
}

#include "../ADT/stack/stack.hpp"

template <typename T, typename Heuristic>
template <typename F>
void Graph<T, Heuristic>::DepthFirstSearch(unsigned int startNodeIndex, const F&f)
{
    Stack<unsigned int> nodeStack;

    f(mNodes[startNodeIndex]);                   // visit node
    mNodes[startNodeIndex].visited = true;       // mark node as visited
    nodeStack.Push(startNodeIndex);              // push node onto stack

    while (!nodeStack.Empty())
    {
        unsigned int currentNode = nodeStack.Top();

        int unvisitedAdjacentNodeIndex;
        if ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNode)) != -1)
        {
            f(mNodes[unvisitedAdjacentNodeIndex]);                                  // visit node
            mNodes[unvisitedAdjacentNodeIndex].visited = true;                      // mark node as visited
            nodeStack.Push(static_cast<unsigned>(unvisitedAdjacentNodeIndex));      // push node onto stack
        }
        else
            nodeStack.Pop();
    }

    for (Node const &node : mNodes)   // reset visited flag
        node.visited = false;
}

template <typename T, typename Heuristic>
template <typename F>
void Graph<T, Heuristic>::DepthFirstSearchRecursive(unsigned int nodeIndex, const F&f)
{
    static unsigned int level = 0U;

    level++;

    f(mNodes[nodeIndex]);                // visit node
    mNodes[nodeIndex].visited = true;    // mark node as visited

    int nextNode;
    while ((nextNode = GetUnvisitedAdjacentNodeIndex(nodeIndex)) != -1)
        DepthFirstSearchRecursive(nextNode, f);           // push node onto (implicit) stack

    level--;

    if (level == 0U)
        for (Node const &node : mNodes)  // reset visited flag
            node.visited = false;
}

#define TYPE_PARAM_HEAP_QUEUE
#include "../ADT/priority queue/priority_queue.hpp"

template <typename T, typename Heuristic>
Vector<typename Graph<T, Heuristic>::Path> Graph<T, Heuristic>::DijkstraShortestPath(unsigned int startNodeIndex) const
{
    for (unsigned int i = 0; i < mNodes.Size(); i++)
    {
        mNodes[i].distance = i == startNodeIndex ? 0.0f : INF;    // all nodes but starting node start with INF as distance
        mNodes[i].visited = false;
        mNodes[i].parentNodeIndex = -1;
    }

    auto comparator = [this](unsigned int v1, unsigned int v2) -> bool { return mNodes[v1].distance < mNodes[v2].distance; };
    PriorityQueue<unsigned int, decltype(comparator)> nodePriorityQueue(comparator);

    nodePriorityQueue.Insert(startNodeIndex);                                 // insert start node index into priority queue

    while (!nodePriorityQueue.Empty())
    { 
        unsigned int currentNodeIndex = nodePriorityQueue.Peek();             // get first node in priority queue 

        mNodes[currentNodeIndex].visited = true;                              // mark node as visited
        nodePriorityQueue.Remove();                                           // remove node from priority queue
        
        for (unsigned int edgeIndex : mAdjacencyList[currentNodeIndex])       // find all unvisited adjacent nodes
        {
            if (mNodes[mEdges[edgeIndex].destinationNodeIndex].visited)
                continue;  

            unsigned int unvisitedAdjacentNodeIndex = mEdges[edgeIndex].destinationNodeIndex;             

            float currentDistance = mNodes[unvisitedAdjacentNodeIndex].distance;
            float newDistance = mNodes[currentNodeIndex].distance + mEdges[edgeIndex].weight;

            if (newDistance < currentDistance)                                // if distance is shorter relax edge 
            {
                if (nodePriorityQueue.Find(unvisitedAdjacentNodeIndex))
                    nodePriorityQueue.Remove(unvisitedAdjacentNodeIndex);

                mNodes[unvisitedAdjacentNodeIndex].distance = newDistance;
                mNodes[unvisitedAdjacentNodeIndex].parentNodeIndex = currentNodeIndex;

                nodePriorityQueue.Insert(unvisitedAdjacentNodeIndex);         // insert node into queue (duplicates)
            }
        }
    }

    Vector<Path> paths;
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

template <typename T, typename Heuristic>
typename Graph<T, Heuristic>::Path Graph<T, Heuristic>::DijkstraShortestPath(unsigned int startNodeIndex, unsigned int endNodeIndex) const
{
    for (const Node &node : mNodes)
    {
        node.visited = false;
        node.distance = INF;
        node.parentNodeIndex = -1;
    }
    mNodes[startNodeIndex].distance = 0.0f;

    auto const &comparator = [this] (unsigned int nodeIndex1, unsigned int nodeIndex2) { return mNodes[nodeIndex1].distance < mNodes[nodeIndex2].distance;};
    PriorityQueue<unsigned int, decltype(comparator)> nodePriorityQueue(comparator);

    nodePriorityQueue.Insert(startNodeIndex);

    while (!nodePriorityQueue.Empty())
    {
        unsigned int currentNodeIndex = nodePriorityQueue.Peek();

        if (currentNodeIndex == endNodeIndex)   // stop algorithm if end node is found
            break;

        mNodes[currentNodeIndex].visited = true;
        nodePriorityQueue.Remove(); 

        for (unsigned int edgeIndex : mAdjacencyList[currentNodeIndex])  
        {
            if (mNodes[mEdges[edgeIndex].destinationNodeIndex].visited)
                continue;

            unsigned int unvisitedAdjacentNodeIndex = mEdges[edgeIndex].destinationNodeIndex;
            
            float distance = mNodes[unvisitedAdjacentNodeIndex].distance;
            float newDistance = mNodes[currentNodeIndex].distance + mEdges[edgeIndex].weight;

            if (newDistance < distance)
            {
                if (nodePriorityQueue.Find(unvisitedAdjacentNodeIndex))
                    nodePriorityQueue.Remove(unvisitedAdjacentNodeIndex);

                mNodes[unvisitedAdjacentNodeIndex].distance = newDistance;
                mNodes[unvisitedAdjacentNodeIndex].parentNodeIndex = currentNodeIndex;

                nodePriorityQueue.Insert(unvisitedAdjacentNodeIndex);
            }
        }       
    }

    Path path;

    unsigned int nodeIndex = endNodeIndex;
    while (nodeIndex != startNodeIndex)
    {
        path.InsertFirst(&mNodes[nodeIndex]);
        nodeIndex = mNodes[nodeIndex].parentNodeIndex;
    }

    path.InsertFirst(&mNodes[startNodeIndex]);

    return path;
}

template <typename T, typename Heuristic>
typename Graph<T, Heuristic>::Path Graph<T, Heuristic>::AStar(unsigned int startNodeIndex, unsigned int endNodeIndex) const
{
    for (Node const &node : mNodes)
    {
        node.visited = false;
        node.distance = INF;
        node.parentEdgeIndex = -1;
        node.heuristic = 0.0f;
    }
    mNodes[startNodeIndex].distance = 0.0f;

    auto comparator = [this](const unsigned int &nodeIndex1, const unsigned int &nodeIndex2) -> bool { return mNodes[nodeIndex1].distance + mNodes[nodeIndex1].heuristic < mNodes[nodeIndex2].distance + mNodes[nodeIndex2].heuristic; };
    PriorityQueue<unsigned int, decltype(comparator)> nodePriorityQueue(comparator);

    nodePriorityQueue.Insert(startNodeIndex);

    while (!nodePriorityQueue.Empty())
    {
        unsigned int currentNodeIndex = nodePriorityQueue.Peek();   // get next node from priority queue

        if (currentNodeIndex == endNodeIndex)  // found end node
            break;

        mNodes[currentNodeIndex].visited = true;
        nodePriorityQueue.Remove();

        for (unsigned int edgeIndex : mAdjacencyList[currentNodeIndex])
        {
            unsigned int adjacentNodeIndex = mEdges[edgeIndex].destinationNodeIndex;

            float currentDistance = mNodes[adjacentNodeIndex].distance;
            float newDistance = mNodes[currentNodeIndex].distance + mEdges[edgeIndex].weight;

            if (newDistance < currentDistance)
            {
                if (nodePriorityQueue.Find(adjacentNodeIndex))
                    nodePriorityQueue.Remove(adjacentNodeIndex);

                mNodes[adjacentNodeIndex].distance = newDistance;
                mNodes[adjacentNodeIndex].parentNodeIndex = currentNodeIndex;

                mNodes[adjacentNodeIndex].heuristic = Heuristic()(this, adjacentNodeIndex, endNodeIndex);  // calculate node's heuristic 

                nodePriorityQueue.Insert(adjacentNodeIndex);              // insert into priority queue
            }
        }
    }

    Path path;

    unsigned int nodeIndex = endNodeIndex;
    while (nodeIndex != startNodeIndex)
    {
        path.InsertFirst(&mNodes[nodeIndex]);
        nodeIndex = mNodes[nodeIndex].parentNodeIndex;
    }

    path.InsertFirst(&mNodes[startNodeIndex]);

    return path;
}