#include "proj6.hpp"
#include "CSVReader.hpp"

#include <string>
#include <vector>
#include <unordered_map>


#include "InterferenceGraph.hpp"

using namespace proj6;

// assignRegisters
//
// This is where you implement the register allocation algorithm
// as mentioned in the README. Remember, you must allocate at MOST
// d(G) + 1 registers where d(G) is the maximum degree of the graph G.
// If num_registers is not enough registers to accomodate the passed in
// graph you should return an empty map. You MUST use registers in the
// range [1, num_registers] inclusive.
RegisterAssignment proj6::assignRegisters(const std::string &path_to_graph,
                                          int num_registers) noexcept {
  InterferenceGraph<Variable> ig = CSVReader::load(path_to_graph);

  RegisterAssignment assignment; // create an unordered_map type RegisterAssignment assignment

  // Check if the number of registers is sufficient for the graph
  unsigned maxDegree = ig.getMaxDegree(); // Max degree of graph ig
  // if the number of registers is not sufficient, return empty map.
  if (maxDegree + 1 > num_registers){
    return assignment;
  }

  // sort the vertices in decreasing order of the degree
  auto sortedVertices = ig.getVerticesSortedByDegree();

  // Allocate the registers to vertices in the order given by sortedVertices.
  for (auto const& vertex: sortedVertices){
    // Create a set of already-assigned registers that interfere with this vertex
    std::unordered_set<Register> interferingRegisters;
    for (const auto& neighbor : ig.getNeighbors(vertex)){
      interferingRegisters.insert(assignment[neighbor]);
    }
    // Find the lowest-numbered register that does not interfere with any neighbors
    Register chosenRegister = 1;
    while (interferingRegisters.find(chosenRegister) != interferingRegisters.end()){
      chosenRegister++;
    }
    assignment[vertex] = chosenRegister;
  }
  return assignment;
}