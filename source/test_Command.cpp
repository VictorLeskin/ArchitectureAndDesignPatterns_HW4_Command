
#include <gtest/gtest.h>

#include "Command.hpp"

using namespace ;

// clang-format off

// gTest grouping class
class test_Command : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_Command : public Command
  {
  public:
    // add here members for free access.
    using Command::Command; // delegate constructors
  };

};
 
TEST_F(test_Command, test_ctor )
{
  Test_Command t;
}

