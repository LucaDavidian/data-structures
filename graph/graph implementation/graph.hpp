#ifndef GRAPH_H
#define GRAPH_H

#include "../../vector/vector.hpp"
#include <limits>

template <typename T>
class NodeVisitor;

template <typename T>
class Graph
{
friend class NodeVisitor<T>;
private:
	struct Node_
	{
		T mData;

		mutable bool mVisited = false;
		mutable bool mInQueue = false;
		mutable int mParentNodeIndex = -1;
		mutable float mCost = std::numeric_limits<float>::max();
		mutable float mHeuristic;
	};
	struct Adjacency
	{
		unsigned int mConnectedNodeIndex;
		float mWeight;
	};
public:
	typedef Node_ Node;
    
	void AddNode(const T &data);

	void RemoveNode(unsigned int nodeIndex);

	void AddEdge(unsigned int node1, unsigned int node2, float weight = 1.0f, bool directed = false);

	bool Connected(unsigned int node1, unsigned int node2) const;

	int GetUnvisitedAdjacentNodeIndex(unsigned int nodeIndex) const;

	void Reset() const;

	T const &GetData(unsigned int nodeIndex) { return mNodes[nodeIndex].mData; }

	template <template <typename> typename  F>
	void DepthFirstSearch(F<T> &visitor, unsigned int startNodeIndex) const;

	template <template <typename> typename  F>
	void DepthFirstSearchRecursive(F<T> &visitor, unsigned int startNodeIndex) const;

	template <template <typename> typename F>
	void BreadthFirstSearch(F<T> &visitor, unsigned int startNodeIndex) const;

	Vector<Vector<unsigned int>> DijkstraShortestPath(unsigned int startNodeIndex) const;
	Vector<unsigned int> DijkstraShortestPath(unsigned int startNodeIndex, unsigned int endNodeIndex) const;

	Vector<unsigned int> AStar(unsigned int startNodeIndex, unsigned int endNodeIndex) const;
private:
	Vector<Node> mNodes;
	Vector<Vector<Adjacency>> mAdjacencyList;

	float GetNodeHeuristic(unsigned int nodeIndex, unsigned int endNodeIndex) const;
};

template <typename T>
void Graph<T>::AddNode(const T &data)
{
	mNodes.InsertLast(Node{data});
	mAdjacencyList.Resize(mNodes.Size());
}

template <typename T>
void Graph<T>::AddEdge(unsigned int nodeIndex1, unsigned int nodeIndex2, float weight, bool directed)
{
	mAdjacencyList[nodeIndex1].InsertLast(Adjacency{nodeIndex2, weight});

	if (!directed)
		mAdjacencyList[nodeIndex2].InsertLast(Adjacency{nodeIndex1, weight});
}

template <typename T>
void Graph<T>::Reset() const
{
	for (auto &node : mNodes)
	{
		node.mVisited = false;
		node.mInQueue = false;
		node.mCost = std::numeric_limits<float>::max();
		node.mParentNodeIndex = -1;
	}
}

template <typename T>
bool Graph<T>::Connected(unsigned int nodeIndex1, unsigned int nodeIndex2) const
{
	for (auto &adjacency : mAdjacencyList[nodeIndex1])
		if (adjacency.mConnectedNodeIndex == nodeIndex2)
			return true;

	return false;
}

template <typename T>
int Graph<T>::GetUnvisitedAdjacentNodeIndex(unsigned int nodeIndex) const
{
	for (auto &adjacency : mAdjacencyList[nodeIndex])
		if (!mNodes[adjacency.mConnectedNodeIndex].mVisited)
			return adjacency.mConnectedNodeIndex;

	return -1;
}

template <typename T>
template <template <typename> class F>
void Graph<T>::DepthFirstSearchRecursive(F<T> &visitor, unsigned int startNodeIndex) const
{
	static unsigned int level = 0U;

	level++;

	const Node &node = mNodes[startNodeIndex];

	visitor.Visit(node);
	node.mVisited = true;

	unsigned int unvisitedAdjacentNodeIndex;
	while ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(startNodeIndex)) != -1)
		DepthFirstSearchRecursive(visitor, unvisitedAdjacentNodeIndex);

	level--;

	if (level == 0U)  // end of recursion - reset node flags
		Reset(); 
}

#include "../../ADT/stack/stack.hpp"

template <typename T>
template <template <typename> class F>
void Graph<T>::DepthFirstSearch(F<T> &visitor, unsigned int startNodeIndex) const
{
	Stack<unsigned int> stack;

	const Node &startNode = mNodes[startNodeIndex];
	visitor.Visit(startNode);
	startNode.mVisited = true;

	stack.Push(startNodeIndex);

	while (!stack.Empty())
	{
		unsigned int currentNodeIndex = stack.Top();

		unsigned int unvisitedAdjacentNodeIndex;
		if ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNodeIndex)) != -1)	
		{
			const Node &node = mNodes[unvisitedAdjacentNodeIndex];
			visitor.Visit(node);
			node.mVisited = true;

			stack.Push(unvisitedAdjacentNodeIndex);
		}
		else
			stack.Pop();
	}	

	// stack.Push(startNodeIndex);

	// while (!stack.Empty())
	// {
	// 	unsigned int currentNodeIndex = stack.Top();

	// 	const Node &node = mNodes[startNodeIndex];
	// 	visitor.Visit(node);
	// 	node.mVisited = true;

	// 	unsigned int unvisitedAdjacentNodeIndex;
	// 	while ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNodeIndex)) != -1)	
	// 		stack.Push(unvisitedAdjacentNodeIndex);
		
	// 	stack.Pop();
	// }

	Reset();
}

#include "../../ADT/queue/queue.hpp"

template <typename T>
template <template <typename> class F>
void Graph<T>::BreadthFirstSearch(F<T> &visitor, unsigned int startNodeIndex) const
{
	Queue<unsigned int> queue;

	const Node &startNode = mNodes[startNodeIndex];
	visitor.Visit(startNode);
	startNode.mVisited = true;

	queue.Enqueue(startNodeIndex);

	while (!queue.Empty())
	{
		unsigned int currentNodeIndex = queue.Front();

		unsigned int unvisitedAdjacentNodeIndex;
		if ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNodeIndex)) != -1)
		{
			const Node &node = mNodes[unvisitedAdjacentNodeIndex];
			visitor.Visit(node);
			node.mVisited = true;

			queue.Enqueue(unvisitedAdjacentNodeIndex);
		}
		else
			queue.Dequeue();
	}

	queue.Enqueue(startNodeIndex);

	Reset();
}

#define TYPE_ERASURE_HEAP_QUEUE
#include "../../ADT/priority queue/priority_queue.hpp"

template <typename T>
Vector<Vector<unsigned int>> Graph<T>::DijkstraShortestPath(unsigned int startNodeIndex) const
{
	PriorityQueue<unsigned int> queue([this](unsigned int nodeIndex1, unsigned int nodeIndex2){ return mNodes[nodeIndex1].mCost < mNodes[nodeIndex2].mCost; });
	
	mNodes[startNodeIndex].mCost = 0.0f;

	queue.Insert(startNodeIndex);

	while (!queue.Empty())
	{
		unsigned int currentNodeIndex = queue.Peek();  // node in queue with minimum current total cost

		for (auto &adjacency : mAdjacencyList[currentNodeIndex])
		{
			if (mNodes[adjacency.mConnectedNodeIndex].mVisited)
				continue;

			float weight = adjacency.mWeight;
			float newCost = mNodes[currentNodeIndex].mCost + weight;

			if (newCost < mNodes[adjacency.mConnectedNodeIndex].mCost)
			{
				mNodes[adjacency.mConnectedNodeIndex].mCost = newCost;
				mNodes[adjacency.mConnectedNodeIndex].mParentNodeIndex = currentNodeIndex;

				if (!mNodes[adjacency.mConnectedNodeIndex].mInQueue)
				{
					queue.Insert(adjacency.mConnectedNodeIndex);
					mNodes[adjacency.mConnectedNodeIndex].mInQueue = true;   // node's cost has been relaxed but all its neighbours have not been examined from it yet
				}
			}
		}

		queue.Remove();
		mNodes[currentNodeIndex].mVisited = true;  // all node's neighbours have been examined from it
	}

	Vector<Vector<unsigned int>> paths;
	paths.Resize(mNodes.Size());

	for (unsigned int i = 0; i < mNodes.Size(); ++i)
	{
		Vector<unsigned int> &path = paths[i];

		unsigned int currentNodeIndex = i;

		while (currentNodeIndex != -1)
		{
			path.InsertFirst(currentNodeIndex);
			currentNodeIndex = mNodes[currentNodeIndex].mParentNodeIndex;
		}
	}

	Reset();

	return paths;
}

template <typename T>
Vector<unsigned int> Graph<T>::DijkstraShortestPath(unsigned int startNodeIndex, unsigned int endNodeIndex) const
{
	PriorityQueue<unsigned int> queue([this](unsigned int nodeIndex1, unsigned int nodeIndex2) { return mNodes[nodeIndex1].mCost < mNodes[nodeIndex2].mCost; });
	
	mNodes[startNodeIndex].mCost = 0.0f;

	queue.Insert(startNodeIndex);

	while (!queue.Empty())
	{
		unsigned int currentNodeIndex = queue.Peek();  // node in queue with minimum current total cost

		if (currentNodeIndex == endNodeIndex)  // found end node, exit (less accurate, but faster)
			break;

		for (auto &adjacency : mAdjacencyList[currentNodeIndex])
		{			
			if (mNodes[adjacency.mConnectedNodeIndex].mVisited)
				continue;

			float weight = adjacency.mWeight;
			float newCost = mNodes[currentNodeIndex].mCost + weight;

			if (newCost < mNodes[adjacency.mConnectedNodeIndex].mCost)
			{
				mNodes[adjacency.mConnectedNodeIndex].mCost = newCost;
				mNodes[adjacency.mConnectedNodeIndex].mParentNodeIndex = currentNodeIndex;

				if (!mNodes[adjacency.mConnectedNodeIndex].mInQueue)
				{
					queue.Insert(adjacency.mConnectedNodeIndex);
					mNodes[adjacency.mConnectedNodeIndex].mInQueue = true;   // node's cost has been relaxed but all its neighbours have not been examined from it yet
				}
			}
		}

		queue.Remove();
		mNodes[currentNodeIndex].mVisited = true;  // all node's neighbours have been examined from it
	}

	Vector<unsigned int> path;

	path.InsertFirst(endNodeIndex);

	unsigned int currentNodeIndex = endNodeIndex;

	while (mNodes[currentNodeIndex].mParentNodeIndex != -1)
	{
		path.InsertFirst(mNodes[currentNodeIndex].mParentNodeIndex);
		currentNodeIndex = mNodes[currentNodeIndex].mParentNodeIndex;
	}

	Reset();

	return path;
}

template <typename T>
Vector<unsigned int> Graph<T>::AStar(unsigned int startNodeIndex, unsigned int endNodeIndex) const
{
	for (unsigned int i = 0U; i < mNodes.Size(); i++)
		mNodes[i].mHeuristic = GetNodeHeuristic(i, endNodeIndex);

	PriorityQueue<unsigned int> queue([this](unsigned int nodeIndex1, unsigned int nodeIndex2) { return mNodes[nodeIndex1].mCost + mNodes[nodeIndex1].mHeuristic < mNodes[nodeIndex2].mCost + mNodes[nodeIndex2].mHeuristic; });

	const Node &startNode = mNodes[startNodeIndex];
	startNode.mCost = 0.0f;

	queue.Insert(startNodeIndex);
	
	while (!queue.Empty())
	{
		unsigned int currentNodeIndex = queue.Peek();
		const Node &currentNode = mNodes[currentNodeIndex];

		if (currentNodeIndex == endNodeIndex)
			break;
		
		for (auto &adjacency : mAdjacencyList[currentNodeIndex])
		{
			unsigned int adjacentNodeIndex = adjacency.mConnectedNodeIndex;
			const Node &adjacentNode = mNodes[adjacentNodeIndex];

			if (adjacentNode.mVisited)
				continue;

			float weight = adjacency.mWeight;
			float newCost = currentNode.mCost + weight;

			if (newCost < adjacentNode.mCost)
			{
				adjacentNode.mCost = newCost;
				adjacentNode.mParentNodeIndex = currentNodeIndex;

				if (!adjacentNode.mInQueue)
				{
					queue.Insert(adjacentNodeIndex);
					adjacentNode.mInQueue = true;
				}
			}
		}

		currentNode.mVisited = true;
		queue.Remove();
	}	

	Vector<unsigned int> path;

	unsigned int currentNodeIndex = endNodeIndex;

	while (currentNodeIndex != -1)
	{	
		path.InsertFirst(currentNodeIndex);
		currentNodeIndex = mNodes[currentNodeIndex].mParentNodeIndex;
	}

	Reset();

	return path;
}

template <typename T>
float Graph<T>::GetNodeHeuristic(unsigned int nodeIndex, unsigned int endNodeIndex) const
{
	return 0.0f;
}

#include <iostream>

template <typename T>
class NodeVisitor
{
public:
	void Visit(typename Graph<T>::Node const &node)  // default behavior: print to stdout
	{
		std::cout << node.mData << std::endl;
	}
};

#endif  // GRAPH_H