#include "MessageTopicClassificationAgent.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "client/WitAiClient.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"

using namespace messageClassificationModule;

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
