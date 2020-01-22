#ifndef GRAPH_H
#define GRAPH_H

#include "../vector/vector.hpp"
#include "../list/singly_linked_list.hpp"
#include <iostream>
#include <exception>
#include <limits>

#define PRINT(s)      std::cout << (s)
#define PRINTLN(s)    PRINT(s) << std::endl;  

class IndexOutOfBoundsException : public std::exception {};

const int INF = std::numeric_limits<int>::max();

/***** Graph - adjacency list representation ****/
template <typename T> 
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
    };
    struct AdjacencyData
    {
        int vertexIndex;
        int edgeWeight;
    };
public:
    bool Empty() const { return mVertices.Empty(); }

    template <typename U>
    void AddVertex(U &&data);
    void AddEdge(int startVertexIndex, int endVertexIndex, int weight = 1, bool directed = false);

    typename SinglyLinkedList<AdjacencyData>::Iterator GetUnvisitedAdjacentVertex(int vertexIndex) const;

    void DepthFirstSearch(int startVertexIndex) const;
    void DepthFirstSearchRecursive(int vertexIndex) const;
    void BreadthFirstSearch(int startVertexIndex) const;

    bool IsConnected(int startVertexIndex, int endVertexIndex) const;
    
    void SingleSourceShortestPath(int startVertexIndex) const;
    void A_Star(int startVertexIndex, int endVertexIndex) const;
private:
    mutable Vector<Vertex> mVertices;
    Vector<SinglyLinkedList<AdjacencyData>> mAdjacencyList;

    void CalculateTransitiveClosure();
};

template <typename T>
template <typename U>
void Graph<T>::AddVertex(U &&data)
{
    mVertices.InsertLast(Vertex{std::forward<U>(data), false});
    
    mAdjacencyList.InsertLast(SinglyLinkedList<AdjacencyData>());
}

template <typename T>
void Graph<T>::AddEdge(int startVertexIndex, int endVertexIndex, int weight, bool directed)
{
    mAdjacencyList[startVertexIndex].InsertFirst(AdjacencyData{endVertexIndex, weight});

    if (!directed)
        mAdjacencyList[endVertexIndex].InsertFirst(AdjacencyData{startVertexIndex, weight});
}

template <typename T>
typename SinglyLinkedList<typename Graph<T>::AdjacencyData>::Iterator Graph<T>::GetUnvisitedAdjacentVertex(int vertexIndex) const 
{
    typename SinglyLinkedList<AdjacencyData>::Iterator it = mAdjacencyList[vertexIndex].Begin();

    while (it != mAdjacencyList[vertexIndex].End())
    {
        if (!mVertices[it->vertexIndex].isVisited && it->edgeWeight != INF)
            return it;

        ++it;
    }

    return it;
}

#include "../ADT/stack/stack.hpp"

template <typename T>
void Graph<T>::DepthFirstSearch(int startVertexIndex) const
{
    if (startVertexIndex < 0 || startVertexIndex >= mVertices.Size())
        throw IndexOutOfBoundsException();

    if (Empty())
        return;

    Stack<int> stack;

    PRINT("depth first search: ");

    PRINT(mVertices[startVertexIndex].data);       // 1. visit vertex
    mVertices[startVertexIndex].isVisited = true;  // 2. mark vertex as visited
    stack.Push(startVertexIndex);                  // 3. push vertex onto stack

    while (!stack.Empty())
    {
        int currentVertexIndex = stack.Top();

        typename SinglyLinkedList<AdjacencyData>::Iterator unvisitedAdjacentVertex = GetUnvisitedAdjacentVertex(currentVertexIndex);

        if (unvisitedAdjacentVertex != mAdjacencyList[currentVertexIndex].End())
        {
            PRINT(mVertices[unvisitedAdjacentVertex->vertexIndex].data);         // 1. visit vertex
            mVertices[unvisitedAdjacentVertex->vertexIndex].isVisited = true;    // 2. mark vertex as visited
            stack.Push(unvisitedAdjacentVertex->vertexIndex);                    // 3. push vertex onto stack
        }
        else
            stack.Pop();
    }

    PRINTLN(""); PRINTLN("");

    // reset vertex flag
    for (int i = 0; i < mVertices.Size(); i++)
        mVertices[i].isVisited = false;
}

template <typename T>
void Graph<T>::DepthFirstSearchRecursive(int vertexIndex) const
{
    static int level = 0;
    
    if (vertexIndex < 0 || vertexIndex >= mVertices.Size())
        throw IndexOutOfBoundsException();

    if (Empty())
        return;

    level++;

    if (level == 1)
        PRINT("depth first search (recursive implementation): ");
    
    PRINT(mVertices[vertexIndex].data);       // 1. visit vertex
    mVertices[vertexIndex].isVisited = true;  // 2. mark vertex as visited
                                                   
    typename SinglyLinkedList<AdjacencyData>::Iterator unvisitedAdjacentVertex = mAdjacencyList[vertexIndex].Begin();

    while ((unvisitedAdjacentVertex = GetUnvisitedAdjacentVertex(vertexIndex)) != mAdjacencyList[vertexIndex].End())
        DepthFirstSearchRecursive(unvisitedAdjacentVertex->vertexIndex);    // 3. push vertex onto (implicit) stack

    if (level == 1)
    {
        PRINTLN(""); PRINTLN("");

        // reset vertex flag
        for (int i = 0; i < mVertices.Size(); i++)
            mVertices[i].isVisited = false;
    }

    level--;
}

#include "../ADT/queue/queue.hpp"

template <typename T>
void Graph<T>::BreadthFirstSearch(int startVertexIndex) const
{
    if (startVertexIndex < 0 || startVertexIndex >= mVertices.Size())
        throw IndexOutOfBoundsException();

    if (Empty())
        return;

    Queue<int> queue;

    PRINT("breadth first search: ");

    PRINT(mVertices[startVertexIndex].data);         // 1. visit vertex
    mVertices[startVertexIndex].isVisited = true;    // 2. mark vertex as visited
    queue.Enqueue(startVertexIndex);                 // 3. put vertex into queue

    while (!queue.Empty())
    {
        int currentVertexIndex = queue.Front();

        typename SinglyLinkedList<AdjacencyData>::Iterator unvisitedAdjacentVertex = GetUnvisitedAdjacentVertex(currentVertexIndex);

        if (unvisitedAdjacentVertex != mAdjacencyList[currentVertexIndex].End())
        {
            PRINT(mVertices[unvisitedAdjacentVertex->vertexIndex].data);        // 1. visit vertex
            mVertices[unvisitedAdjacentVertex->vertexIndex].isVisited = true;   // 2. mark vertex as visited
            queue.Enqueue(unvisitedAdjacentVertex->vertexIndex);                // 3. put vertex into queue
        }
        else
            queue.Dequeue();
    }

    PRINTLN(""); PRINTLN("");

    // reset vertex flag
    for (int i = 0; i < mVertices.Size(); i++)
        mVertices[i].isVisited = false;
}

#define TYPE_PARAM_HEAP_QUEUE
#include "../ADT/priority queue/priority_queue.hpp"

template <typename T>
void Graph<T>::SingleSourceShortestPath(int startVertexIndex) const
{
    if (startVertexIndex < 0 || startVertexIndex >= mVertices.Size())
        throw IndexOutOfBoundsException();

    if (Empty())
        return;

    for (int i = 0; i < mVertices.Size(); i++)
    {
        mVertices[i].isVisited = false;
        mVertices[i].isInQueue = false;
        mVertices[i].parentVertexIndex = -1;
        mVertices[i].distance = startVertexIndex == i ? 0 : INF;
    }

    const auto &comparator = [this](int v1, int v2) { return mVertices[v1].distance < mVertices[v2].distance; };
    PriorityQueue<int, decltype(comparator)> priorityQueue(comparator);

    priorityQueue.Insert(startVertexIndex);
    mVertices[startVertexIndex].isInQueue = true;

    while (!priorityQueue.Empty())
    {
        int currentVertexIndex = priorityQueue.Peek();     // 1. get vertex with min distance
        mVertices[currentVertexIndex].isVisited = true;    // 2. mark vertex as visited
        priorityQueue.Remove();                            // 3. remove vertex from priority queue

        typename SinglyLinkedList<AdjacencyData>::Iterator unvisitedAdjacentVertex = mAdjacencyList[currentVertexIndex].Begin();  
        while (unvisitedAdjacentVertex != mAdjacencyList[currentVertexIndex].End())
        {
            int currentDistance = mVertices[unvisitedAdjacentVertex->vertexIndex].distance;       
            int newDistance = mVertices[currentVertexIndex].distance + unvisitedAdjacentVertex->edgeWeight;
            
            if (newDistance < currentDistance)   // 4. if distance is less, update path
            {
                mVertices[unvisitedAdjacentVertex->vertexIndex].distance = newDistance;
                mVertices[unvisitedAdjacentVertex->vertexIndex].parentVertexIndex = currentVertexIndex;

                if (!mVertices[unvisitedAdjacentVertex->vertexIndex].isInQueue)   // 5. if vertex not in priority queue, put into queue and mark as in queue
                {
                    priorityQueue.Insert(unvisitedAdjacentVertex->vertexIndex);
                    mVertices[unvisitedAdjacentVertex->vertexIndex].isInQueue = true;
                }
            }

            ++unvisitedAdjacentVertex;
        }
    }

    // display path
    for (int i = 0; i < mVertices.Size(); i++)
    {
        if (i == startVertexIndex)
            continue;
            
        PRINT("Shortest path from "); 
        PRINT(mVertices[startVertexIndex].data); 
        PRINT(" to "); 
        PRINT(mVertices[i].data);
        PRINT(": ");

        Vector<int> path(mVertices.Size());
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
            
            if (j > 0)
            {
                typename SinglyLinkedList<AdjacencyData>::Iterator it = mAdjacencyList[path[j]].Begin();
                while (it != mAdjacencyList[path[j]].End())
                {
                    if (it->vertexIndex == path[j - 1])
                    {
                        cost += it->edgeWeight;
                        break;
                    }

                    ++it;
                }
            }
        }

        PRINT(" cost: "); PRINT(cost);

        PRINTLN("");    
    }

    PRINTLN(""); 

    // reset vertex flag     
    for (int i = 0; i < mVertices.Size(); i++)
        mVertices[i].isVisited = false;      
}

template <typename T>
void Graph<T>::A_Star(int startVertexIndex, int endVertexIndex) const
{
    if (startVertexIndex < 0 || startVertexIndex >= mNumVertices || endVertexIndex < 0 || endVertexIndex >= mNumVertices)
        throw IndexOutOfBoundsException();

    if (Empty())
        return;

    for (int i = 0; i < mNumvertices; i++)
    {
        mVertices[i].distance = i == startVertexIndex ? 0 : INF;
        mVertices[i].parentVertexIndex = -1;
        mVertices[i].isVisited = false;
        mVertices[i] = isInQueue = false;
    }

    auto comparator = [this](int a, int b) -> bool { return mVertices[a].distance + mVertices[a].heuristic < mVertices[b].distance + mVertices[b].heuristic; }; 
    
    PriorityQueue<int, decltype(comparator)> priorityQueue(comparator);

    priorityQueue.Insert(startVertexIndex);
    mVertices[startVertexIndex].isInQueue = true;

    while (!priorityQueue.Empty())
    {
        int currentVertexIndex = priorityQueue.Peek();
        mVertices[currentVertexIndex].isVisited = true;
        priorityQueue.Remove();

        
    }
}

#endif  // GRAPH_H