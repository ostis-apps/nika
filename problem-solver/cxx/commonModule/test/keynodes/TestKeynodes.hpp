#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "TestKeynodes.generated.hpp"

namespace commonTest
{

class TestKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:

  SC_PROPERTY(Keynode("test_node"), ForceCreate)
  static ScAddr test_node;

  SC_PROPERTY(Keynode("assign_dynamic_argument_test_action"), ForceCreate)
  static ScAddr assign_dynamic_argument_test_action;

  SC_PROPERTY(Keynode("check_dynamic_argument_test_action"), ForceCreate)
  static ScAddr check_dynamic_argument_test_action;

  SC_PROPERTY(Keynode("successfully_finished_test_action"), ForceCreate)
  static ScAddr successfully_finished_test_action;

  SC_PROPERTY(Keynode("unsuccessfully_finished_test_action"), ForceCreate)
  static ScAddr unsuccessfully_finished_test_action;

  SC_PROPERTY(Keynode("finished_test_action"), ForceCreate)
  static ScAddr finished_test_action;
};

} // namespace common
