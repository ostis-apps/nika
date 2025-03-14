#include "AlternativeMessageTopicClassificationAgent.hpp"

#include "keynodes/MessageClassificationKeynodes.hpp"
#include <common/utils/ActionUtils.hpp>
#include <inference/inference_manager_abstract.hpp>
#include <inference/inference_manager_factory.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace messageClassificationModule;

AlternativeMessageTopicClassificationAgent::AlternativeMessageTopicClassificationAgent()
{
  m_logger = utils::ScLogger(utils::ScLogger::ScLogType::Console, "", utils::ScLogLevel::Debug);
}

ScResult AlternativeMessageTopicClassificationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddrVector answerElements;

  ScAddr const & messageAddr = action.GetArgument(ScKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
    m_logger.Error("Action doesn't have a message node.");
    return action.FinishUnsuccessfully();
  }

  ScAddr const & outputStructure = m_context.GenerateNode(ScType::ConstNodeStructure);
  inference::InferenceParams const & inferenceParams{
      MessageClassificationKeynodes::concept_classify_message_rule, {messageAddr}, {}, outputStructure};
  inference::InferenceConfig const & inferenceConfig{
      inference::GENERATE_ALL_FORMULAS,
      inference::REPLACEMENTS_ALL,
      inference::TREE_ONLY_OUTPUT_STRUCTURE,
      inference::SEARCH_IN_ALL_KB};
  std::unique_ptr<inference::InferenceManagerAbstract> iterationStrategy =
      inference::InferenceManagerFactory::ConstructDirectInferenceManagerAll(&m_context, inferenceConfig);
  try
  {
    iterationStrategy->ApplyInference(inferenceParams);
  }
  catch (utils::ScException & exception)
  {
    m_logger.Error(exception.Description());
    ActionUtils::wrapActionResultToScStructure(&m_context, action, answerElements);
    return action.FinishUnsuccessfully();
  }

  if (!utils::IteratorUtils::getAnyFromSet(&m_context, outputStructure).IsValid())
  {
    m_logger.Debug("Message is not classified.");
    ActionUtils::wrapActionResultToScStructure(&m_context, action, answerElements);
    return action.FinishUnsuccessfully();
  }
  else
  {
    m_logger.Debug("Message is classified.");
  }

  ActionUtils::wrapActionResultToScStructure(&m_context, action, answerElements);

  return action.FinishSuccessfully();
}

ScAddr AlternativeMessageTopicClassificationAgent::GetActionClass() const
{
  return MessageClassificationKeynodes::action_alternative_message_topic_classification;
}
