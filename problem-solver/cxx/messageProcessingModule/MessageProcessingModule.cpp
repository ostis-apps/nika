#include "MessageProcessingModule.hpp"

namespace messageProcessingModule
{
// Регистрация модуля
SC_IMPLEMENT_MODULE(MessageProcessingModule)

sc_result MessageProcessingModule::InitializeImpl()
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
    SC_AGENT_REGISTER(FindWordInSetByFirstLetterAgent);
  }

  return SC_RESULT_OK;
}

sc_result MessageProcessingModule::ShutdownImpl()
{
  // Дерегистрация агентов модуля
  SC_AGENT_UNREGISTER(FindWordInSetByFirstLetterAgent);

  return SC_RESULT_OK;
}

}  // namespace messageProcessingModule
