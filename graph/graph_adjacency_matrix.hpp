#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <exception>
#include <limits>

#define PRINT(s)      std::cout << (s)
#define PRINTLN(s)    PRINT(s) << std::endl;  

class IndexOutOfBoundsException : public std::exception {};

const int INF = std::numeric_limits<int>::max();

/***** Graph - adjacency matrix representation *****/
template <typename T, unsigned int N>
class Graph
{
private:
    struct Vertex
    {
        T data;
        bool isVisited;
        bool isInQueue;
        int distance;
        int parentVertexIndex;
        int heuristic;
    };
public:
    Graph();
    ~Graph() = default;

    bool Empty() const { return mNumVertices == 0; }

    template <typename U>
    void AddVertex(U &&data, int heuristic = 0);
    void AddEdge(int startVertexIndex, int endVertexIndex, int weight = 1, bool directed = false);

    int GetUnvisitedAdjacentVertexIndex(int vertexIndex) const;

    void DepthFirstSearch(int startVertexIndex) const;
    void DepthFirstSearchRecursive(int vertexIndex) const;
    void BreadthFirstSearch(int startVertexIndex) const;

    bool IsConnected(int startVertexIndex, int endVertexIndex) const;
    
    void SingleSourceShortestPath(int startVertexIndex) const;
    void A_Star(int startVertexIndex, int endVertexIndex) const;
private:
    mutable Vertex mVertices[N];
    int mNumVertices;
    int mAdjacencyMatrix[N][N];

    void CalculateTransitiveClosure();
    int mConnectivityTable[N][N];
};

template <typename T, unsigned int N>
Graph<T,N>::Graph() :  mNumVertices(0)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            mAdjacencyMatrix[i][j] = INF;
}

template <typename T, unsigned int N>
template <typename U>
void Graph<T,N>::AddVertex(U &&data, int heuristic)
{
    mVertices[mNumVertices].data = std::forward<U>(data);
    mVertices[mNumVertices].isVisited = false;

    if (heuristic)
        mVertices[mNumVertices].heuristic = heuristic;

    mNumVertices++;
}

template <typename T, unsigned int N>
void Graph<T,N>::AddEdge(int startVertexIndex, int endVertexIndex, int weight, bool directed)
{
    mAdjacencyMatrix[startVertexIndex][endVertexIndex] = weight;

    if (!directed)
        mAdjacencyMatrix[endVertexIndex][startVertexIndex] = weight;
}

template <typename T, unsigned int N>
int Graph<T,N>::GetUnvisitedAdjacentVertexIndex(int vertexIndex) const
{
    for (int i = 0; i < mNumVertices; i++)
    {
        if (i == vertexIndex)
            continue;

        if (!mVertices[i].isVisited && mAdjacencyMatrix[vertexIndex][i] != INF)
            return i;
    }

    return -1;
}

#include "../ADT/stack/stack.hpp"

template <typename T, unsigned int N>
void Graph<T,N>::DepthFirstSearch(int startVertexIndex) const
{
    if (startVertexIndex < 0 || startVertexIndex >= mNumVertices)
        throw IndexOutOfBoundsException();
        
    if (Empty())
        return;

    Stack<int> stack;

    PRINT("depth first search: ");

    PRINT(mVertices[startVertexIndex].data);           // 1. visit vertex
    mVertices[startVertexIndex].isVisited = true;      // 2. mark vertex as visited
    stack.Push(startVertexIndex);                      // 3. push vertex onto stack
 
    while (!stack.Empty())
    {
        int currentVertexIndex = stack.Top();

        int unvisitedAdjacentVertexIndex = GetUnvisitedAdjacentVertexIndex(currentVertexIndex);

        if (unvisitedAdjacentVertexIndex != -1)
        {
            PRINT(mVertices[unvisitedAdjacentVertexIndex].data);         // 1. visit vertex
            mVertices[unvisitedAdjacentVertexIndex].isVisited = true;    // 2. mark vertex as visited
            stack.Push(unvisitedAdjacentVertexIndex);                    // 3. push vertex onto stack
        } 
        else
            stack.Pop();
    }

    PRINTLN(""); PRINTLN("");

    // reset vertex flag 
    for (int i = 0; i < mNumVertices; i++)
        mVertices[i].isVisited = false;
}

template <typename T, unsigned int N>
void Graph<T,N>::DepthFirstSearchRecursive(int vertexIndex) const
{
    static int level = 0;

    if (vertexIndex < 0 || vertexIndex >= mNumVertices)
        throw IndexOutOfBoundsException();
        
    if (Empty())
        return;

    level++;

    if (level == 1)
        PRINT("depth first search (recursive implementation): ");

    PRINT(mVertices[vertexIndex].data);             // 1. visit vertex
    mVertices[vertexIndex].isVisited = true;        // 2. mark vertex as visited

    int unvisitedAdjacentVertexIndex;
    
    while ((unvisitedAdjacentVertexIndex = GetUnvisitedAdjacentVertexIndex(vertexIndex)) != -1)
        DepthFirstSearchRecursive(unvisitedAdjacentVertexIndex);     // 3. push vertex onto (implicit) stack

    if (level == 1)
    {
        PRINTLN(""); PRINTLN("");

        // reset vertex flag 
        for (int i = 0; i < mNumVertices; i++)
            mVertices[i].isVisited = false;
    }

    level--;
}

#include "../ADT/queue/queue.hpp"

template <typename T, unsigned int N>
void Graph<T,N>::BreadthFirstSearch(int startVertexIndex) const
{
    if (startVertexIndex < 0 || startVertexIndex >= mNumVertices)
        throw IndexOutOfBoundsException();

    if (Empty())
        return;

    Queue<int> queue;

    PRINT("breadth first search: ");

    PRINT(mVertices[startVertexIndex].data);            // 1. visit vertex
    mVertices[startVertexIndex].isVisited = true;       // 2. mark vertex as visited
    queue.Enqueue(startVertexIndex);                    // 3. put vertex into queue

    while (!queue.Empty())
    {
        int currentVertexIndex = queue.Front();

        int unvisitedAdjacentVertexIndex = GetUnvisitedAdjacentVertexIndex(currentVertexIndex);

        if (unvisitedAdjacentVertexIndex != -1)
        {
            PRINT(mVertices[unvisitedAdjacentVertexIndex].data);
            mVertices[unvisitedAdjacentVertexIndex].isVisited = true;
            queue.Enqueue(unvisitedAdjacentVertexIndex);
        }
        else    
            queue.Dequeue();
    }

    PRINTLN(""); PRINTLN("");

    // reset vertex flag     
    for (int i = 0; i < mNumVertices; i++)
        mVertices[i].isVisited = false;
}

#define TYPE_ERASURE_HEAP_QUEUE
#include "../ADT/priority queue/priority_queue.hpp"

// Dijkstra's single source shortest path algorithm
template <typename T, unsigned int N>
void Graph<T,N>::SingleSourceShortestPath(int startVertexIndex) const 
{
    if (startVertexIndex < 0 || startVertexIndex >= mNumVertices)
        throw IndexOutOfBoundsException();

    if (Empty())
        return;

    for (int i = 0; i < mNumVertices; i++)
    {
        mVertices[i].isVisited = false;
        mVertices[i].isInQueue = false;
        mVertices[i].parentVertexIndex = -1;
        mVertices[i].distance = startVertexIndex == i ? 0 : INF;
    }

    PriorityQueue<int> priorityQueue([this](const int &v1, const int &v2) {return this->mVertices[v1].distance < this->mVertices[v2].distance; });

    priorityQueue.Insert(startVertexIndex);
    mVertices[startVertexIndex].isInQueue = true;

    while (!priorityQueue.Empty())
    {
        int currentVertexIndex = priorityQueue.Peek();      // 1. get vertex with min distance
        mVertices[currentVertexIndex].isVisited = true;     // 2. mark vertex as visited
        priorityQueue.Remove();                             // 3. remove vertex from priority queue

        for (int i = 0; i < mNumVertices; i++)
        {
            if (i == currentVertexIndex || mVertices[i].isVisited || mAdjacencyMatrix[currentVertexIndex][i] == INF)
                continue;

            int currentDistance = mVertices[i].distance;
            int newDistance = mVertices[currentVertexIndex].distance + mAdjacencyMatrix[currentVertexIndex][i];

            if (newDistance < currentDistance)  // 4. if distance is less, update path
            {
                mVertices[i].distance = newDistance;
                mVertices[i].parentVertexIndex = currentVertexIndex;

                if (!mVertices[i].isInQueue)    // 5. if vertex not in priority queue, put into queue and mark as in queue
                {
                    priorityQueue.Insert(i);
                    mVertices[i].isInQueue = true;
                }
            }
        }
    }

    // display path
    for (int i = 0; i < mNumVertices; i++)
    {
        if (i == startVertexIndex)
            continue;
            
        PRINT("Shortest path from "); 
        PRINT(mVertices[startVertexIndex].data); 
        PRINT(" to "); 
        PRINT(mVertices[i].data);
        PRINT(": ");

        int path[N];
        int j = 0;

        path[j++] = i;
        int index = i;

        if (mVertices[i].parentVertexIndex == -1)
        {
            PRINTLN("no path");
            continue;
        }

        while (index != startVertexIndex)
        {
            path[j++] = mVertices[index].parentVertexIndex;
            index = mVertices[index].parentVertexIndex;
        }

        int cost = 0;

        while (--j >= 0)
        {
            PRINT(mVertices[path[j]].data);
            
            if (j)
                cost += mAdjacencyMatrix[path[j]][path[j - 1]];
        }

        PRINT(" cost: "); PRINT(cost);

        PRINTLN("");    
    }

    PRINTLN(""); 

    // reset vertex flag     
    for (int i = 0; i < mNumVertices; i++)
        mVertices[i].isVisited = false;      
}

template <typename T, unsigned int N>
void Graph<T,N>::A_Star(int startVertexIndex, int endVertexIndex) const
{
    if (startVertexIndex < 0 || startVertexIndex >= mNumVertices || endVertexIndex < 0 || endVertexIndex >= mNumVertices)
        throw IndexOutOfBoundsException();

    if (Empty())
        return;

    for (int i = 0; i < mNumVertices; i++)
    {
        mVertices[i].isVisited = false;
        mVertices[i].isInQueue = false;
        mVertices[i].parentVertexIndex = -1;
        mVertices[i].distance = i == startVertexIndex ? 0 : INF;
    }

    PriorityQueue<int> priorityQueue([this](const int &v1, const int &v2) {return mVertices[v1].distance + mVertices[v1].heuristic < mVertices[v2].distance + mVertices[v2].heuristic; });

    priorityQueue.Insert(startVertexIndex);
    mVertices[startVertexIndex].isInQueue = true;    

    while (!priorityQueue.Empty())
    {
        int currentVertexIndex = priorityQueue.Peek();
        mVertices[currentVertexIndex].isVisited = true;
        priorityQueue.Remove();

        if (currentVertexIndex == endVertexIndex)
            break;

        for (int i = 0; i < mNumVertices; i++)
        {
            if (i == currentVertexIndex || mVertices[i].isVisited || mAdjacencyMatrix[currentVertexIndex][i] == INF)
                continue;

            int currentDistance = mVertices[i].distance;
            int newDistance = mVertices[currentVertexIndex].distance + mAdjacencyMatrix[currentVertexIndex][i];

            if (newDistance < currentDistance)
            {
                mVertices[i].distance = newDistance;
                mVertices[i].parentVertexIndex = currentVertexIndex;

                if (!mVertices[i].isInQueue)
                {
                    priorityQueue.Insert(i);
                    mVertices[i].isInQueue = true;
                }
            }
        }
    }

    // display path
    PRINT("Shortest path from "); 
    PRINT(mVertices[startVertexIndex].data); 
    PRINT(" to "); 
    PRINT(mVertices[endVertexIndex].data);
    PRINT(": ");

    int path[N], i = 0;
    path[i] = endVertexIndex;

    while (path[i] != startVertexIndex)
    {
        path[i + 1] = mVertices[path[i]].parentVertexIndex;
        i++;
    }

    while (i >= 0)
        PRINT(mVertices[path[i--]].data);

    PRINTLN(""); PRINTLN("");

    // reset vertex flag     
    for (int i = 0; i < mNumVertices; i++)
        mVertices[i].isVisited = false;  
}

#endif  // GRAPH_H