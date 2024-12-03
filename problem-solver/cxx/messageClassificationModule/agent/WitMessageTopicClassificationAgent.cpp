#include "WitMessageTopicClassificationAgent.hpp"

#include "client/ClientInterface.hpp"
#include "client/WitAiClient.hpp"

#include "keynodes/MessageClassificationKeynodes.hpp"
#include "utils/ActionUtils.hpp"

#include "manager/WitMessageTopicClassificationManager.hpp"

using namespace messageClassificationModule;

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
    SC_AGENT_LOG_ERROR(exception.Description());
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
  std::unique_ptr<ClientInterface> client = std::make_unique<WitAiClient>();
  this->manager = std::make_unique<WitMessageTopicClassificationManager>(&m_context);
}
