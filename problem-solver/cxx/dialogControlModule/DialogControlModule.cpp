#include "DialogControlModule.hpp"

using namespace dialogControlModule;

SC_IMPLEMENT_MODULE(DialogControlModule)

sc_result DialogControlModule::InitializeImpl()
{
  if (!dialogControlModule::CoreKeynodes::InitGlobal())
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
    SC_AGENT_REGISTER(PhraseGenerationAgent);
  }

  if (ActionUtils::isActionDeactivated(&ctx, MessageKeynodes::action_standard_message_reply))
  {
    SC_LOG_DEBUG("action_standard_message_reply is deactivated");
  }
  else
  {
    SC_AGENT_REGISTER(StandardMessageReplyAgent);
  }

  return SC_RESULT_OK;
}

sc_result DialogControlModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(PhraseGenerationAgent);
  SC_AGENT_UNREGISTER(StandardMessageReplyAgent);

  return SC_RESULT_OK;
}
