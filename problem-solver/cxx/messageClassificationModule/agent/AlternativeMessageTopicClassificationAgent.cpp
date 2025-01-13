#include "AlternativeMessageTopicClassificationAgent.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "factory/InferenceManagerFactory.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace messageClassificationModule;

ScResult AlternativeMessageTopicClassificationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  return action.FinishUnsuccessfully();
  ScAddrVector answerElements;

  ScAddr const & messageAddr = action.GetArgument(ScKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
    SC_AGENT_LOG_ERROR("Action doesn't have a message node.");
    return action.FinishSuccessfully();
  }

  ScAddr const & outputStructure = m_context.GenerateNode(ScType::NodeConstStruct);
  inference::InferenceParams const & inferenceParams{
      MessageClassificationKeynodes::concept_classify_message_rule, {messageAddr}, {}, outputStructure};
  inference::InferenceConfig const & inferenceConfig{
      inference::GENERATE_ALL_FORMULAS,
      inference::REPLACEMENTS_ALL,
      inference::TREE_ONLY_OUTPUT_STRUCTURE,
      inference::SEARCH_IN_ALL_KB};
  std::unique_ptr<inference::InferenceManagerAbstract> iterationStrategy =
      inference::InferenceManagerFactory::constructDirectInferenceManagerAll(&m_context, inferenceConfig);
  try
  {
    iterationStrategy->applyInference(inferenceParams);
  }
  catch (utils::ScException & exception)
  {
    SC_AGENT_LOG_ERROR(exception.Description());
    ActionUtils::wrapActionResultToScStructure(&m_context, action, answerElements);
    return action.FinishUnsuccessfully();
  }

  if (!utils::IteratorUtils::getAnyFromSet(&m_context, outputStructure).IsValid())
  {
    SC_AGENT_LOG_DEBUG("Message is not classified.");
    ActionUtils::wrapActionResultToScStructure(&m_context, action, answerElements);
    return action.FinishUnsuccessfully();
  }
  else
  {
    SC_AGENT_LOG_DEBUG("Message is classified.");
  }

  ActionUtils::wrapActionResultToScStructure(&m_context, action, answerElements);

}

ScAddr AlternativeMessageTopicClassificationAgent::GetActionClass() const
{
  return MessageClassificationKeynodes::action_alternative_message_topic_classification;
}
