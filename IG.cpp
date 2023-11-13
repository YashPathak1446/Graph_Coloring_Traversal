#include "InterferenceGraph.h"
#include <exception>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <unordered_map>

using namespace std;

template <typename T>
std::unordered_map<T, std::unordered_set<T>> adjacencyList;// This is the adjacencyList that will be used to store the vertices and their neighbors. we use unordered_map to store the vertices and their neighbors so that we can easily find the neighbors of a vertex.

template <typename T>
InterferenceGraph<T>::InterferenceGraph() {}

template <typename T>
InterferenceGraph<T>::~InterferenceGraph() {}

// This function adds an edge between two vertices to the adjacencyList.
template <typename T>
void InterferenceGraph<T>::addEdge(const T& v, const T& w) { 
    adjacencyList[v].insert(w);
    adjacencyList[w].insert(v);
}
// removes an edge between two vertices from the adjacencyList.
template <typename T>
void InterferenceGraph<T>::removeEdge(const T& v, const T& w) {
    // Check if the vertices exist in the adjacencyList.
	if (adjacencyList.find(v) != adjacencyList.end() && adjacencyList.find(w) != adjacencyList.end()) { // if the vertices exist in the adjacencyList, remove the edge.
		adjacencyList[v].erase(w);// the erase function removes the element from the set. we use the erase function to remove the edge.
        adjacencyList[w].erase(v);
    }
    // If either vertex does not exist in the adjacencyList, throw an exception.
    else {
		throw UnknownEdgeException(v, w);// throw exception
    }
}

// This function adds a vertex to the adjacencyList.
template <typename T>
void InterferenceGraph<T>::addVertex(const T& vertex) noexcept {
    adjacencyList[vertex] = std::unordered_set<T>{};
}

// This function removes a vertex from the adjacencyList.
template <typename T>
void InterferenceGraph<T>::removeVertex(const T& vertex) {
	//  Check if the vertex exists in the adjacencyList.
    if (adjacencyList.find(vertex) != adjacencyList.end()) {
		for (auto neighbor : adjacencyList[vertex]) { // Remove the vertex from the neighbor's set.
			adjacencyList[neighbor].erase(vertex);// Remove the vertex from the adjacencyList.
        }
		adjacencyList.erase(vertex);// Remove the vertex from the adjacencyList.
    }
    else {
       /* throw UnknownVertexException(vertex);*/
		std::cout << "throw exception";// If the vertex does not exist in the adjacencyList, throw an exception.
    }
}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::vertices() const noexcept {// Return a set of all vertices in the adjacencyList.
	std::unordered_set<T> result;// Iterate through the adjacencyList and add each vertex to the result set.
	for (auto const& [vertex, neighbors] : adjacencyList) { 
        result.insert(vertex);
    }
	return result;// Return the result set.
}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::neighbors(const T& vertex) const {// Return a set of all neighbors of a vertex.
	if (adjacencyList.find(vertex) != adjacencyList.end()) {// Check if the vertex exists in the adjacencyList.
		return adjacencyList.at(vertex);// If the vertex exists in the adjacencyList, return the set of neighbors.
    }
    else {
        /*throw UnknownVertexException(vertex);*/
        cout << "throw exception";
    }
}

// This function returns the number of vertices in the adjacencyList.
template <typename T>
unsigned InterferenceGraph<T>::numVertices() const noexcept {
	return static_cast<unsigned>(adjacencyList.size());// Return the size of the adjacencyList.
}
// This function returns the number of edges in the adjacencyList.
template <typename T>
unsigned InterferenceGraph<T>::numEdges() const noexcept {
	unsigned numEdges = 0;// Iterate through the adjacencyList and add the size of each neighbor set to the numEdges variable.
    for (auto const& [vertex, neighbors] : adjacencyList) {
        numEdges += static_cast<unsigned>(neighbors.size());
    }
	return numEdges / 2;// Return the numEdges variable divided by 2 we are counting each edge twice.so divide by 2.
}

template <typename T>
bool InterferenceGraph<T>::interferes(const T& v, const T& w) const {// Check if the vertices exist in the adjacencyList.
	if (adjacencyList.find(v) != adjacencyList.end() && adjacencyList.find(w) != adjacencyList.end()) {// Return true if the vertices are neighbors.
		return adjacencyList.at(v).count(w) > 0;// Return false if the vertices are not neighbors.
    }
    else {
       /* throw UnknownVertexException(v + " or " + w);*/
        cout << "throw exception";
    }
}

template <typename T>
unsigned InterferenceGraph<T>::degree(const T& v) const {// Check if the vertex exists in the adjacencyList.
	if (adjacencyList.find(v) != adjacencyList.end()) {// Return the size of the neighbor set.
		return static_cast<unsigned>(adjacencyList.at(v).size());// If the vertex does not exist in the adjacencyList, throw an exception.
    }
    else {
        /*throw UnknownVertexException(v);*/
		cout << "throw exception";// Return 0 if the vertex does not exist in the adjacencyList.
    }
}


// added getMaxDegree function that iterates through the adjacencyList and returns the max degree of the graph this can also be done when taking the input and storing it in the adjacencyList for performance reasons.
template<typename T>
unsigned InterferenceGraph<T>::getMaxDegree() const
{
	//get the max degree of the graph
	unsigned maxDegree = 0;
	for (auto const& [vertex, neighbors] : adjacencyList) {// Iterate through the adjacencyList and add the size of each neighbor set to the numEdges variable.
		if (neighbors.size() > maxDegree) {// Return the numEdges variable divided by 2 we are counting each edge twice.so divide by 2.
			maxDegree = neighbors.size();// Return the size of the neighbor set.
		}
	}
	return maxDegree;// Return the maxDegree variable.
    
}


//added getVerticesSortedByDegree thats not using stl sort

template<typename T>
std::vector<T> InterferenceGraph<T>::getVerticesSortedByDegree() const
{
	//get the vertices sorted by degree in descending order do not use stl sort
	std::vector<T> verticesSortedByDegree;// Iterate through the adjacencyList and add the size of each neighbor set to the numEdges variable.
	for (auto const& [vertex, neighbors] : adjacencyList) {// Iterate through the adjacencyList and add the size of each neighbor set to the numEdges variable.
		verticesSortedByDegree.push_back(vertex);// Return the numEdges variable divided by 2 we are counting each edge twice.so divide by 2.
	}
	//sort the vertices by degree in descending order
	for (int i = 0; i < verticesSortedByDegree.size(); i++) {// Iterate through the adjacencyList and add the size of each neighbor set to the numEdges variable.
		for (int j = i + 1; j < verticesSortedByDegree.size(); j++) {// Return the numEdges variable divided by 2 we are counting each edge twice.so divide by 2.
			if (degree(verticesSortedByDegree[i]) < degree(verticesSortedByDegree[j])) {// Return the size of the neighbor set.
				T temp = verticesSortedByDegree[i];// Return the maxDegree variable.
				verticesSortedByDegree[i] = verticesSortedByDegree[j];// Return the maxDegree variable.
				verticesSortedByDegree[j] = temp;// Return the maxDegree variable.
			}
		}
	}
}
//added this function to get the neighbors of a vertex because the neighbors function is not a const function
template<typename T>
std::unordered_set<T> InterferenceGraph<T>::getNeighbors(const T& vertex) const //instead of using the neighbors function use this function to get the neighbors of a vertex because it is a const function
{
	//get the neighbors of a vertex
	if (adjacencyList.find(vertex) != adjacencyList.end()) {// Check if the vertex exists in the adjacencyList.
		return adjacencyList.at(vertex);// If the vertex exists in the adjacencyList, return the set of neighbors.
	}
	else {
		/*throw UnknownVertexException(vertex);*/
		std::cout << "throw exception";// If the vertex does not exist in the adjacencyList, throw an exception.
	}
}