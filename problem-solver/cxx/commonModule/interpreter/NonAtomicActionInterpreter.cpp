#include "NonAtomicActionInterpreter.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "agent/NonAtomicActionInterpreterAgent.hpp"
#include "keynodes/Keynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace commonModule;
NonAtomicActionInterpreter::NonAtomicActionInterpreter(ScAgentContext * ms_context)
{
  this->context = ms_context;
}

void NonAtomicActionInterpreter::interpret(ScAddr const & nonAtomicActionAddr)
{
  ScAddr decompositionTuple =
      utils::IteratorUtils::getAnyByInRelation(context, nonAtomicActionAddr, Keynodes::nrel_decomposition_of_action);
  ScAddr action = getFirstSubAction(decompositionTuple);
  while (action.IsValid())
  {
    if (!context->CheckConnector(decompositionTuple, action, ScType::EdgeAccessConstPosPerm))
    {
      throw std::runtime_error("Action is not belongs to nonatomic action decomposition.");
    }
    applyAction(action);
    action = getNextAction(action);
  }
}

ScAddr NonAtomicActionInterpreter::getFirstSubAction(ScAddr const & decompositionTuple)
{
  ScAddr firstAction = utils::IteratorUtils::getAnyByOutRelation(context, decompositionTuple, ScKeynodes::rrel_1);
  if (!firstAction.IsValid())
  {
    throw std::runtime_error("Non atomic action structure is incorrect. Failed to find first action.");
  }
  return firstAction;
}

void NonAtomicActionInterpreter::applyAction(ScAddr const & actionAddr)
{
  SC_LOG_DEBUG("Waiting for atomic action finish.");
  ScAction action = context->ConvertToAction(actionAddr);
  if (!action.InitiateAndWait(WAIT_TIME))
  {
    throw std::runtime_error("Action wait time expired.");
  }
  SC_LOG_DEBUG("Atomic action finished.");
}

ScAddr NonAtomicActionInterpreter::getNextAction(ScAddr const & actionAddr)
{
  ScAddr nextAction;
  if (context->CheckConnector(ScKeynodes::action_finished_successfully, actionAddr, ScType::EdgeAccessConstPosPerm))
  {
    SC_LOG_DEBUG("Atomic action finished successfully.");
    nextAction = getThenAction(actionAddr);
  }
  else if (context->CheckConnector(
               ScKeynodes::action_finished_unsuccessfully, actionAddr, ScType::EdgeAccessConstPosPerm))
  {
    SC_LOG_DEBUG("Atomic action finished unsuccessfully.");
    nextAction = getElseAction(actionAddr);
  }
  else
  {
    SC_LOG_DEBUG("Atomic action finished with unknown result.");
    nextAction = getGoToAction(actionAddr);
  }

  return nextAction;
}

ScAddr NonAtomicActionInterpreter::getThenAction(ScAddr const & actionAddr)
{
  ScAddr nextAction = utils::IteratorUtils::getAnyByOutRelation(context, actionAddr, Keynodes::nrel_then);
  if (!nextAction.IsValid())
  {
    SC_LOG_DEBUG("Action with nrel_then relation not found, searching for nrel_goto instead");
    nextAction = getGoToAction(actionAddr);
  }
  return nextAction;
}

ScAddr NonAtomicActionInterpreter::getElseAction(ScAddr const & actionAddr)
{
  ScAddr nextAction = utils::IteratorUtils::getAnyByOutRelation(context, actionAddr, Keynodes::nrel_else);
  if (!nextAction.IsValid())
  {
    SC_LOG_DEBUG("Action with nrel_else relation not found, searching for nrel_goto instead");
    nextAction = getGoToAction(actionAddr);
  }
  return nextAction;
}

ScAddr NonAtomicActionInterpreter::getGoToAction(ScAddr const & actionAddr)
{
  return utils::IteratorUtils::getAnyByOutRelation(context, actionAddr, Keynodes::nrel_goto);
}
