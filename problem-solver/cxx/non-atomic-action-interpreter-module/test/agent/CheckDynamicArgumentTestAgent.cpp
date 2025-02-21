#include "CheckDynamicArgumentTestAgent.hpp"

#include "keynodes/TestKeynodes.hpp"

using namespace commonTest;

ScResult CheckDynamicArgumentTestAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScTemplate scTemplate;
  scTemplate.Quintuple(
      action, ScType::VarPermPosArc, ScType::VarNode >> "_dynamic_argument", ScType::VarPermPosArc, ScKeynodes::rrel_1);
  scTemplate.Triple("_dynamic_argument", ScType::VarTempPosArc, TestKeynodes::test_node);
  ScTemplateSearchResult results;
  m_context.SearchByTemplate(scTemplate, results);

  return results.Size() == 1 ? action.FinishSuccessfully() : action.FinishUnsuccessfully();
}

ScAddr CheckDynamicArgumentTestAgent::GetActionClass() const
{
  return TestKeynodes::check_dynamic_argument_test_action;
}
