#ifndef __INTERFERENCE_GRAPH__HPP

#define __INTERFERENCE_GRAPH__HPP

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class UnknownVertexException : public std::runtime_error {
 public:
  UnknownVertexException(const std::string &v)

      : std::runtime_error("Unknown vertex " + v) {}
};

class UnknownEdgeException : public std::runtime_error {
 public:
  UnknownEdgeException(const std::string &v, const std::string &w)

      : std::runtime_error("Unknown edge " + v + " - " + w) {}
};

// InterferenceGraph

//

// This is a class representing an interference graph

// as described in "Part 1: Interference Graph" of the README.md

// file. Though this class is templated, because of the usage of exceptions

// UnknownVertexException and UnknownEdgeException it will

// ONLY be tested with strings.

template <typename T>
class InterferenceGraph {
 public:
  // Custom type used to represent edges. This is mainly

  // used in the utility function for reading and writing

  // the graph structure to/from files. You don't need to use it.

  using EdgeTy = std::pair<T, T>;

  InterferenceGraph();

  ~InterferenceGraph();

  void addEdge(const T &v, const T &w);

  void addVertex(const T &vertex) noexcept;

  void removeEdge(const T &v, const T &w);

  void removeVertex(const T &vertex);

  std::unordered_set<T> vertices() const noexcept;

  std::unordered_set<T> neighbors(const T &vertex) const;

  unsigned numVertices() const noexcept;

  unsigned numEdges() const noexcept;

  bool interferes(const T &v, const T &w) const;

  unsigned degree(const T &v) const;

  unsigned getMaxDegree() const;

  std::vector<T> getVerticesSortedByDegree() const;

  std::unordered_set<T> getNeighbors(const T &vertex) const;

 private:
  // Private member variables here.

  // This is the adjacencyList that will be used to store the

  // vertices and their neighbors. we use unordered_map to

  // store the vertices and their neighbors so that we can

  // easily find the neighbors of a vertex.

  std::unordered_map<T, std::unordered_set<T>> adjacencyList;
};

template <typename T>
InterferenceGraph<T>::InterferenceGraph() {}

template <typename T>
InterferenceGraph<T>::~InterferenceGraph() {}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::neighbors(const T &vertex) const {
  std::unordered_set<T> neighbors;

  // check if vertex is in adjacencyList or not

  if (adjacencyList.find(vertex) != adjacencyList.end()) {
    // if it exists in adjacencyList, return the set of neighbors of that vertex

    return adjacencyList.at(vertex);

  }

  else {
    // throw exception

    throw UnknownVertexException(vertex);
  }
}

template <typename T>

std::unordered_set<T> InterferenceGraph<T>::vertices() const noexcept {
  std::unordered_set<T> result;

  // Iterate through the adjacencyList and add each vertex to the result set

  for (auto const &[vertex, neighbors] : adjacencyList) {
    result.insert(vertex);
  }

  return result;
}

template <typename T>

unsigned InterferenceGraph<T>::numVertices() const noexcept {
  return static_cast<unsigned>(adjacencyList.size());
}

template <typename T>
unsigned InterferenceGraph<T>::numEdges() const noexcept {
  unsigned int numEdges = 0;

  for (const auto &[vertex, neighbors] : adjacencyList) {
    numEdges += static_cast<unsigned>(neighbors.size());
  }

  // we divide numEdges by 2 because we are counting each edge twice

  return numEdges / 2;
}

template <typename T>

void InterferenceGraph<T>::addEdge(const T &v, const T &w) {
  adjacencyList[v].insert(w);

  adjacencyList[w].insert(v);
}

template <typename T>

void InterferenceGraph<T>::removeEdge(const T &v, const T &w) {
  // Check if the vertices exist in the adjacencyList

  if (adjacencyList.find(v) != adjacencyList.end() &&
      adjacencyList.find(w) != adjacencyList.end()) {
    // if vertices exist, then remove the edge

    adjacencyList[v].erase(w);

    adjacencyList[w].erase(v);

  }

  // if either vertex does not exist in the adjacencyList, throw an excpetion

  else {
    throw UnknownEdgeException(v, w);
  }
}

template <typename T>

void InterferenceGraph<T>::addVertex(const T &vertex) noexcept {
  // Make sure vertex is already not in adjacencyList before adding

  if (adjacencyList.find(vertex) == adjacencyList.end()) {
    adjacencyList[vertex] = std::unordered_set<T>{};
  }
}

template <typename T>

void InterferenceGraph<T>::removeVertex(const T &vertex) {
  // Check if the vertex exists in the adjacencyList or not

  if (adjacencyList.find(vertex) != adjacencyList.end()) {
    for (auto neighbor : adjacencyList[vertex]) {
      // remove vertex from neighbor's set.

      adjacencyList[neighbor].erase(vertex);
    }

    // remove the vertex from the adjacencyList

    adjacencyList.erase(vertex);

  }

  // if not in adjacencyList

  else {
    throw UnknownVertexException(vertex);
  }
}

template <typename T>

bool InterferenceGraph<T>::interferes(const T &v, const T &w) const {
  // Check if the vertex exists in the graph or not

  if (adjacencyList.find(v) != adjacencyList.end() &&
      adjacencyList.find(w) != adjacencyList.end()) {
    // return true if vertices are neighbors, false if not neighbors.

    return adjacencyList.at[v].count(w) > 0;

  }

  else {
    if (adjacencyList.find(v) == adjacencyList.end()) {
      throw UnknownVertexException(v);
    }

    throw UnknownVertexException(w);
  }
}

template <typename T>

unsigned InterferenceGraph<T>::degree(const T &v) const {
  // check if vertex is in the adjacencyList

  if (adjacencyList.find(v) != adjacencyList.end()) {
    return static_cast<unsigned>(adjacencyList.at(v).size());

  }

  else {
    throw UnknownVertexException(v);
  }
}

// added getMaxDegree function that iterates through the adjacencyList

// and returns the max degree of the graph

template <typename T>

unsigned InterferenceGraph<T>::getMaxDegree() const {
  unsigned maxDegree = 0;

  for (auto const &[vertex, neighbors] : adjacencyList) {
    if (neighbors.size() > maxDegree) {
      maxDegree = neighbors.size();
    }
  }

  return maxDegree;
}

// vertices vector sorted in descending order

template <typename T>

std::vector<T> InterferenceGraph<T>::getVerticesSortedByDegree() const {
  std::vector<T> verticesSortedByDegree;

  // add all vertices to a vector

  for (auto const &[vertex, neighbors] : adjacencyList) {
    verticesSortedByDegree.push_back(vertex);
  }

  // actual sorting in DESCENDING order

  for (int i = 0; i < verticesSortedByDegree.size(); i++) {
    for (int j = i + 1; j < verticesSortedByDegree.size(); j++) {
      if (degree(verticesSortedByDegree[i]) <
          degree(verticesSortedByDegree[j])) {
        T temp = verticesSortedByDegree[i];

        verticesSortedByDegree[i] = verticesSortedByDegree[j];

        verticesSortedByDegree[j] = temp;
      }
    }
  }

  return verticesSortedByDegree;
}

template <typename T>

std::unordered_set<T> InterferenceGraph<T>::getNeighbors(
    const T &vertex) const {
  if (adjacencyList.find(vertex) != adjacencyList.end()) {
    return adjacencyList.at(vertex);

  }

  else {
    throw UnknownVertexException(vertex);
  }
}

#endif
