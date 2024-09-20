#include "AlternativeMessageTopicClassificationAgent.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "factory/InferenceManagerFactory.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"

using namespace messageClassificationModule;

// todo(codegen-removal): remove agent starting and finishing logs, sc-machine is printing them now
// todo(codegen-removal): if your agent is ScActionInitiatedAgent and uses event only to get action node via
// event.GetOtherElement() then you can remove event from method arguments and use ScAction & action instead of your
// action node todo(codegen-removal): if your agent is having method like CheckActionClass(ScAddr actionAddr) that
// checks connector between action class and actionAddr then you can remove it. Before agent is started sc-machine check
// that action belongs to class returned by GetActionClass() todo(codegen-removal): use action.SetResult() to pass
// result of your action instead of using answer or answerElements todo(codegen-removal): use SC_AGENT_LOG_SOMETHING()
// instead of SC_LOG_SOMETHING to automatically include agent name to logs messages todo(codegen-removal): use auto
// const & [names of action arguments] = action.GetArguments<amount of arguments>(); to get action arguments
ScResult AlternativeMessageTopicClassificationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  ScAddrVector answerElements;

  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_context, action, ScKeynodes::rrel_1);
  if (!messageAddr.IsValid())
  {
    SC_AGENT_LOG_ERROR("Action doesn't have a message node.");
    return action.FinishUnsuccessfully();
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
    SC_LOG_ERROR(exception.Description());
    ScStructure result = m_context.GenerateStructure();
    for (auto const & element : answerElements)
      result << element;
    action.SetResult(result);
    return action.FinishUnsuccessfully();
  }

  if (!utils::IteratorUtils::getAnyFromSet(&m_context, outputStructure).IsValid())
  {
    SC_AGENT_LOG_DEBUG("Message is not classified.");
    ScStructure result = m_context.GenerateStructure();
    for (auto const & element : answerElements)
      result << element;
    action.SetResult(result);
    return action.FinishUnsuccessfully();
  }
  else
  {
    SC_AGENT_LOG_DEBUG("Message is classified.");
  }

  ScStructure result = m_context.GenerateStructure();
  for (auto const & element : answerElements)
    result << element;
  action.SetResult(result);

  return action.FinishSuccessfully();
}

ScAddr AlternativeMessageTopicClassificationAgent::GetActionClass() const
{
  return MessageClassificationKeynodes::action_alternative_message_topic_classification;
}
