#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "test/keynodes/TestKeynodes.hpp"

#include "AssignDynamicArgumentTestAgent.hpp"

using namespace commonTest;

ScResult AssignDynamicArgumentTestAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr dynamicArgument = utils::IteratorUtils::getAnyByOutRelation(
        &m_context, action,
        ScKeynodes::rrel_1);

  m_context.GenerateConnector(
        ScType::EdgeAccessConstPosTemp,
        dynamicArgument,
        TestKeynodes::test_node);

  return action.FinishSuccessfully();
}

ScAddr AssignDynamicArgumentTestAgent::GetActionClass() const
{
  return TestKeynodes::assign_dynamic_argument_test_action;
}

