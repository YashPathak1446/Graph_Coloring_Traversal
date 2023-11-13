#ifndef __VERIFIER__HPP
#define __VERIFIER__HPP

#include "gtest/gtest.h"
#include "proj6.hpp"

using namespace proj6;

testing::AssertionResult verifyAllocation(const std::string &path_to_graph,
                                          int num_registers,
                                          const RegisterAssignment &mapping);

#endif