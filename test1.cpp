#include "CSVReader.hpp"
#include "IGWriter.hpp"
#include "InterferenceGraph.hpp"
#include "gtest/gtest.h"
#include "proj6.hpp"
#include "verifier.hpp"

// Warning: These are *NOT* exhaustive tests.
// You should consider creating your own unit tests
// to test the functionality of your code entirely.

namespace {

using namespace proj6;

TEST(RequiredPart1, SimpleAddEdgesAndVertices) {
  const auto &GRAPH = "gtest/graphs/simple.csv";

  const InterferenceGraph<Variable> &ig = CSVReader::load(GRAPH);

  EXPECT_EQ(ig.numEdges(), 3);
  EXPECT_EQ(ig.numVertices(), 3);
}

TEST(RequiredPart1, Neighbors) {
  const auto &GRAPH = "gtest/graphs/simple.csv";

  const InterferenceGraph<Variable> &ig = CSVReader::load(GRAPH);
  const std::unordered_set<Variable> &expected_neighbors = {"y", "z"};

  EXPECT_EQ(ig.neighbors("x"), expected_neighbors);
}

TEST(RequiredPart2, SimpleSuccess) {
  // Load the graph pointed to by simple.csv
  const auto &GRAPH = "gtest/graphs/simple.csv";
  const auto NUM_REGS = 3;

  // Provide 3 registers to the register allocation algorithm.
  // Note, the highest degree in this graph is two, so all three
  // will be needed.
  const auto &allocation = assignRegisters(GRAPH, NUM_REGS);
  IGWriter::write(CSVReader::load(GRAPH), "gtest/graphs/simple_success.dot",
                  allocation);

  EXPECT_TRUE(verifyAllocation(GRAPH, NUM_REGS, allocation));
}

TEST(RequiredPart2, SimpleFail) {
  // Load the graph pointed to by simple.csv
  const auto &GRAPH = "gtest/graphs/simple.csv";
  const auto NUM_REGS = 2;

  // Provide 2 registers to the register allocation algorithm.
  // Note, the highest degree in this graph is two, so the allocation
  // cannot be completed.
  const auto &allocation = assignRegisters(GRAPH, NUM_REGS);
  EXPECT_TRUE(allocation.empty());
}

}  // end namespace
