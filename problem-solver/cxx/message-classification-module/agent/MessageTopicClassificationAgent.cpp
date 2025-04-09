#include "MessageTopicClassificationAgent.hpp"

#include "client/WitAiClient.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"
#include <common/utils/ActionUtils.hpp>

using namespace messageClassificationModule;

MessageTopicClassificationAgent::MessageTopicClassificationAgent()
{
  m_logger = utils::ScLogger(utils::ScLogger::ScLogType::Console, "", utils::ScLogLevel::Debug);
}

ScResult MessageTopicClassificationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  initFields();
  ScAddrVector answerElements;

  ScAddr const & messageAddr = action.GetArgument(ScKeynodes::rrel_1);

  try
  {
    if (!messageAddr.IsValid())
      SC_THROW_EXCEPTION(utils::ExceptionInvalidParams, "Invalid message node.");

    answerElements = manager->manage({messageAddr});
  }
  catch (utils::ScException & exception)
  {
    m_logger.Error(exception.Description());
    ActionUtils::wrapActionResultToScStructure(&m_context, action, answerElements);

    return action.FinishUnsuccessfully();
  }
  ActionUtils::wrapActionResultToScStructure(&m_context, action, answerElements);
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
