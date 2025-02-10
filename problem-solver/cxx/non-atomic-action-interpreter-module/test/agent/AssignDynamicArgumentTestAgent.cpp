#include "AssignDynamicArgumentTestAgent.hpp"

#include "keynodes/TestKeynodes.hpp"

using namespace commonTest;

ScResult AssignDynamicArgumentTestAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddr dynamicArgument = action.GetArgument(ScKeynodes::rrel_1);
  m_context.GenerateConnector(ScType::ConstTempPosArc, dynamicArgument, TestKeynodes::test_node);

  return action.FinishSuccessfully();
}

ScAddr AssignDynamicArgumentTestAgent::GetActionClass() const
{
  return TestKeynodes::assign_dynamic_argument_test_action;
}
