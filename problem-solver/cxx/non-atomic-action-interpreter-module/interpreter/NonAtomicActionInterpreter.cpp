#include "NonAtomicActionInterpreter.hpp"

#include <common/keynodes/Keynodes.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace nonAtomicActionInterpreterModule;
NonAtomicActionInterpreter::NonAtomicActionInterpreter(ScAgentContext * context)
  : context(context)
{
}

void NonAtomicActionInterpreter::interpret(ScAddr const & nonAtomicActionAddr)
{
  ScAddr decompositionTuple = utils::IteratorUtils::getAnyByInRelation(
      context, nonAtomicActionAddr, commonModule::Keynodes::nrel_decomposition_of_action);
  ScAddr action = getFirstSubAction(decompositionTuple);
  while (action.IsValid())
  {
    if (!context->CheckConnector(decompositionTuple, action, ScType::ConstPermPosArc))
    {
      SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "Action is not belongs to nonatomic action decomposition.");
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
    SC_THROW_EXCEPTION(
        utils::ExceptionItemNotFound, "Non atomic action structure is incorrect. Failed to find first action.");
  }
  return firstAction;
}

void NonAtomicActionInterpreter::applyAction(ScAddr const & actionAddr)
{
  SC_LOG_DEBUG(getClassNameForLog() + ": Waiting for atomic action finish.");
  ScAction action = context->ConvertToAction(actionAddr);
  if (!action.InitiateAndWait(WAIT_TIME))
    SC_THROW_EXCEPTION(utils::ExceptionCritical, "Action wait time expired.");

  SC_LOG_DEBUG(getClassNameForLog() + ": Atomic action finished.");
}

ScAddr NonAtomicActionInterpreter::getNextAction(ScAddr const & actionAddr)
{
  ScAddr nextAction;
  if (context->CheckConnector(ScKeynodes::action_finished_successfully, actionAddr, ScType::ConstPermPosArc))
  {
    SC_LOG_DEBUG(getClassNameForLog() + ": Atomic action finished successfully.");
    nextAction = getThenAction(actionAddr);
  }
  else if (context->CheckConnector(ScKeynodes::action_finished_unsuccessfully, actionAddr, ScType::ConstPermPosArc))
  {
    SC_LOG_DEBUG(getClassNameForLog() + ": Atomic action finished unsuccessfully.");
    nextAction = getElseAction(actionAddr);
  }
  else
  {
    SC_LOG_DEBUG(getClassNameForLog() + ": Atomic action finished with unknown result.");
    nextAction = getGoToAction(actionAddr);
  }

  return nextAction;
}

ScAddr NonAtomicActionInterpreter::getThenAction(ScAddr const & actionAddr)
{
  ScAddr nextAction = utils::IteratorUtils::getAnyByOutRelation(context, actionAddr, commonModule::Keynodes::nrel_then);
  if (!nextAction.IsValid())
  {
    SC_LOG_DEBUG(getClassNameForLog() + ": Action with nrel_then relation not found, searching for nrel_goto instead");
    nextAction = getGoToAction(actionAddr);
  }
  return nextAction;
}

ScAddr NonAtomicActionInterpreter::getElseAction(ScAddr const & actionAddr)
{
  ScAddr nextAction = utils::IteratorUtils::getAnyByOutRelation(context, actionAddr, commonModule::Keynodes::nrel_else);
  if (!nextAction.IsValid())
  {
    SC_LOG_DEBUG(getClassNameForLog() + ": Action with nrel_else relation not found, searching for nrel_goto instead");
    nextAction = getGoToAction(actionAddr);
  }
  return nextAction;
}

ScAddr NonAtomicActionInterpreter::getGoToAction(ScAddr const & actionAddr)
{
  return utils::IteratorUtils::getAnyByOutRelation(context, actionAddr, commonModule::Keynodes::nrel_goto);
}

std::string NonAtomicActionInterpreter::getClassNameForLog()
{
  static std::string const className = "NonAtomicActionInterpreter";
  return className;
}
