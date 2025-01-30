#pragma once

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

template <typename VertexT, typename WeightT>
class graph {
 private:
  // adjacency list repersentation:
  // Vertex → neighbors, weighted
  // map<string, map<string, int>>

  // unordered map for O(1) speed
  // unordered_map<vertex, unordered_map<neighbor, weight of edge>>

  unordered_map<VertexT, unordered_map<VertexT, WeightT>> adjacencyList;
  int numberEdges = 0;

 public:
  /// Default constructor
  graph() {
  }

  bool addVertex(VertexT v) {
    // if the v-vertex is not in the graph
    // at v-vertex's key, create an empty value
    if (adjacencyList.find(v) == adjacencyList.end()) {
      adjacencyList[v] = unordered_map<VertexT, WeightT>();
      return true;
    }
    // else return false
    return false;
  }

  bool addEdge(VertexT from, VertexT to, WeightT weight) {
    // if the from-vertex or to-vertex is not in the graph, return false
    if (adjacencyList.find(from) == adjacencyList.end() ||
        adjacencyList.find(to) == adjacencyList.end()) {
      return false;
    }

    // if the edge doesn't exist, create a new edge and increment the number of
    // edges
    if (adjacencyList[from].find(to) == adjacencyList[from].end()) {
      adjacencyList[from][to] = weight;
      numberEdges++;
    }
    // if the edge exists,, override with a new weight
    else {
      adjacencyList[from][to] = weight;
    }
    return true;
  }

  // Note: .at() will get the values at that vertex
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
    // if the from-vertex is not in the graph, return false
    if (adjacencyList.find(from) == adjacencyList.end()) {
      return false;
    }
    // starting at the from-vertex, check it's neighbors for the to-vertex
    // if the to-vertex is not in the graph, return false
    if (adjacencyList.at(from).find(to) == adjacencyList.at(from).end()) {
      return false;
    }

    // the weight is the edge between from and to
    weight = adjacencyList.at(from).at(to);
    return true;
  }

  set<VertexT> neighbors(VertexT v) const {
    set<VertexT> neighbors_set;
    // if the v-vertex exists in the adjacencyList
    if (adjacencyList.find(v) != adjacencyList.end()) {
      // iterate through each of it's values (aka neighbors)
      for (auto neighbor : adjacencyList.at(v)) {
        // add only the vertex of the neighbors to your set
        neighbors_set.insert(neighbor.first);
      }
    }
    // return the set of neighbors
    return neighbors_set;
  }

  vector<VertexT> getVertices() const {
    vector<VertexT> vertices;
    // iterate thru all the key-value pairs in adjacencyList
    for (auto vertex : adjacencyList) {
      // add only the vertex to the set
      vertices.push_back(vertex.first);
    }
    // return the set of vertices
    return vertices;
  }

  size_t numVertices() const {
    // .size() returns the number of elements in a map
    return adjacencyList.size();
  }

  size_t numEdges() const {
    // returns the number of edges in the graph
    return numberEdges;
  }
};
