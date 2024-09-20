#include "MessageProcessingModule.hpp"

namespace messageProcessingModule
{
SC_MODULE_REGISTER(MessageProcessingModule)->Agent<FindWordInSetByFirstLetterAgent>();

// todo(codegen-removal): if needed override ScModule::Initialize and move all non-keynodes and non-agents code from
// previous initialization method
/*
{
  if (!MessageProcessingKeynodes::InitGlobal())
    return SC_RESULT_ERROR;

  // Регистрация агентов модуля
  ScMemoryContext ctx(sc_access_lvl_make_min, "MessageProcessingModule");
  if (ActionUtils::isActionDeactivated(&ctx, MessageProcessingKeynodes::action_find_word_in_set_by_first_letter))
  {
    SC_LOG_ERROR("action_letter_search is deactivated");
  }
  else
  {
    //todo(codegen-removal): Use agentContext.SubscribeAgent<FindWordInSetByFirstLetterAgent> or UnsubscribeAgent; to
register and unregister agent SC_AGENT_REGISTER(FindWordInSetByFirstLetterAgent);
  }

  return SC_RESULT_OK;
}
*/

// todo(codegen-removal): if needed override ScModule::Shutdown and move all non-agents code from previous shutdown
// method
/*
{
  // Дерегистрация агентов модуля
  //todo(codegen-removal): Use agentContext.SubscribeAgent<FindWordInSetByFirstLetterAgent> or UnsubscribeAgent; to
register and unregister agent SC_AGENT_UNREGISTER(FindWordInSetByFirstLetterAgent);

  return SC_RESULT_OK;
}
*/
}  // namespace messageProcessingModule
