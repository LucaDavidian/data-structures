#ifndef GRAPH_H
#define GRAPH_H

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

		mutable bool mVisited{false};
		mutable int mParentNodeIndex{-1};
		mutable float mCost{std::numeric_limits<float>::max()};
		mutable float mHeuristic{0.0f};
	};

	struct Adjacency
	{
		unsigned int mConnectedNodeIndex;
		float mWeight;
	};

public:
	void AddNode(const T &data);

	void RemoveNode(unsigned int nodeIndex);

	void AddEdge(unsigned int node1, unsigned int node2, float weight = 1.0f, bool directed = false);

	unsigned int Size() const { return mNodes.Size(); }

	bool Connected(unsigned int node1, unsigned int node2) const;

	int GetUnvisitedAdjacentNodeIndex(unsigned int nodeIndex) const;
	
    void Clear() { mNodes.Clear(); mAdjacencyList.Clear(); }

	void Reset() const;

	T const &GetData(unsigned int nodeIndex) { return mNodes[nodeIndex].mData; }

	template <template <typename> typename  F>
	void DepthFirstSearch(unsigned int startNodeIndex, const F<T> &visitor) const;

	template <template <typename> typename  F>
	void DepthFirstSearchRecursive(unsigned int startNodeIndex, const F<T> &visitor) const;

	template <template <typename> typename F>
	void BreadthFirstSearch(unsigned int startNodeIndex, const F<T> &visitor) const;

	Vector<Vector<const Node*>> DijkstraShortestPath(unsigned int startNodeIndex) const;
	Vector<const Node*> DijkstraShortestPath(unsigned int startNodeIndex, unsigned int endNodeIndex) const;

	Vector<const Node*> AStar(unsigned int startNodeIndex, unsigned int endNodeIndex) const;

private:
	Vector<Node> mNodes;
	Vector<Vector<Adjacency>> mAdjacencyList;

	float GetNodeHeuristic(unsigned int nodeIndex, unsigned int endNodeIndex) const;
};

template <typename T, typename Heuristic>
void Graph<T, Heuristic>::AddNode(const T &data)
{
	mNodes.InsertLast(Node{data});
	mAdjacencyList.Resize(mNodes.Size());
}

template <typename T, typename Heuristic>
void Graph<T, Heuristic>::RemoveNode(unsigned int nodeIndex)
{
	mNodes.Remove(nodeIndex);
	mAdjacencyList.Remove(nodeIndex);

	for (auto &nodeAdjacencyList: mAdjacencyList)
		for (auto it = nodeAdjacencyList.Begin(), end = nodeAdjacencyList.End(); it != end; ++it)
			if (it->mConnectedNodeIndex == nodeIndex)
			{
				nodeAdjacencyList.Remove(it);
				break;
			}
}

template <typename T, typename Heuristic>
void Graph<T, Heuristic>::AddEdge(unsigned int nodeIndex1, unsigned int nodeIndex2, float weight, bool directed)
{
    if (nodeIndex1 < mNodes.Size() && nodeIndex2 < mNodes.Size())
    {
	    mAdjacencyList[nodeIndex1].InsertLast(Adjacency{ nodeIndex2, weight });

	    if (!directed)
		    mAdjacencyList[nodeIndex2].InsertLast(Adjacency{ nodeIndex1, weight });
    }
}

template <typename T, typename Heuristic>
void Graph<T, Heuristic>::Reset() const
{
	for (auto &node : mNodes)
	{
		node.mVisited = false;
		node.mCost = std::numeric_limits<float>::max();
		node.mParentNodeIndex = -1;
	}
}

template <typename T, typename Heuristic>
bool Graph<T, Heuristic>::Connected(unsigned int nodeIndex1, unsigned int nodeIndex2) const
{
	for (auto &adjacency : mAdjacencyList[nodeIndex1])
		if (adjacency.mConnectedNodeIndex == nodeIndex2)
			return true;

	return false;
}

template <typename T, typename Heuristic>
int Graph<T, Heuristic>::GetUnvisitedAdjacentNodeIndex(unsigned int nodeIndex) const
{
	for (const auto &adjacency : mAdjacencyList[nodeIndex])
		if (!mNodes[adjacency.mConnectedNodeIndex].mVisited)
			return adjacency.mConnectedNodeIndex;

	return -1;
}

template <typename T, typename Heuristic>
template <template <typename> class F>
void Graph<T, Heuristic>::DepthFirstSearchRecursive(unsigned int startNodeIndex, const F<T> &visitor) const
{
	static unsigned int level = 0U;

	level++;

	const Node &node = mNodes[startNodeIndex];

	visitor(node);
	node.mVisited = true;

	unsigned int unvisitedAdjacentNodeIndex;
	while ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(startNodeIndex)) != -1)
		DepthFirstSearchRecursive(unvisitedAdjacentNodeIndex, visitor);

	level--;

	if (level == 0U)  // end of recursion - reset node flags
		Reset();
}

#include "../ADT/stack/stack.hpp"

template <typename T, typename Heuristic>
template <template <typename> class F>
void Graph<T, Heuristic>::DepthFirstSearch(unsigned int startNodeIndex, const F<T> &visitor) const
{
	Stack<unsigned int> stack;

	const Node &startNode = mNodes[startNodeIndex];
	visitor(startNode);
	startNode.mVisited = true;

	stack.Push(startNodeIndex);

	while (!stack.Empty())
	{
		unsigned int currentNodeIndex = stack.Top();

		unsigned int unvisitedAdjacentNodeIndex;
		if ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNodeIndex)) != -1)
		{
			const Node &node = mNodes[unvisitedAdjacentNodeIndex];
			visitor(node);
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

	// 	const Node &node = mNodes[currentNodeIndex];
    //  if (!node.mVisited)
    //  {
	// 	  visitor(node);
	// 	  node.mVisited = true;
    //  }

	// 	unsigned int unvisitedAdjacentNodeIndex;
	// 	if ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNodeIndex)) != -1)	
	// 		stack.Push(unvisitedAdjacentNodeIndex);
    //  else
	// 	    stack.Pop();
	// }

	Reset();
}

#include "../ADT/queue/queue.hpp"

template <typename T, typename Heuristic>
template <template <typename> class F>
void Graph<T, Heuristic>::BreadthFirstSearch(unsigned int startNodeIndex, const F<T> &visitor) const
{
	Queue<unsigned int> queue;

	const Node &startNode = mNodes[startNodeIndex];
	visitor(startNode);
	startNode.mVisited = true;

	queue.Enqueue(startNodeIndex);

	while (!queue.Empty())
	{
		unsigned int currentNodeIndex = queue.Front();

		unsigned int unvisitedAdjacentNodeIndex;
		if ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNodeIndex)) != -1)
		{
			const Node &node = mNodes[unvisitedAdjacentNodeIndex];
			visitor(node);
			node.mVisited = true;

			queue.Enqueue(unvisitedAdjacentNodeIndex);
		}
		else
			queue.Dequeue();
	}

    // queue.Enqueue(startNodeIndex);

	// while (!queue.Empty())
	// {
	// 	unsigned int currentNodeIndex = queue.Front();

    // 	const Node &node = mNodes[currentNodeIndex];
    //  if (!node.mVisited)
    //  {
	// 	  visitor(node);
	// 	  node.mVisited = true;
    //  }

	// 	unsigned int unvisitedAdjacentNodeIndex;
	// 	if ((unvisitedAdjacentNodeIndex = GetUnvisitedAdjacentNodeIndex(currentNodeIndex)) != -1)
	// 		queue.Enqueue(unvisitedAdjacentNodeIndex);
	// 	else
	// 		queue.Dequeue();
	// }

	Reset();
}

#define TYPE_PARAM_HEAP_QUEUE
#include "../ADT/priority queue/priority_queue.hpp"

template <typename T, typename Heuristic>
Vector<Vector<const typename Graph<T, Heuristic>::Node*>> Graph<T, Heuristic>::DijkstraShortestPath(unsigned int startNodeIndex) const
{
	auto comparator = [this](unsigned int nodeIndex1, unsigned int nodeIndex2) { return mNodes[nodeIndex1].mCost < mNodes[nodeIndex2].mCost; };
	PriorityQueue<unsigned int, decltype(comparator)> queue(comparator);

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
                if (queue.Find(adjacency.mConnectedNodeIndex))
                    queue.Remove(adjacency.mConnectedNodeIndex);

				mNodes[adjacency.mConnectedNodeIndex].mCost = newCost;
				mNodes[adjacency.mConnectedNodeIndex].mParentNodeIndex = currentNodeIndex;

				queue.Insert(adjacency.mConnectedNodeIndex);
			}
		}

		mNodes[currentNodeIndex].mVisited = true;  // all node's neighbours have been examined 
		queue.Remove();
	}

	Vector<Vector<const Node*>> paths;
	paths.Resize(mNodes.Size());

	for (unsigned int i = 0; i < mNodes.Size(); ++i)
	{
		Vector<const Node*> &path = paths[i];

		unsigned int currentNodeIndex = i;

		while (currentNodeIndex != -1)
		{
			path.InsertFirst(&mNodes[currentNodeIndex]);
			currentNodeIndex = mNodes[currentNodeIndex].mParentNodeIndex;
		}
	}

	Reset();

	return paths;
}

template <typename T, typename Heuristic>
Vector<const typename Graph<T, Heuristic>::Node*> Graph<T, Heuristic>::DijkstraShortestPath(unsigned int startNodeIndex, unsigned int endNodeIndex) const
{
	auto comparator = [this](unsigned int nodeIndex1, unsigned int nodeIndex2) { return mNodes[nodeIndex1].mCost < mNodes[nodeIndex2].mCost; };
	PriorityQueue<unsigned int, decltype(comparator)> queue(comparator);

	mNodes[startNodeIndex].mCost = 0.0f;

	queue.Insert(startNodeIndex);

	while (!queue.Empty())
	{
		unsigned int currentNodeIndex = queue.Peek();  // node in queue with minimum current total cost

		if (currentNodeIndex == endNodeIndex)          // found end node, exit (less accurate, but faster)
			break;

		for (auto &adjacency : mAdjacencyList[currentNodeIndex])
		{
			if (mNodes[adjacency.mConnectedNodeIndex].mVisited)
				continue;

			float weight = adjacency.mWeight;
			float newCost = mNodes[currentNodeIndex].mCost + weight;

			if (newCost < mNodes[adjacency.mConnectedNodeIndex].mCost)
			{
                if (queue.Find(adjacency.mConnectedNodeIndex))
                    queue.Remove(adjacency.mConnectedNodeIndex);

				mNodes[adjacency.mConnectedNodeIndex].mCost = newCost;
				mNodes[adjacency.mConnectedNodeIndex].mParentNodeIndex = currentNodeIndex;

				queue.Insert(adjacency.mConnectedNodeIndex);
			}
		}

		mNodes[currentNodeIndex].mVisited = true;  // all node's neighbours have been examined from it
		queue.Remove();
	}

	Vector<const Node*> path;

	unsigned int currentNodeIndex = endNodeIndex;

	while (currentNodeIndex != -1)
	{
		path.InsertFirst(&mNodes[currentNodeIndex]);
		currentNodeIndex = mNodes[currentNodeIndex].mParentNodeIndex;
	}

	Reset();

	return path;
}

template <typename T, typename Heuristic>
Vector<const typename Graph<T, Heuristic>::Node*> Graph<T, Heuristic>::AStar(unsigned int startNodeIndex, unsigned int endNodeIndex) const
{
	for (unsigned int i = 0U; i < mNodes.Size(); i++)
		mNodes[i].mHeuristic = Heuristic()(this, i, endNodeIndex);

	auto comparator = [this](unsigned int nodeIndex1, unsigned int nodeIndex2) { return mNodes[nodeIndex1].mCost < mNodes[nodeIndex2].mCost; };
	PriorityQueue<unsigned int, decltype(comparator)> queue(comparator);

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

			// if (adjacentNode.mVisited)
			// 	continue;

			float weight = adjacency.mWeight;
			float newCost = currentNode.mCost + weight;

			if (newCost < adjacentNode.mCost)
			{
                if (queue.Find(adjacentNodeIndex))
                    queue.Remove(adjacentNodeIndex);

				adjacentNode.mCost = newCost;
				adjacentNode.mParentNodeIndex = currentNodeIndex;

				queue.Insert(adjacentNodeIndex);
			}
		}

		currentNode.mVisited = true;
		queue.Remove();
	}

	Vector<const Node*> path;

	unsigned int currentNodeIndex = endNodeIndex;

	while (currentNodeIndex != -1)
	{
		path.InsertFirst(&mNodes[currentNodeIndex]);
		currentNodeIndex = mNodes[currentNodeIndex].mParentNodeIndex;
	}

	Reset();

	return path;
}

#endif  // GRAPH_H