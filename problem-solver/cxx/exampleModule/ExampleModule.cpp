#include "ExampleModule.hpp"

namespace exampleModule
{
	// регистрация модуля
	SC_IMPLEMENT_MODULE(ExampleModule)

	sc_result ExampleModule::InitializeImpl()
	{
		if (!LetterKeynodes::InitGlobal())
    		return SC_RESULT_ERROR;
		// регистрация агентов модуля
    ScMemoryContext ctx(sc_access_lvl_make_min, "ExampleModule");
    if (ActionUtils::isActionDeactivated(&ctx, LetterKeynodes::action_letter_search))
    {
      SC_LOG_ERROR("action_letter_search is deactivated");
    }
    else
    {
      SC_AGENT_REGISTER(LetterAgent);
    }

		return SC_RESULT_OK;
	}

	sc_result ExampleModule::ShutdownImpl()
	{
		// дерегистрация агентов модуля
		SC_AGENT_UNREGISTER(LetterAgent);

		return SC_RESULT_OK;
	}
}