#include "agent/ExampleAgent.hpp"

#include "keynodes/ExampleKeynodes.hpp"

#include "utils/ActionUtils.hpp"

#include "ExampleModule.hpp"

using namespace exampleModule;

SC_IMPLEMENT_MODULE(ExampleModule)

sc_result ExampleModule::InitializeImpl() {
    if (!ExampleKeynodes::InitGlobal()) {
        return SC_RESULT_ERROR;
    }

    ScMemoryContext ctx(sc_access_lvl_make_min, "exampleModule");
    if (ActionUtils::isActionDeactivated(&ctx, ExampleKeynodes::action_example)) {
        SC_LOG_DEBUG("action_example is deactivated");
    } else {
        SC_AGENT_REGISTER(ExampleAgent);
    }

    return SC_RESULT_OK;
}

sc_result ExampleModule::ShutdownImpl() {
    SC_AGENT_UNREGISTER(ExampleAgent);

    return SC_RESULT_OK;
}
