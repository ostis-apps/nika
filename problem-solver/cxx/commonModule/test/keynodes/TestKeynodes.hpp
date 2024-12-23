#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace commonTest
{

class TestKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const test_node{"test_node", ScType::ConstNode};

  static inline ScKeynode const assign_dynamic_argument_test_action{
      "assign_dynamic_argument_test_action",
      ScType::ConstNodeClass};

  static inline ScKeynode const check_dynamic_argument_test_action{
      "check_dynamic_argument_test_action",
      ScType::ConstNodeClass};

  static inline ScKeynode const successfully_finished_test_action{
      "successfully_finished_test_action",
      ScType::ConstNodeClass};

  static inline ScKeynode const unsuccessfully_finished_test_action{
      "unsuccessfully_finished_test_action",
      ScType::ConstNodeClass};

  static inline ScKeynode const finished_test_action{"finished_test_action", ScType::ConstNodeClass};
};

}  // namespace commonTest
