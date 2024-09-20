#include "MessageReplyModule.hpp"

using namespace messageReplyModule;

SC_MODULE_REGISTER(MessageReplyModule)->Agent<MessageReplyAgent>();

// todo(codegen-removal): if needed override ScModule::Initialize and move all non-keynodes and non-agents code from
// previous initialization method
/*
{
  if (!messageReplyModule::MessageReplyKeynodes::InitGlobal())
  {
    return SC_RESULT_ERROR;
  }

  ScMemoryContext ctx(sc_access_lvl_make_min, "MessageReplyModule");
  if (ActionUtils::isActionDeactivated(&ctx, MessageReplyKeynodes::action_reply_to_message))
  {
    SC_LOG_ERROR("action_reply_to_message is deactivated");
  }
  else
  {
    //todo(codegen-removal): Use agentContext.SubscribeAgent<MessageReplyAgent> or UnsubscribeAgent; to register and
unregister agent SC_AGENT_REGISTER(MessageReplyAgent);
  }

  return SC_RESULT_OK;
}
*/

// todo(codegen-removal): if needed override ScModule::Shutdown and move all non-agents code from previous shutdown
// method
/*
{
  //todo(codegen-removal): Use agentContext.SubscribeAgent<MessageReplyAgent> or UnsubscribeAgent; to register and
unregister agent SC_AGENT_UNREGISTER(MessageReplyAgent);

  return SC_RESULT_OK;
}
*/
