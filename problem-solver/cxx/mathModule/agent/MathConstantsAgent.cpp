#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/Keynodes.hpp"
#include "utils/ActionUtils.hpp"

#include "MathConstantsAgent.hpp"

namespace mathModule
{
SC_AGENT_IMPLEMENTATION(MathConstantsAgent)
{
  ScAddr actionAddr = otherAddr;
  if (!checkActionClass(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_INFO("MathConstantsAgent started");

  initFields();
  ScAddrVector answerElements;

  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
    SC_LOG_ERROR("The message isn’t valid");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  if (!m_memoryCtx.HelperCheckEdge(
          Keynodes::concept_message_about_math_constant, messageAddr, ScType::EdgeAccessConstPosPerm))
  {
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, ScAddrVector(), true);
    return SC_RESULT_OK;
  }

  ScAddr const & mathConstantAddr = utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, messageAddr, Keynodes::rrel_entity);
  if (!mathConstantAddr.IsValid())
  {
    SC_LOG_ERROR("The math constant isn’t valid");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  try
  {
    answerElements = manager->manage({mathConstantAddr});
  }
  catch (utils::ScException & exception)
  {
    SC_LOG_ERROR(exception.Description());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answerElements, false);
    SC_LOG_DEBUG("MathConstantsAgent finished");
    return SC_RESULT_ERROR;
  }

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answerElements, true);
  SC_LOG_DEBUG("MathConstantsAgent finished");
  return SC_RESULT_OK;
}

bool MathConstantsAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      Keynodes::action_get_math_constant_value, actionAddr, ScType::EdgeAccessConstPosPerm);
}

void MathConstantsAgent::initFields()
{
  this->manager = std::make_unique<MathConstantsManager>(&m_memoryCtx);
}
}
