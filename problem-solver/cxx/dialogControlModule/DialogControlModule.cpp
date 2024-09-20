#include "DialogControlModule.hpp"

using namespace dialogControlModule;

SC_MODULE_REGISTER(DialogControlModule)->Agent<PhraseGenerationAgent>()->Agent<StandardMessageReplyAgent>();

// todo(codegen-removal): if needed override ScModule::Initialize and move all non-keynodes and non-agents code from
// previous initialization method
/*
{
  if (!dialogControlModule::DialogKeynodes::InitGlobal())
  {
    return SC_RESULT_ERROR;
  }
  if (!dialogControlModule::MessageKeynodes::InitGlobal())
  {
    return SC_RESULT_ERROR;
  }

  ScMemoryContext ctx(sc_access_lvl_make_min, "dialogControlModule");
  if (ActionUtils::isActionDeactivated(&ctx, MessageKeynodes::action_phrase_generation))
  {
    SC_LOG_DEBUG("action_phrase_generation is deactivated");
  }
  else
  {
    //todo(codegen-removal): Use agentContext.SubscribeAgent<PhraseGenerationAgent> or UnsubscribeAgent; to register and
unregister agent SC_AGENT_REGISTER(PhraseGenerationAgent);
  }

  if (ActionUtils::isActionDeactivated(&ctx, MessageKeynodes::action_standard_message_reply))
  {
    SC_LOG_DEBUG("action_standard_message_reply is deactivated");
  }
  else
  {
    //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register
and unregister agent SC_AGENT_REGISTER(StandardMessageReplyAgent);
  }

  return SC_RESULT_OK;
}
*/

// todo(codegen-removal): if needed override ScModule::Shutdown and move all non-agents code from previous shutdown
// method
/*
{
  //todo(codegen-removal): Use agentContext.SubscribeAgent<PhraseGenerationAgent> or UnsubscribeAgent; to register and
unregister agent SC_AGENT_UNREGISTER(PhraseGenerationAgent);
  //todo(codegen-removal): Use agentContext.SubscribeAgent<StandardMessageReplyAgent> or UnsubscribeAgent; to register
and unregister agent SC_AGENT_UNREGISTER(StandardMessageReplyAgent);

  return SC_RESULT_OK;
}
*/
