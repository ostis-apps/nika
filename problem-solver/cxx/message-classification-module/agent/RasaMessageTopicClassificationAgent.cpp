#include "RasaMessageTopicClassificationAgent.hpp"

#include "client/ClientInterface.hpp"
#include "client/RasaClient.hpp"

#include "manager/RasaMessageTopicClassificationManager.hpp"

#include "keynodes/MessageClassificationKeynodes.hpp"
#include <common/utils/ActionUtils.hpp>

using namespace messageClassificationModule;

RasaMessageTopicClassificationAgent::RasaMessageTopicClassificationAgent()
{m_logger = utils::ScLogger(
      utils::ScLogger::ScLogType::Console, "", utils::ScLogLevel(utils::ScLogLevel::Debug), true);
}

ScResult RasaMessageTopicClassificationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
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

  if (answerElements.size() == 0)
  {
    m_logger.Message(utils::ScLogLevel(utils::ScLogLevel::Info), "answerElements.size() == 0");  
    return action.FinishUnsuccessfully();
  }

  ActionUtils::wrapActionResultToScStructure(&m_context, action, answerElements);
  return action.FinishSuccessfully();
}

ScAddr RasaMessageTopicClassificationAgent::GetActionClass() const
{
  return MessageClassificationKeynodes::action_rasa_message_topic_classification;
}

void RasaMessageTopicClassificationAgent::initFields()
{
  std::unique_ptr<ClientInterface> client = std::make_unique<RasaClient>();
  this->manager = std::make_unique<RasaMessageTopicClassificationManager>(&m_context, &m_logger);
}
