#include "ExampleModule.hpp"

#include "sc-memory/sc_memory.hpp"

#include "keynodes/Keynodes.hpp"
#include "agent/LetterAgent.hpp"

namespace exampleModule
{
	// регистрация модуля
	SC_IMPLEMENT_MODULE(ExampleModule)

	sc_result ExampleModule::InitializeImpl()
	{
		if (!Keynodes::InitGlobal())
    		return SC_RESULT_ERROR;
		// регистрация агентов модуля
		SC_AGENT_REGISTER(LetterAgent);
		return SC_RESULT_OK;
	}

	sc_result ExampleModule::ShutdownImpl()
	{
		// дерегистрация агентов модуля
		SC_AGENT_UNREGISTER(LetterAgent);
		return SC_RESULT_OK;
	}
}