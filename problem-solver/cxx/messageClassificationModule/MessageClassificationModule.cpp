#include "MessageClassificationModule.hpp"

#include "sc-memory/sc_memory.hpp"

#include "agent/MessageTopicClassificationAgent.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"
#include "utils/ActionUtils.hpp"

using namespace messageClassificationModule;

SC_IMPLEMENT_MODULE(MessageClassificationModule)

sc_result MessageClassificationModule::InitializeImpl()
{
  if (!MessageClassificationKeynodes::InitGlobal())
    return SC_RESULT_ERROR;

  ScMemoryContext ctx(sc_access_lvl_make_min, "messageClassificationModule");
  if (ActionUtils::isActionDeactivated(&ctx, MessageClassificationKeynodes::action_message_topic_classification))
    SC_LOG_ERROR("action_message_topic_classification is deactivated");
  else
    SC_AGENT_REGISTER(MessageTopicClassificationAgent)

  return SC_RESULT_OK;
}

sc_result MessageClassificationModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(MessageTopicClassificationAgent)

  return SC_RESULT_OK;
}
