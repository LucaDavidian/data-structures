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
    void AddNode(const T &data) { mNodes.InsertLast(Node{data}); }

    void AddEdge(unsigned int nodeIndex1, unsigned int nodeIndex2, float weight = 1.0f, bool directed = false) 
    { 
        if (nodeIndex1 < mNodes.Size() && nodeIndex2 < mNodes.Size())
        {
            mEdges.InsertLast(Edge{nodeIndex1, nodeIndex2, weight}); 

            if (!directed)
                mEdges.InsertLast(Edge{nodeIndex2, nodeIndex1, weight}); 
        }
    }

    int GetUnvisitedAdjacentNodeIndex(unsigned int nodeIndex) const;

    template <class F>
    void BreadthFirstSearch(unsigned int startNodeIndex, const F &f) const;
    
    template <class F>
    void DepthFirstSearch(unsigned int startNodeIndex, const F &f) const;
    
    template <typename F>
    void DepthFirstSearchRecursive(unsigned int nodeIndex, const F&f);

    Vector<Path> DijkstraShortestPath(unsigned int startNodeIndex) const;

    Path DijkstraShortestPath(unsigned int startNodeIndex, unsigned int endNodeIndex) const; 

    Path AStar(unsigned int startNodeIndex, unsigned int endNodeIndex) const;

private:
    static const float INF;
    Vector<Node> mNodes;   // list of nodes
    Vector<Edge> mEdges;   // list of edges

    void GetHeuristic(unsigned int nodeIndex, unsigned int startNodeIndex) const;
};

template <typename T, typename Heuristic>
const float Graph<T, Heuristic>::INF = std::numeric_limits<float>::max();

template <typename T, typename Heuristic>
int Graph<T, Heuristic>::GetUnvisitedAdjacentNodeIndex(unsigned int nodeIndex) const
{
    for (const Edge &edge : mEdges)
        if (nodeIndex == edge.sourceNodeIndex && !mNodes[edge.destinationNodeIndex].visited)
            return edge.destinationNodeIndex;

    return -1;
}

#include "../ADT/queue/queue.hpp"

template <typename T, typename Heuristic>
template <class F>
void Graph<T, Heuristic>::BreadthFirstSearch(unsigned int startNodeIndex, const F &f) const
{
    Queue<unsigned int> nodeQueue;

    f(mNodes[startNodeIndex]);              // visit start node
    mNodes[startNodeIndex].visited = true;  // mark start node as visited
    nodeQueue.Enqueue(startNodeIndex);      // insert start node index into queue

    while (!nodeQueue.Empty())
    {
        unsigned int currentNodeIndex = nodeQueue.Front();  // get current node index from queue

        int unvisitedAdjacentNodeIndex;
        while ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNodeIndex)) != -1)
        {
            f(mNodes[unvisitedAdjacentNodeIndex]);                                  // visit adjacent node
            mNodes[unvisitedAdjacentNodeIndex].visited = true;                      // mark adjacent node as visited
            nodeQueue.Enqueue(static_cast<unsigned>(unvisitedAdjacentNodeIndex));   // insert adjacent node index into queue
        }

        nodeQueue.Dequeue();  // remove current node index from queue
    }

    for (const Node &node : mNodes)   // reset visited flag
        node.visited = false;
}

#include "../ADT/stack/stack.hpp"

template <typename T, typename Heuristic>
template <class F>
void Graph<T, Heuristic>::DepthFirstSearch(unsigned int startNodeIndex, const F &f) const
{
    Stack<unsigned int> nodeStack;

    f(mNodes[startNodeIndex]);                  // visit start node
    mNodes[startNodeIndex].visited = true;      // mark start node as visited
    nodeStack.Push(startNodeIndex);             // push start node index onto stack

    while (!nodeStack.Empty())
    {
        unsigned int currentNodeIndex = nodeStack.Top();  // get current node index from stack

        int unvisitedAdjacentNodeIndex;
        if ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNodeIndex)) != -1)
        {
            f(mNodes[unvisitedAdjacentNodeIndex]);                                 // visit adjacent node
            mNodes[unvisitedAdjacentNodeIndex].visited = true;                     // mark adjacent node as visited
            nodeStack.Push(static_cast<unsigned>(unvisitedAdjacentNodeIndex));     // push adjacent node index onto stack
        }
        else    
            nodeStack.Pop();   // remove current node index from stack
    }

    for (Node const &node : mNodes)  // reset visited flag
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

template <typename T, typename Heuristic>
Vector<typename Graph<T, Heuristic>::Path> Graph<T, Heuristic>::DijkstraShortestPath(unsigned int startNodeIndex) const
{
    for (unsigned int i = 0; i < mNodes.Size(); i++)
    {
        mNodes[i].distance = i == startNodeIndex ? 0.0f : INF;  // all nodes but starting node start with INF as distance
        mNodes[i].visited = false;
        mNodes[i].parentNodeIndex = -1;
    }

    auto comparator = [this](unsigned int nodeIndex1, unsigned int nodeIndex2) -> bool { return this->mNodes[nodeIndex1].distance < this->mNodes[nodeIndex2].distance; };
    PriorityQueue<unsigned int, decltype(comparator)> nodePriorityQueue(comparator);

    nodePriorityQueue.Insert(startNodeIndex);                                 // insert start node index into priority queue

    while (!nodePriorityQueue.Empty())
    {
        unsigned int currentNodeIndex = nodePriorityQueue.Peek();             // get first node in priority queue 
        nodePriorityQueue.Remove();                                           // remove current node from priority queue
        mNodes[currentNodeIndex].visited = true;                              // mark node as visited

        for (const Edge &edge : mEdges)                                       // find all unvisited adjacent nodes
        {
            if (currentNodeIndex != edge.sourceNodeIndex || mNodes[edge.destinationNodeIndex].visited)
                continue;
            
            unsigned int unvisitedAdjacentNodeIndex = edge.destinationNodeIndex;

            float currentDistance = mNodes[unvisitedAdjacentNodeIndex].distance;
            float newDistance = mNodes[currentNodeIndex].distance + edge.weight;

            if (newDistance < currentDistance)                                            // if distance of adjacent node from current node is shorter: 
            {
                if (nodePriorityQueue.Find(unvisitedAdjacentNodeIndex))
                    nodePriorityQueue.Remove(unvisitedAdjacentNodeIndex);                 // remove node from queue if already present
                
                mNodes[unvisitedAdjacentNodeIndex].distance = newDistance;                // relax edge distance
                mNodes[unvisitedAdjacentNodeIndex].parentNodeIndex = currentNodeIndex;    // set parent node  
                
                nodePriorityQueue.Insert(unvisitedAdjacentNodeIndex);                     // insert node into queue  
            }
        }
    }

    // create shortest paths
    Vector<Path> paths;
    paths.Resize(mNodes.Size());

    for (unsigned int i = 0; i < mNodes.Size(); i++)
    {
        unsigned int currentNodeIndex = i;

        while (currentNodeIndex != startNodeIndex)
        {
            paths[i].InsertFirst(&mNodes[currentNodeIndex]);
            currentNodeIndex = mNodes[currentNodeIndex].parentNodeIndex;
        } 

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

        nodePriorityQueue.Remove();
        mNodes[currentNodeIndex].visited = true;

        for (const Edge &edge : mEdges) 
        {
            if (currentNodeIndex != edge.sourceNodeIndex || mNodes[edge.destinationNodeIndex].visited)
                continue;
            
            unsigned int unvisitedAdjacentNodeIndex = edge.destinationNodeIndex;
            
            float currentDistance = mNodes[unvisitedAdjacentNodeIndex].distance;
            float newDistance = mNodes[currentNodeIndex].distance + edge.weight;
            
            if (newDistance < currentDistance)
            {
                if (nodePriorityQueue.Find(unvisitedAdjacentNodeIndex))
                    nodePriorityQueue.Remove(unvisitedAdjacentNodeIndex);
                    
                mNodes[unvisitedAdjacentNodeIndex].distance = newDistance;
                mNodes[unvisitedAdjacentNodeIndex].parentNodeIndex = currentNodeIndex;

                nodePriorityQueue.Insert(unvisitedAdjacentNodeIndex);
            }
        }       
    }

    // create shortest path
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
        node.parentNodeIndex = -1;
        node.heuristic = 0.0f;
    }
    mNodes[startNodeIndex].distance = 0.0f;

    auto comparator = [this](unsigned int nodeIndex1, unsigned int nodeIndex2) -> bool { return mNodes[nodeIndex1].distance + mNodes[nodeIndex1].heuristic < mNodes[nodeIndex2].distance + mNodes[nodeIndex2].heuristic; };
    PriorityQueue<unsigned int, decltype(comparator)> nodePriorityQueue(comparator);

    nodePriorityQueue.Insert(startNodeIndex);

    while (!nodePriorityQueue.Empty())
    {
        unsigned int currentNodeIndex = nodePriorityQueue.Peek();

        if (currentNodeIndex == endNodeIndex)   // stop algorithm if end node is found
            break;

        nodePriorityQueue.Remove();
        mNodes[currentNodeIndex].visited = true;

        for (const Edge &edge : mEdges) 
        {
            if (currentNodeIndex != edge.sourceNodeIndex)
                continue;
            
            unsigned int adjacentNodeIndex = edge.destinationNodeIndex;

            float currentDistance = mNodes[adjacentNodeIndex].distance;
            float newDistance = mNodes[currentNodeIndex].distance + edge.weight;

            if (newDistance < currentDistance)
            {
                if (nodePriorityQueue.Find(adjacentNodeIndex))
                    nodePriorityQueue.Remove(adjacentNodeIndex);

                mNodes[adjacentNodeIndex].distance = newDistance;
                mNodes[adjacentNodeIndex].parentNodeIndex = currentNodeIndex;

                mNodes[adjacentNodeIndex].heuristic = Heuristic()(this, adjacentNodeIndex, endNodeIndex);    // calculate node's heuristic 

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
