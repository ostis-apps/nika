#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "factory/InferenceManagerFactory.hpp"

#include "keynodes/MessageClassificationKeynodes.hpp"

#include "AlternativeMessageTopicClassificationAgent.hpp"

using namespace messageClassificationModule;

SC_AGENT_IMPLEMENTATION(AlternativeMessageTopicClassificationAgent)
{
  ScAddr const & actionAddr = otherAddr;
  if (!checkActionClass(actionAddr))
    return SC_RESULT_OK;

  SC_LOG_DEBUG("AlternativeMessageTopicClassificationAgent started");

  ScAddrVector answerElements;

  ScAddr const & messageAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, scAgentsCommon::CoreKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
    SC_LOG_ERROR("Action doesn't have a message node.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScAddr const & outputStructure = m_memoryCtx.CreateNode(ScType::NodeConstStruct);
  InferenceParams const & inferenceParams{MessageClassificationKeynodes::concept_classify_message_rule, {messageAddr}, {}, outputStructure};
  InferenceConfig const & inferenceConfig{
        GENERATE_ALL_FORMULAS, REPLACEMENTS_ALL, TREE_ONLY_OUTPUT_STRUCTURE, SEARCH_IN_ALL_KB};
  std::unique_ptr<inference::InferenceManagerAbstract> iterationStrategy =
        inference::InferenceManagerFactory::constructDirectInferenceManagerAll(&m_memoryCtx, inferenceConfig);
  try
  {
    iterationStrategy->applyInference(inferenceParams);
  }
  catch (utils::ScException & exception)
  {
    SC_LOG_ERROR(exception.Description());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answerElements, false);
    SC_LOG_DEBUG("AlternativeMessageTopicClassificationAgent finished");
    return SC_RESULT_ERROR;
  }

  if (!utils::IteratorUtils::getAnyFromSet(&m_memoryCtx, outputStructure).IsValid())
  {
    SC_LOG_DEBUG("Message is not classified.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answerElements, false);
    SC_LOG_DEBUG("AlternativeMessageTopicClassificationAgent finished");
    return SC_RESULT_ERROR;
  }
  else
  {
    SC_LOG_DEBUG("Message is classified.");
  }

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, answerElements, true);
  SC_LOG_DEBUG("AlternativeMessageTopicClassificationAgent finished");
  return SC_RESULT_OK;
}

bool AlternativeMessageTopicClassificationAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      MessageClassificationKeynodes::action_alternative_message_topic_classification, actionAddr, ScType::EdgeAccessConstPosPerm);
}
