#include "MessageClassificationModule.hpp"

#include "sc-memory/sc_memory.hpp"

#include "agent/AlternativeMessageTopicClassificationAgent.hpp"
#include "agent/MessageTopicClassificationAgent.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace messageClassificationModule;

SC_MODULE_REGISTER(MessageClassificationModule)
    ->Agent<MessageTopicClassificationAgent>()
    ->Agent<AlternativeMessageTopicClassificationAgent>();

// todo(codegen-removal): if needed override ScModule::Initialize and move all non-keynodes and non-agents code from
// previous initialization method
/*
{
  if (!MessageClassificationKeynodes::InitGlobal())
    return SC_RESULT_ERROR;

  ScMemoryContext ctx(sc_access_lvl_make_min, "messageClassificationModule");
  if (ActionUtils::isActionDeactivated(&ctx, MessageClassificationKeynodes::action_message_topic_classification))
    SC_LOG_ERROR("action_message_topic_classification is deactivated");
  else
    //todo(codegen-removal): Use agentContext.SubscribeAgent<MessageTopicClassificationAgent> or UnsubscribeAgent; to
register and unregister agent SC_AGENT_REGISTER(MessageTopicClassificationAgent)

  if (ActionUtils::isActionDeactivated(&ctx,
MessageClassificationKeynodes::action_alternative_message_topic_classification))
    SC_LOG_ERROR("action_alternative_message_topic_classification is deactivated");
  else
    //todo(codegen-removal): Use agentContext.SubscribeAgent<AlternativeMessageTopicClassificationAgent> or
UnsubscribeAgent; to register and unregister agent SC_AGENT_REGISTER(AlternativeMessageTopicClassificationAgent)

  return SC_RESULT_OK;
}
*/

// todo(codegen-removal): if needed override ScModule::Shutdown and move all non-agents code from previous shutdown
// method
/*
{
  //todo(codegen-removal): Use agentContext.SubscribeAgent<MessageTopicClassificationAgent> or UnsubscribeAgent; to
register and unregister agent SC_AGENT_UNREGISTER(MessageTopicClassificationAgent)
  //todo(codegen-removal): Use agentContext.SubscribeAgent<AlternativeMessageTopicClassificationAgent> or
UnsubscribeAgent; to register and unregister agent SC_AGENT_UNREGISTER(AlternativeMessageTopicClassificationAgent)

  return SC_RESULT_OK;
}
*/
