#include "WitMessageTopicClassificationAgent.hpp"

#include "client/WitAiClient.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"
#include <common/utils/ActionUtils.hpp>

using namespace messageClassificationModule;

WitMessageTopicClassificationAgent::WitMessageTopicClassificationAgent()
{
  m_logger = utils::ScLogger(
      utils::ScLogger::ScLogType::File, "logs/WitMessageTopicClassificationAgent.log", utils::ScLogLevel::Debug, true);
}

ScResult WitMessageTopicClassificationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
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

ScAddr WitMessageTopicClassificationAgent::GetActionClass() const
{
  return MessageClassificationKeynodes::action_message_topic_classification;
}

void WitMessageTopicClassificationAgent::initFields()
{
  std::unique_ptr<WitAiClient> client = std::make_unique<WitAiClient>(&m_logger);
  this->manager = std::make_unique<WitMessageTopicClassificationManager>(&m_context, &m_logger);
}
