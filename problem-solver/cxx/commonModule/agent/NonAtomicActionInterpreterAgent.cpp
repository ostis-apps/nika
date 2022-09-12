#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "keynodes/Keynodes.hpp"
#include "utils/RelationUtils.hpp"

#include "NonAtomicActionInterpreterAgent.hpp"

using namespace commonModule;
using namespace scAgentsCommon;

SC_AGENT_IMPLEMENTATION(NonAtomicActionInterpreterAgent)
{
  ScAddr actionAddr = otherAddr;
  if (!checkActionClass(actionAddr))
  {
    return SC_RESULT_OK;
  }
  SC_LOG_DEBUG("NonAtomicActionInterpreterAgent started");

  ScAddr nonAtomicActionAddr;
  try
  {
    ScAddr nonAtomicActionTemplateAddr =
        utils::IteratorUtils::getFirstByOutRelation(&m_memoryCtx, actionAddr, CoreKeynodes::rrel_1);
    if (!nonAtomicActionTemplateAddr.IsValid())
    {
      throw std::runtime_error("Action params are not formed correctly.");
    }
    ScAddr argumentsSet = utils::IteratorUtils::getFirstByOutRelation(&m_memoryCtx, actionAddr, CoreKeynodes::rrel_2);

    ScTemplateParams templateParams = createTemplateParams(nonAtomicActionTemplateAddr, argumentsSet);
    nonAtomicActionAddr = replaceNonAtomicAction(nonAtomicActionTemplateAddr, templateParams);
    generateNonAtomicActionTemplate(nonAtomicActionTemplateAddr, templateParams);
  }
  catch (std::exception & ex)
  {
    SC_LOG_ERROR(ex.what());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  initFields();
  try
  {
    nonAtomicActionInterpreter->interpret(nonAtomicActionAddr);
  }
  catch (std::runtime_error & ex)
  {
    deleteFields();
    SC_LOG_ERROR(ex.what());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }
  deleteFields();
  SC_LOG_DEBUG("NonAtomicActionInterpreterAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, true);
  return SC_RESULT_OK;
}

bool NonAtomicActionInterpreterAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      Keynodes::action_interpret_non_atomic_action, actionAddr, ScType::EdgeAccessConstPosPerm);
}

void NonAtomicActionInterpreterAgent::generateNonAtomicActionTemplate(
    ScAddr const & nonAtomicActionTemplateAddr,
    ScTemplateParams const & templateParams)
{
  ScTemplate nonAtomicActionTemplate;
  m_memoryCtx.HelperBuildTemplate(nonAtomicActionTemplate, nonAtomicActionTemplateAddr);
  ScTemplateGenResult templateGenResult;
  if (!m_memoryCtx.HelperGenTemplate(nonAtomicActionTemplate, templateGenResult, templateParams))
  {
    throw std::runtime_error("Template generation error.");
  }
}

ScAddr NonAtomicActionInterpreterAgent::getTemplateKeyElement(ScAddr const & templateAddr)
{
  ScAddr templateKeyElement =
      utils::IteratorUtils::getFirstByOutRelation(&m_memoryCtx, templateAddr, CoreKeynodes::rrel_key_sc_element);

  if (!templateKeyElement.IsValid())
  {
    throw std::runtime_error("Template key element not found.");
  }

  return templateKeyElement;
}

ScTemplateParams NonAtomicActionInterpreterAgent::createTemplateParams(
    ScAddr const & nonAtomicActionTemplate,
    ScAddr const & argumentsSet)
{
  ScTemplateParams templateParams;
  if (argumentsSet.IsValid())
  {
    ScAddr templateKeyElement;
    templateKeyElement = getTemplateKeyElement(nonAtomicActionTemplate);
    for (int index = 1;; index++)
    {
      ScAddr role = RelationUtils::getIndexRelation(&m_memoryCtx, index);
      if (!role.IsValid())
      {
        break;
      }
      ScAddr argument = utils::IteratorUtils::getFirstByOutRelation(&m_memoryCtx, argumentsSet, role);
      if (!argument.IsValid())
      {
        break;
      }
      ScIterator5Ptr variablesIterator5 = m_memoryCtx.Iterator5(
          templateKeyElement, ScType::EdgeAccessVarPosPerm, ScType::NodeVar, ScType::EdgeAccessVarPosPerm, role);
      if (variablesIterator5->Next())
      {
        templateParams.Add(m_memoryCtx.HelperGetSystemIdtf(variablesIterator5->Get(2)), argument);
      }
    }
  }

  return templateParams;
}

ScAddr NonAtomicActionInterpreterAgent::replaceNonAtomicAction(
    ScAddr const & templateAddr,
    ScTemplateParams & templateParams)
{
  ScAddr keyElementReplacement = m_memoryCtx.CreateNode(ScType::NodeConst);
  ScAddr templateKeyElement;
  templateKeyElement = getTemplateKeyElement(templateAddr);
  templateParams.Add(m_memoryCtx.HelperGetSystemIdtf(templateKeyElement), keyElementReplacement);

  return keyElementReplacement;
}

void NonAtomicActionInterpreterAgent::initFields()
{
  this->nonAtomicActionInterpreter = new NonAtomicActionInterpreter(&m_memoryCtx);
}

void NonAtomicActionInterpreterAgent::deleteFields()
{
  delete this->nonAtomicActionInterpreter;
}
