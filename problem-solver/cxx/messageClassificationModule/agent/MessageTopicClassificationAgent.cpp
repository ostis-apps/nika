#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/MessageClassificationKeynodes.hpp"

#include "client/WitAiClient.hpp"
#include "MessageTopicClassificationAgent.hpp"

using namespace messageClassificationModule;

SC_AGENT_IMPLEMENTATION(MessageTopicClassificationAgent)
{
  ScAddr const & actionAddr = otherAddr;
  if (!checkActionClass(actionAddr))
    return SC_RESULT_OK;

  SC_LOG_DEBUG("MessageTopicClassificationAgent started");

  initFields();
  ScAddrVector answerElements;

  ScAddr const & messageAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, scAgentsCommon::CoreKeynodes::rrel_1);

  try
  {
    if (!messageAddr.IsValid())
      SC_THROW_EXCEPTION(utils::ExceptionInvalidParams, "Invalid message node.");

    answerElements = manager->manage({messageAddr});
  }
  catch (utils::ScException & exception)
  {
    SC_LOG_ERROR(exception.Description());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answerElements, false);
    SC_LOG_DEBUG("MessageTopicClassificationAgent finished");
    return SC_RESULT_ERROR;
  }

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answerElements, true);
  SC_LOG_DEBUG("MessageTopicClassificationAgent finished");
  return SC_RESULT_OK;
}

void MessageTopicClassificationAgent::initFields()
{
  std::unique_ptr<WitAiClient> client = std::make_unique<WitAiClient>();
  this->manager = std::make_unique<MessageTopicClassificationManager>(&m_memoryCtx);
}

bool MessageTopicClassificationAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      MessageClassificationKeynodes::action_message_topic_classification, actionAddr, ScType::EdgeAccessConstPosPerm);
}
