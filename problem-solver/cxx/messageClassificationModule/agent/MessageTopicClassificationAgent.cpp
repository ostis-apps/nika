#include "MessageTopicClassificationAgent.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "client/WitAiClient.hpp"
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
ScResult MessageTopicClassificationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  initFields();
  ScAddrVector answerElements;

  ScAddr const & messageAddr = utils::IteratorUtils::getAnyByOutRelation(&m_context, action, ScKeynodes::rrel_1);

  try
  {
    if (!messageAddr.IsValid())
      SC_THROW_EXCEPTION(utils::ExceptionInvalidParams, "Invalid message node.");

    answerElements = manager->manage({messageAddr});
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

  ScStructure result = m_context.GenerateStructure();
  for (auto const & element : answerElements)
    result << element;
  action.SetResult(result);
  return action.FinishSuccessfully();
}

ScAddr MessageTopicClassificationAgent::GetActionClass() const
{
  return MessageClassificationKeynodes::action_message_topic_classification;
}

void MessageTopicClassificationAgent::initFields()
{
  std::unique_ptr<WitAiClient> client = std::make_unique<WitAiClient>();
  this->manager = std::make_unique<MessageTopicClassificationManager>(&m_context);
}
